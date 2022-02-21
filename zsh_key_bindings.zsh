fff_file_widget() {
	echo
	output=`FFF_COMMAND="find -L . -mindepth 1 -not -path '*/venv*' -not -path '*/.git/objects*' 2> /dev/null" fff | tee /dev/tty | tail -1`
	zle reset-prompt
	RBUFFER=$output
	CURSOR=$#BUFFER
}

fff_git_commands_widget() {
	echo
	git | grep "^   [a-z]"| sed 's/^   //g' | fff | tee /dev/tty | tail -1 | cut -d' ' -f1
	zle reset-prompt
	RBUFFER=$output
	CURSOR=$#BUFFER
}

fff_branch_widget() {
	echo
	git --no-pager branch -a |cut -b 3-512 | fff | tee /dev/tty | tail -1 | tr -d ' '
	zle reset-prompt
	RBUFFER=$output
	CURSOR=$#BUFFER
}

fff_ps_widget() {
	echo
	output=`ps ax | tail +2 | cut -b 3-512 | fff | tee /dev/tty | tail -1`
	pid=" "`echo $output | cut -b 28-120 | tr -d ' '`
	comment=" # "`echo $output | cut -b 28-120 | tr -d ' '`
	zle reset-prompt
	RBUFFER="$pid $comment"
	CURSOR=$#BUFFER
}

fff_history_widget() {
	echo
	history | cut -b 8-60 | fff | tee /dev/tty | tail -1
	zle reset-prompt
	RBUFFER=$output
	CURSOR=$#BUFFER
}

fff_pods_widget() {
	echo
	kubectl get pods | tail +2 | fff | tee /dev/tty | tail -1
	zle reset-prompt
	RBUFFER=$output
	CURSOR=$#BUFFER
}

fff_ssh_widget() {
	echo
	cat ~/.ssh/known_hosts | cut -d',' -f1 | cut -d' ' -f1 | fff | tee /dev/tty | tail -1 | cut -d' ' -f1
	zle reset-prompt
	RBUFFER=$output
	CURSOR=$#BUFFER
}

fff_handler_widget() {
	cmd=`echo $BUFFER | cut -d' ' -f1`
	cmd_arg=`echo $BUFFER | cut -d; ; -f2`

	case "$cmd" in
		git)
		if [[ $cmd_arg == 'checkout' ]]; then
			fff_branch_widget
		else
			fff_git_commands_widget
		fi
		;;
		ps)
		fff_ssh_widget
		;;
		pods)
		fff_pods_widget
		;;
		ssh)
		fff_ssh_widget
		;;
		*)
		fff_file_widget
		;;
	esac
}


zle -N fff_file fff_file_widget
zle -N fff_branch fff_branch_widget
zle -N fff_ps fff_ps_widget
zle -N fff_history fff_history_widget
zle -N fff_pods fff_pods_widget
zle -N fff_ssh fff_ssh_widget
zle -N fff_handler fff_handler_widget

bindkey "\C-f" fff_handler

bindkey "\C-t" fff_file
bindkey "\C-b" fff_branch
bindkey "\C-p" fff_ps
bindkey "\C-h" fff_history
bindkey "\C-o" fff_pods
bindkey "\C-y" fff_ssh

