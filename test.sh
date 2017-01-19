output=res1.txt
total=$[3*10**6]

Arr=(0 1 2 3 4 5 6)
for i in ${Arr[*]}
do
	for j in ${Arr[*]}
	do
		for k in ${Arr[*]}
		do
			field1=$[10**i]
			field2=$[10**j]
			field3=$[10**k]
			./Release/DBI-Project $total $field1 $field2 $field3 >> $output
		done
	done
	
	
done

date
systemctl suspend