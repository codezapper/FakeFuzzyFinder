if exists("g:loaded_fff")
    finish
endif
let g:loaded_fff = 1

command! -nargs=0 Files call fff#FFFFiles()
