#!/bin/sh

compilo="../../compilateur/compilo"

if [[ -z "$compilo" ]]; then
	echo "Compilateur introuvable : $compilo"
	echo ""
	exit
fi

"$compilo" "-h" >/dev/null 2>/dev/null
retVal=$?
if [ "$retVal" != "0" ]; then
	echo "Impossible d'executer : $compilo"
	echo ""
	exit
fi


notPassed=0

for file in ./*
do
	mime=$(file -b --mime-type "$file")
	if [ "$mime" = "text/x-c" ]; then
		echo "------------  $file ------------"
		#echo ""
		"$compilo" "$file"
		#"$compilo" "$file"  >/dev/null 2>/dev/null
		retVal=$?
		echo ""
		echo "-> $retVal"
		if [ "$retVal" = "0" ]; then
			echo "not passed"
			notPassed=$(($notPassed+1))
		else
			echo "passed"
		fi
		echo ""
	fi
done

	echo "------------------------"
	echo ""
	
if [ "$notPassed" != "0" ]; then
	echo ""
	echo "/!\\ $notPassed script(s) ne produisent pas d'erreur alors qu'il le devraient"
	echo ""
else
	echo ""
	echo "Alright !"
	echo ""
fi
