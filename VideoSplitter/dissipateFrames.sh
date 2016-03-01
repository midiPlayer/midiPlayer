#!/bin/bash
#Author: Jonas Vollhardt

#$1 -> numB
#$2 -> numH
#$3 -> proportion
#$4 -> cutTypeB
#$5 -> cutTypeH
#$6 -> margin
#$7 -> force-tiff
#$8 -> extractAsTiff (false, falls Videoframes als jpg gespeichert wurden)
#$9 -> startNumber der Frames
#$10 -> id, nötig, weil das Skript wahrscheinlich mehrfach parallel läuft
#$11 -> frameNum (Anzahl der Frames, die von diesem Script zerhackstückelt werden)


#prüfen, ob $1 gesetzt ist
if [ -z "$1" ]
then
	exit 4;
else
	numB=$1;
fi

#prüfen, ob $2 gesetzt ist
if [ -z "$2" ]
then
	exit 4;
else
	numH=$2;
fi


#prüfen, ob $3 gesetzt ist
if [ -z "$3" ]
then
	exit 4;
else
	proportion=$3;
fi

#prüfen, ob $4 gesetzt ist
if [ -z "$4" ]
then
	exit 4;
else
	cutTypeB=$4;
fi

#prüfen, ob $5 gesetzt ist
if [ -z "$5" ]
then
	exit 4;
else
	cutTypeH=$5;
fi

#prüfen, ob $6 gesetzt ist
if [ -z "$6" ]
then
	exit 4;
else
	margin=$6;
fi

#prüfen, ob $7 gesetzt ist
if [ -z "$7" ]
then
	exit 4;
else
	forcetiff=$7;
fi

#prüfen, ob $8 gesetzt ist
if [ -z "$8" ]
then
	exit 4;
else
	extractAsTiff=$8;
fi

#prüfen, ob $9 gesetzt ist
if [ -z "$8" ]
then
	exit 4;
else
	startNumber=$9;
fi

#prüfen, ob $10 gesetzt ist
if [ -z ${10} ]
then
	exit 4;
else
	pid=${10};
fi


#prüfen, ob $11 gesetzt ist
if [ -z ${11} ]
then
	exit 4;
else
	frameNum=${11};
fi

#erstelle Unterordner für die Ausgabebilder dieses Skripts

#mkdir -p meta/$pid;

# counter=0;

#teile jedes Einzelbild mithilfe ./dissipateImage.sh in Einzelbilder auf, und speichere diese in entsprechenden Unterordnern
for (( i=$startNumber; i < $(($frameNum+$startNumber)); ++i ))
do
	if [[ $extractAsTiff == true ]]
	then
		./dissipateImage.sh ./meta/meta$i.tiff $proportion $numB $numH tiff $cutTypeH $cutTypeB $margin no-questions $pid >/dev/null;
	else
		./dissipateImage.sh ./meta/meta$i.jpg $proportion $numB $numH tiff $cutTypeH $cutTypeB $margin no-questions $pid >/dev/null;
	fi
	
# 	#für die Statusanzeige des Video-Skripts
# 	counter=$(($counter+1));
# 	echo $counter > ./log/log$pid.log;

	#gebe die Bildnummer aus, die gerade bearbeitet wird
	echo $i;
	
	
	#gerade erstellte Teilbilder in den richtigen Unterordner legen
	for (( j=1; j <= $(($numB*$numH)); j++ ))
	do
		if [ $(($j%$numB)) != 0 ]
		then
			nb=$(($j%$numB));
		else
			nb=$numB;
		fi
	
		nh=$(($((`echo "$(($j-1)) $numB" | awk '{printf "%.6f \n", ($1/$2)}' | cut -d '.' -f 1`))+1));
	
# 		if [[ $forcetiff == false ]]
# 		then
			#und dabei richtig benennen ##eigentlich ist doch $i die gültige ID ##so ist es im Folgenden ersetzt
			#id=`find meta/$pid/files_"$nh"_"$nb" -type f -iname "*.jpg" | wc -l`;
		
			mv ./finalp_"$pid"/final-"$pid"-_"$nh"_"$nb".tiff ./meta/$pid/files_"$nh"_"$nb"/$i.tiff;
# 		else
			#und dabei richtig benennen
			#id=`find meta/$pid/files_"$nh"_"$nb" -type f -iname "*.tiff" | wc -l`;
		
# 			mv ./finalp_"$pid"/final-"$pid"-_"$nh"_"$nb".tiff ./meta/$pid/files_"$nh"_"$nb"/$i.tiff;
# 		fi


		#wenn forcetiff == false
		if [ $forcetiff == false ]
		then
			convert ./meta/$pid/files_"$nh"_"$nb"/$i.tiff ./meta/$pid/files_"$nh"_"$nb"/$i.jpg;
			rm ./meta/$pid/files_"$nh"_"$nb"/$i.tiff;
		fi
		
	done
	
	if [[ $extractAsTiff == true ]]
	then
		rm ./meta/meta$i.tiff;
	else
		rm ./meta/meta$i.jpg;
	fi
		
	#finalp-Ordner löschen
	rm -r ./finalp_"$pid";
	
done