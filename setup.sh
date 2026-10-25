#!/usr/bin/env bash
# 在 WSL 终端中运行：bash setup.sh
# 安装 XeLaTeX + 中文支持 + latexmk + biber + vimtex + zathura，并安装 ~/.vimrc
set -euo pipefail

cd "$(dirname "$0")"

sudo apt-get update
sudo apt-get install -y \
  texlive-xetex \
  texlive-lang-chinese \
  texlive-lang-cjk \
  texlive-latex-recommended \
  texlive-latex-extra \
  texlive-fonts-recommended \
  texlive-bibtex-extra \
  biber \
  latexmk \
  vim-vimtex \
  zathura \
  zathura-pdf-poppler \
  xdotool

if [ -e "$HOME/.vimrc" ]; then
  echo
  echo "已存在 ~/.vimrc，未覆盖。请手动把 $(pwd)/vimrc 的内容合并进去。"
else
  cp vimrc "$HOME/.vimrc"
  echo "已安装 ~/.vimrc"
fi

echo
echo "完成。试着编译：latexmk   （PDF 在 build/main.pdf）"
