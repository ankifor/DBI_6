output=/home/ankifor/workspace/DBI-Project/res2.txt

times=3
methods=("none" "cube_unordMap_union" "cube_unordMap" "cube_myHash" "cube_final" "rollup_union" "rollup_final")




#experiment 1
echo 'experiment1'
Arr1=(2 4 6)
Arr2=(0 1 2 3 4 5 6)
field1=1
total=$[10**7]

for i in ${Arr1[*]}
do
field2=$[10**i]

for j in ${Arr2[*]}	
do
field3=$[10**j]

	for m in ${methods[*]}
	do
		/usr/bin/time --format='mem %M' ./Release/DBI-Project $total $field1 $field2 $field3 $times $m &>> $output
		#echo $total $field1 $field2 $field3 $times $m 
	done
	
done	
done

#experiment 2
echo 'experiment2'
field1=$[10**2]
field2=$[10**2]
field3=$[10**2]
totals=($[10**5] $[10**6] $[2*10**6] $[5*10**6] $[10**7] $[2*10**7] $[5*10**7] $[10**8])
for total in ${totals[*]}
do

	for m in ${methods[*]}
	do
		/usr/bin/time --format='mem %M' ./Release/DBI-Project $total $field1 $field2 $field3 $times $m &>> $output
		#echo $total $field1 $field2 $field3 $times $m 
	done
	
	
done

date
systemctl suspend