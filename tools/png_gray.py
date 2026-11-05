#!/usr/bin/env python3
"""把一张 8 位的 PNG（RGB 或 RGBA，不隔行）转换成灰度 PNG，用作封面照片。

用法：python3 tools/png_gray.py 输入.png 输出.png [对比度]
只用 Python 标准库（zlib），不需要安装 PIL。对比度默认 1.15，1.0 表示不变。
"""
import struct
import sys
import zlib


def read_png(path):
    with open(path, "rb") as f:
        data = f.read()
    if data[:8] != b"\x89PNG\r\n\x1a\n":
        sys.exit(f"{path}: 不是 PNG 文件")
    pos, idat = 8, []
    width = height = None
    while pos < len(data):
        length, kind = struct.unpack(">I4s", data[pos:pos + 8])
        body = data[pos + 8:pos + 8 + length]
        if kind == b"IHDR":
            width, height, depth, color, _, _, interlace = struct.unpack(">IIBBBBB", body)
            if depth != 8 or color not in (2, 6) or interlace != 0:
                sys.exit(f"{path}: 只支持 8 位、RGB/RGBA、不隔行的 PNG")
            channels = 3 if color == 2 else 4
        elif kind == b"IDAT":
            idat.append(body)
        elif kind == b"IEND":
            break
        pos += 12 + length
    return width, height, channels, zlib.decompress(b"".join(idat))


def unfilter(width, height, channels, raw):
    """去掉每一行的 PNG 过滤（filter 0–4），返回每一行的像素字节。"""
    stride = width * channels
    rows, prev, pos = [], bytearray(stride), 0
    for _ in range(height):
        ftype = raw[pos]
        line = bytearray(raw[pos + 1:pos + 1 + stride])
        pos += 1 + stride
        if ftype == 1:
            for i in range(channels, stride):
                line[i] = (line[i] + line[i - channels]) & 0xFF
        elif ftype == 2:
            for i in range(stride):
                line[i] = (line[i] + prev[i]) & 0xFF
        elif ftype == 3:
            for i in range(stride):
                left = line[i - channels] if i >= channels else 0
                line[i] = (line[i] + ((left + prev[i]) >> 1)) & 0xFF
        elif ftype == 4:
            for i in range(stride):
                a = line[i - channels] if i >= channels else 0
                b = prev[i]
                c = prev[i - channels] if i >= channels else 0
                p = a + b - c
                pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
                pred = a if pa <= pb and pa <= pc else (b if pb <= pc else c)
                line[i] = (line[i] + pred) & 0xFF
        rows.append(line)
        prev = line
    return rows


def write_gray_png(path, width, height, rows):
    def chunk(kind, body):
        return (struct.pack(">I", len(body)) + kind + body
                + struct.pack(">I", zlib.crc32(kind + body) & 0xFFFFFFFF))
    raw = b"".join(b"\x00" + bytes(r) for r in rows)
    with open(path, "wb") as f:
        f.write(b"\x89PNG\r\n\x1a\n")
        f.write(chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 0, 0, 0, 0)))
        f.write(chunk(b"IDAT", zlib.compress(raw, 9)))
        f.write(chunk(b"IEND", b""))


def main():
    if len(sys.argv) not in (3, 4):
        sys.exit(__doc__)
    src, dst = sys.argv[1], sys.argv[2]
    contrast = float(sys.argv[3]) if len(sys.argv) == 4 else 1.15
    width, height, channels, raw = read_png(src)
    rows = unfilter(width, height, channels, raw)
    # 亮度 Y = 0.299 R + 0.587 G + 0.114 B，再以 128 为中心稍微拉开对比度
    lut = [min(255, max(0, round((v - 128) * contrast + 128))) for v in range(256)]
    gray = []
    for line in rows:
        out = bytearray(width)
        for x in range(width):
            i = x * channels
            y = (299 * line[i] + 587 * line[i + 1] + 114 * line[i + 2]) // 1000
            out[x] = lut[y]
        gray.append(out)
    write_gray_png(dst, width, height, gray)
    print(f"{dst}: {width}x{height} 灰度")


if __name__ == "__main__":
    main()
