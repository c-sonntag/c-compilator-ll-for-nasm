#!/bin/sh
for file in ./bin/*
do
	mime=$(file -b --mime-type "$file")
	if [ "$mime" = "application/x-executable" ]; then
		echo "------------  $file ------------"
		echo ""
		"$file"
		retVal=$?
		echo ""
		echo "-> $retVal"
		echo ""
	fi
  
done