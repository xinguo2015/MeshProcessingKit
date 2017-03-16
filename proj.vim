
let g:projectvim="proj.vim"

set path+=.
set path+=src
set path+=src/inc

if has("macunix")
	"a hack to make 'YouCompleteMe' happy
	set path+=~/.opengl.headers
endif


""""""""""""""""""""""""""""""""""""""""""""""""""""""
" F5 - 保存、make
""""""""""""""""""""""""""""""""""""""""""""""""""""""
let buildfold='build'
map <F5> :w<Esc><CR>:exe 'make -C' buildfold<CR>:cw<CR><CR>
imap <F5> <Esc><F5>
""""""""""""""""""""""""""""""""""""""""""""""""""""""

