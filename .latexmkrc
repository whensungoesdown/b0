# 使用 XeLaTeX 编译（中文需要 XeLaTeX）
$pdf_mode = 5;
# -shell-escape：minted 要调用外部程序 latexminted。TeX Live 的受限模式虽然允许
# latexminted，但不会用 tools/ 下修复过的版本，所以这里打开完整的 shell-escape。
$xelatex  = 'xelatex -shell-escape -synctex=1 -interaction=nonstopmode -file-line-error %O %S';

# 编译产生的辅助文件和 PDF 都放到 build/ 目录
$out_dir = 'build';

# 注意：同一个项目同时只能有一个 latexmk 在编译。例如 Vim 中已经用 \ll 开着持续编译时，
# 不要再在终端里运行 latexmk，否则两个进程会同时改写 build/ 中的文件，出现 biber 报
# “.bcf is malformed”、xdvipdfmx 报“File ended prematurely”等莫名其妙的错误。

# 代码高亮用 minted，它要调用辅助程序 latexminted。
# tools/latexminted 修复了 Ubuntu 版本在 Python 3.14 下的崩溃，放到 PATH 最前面。
use Cwd ();
$ENV{PATH} = Cwd::getcwd() . "/tools:$ENV{PATH}";

# 中英文两版：build/main-zh.pdf、build/main-en.pdf。只编译一个版本：latexmk main-zh.tex
# 各章用 \input 而不是 \include：英文版还没翻译的章先用中文版占位，两个版本 \include 同一个文件时，
# 会争着改写 build/ 下同一个 .aux。
@default_files = ('main-zh.tex', 'main-en.tex');

# latexmk -c 时一并清理这些文件
$clean_ext = 'bbl run.xml synctex.gz xdv';
