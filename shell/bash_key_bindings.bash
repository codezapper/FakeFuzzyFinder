fff_file_select() {
	echo
	FFF_COMMAND="find -L . -mindepth 1 -not -path '*/venv*' -not -path '*/.git/objects*' 2> /dev/null" fff | tee /dev/tty | tail -1
}

fff_git_commands() {
	git | grep "^   [a-z]"| sed 's/^   //g' | fff | tee /dev/tty | tail -1 | cut -d' ' -f1
}

fff_branch() {
	git --no-pager branch -a |cut -b 3-512 | fff | tee /dev/tty | tail -1 | tr -d ' '
}

fff_ps() {
	output=`ps ax | tail +2 | cut -b 3-512 | fff | tee /dev/tty | tail -1`
	echo -n ${output:0:6} # ${output:6}
}

fff_history() {
	history | cut -b 8-60 | fff | tee /dev/tty | tail -1
}

fff_pods() {
	kubectl get pods | tail +2 | fff | tee /dev/tty | tail -1
}

fff_ssh() {
	cat ~/.ssh/known_hosts | cut -d',' -f1 | cut -d' ' -f1 | fff | tee /dev/tty | tail -1 | cut -d' ' -f1
}

bind "\C-g":redraw-current-line
bind '"\C-t": "`fff_file_select`\e\C-e\C-g"'
bind '"\C-b": "`fff_branch`\e\C-e\C-g"'
bind '"\C-p": "`fff_ps`\e\C-e\C-g"'
bind '"\C-h": "`fff_history`\e\C-e\C-g"'
bind '"\C-o": "`fff_pods`\e\C-e\C-g"'
bind '"\C-y": "`fff_ssh`\e\C-e\C-g"'
bind '"\C-u": "`fff_git_commands`\e\C-e\C-g"'

