# 使用 XeLaTeX 编译（中文需要 XeLaTeX）
$pdf_mode = 5;
$xelatex  = 'xelatex -synctex=1 -interaction=nonstopmode -file-line-error %O %S';

# 编译产生的辅助文件和 PDF 都放到 build/ 目录
$out_dir = 'build';

# 只编译主文件
@default_files = ('main.tex');

# latexmk -c 时一并清理这些文件
$clean_ext = 'bbl run.xml synctex.gz xdv';
