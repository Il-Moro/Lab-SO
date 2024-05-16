#!/bin/bash
file="address-book-database.csv"
if [[ -f $file ]]; then

	if [[ $1 == "view" ]]; then
		if [[ $# != 1 ]]; then 
			echo "Error, expected 1 argument"
			exit 1
		fi
		
	   	column -t -s "," $file | head -n 1 
		tail -n +2 $file | column -t -s "," | sort -k 4
		
	elif [[ $1 == "search" ]]; then
		if [[ $# != 2 ]]; then 
			echo "Error, expected 2 argument"
			exit 
		fi
		
		if (( $(grep -c -w $2 $file) == 0 )); then
			echo "Not found" 
		else		
			IFS=$'\n,'
			dati=$(grep -w $2 $file)
					
			count=1		
			for item in $dati; do
				
				pre=$(head -n 1 $file | cut -d ',' -f$count); pre="${pre^}"
				
			 	echo "$pre: $item"
			 	#echo "$pre, $count"
			 	count=$(( count + 1 ))
			 	
			 	if (( $count > 6 )); then
			 		echo -e
			 		count=1
			 	fi
			done
		fi
		
	elif [[ $1 == "insert" ]]; then
		if [[ $# != 1 ]]; then 
			echo "Error, expected 1 argument"
			exit 1
		fi
		
		# Variabile per memorizzare la nuova riga
		new_entry=""
		
		for ((i=1; i<=6; i++)); do
			pre="$(head -n 1 $file | cut -d "," -f$i)"; pre="${pre^}"
			read -p "$pre: " value

			if (( i == 4 )) && (( $(grep -c -w "$value" "$file") == 1 )); then
				echo "Error"
				exit 1
			fi

			if (( i == 6 )); then
				new_entry+="$value"
			else
				new_entry+="$value,"
			fi
		done

		# Aggiungi la nuova riga al file con un a capo
		echo -e "\n$new_entry" >> "$file"
		echo "Added"
		
	elif [[ $1 == "delete" ]]; then
			
		if (( $# != 2 )); then
		 	echo "Error, expected 2 argument"
		 	exit 1
		 
		elif [[ $(head -n 1 $file | grep -c -w $2 $value) != 0 ]]; then
			echo "Error, cannot execute command"
			exit 1
		else
			if (( $(grep -c -w $2 $file) == 0 )); then
					echo "Cannot find any record" 
				else
					email=$2
					sed -i "/$email/d" "$file"
					echo "Delete"	
			fi
		fi
	else 
		echo "Not expected this type of argument"	
	fi
else 
	echo "file not found"
fi