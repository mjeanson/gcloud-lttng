#!/bin/bash

# SPDX-FileCopyrightText: 2022 Michael Jeanson <mjeanson@efficios.com>
#
# SPDX-License-Identifier: MIT

_echo() {
	tput setaf 2
	echo "$1"
	tput sgr0
}

echo_green() {
	_echo "$1"
}
