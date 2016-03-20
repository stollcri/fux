#!/bin/bash

app=bin/fux
pass=0
fail=0

prove() {
	input=$1
	output=$2
	if [ -z "$input" -o -z "$output" ]; then
		echo " NOK -- INVALID PARAMETERS"
		return
	fi

	result=$(echo "$input" | $app)
	if [ "$result" == "$output" ]; then
		echo "  OK $input ===> $output"
		pass=$((pass + 1))
	else
		echo " NOK $input =!=> $result"
		fail=$((fail + 1))
	fi
}

#
# SAFE
#

prove "sduo apt-get inst vim" "# sudo apt-get install vim"
prove "apt-get install vim" "# sudo apt-get install vim"
prove "atp-get install vim" "# sudo apt-get install vim"
prove "aptget install vim" "# sudo apt-get install vim"
prove "atpget install vim" "# sudo apt-get install vim"
prove "atp-get inst vim" "# sudo apt-get install vim"
prove "apt install vim" "# sudo apt-get install vim"

prove "apt-get install docker" "# sudo apt-get install docker"
prove "apt-get inst docker" "# sudo apt-get install docker"
prove "apt-get inst npm" "# sudo apt-get install npm"

prove "apt-get remove vim" "# sudo apt-get remove vim"
prove "aptget remove vim" "# sudo apt-get remove vim"
prove "apt remove vim" "# sudo apt-get remove vim"

prove "brew install vim" "# brew install vim"
prove "berw inst vim" "# brew install vim"

prove "dockr up" "# docker up"
prove "fock up" "# docker up"

prove "histroy" "# history"
prove "hist" "# history"

#
# UNSAFE
#

prove "hsitroy" "# #history"

echo "Pass: $pass"
echo "Fail: $fail"
