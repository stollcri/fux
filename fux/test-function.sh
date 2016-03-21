#!/bin/bash

app=bin/fux
dbg=1
pass=0
fail=0

prove() {
	input=$1
	if [ "$dbg" -eq 1 ]; then
		output="# $2"
	else
		output=$2
	fi
	if [ -z "$input" -o -z "$output" ]; then
		echo " NOK -- INVALID PARAMETERS"
		return
	fi

	result=$(echo "$input" | $app)
	if [ "$result" == "$output" ]; then
		echo "  OK \"$input\" ===> \"$output\""
		pass=$((pass + 1))
	else
		echo " NOK \"$input\" ===> \"$result\""
		echo "     \"$input\"  !=  \"$output\""
		fail=$((fail + 1))
	fi
}

#
# SAFE
#

prove "git co https://git.com//fux.git" "git checkout https://git.com//fux.git"
prove "git push push origin master" "git push origin master"

prove "sudo apt-get inst git python pip" "sudo apt-get install  git python pip"
prove "sduo atpget inst git python pip" "sudo apt-get install  git python pip"
prove "sduo apt-get inst vim" "sudo apt-get install vim"
prove "apt-get install vim" "sudo apt-get install vim"
prove "atp-get install vim" "sudo apt-get install vim"
prove "aptget install vim" "sudo apt-get install vim"
prove "atpget install vim" "sudo apt-get install vim"
prove "atp-get inst vim" "sudo apt-get install vim"
prove "apt install vim" "sudo apt-get install vim"

prove "apt-get install docker" "sudo apt-get install docker"
prove "apt-get inst docker" "sudo apt-get install docker"
prove "apt-get inst npm" "sudo apt-get install npm"

prove "apt-get remove vim" "sudo apt-get remove vim"
prove "aptget remove vim" "sudo apt-get remove vim"
prove "apt remove vim" "sudo apt-get remove vim"

prove "brew install vim" "brew install vim"
prove "brew insta11 vim" "brew install vim"
prove "berw inst vim" "brew install vim"

prove "dockr up" "docker up"
prove "fock up" "docker up"

prove "git stastus" "git status"
prove "histroy" "history"
prove "hist" "history"

#
# UNSAFE
#

prove "hsitroy" "#history"

echo "Pass: $pass"
echo "Fail: $fail"
