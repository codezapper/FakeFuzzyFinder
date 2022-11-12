function! fff#DisplayTime(...)
autoload
plugin
    if a:0 > 0 && (a:1 == "d" || a:1 == "t")
        if a:1 == "d"
            echo strftime("%b %d")
        elseif a:1 == "t"
            echo strftime("%H:%M")
        endif
    else
        echo strftime("%b %d %H:%M")
    endif
endfunction


function! fff#FFFFiles()
    let FFFTempFile = tempname()
    let $FFF_TEMP_FILE = FFFTempFile
    let $VIM_RUNNING_DIR = getcwd()
    silent !run_fff.sh
    let result=readfile($FFF_TEMP_FILE)
    redraw!
    execute 'edit' result[0]
    call delete(FFFTempFile)
endfunction
