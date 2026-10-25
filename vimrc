" ~/.vimrc —— VimTeX 配置（由 mypaper/setup.sh 安装）
" vimtex 由 apt 包 vim-vimtex 提供，Vim 会自动加载，无需插件管理器。

set nocompatible
filetype plugin indent on
syntax enable

set encoding=utf-8
set fileencodings=utf-8,gb18030,latin1
set number
set wrap linebreak
set formatoptions+=mM      " 中文换行和合并行时不插入多余空格
set spelllang=en,cjk       " 拼写检查忽略中文

" 反向搜索（在 PDF 中 Ctrl+点击跳回 Vim）需要 Vim 的 clientserver
if empty(v:servername) && exists('*remote_startserver')
  call remote_startserver('VIM')
endif

" ---------- VimTeX ----------
let g:tex_flavor = 'latex'
let g:vimtex_compiler_method = 'latexmk'
let g:vimtex_compiler_latexmk_engines = {'_': '-xelatex'}
let g:vimtex_compiler_latexmk = {
      \ 'out_dir'    : 'build',
      \ 'continuous' : 1,
      \ 'options'    : [
      \   '-verbose',
      \   '-file-line-error',
      \   '-synctex=1',
      \   '-interaction=nonstopmode',
      \ ],
      \}

" PDF 查看器：zathura（通过 WSLg 显示窗口）
let g:vimtex_view_method = 'zathura'

" 编译出错时打开 quickfix 窗口，但光标不跳过去
let g:vimtex_quickfix_mode = 2
" 忽略常见的无害警告
let g:vimtex_quickfix_ignore_filters = [
      \ 'Underfull',
      \ 'Overfull',
      \]
