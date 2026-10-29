# 使用 XeLaTeX 编译（中文需要 XeLaTeX）
$pdf_mode = 5;
# -shell-escape：minted 要调用外部程序 latexminted。TeX Live 的受限模式虽然允许
# latexminted，但不会用 tools/ 下修复过的版本，所以这里打开完整的 shell-escape。
$xelatex  = 'xelatex -shell-escape -synctex=1 -interaction=nonstopmode -file-line-error %O %S';

use Cwd ();
use File::Path ();
my $cwd = Cwd::getcwd();

# PDF 和 synctex 文件放到 build/ 目录
$out_dir = 'build';

# 其余辅助文件（.aux、.xdv、minted 的缓存等）的位置：
# 项目在 Windows 磁盘上（WSL 中的 /mnt/c、/mnt/d……）时，编译过程中的文件读写偶尔会
# 不同步，出现 biber 读到不完整的 .bcf、xdvipdfmx 读到不完整的 .xdv 等错误。
# 所以这种情况下把辅助文件放到 Linux 文件系统中的 ~/.cache/latexmk-aux/ 下。
# 注意：vimtex 会用正则表达式读取本文件中 $aux_dir = "..." 形式的赋值，并把引号里的
# 内容原样当作路径，所以这里先算出路径，再赋给 $aux_dir。vimrc 中有一段相同的规则。
if ($cwd =~ m{^/mnt/[a-z]/}) {
    (my $key = $cwd) =~ s{^/mnt/}{}; $key =~ s{/}{_}g;
    my $linux_aux = "$ENV{HOME}/.cache/latexmk-aux/$key";
    $aux_dir = $linux_aux;
} else {
    $aux_dir = $out_dir;
}

# \include 的文件在子目录中时，XeLaTeX 要在辅助目录下写同名子目录里的 .aux，
# 但它不会自己创建这些子目录，这里预先建好
File::Path::make_path($out_dir, map { "$aux_dir/$_" } qw(frontmatter chapters backmatter));

# 代码高亮用 minted，它要调用辅助程序 latexminted。
# tools/latexminted 修复了 Ubuntu 版本在 Python 3.14 下的崩溃，放到 PATH 最前面。
$ENV{PATH} = "$cwd/tools:$ENV{PATH}";

# \include 的每一章都会在自己的 .aux 中记录所有计数器的值。其中几个内部用的临时计数器
# （minted 行号用的 LN@truepage、instcount，biblatex 的 cbx@tempcnt*）每次编译都不一样，
# latexmk 会因此认为需要重新编译，一直跑到次数上限也停不下来。比较 .aux 时忽略这几行。
$hash_calc_ignore_pattern{'aux'} = '^\\\\setcounter\{(?:LN@truepage|instcount|cbx@tempcnt\w)\}';

# 只编译主文件
@default_files = ('main.tex');

# latexmk -c 时一并清理这些文件
$clean_ext = 'bbl run.xml synctex.gz xdv';
