# b0 —— 中文书籍 LaTeX 模板（ctexbook + XeLaTeX + VimTeX）

WSL 中的路径：`/mnt/d/work/mypaper/b0`

## 第一次使用

```bash
cd /mnt/d/work/mypaper/b0
bash setup.sh        # 只需在新电脑上运行一次（本机已完成）；需要 sudo 密码
latexmk              # 编译中英文两版，生成 build/main-zh.pdf、build/main-en.pdf
latexmk main-en.tex  # 只编译英文版
```

## 目录结构

```
main-zh.tex           中文版的主文件：文档类、章节列表
main-en.tex           英文版的主文件：文档类、章节列表（未翻译的章用中文版占位）
style/book.tex        两个版本共用的导言区：宏包、页面、代码高亮
style/lang-zh.tex     中文版特有的设置：书名、“代码”“定理”这些名字、各章引言、页脚
style/lang-en.tex     英文版特有的设置：同上，另外把“第一章”“目录”“图”“表”换成英文
style/lazy-*.tex      版式：页眉页脚、章首页、节标题、页边注（两个版本共用）
frontmatter/          封面、献词（两个版本共用）；zh/、en/ 下是各自的前言
chapters/zh/          中文版正文，一章一个文件
chapters/en/          英文版正文，文件名和中文版一一对应
backmatter/zh/、en/   附录
figures/              图片
code/                 书中引用的源代码，按章分目录；各目录中运行 make 可以编译和测试
refs.bib              参考文献（GB/T 7714-2015 格式输出）
.latexmkrc            latexmk 设置：XeLaTeX，-shell-escape，输出到 build/
tools/latexminted     minted 辅助程序的修复版（见下面的“代码高亮”）
tools/translation-status.sh  检查英文版是否跟上了中文版（见下面的“中英文两个版本”）
vimrc                 Vim 配置，setup.sh 会复制为 ~/.vimrc
```

## 代码高亮（minted）

书中的代码用 minted 排版，颜色由 Python 的 Pygments 生成。

- 带题注、编入“代码清单”的代码文件：`\codefile{code/ch03/alu.v}{题注}{lst:标签}`；语言不是 C 时写在方括号里，如 `\codefile[verilog]{...}{...}{...}`、`\codefile[gas]{...}{...}{...}`（GNU 汇编）。
- 正文中的短代码：`\begin{minted}[linenos=false]{c} ... \end{minted}`。
- 支持的语言名：在终端运行 `pygmentize -L lexers`。
- minted 要调用外部程序，所以 `.latexmkrc` 中打开了 `-shell-escape`。
- Ubuntu 自带的 `latexminted` 在 Python 3.14 下会崩溃，`tools/latexminted` 是修复版，`.latexmkrc` 会把它放到 PATH 最前面。以后 Ubuntu 修复了这个问题，可以删掉 `tools/` 和 `.latexmkrc` 中修改 PATH 的那一行。

新增一章：在 `chapters/zh/` 下新建 `ch06.tex`，然后在 `main-zh.tex` 中加一行 `\input{chapters/zh/ch06}`，在 `main-en.tex` 中也加上这一行（先用中文版占位）。

## 中英文两个版本

两个版本放在同一个分支里，共用版式、图片和 `code/` 下的程序；不同的只是正文和 `style/lang-*.tex`。改版式、改程序，两个版本一起生效。

翻译一章的步骤（以第 2 章为例）：

1. 新建 `chapters/en/ch02.tex`，开头两行写：
   ```
   %! TeX root = ../../main-en.tex
   % 译自: chapters/zh/ch02.tex @ <提交>:chapters/zh/ch02.tex
   ```
   `<提交>` 是中文版最后一次修改这一章的提交，用 `git log -1 --format=%h -- chapters/zh/ch02.tex` 查。
2. 在 `main-en.tex` 中把 `\input{chapters/zh/ch02}` 改成 `\input{chapters/en/ch02}`。
3. 示例程序：只含英文的程序两个版本共用；输出中文、注释是中文的程序，英文版另写一份放在 `code/en/ch02/`，书里引用这份。

以后中文版改了，运行 `sh tools/translation-status.sh` 看看哪些英文文件落后了（标着“旧”），加上 `-d` 可以看到中文版具体改了什么。英文版跟着改好以后，先提交，再把“译自”一行 `@` 后面换成新的提交。

标签（`\label`）在两个版本里保持一样，这样别的章引用它时，不管那一章翻译了没有，都能找到。

## 在 Vim 中写作

用 `vim main-zh.tex` 或 `vim main-en.tex` 打开。章节文件第一行的 `%! TeX root = ../../main-zh.tex` 告诉 vimtex 这个文件属于哪个版本，新建章节文件时记得也写上（英文版写 `main-en.tex`）。常用快捷键（`\` 是 localleader）：

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

如果想用 Windows 的宋体、黑体、楷体、仿宋，把字体复制到 WSL 中，然后把 `main-zh.tex`、`main-en.tex` 里的 `fontset=fandol` 改为 `fontset=windows`：

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
