#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Usage: $0 Input_file"
else
	sed -n '/export/p' $1 | sed 's/[()]//g' > $1.export
fi

