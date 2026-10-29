# b0 —— 中文书籍 LaTeX 模板（ctexbook + XeLaTeX + VimTeX）

WSL 中的路径：`/mnt/d/work/mypaper/b0`

## 第一次使用

```bash
cd /mnt/d/work/mypaper/b0
bash setup.sh        # 只需在新电脑上运行一次（本机已完成）；需要 sudo 密码
latexmk              # 编译，生成 build/main.pdf
```

## 目录结构

```
main.tex              主文件：文档类、宏包、书名、章节列表
frontmatter/          前言等（罗马数字页码，不编号）
chapters/             正文各章，一章一个文件
backmatter/           附录
figures/              图片
code/                 书中引用的源代码，按章分目录；各目录中运行 make 可以编译和测试
refs.bib              参考文献（GB/T 7714-2015 格式输出）
.latexmkrc            latexmk 设置：XeLaTeX，-shell-escape，输出到 build/
tools/latexminted     minted 辅助程序的修复版（见下面的“代码高亮”）
vimrc                 Vim 配置，setup.sh 会复制为 ~/.vimrc
```

## 代码高亮（minted）

书中的代码用 minted 排版，颜色由 Python 的 Pygments 生成。

- 带题注、编入“代码清单”的代码文件：`\codefile{code/ch03/alu.v}{题注}{lst:标签}`；语言不是 C 时写在方括号里，如 `\codefile[verilog]{...}{...}{...}`、`\codefile[gas]{...}{...}{...}`（GNU 汇编）。
- 正文中的短代码：`\begin{minted}[linenos=false]{c} ... \end{minted}`。
- 支持的语言名：在终端运行 `pygmentize -L lexers`。
- minted 要调用外部程序，所以 `.latexmkrc` 中打开了 `-shell-escape`。
- Ubuntu 自带的 `latexminted` 在 Python 3.14 下会崩溃，`tools/latexminted` 是修复版，`.latexmkrc` 会把它放到 PATH 最前面。以后 Ubuntu 修复了这个问题，可以删掉 `tools/` 和 `.latexmkrc` 中修改 PATH 的那一行。

新增一章：在 `chapters/` 下新建 `ch03.tex`，然后在 `main.tex` 中加一行 `\include{chapters/ch03}`。

## 在 Vim 中写作

用 `vim main.tex` 打开（编辑章节文件时，vimtex 会自动找到 main.tex）。常用快捷键（`\` 是 localleader）：

| 按键 | 作用 |
|---|---|
| `\ll` | 开始/停止持续编译（保存文件后自动重新编译） |
| `\lv` | 在 zathura 中打开 PDF，并跳到光标所在位置 |
| `\le` | 查看编译错误 |
| `\lc` | 清理辅助文件 |
| `\lt` | 显示目录 |
| 在 zathura 中 Ctrl+左键 | 跳回 Vim 中对应的源码行 |

## 字体

默认使用 `fontset=fandol`（TeX Live 自带的开源中文字体），在任何 Linux 上都能编译。

如果想用 Windows 的宋体、黑体、楷体、仿宋，把字体复制到 WSL 中，然后把 `main.tex` 里的 `fontset=fandol` 改为 `fontset=windows`：

```bash
mkdir -p ~/.local/share/fonts
cp /mnt/c/Windows/Fonts/{simsun.ttc,simhei.ttf,simkai.ttf,simfang.ttf} ~/.local/share/fonts/
fc-cache -f
```

## 提示

- 项目在 Windows 磁盘上（`/mnt/d`），在 WSL 中编译比放在 Linux 目录（如 `~/b0`）慢一些；如果嫌慢，可以把项目移到 `~` 下。
- 文件请保存为 UTF-8 编码、LF 换行。
- 同一时间只用一种方式编译：Vim 中按 `\ll` 开着持续编译时，不要再在终端里运行 `latexmk`。两个编译进程同时改写 `build/`，会出现 “.bcf is malformed”“File ended prematurely” 这类莫名其妙的错误。遇到这类错误，先停掉多余的编译，再 `rm -rf build` 重新编译。
- `.latexmkrc` 是隐藏文件，`mv *` / `cp *` 不会带上它；缺少它时 `latexmk` 不会用 XeLaTeX，编译会失败。
- 新建一本书时，复制整个目录（含隐藏文件），并去掉旧的编译结果和 git 记录：
  ```bash
  cd /mnt/d/work/mypaper
  cp -r b0 b1 && rm -rf b1/build b1/.git
  ```
