#!/bin/bash
#Author: Jonas Vollhardt

#$1 -> Video, das zerlegt werden soll
#$2 -> Seitenverhältnis der Einzelvideos
#$3 -> Anzahl der horizontalen Teile, in die das Video zerlegt werden soll
#$4 -> Anzahl der vertikalen Teile, in die das Video zerlegt werden soll
#$5 -> Dateiformat der Ausgabevideos; 'mp4' wenn nicht anders angegeben
#$6 -> "oben", "unten" oder "gleich": gibt an, ob das Video - falls es in der Höhe zugeschnitten werden muss - nur oben, nur unten oder oben und unten gleichmäßig abgeschnitten wird
#$7 -> "links", "rechts" oder "gleich": gibt an, ob das Video - falls es in der Breite zugeschnitten werden muss - nur links, nur rechts oder oben und unten gleichmäßig abgeschnitten wird
#$8 -> Freiraum zwischen den Einzelvideos
#$9 -> optional "v" für verbosity
#$10 -> optional "force-tiff", falls die Einzelbilder niemals in andere Dateiformate z. B. jpg umgewandelt werden sollen
#$11 -> optional "numThreads", Anzahl der Threads, die danach vom Skript "./dissipateFrames.sh" gleichzeitig laufen


#prüfen, ob $1 gesetzt und vorhanden ist
if [ -z "$1" ]
then
	echo "Es wurde keine Eingabedatei festgelegt! - Bitte erneut versuchen!";
	exit 4;
elif [ ! -f "$1" ]
then
	echo "Die Eingabedatei \""$1"\" konnte nicht gefunden werden! - Bitte erneut versuchen!";
	exit 4
fi



#prüfen, ob $2 gesetzt ist
#TODO: prüfen ob float
if [ -z "$2" ]
then
	echo "Es wurde kein Seitenverhältnis für die Einzelvideos festgelegt! - Bitte erneut versuchen.";
	exit 4;
else
	proportion=$2;
fi


#prüfen, ob $3 gesetzt ist
if [ -z "$3" ]
then
	echo "Es wurde keine Anzahl für die horizontalen Teile festgelegt! - Bitte erneut versuchen.";
	exit 4;
else
	if  [ "$3" -eq "$3" ] 2>/dev/null
	then
		if [ $3 -gt 0 ]
		then
			numB=$3;
		else
			echo "Es können nicht null Bilder ausgegeben werden! - Bitte erneut versuchen!";
			exit 4
		fi
	else
		echo "\""$3"\" ist keine natürliche Zahl! - Bitte erneut versuchen!;";
		exit 4
	fi
fi


#prüfen, ob $4 gesetzt ist
if [ -z "$4" ]
then
	echo "Es wurde keine Anzahl für die vertikalen Teile festgelegt! - Bitte erneut versuchen.";
	exit 4;
else
	if  [ "$4" -eq "$4" ] 2>/dev/null
	then
		if [ $4 -gt 0 ]
		then
			numH=$4;
		else
			echo "Es können nicht null Bilder ausgegeben werden! - Bitte erneut versuchen!";
			exit 4
		fi
	else
		echo "\""$4"\" ist keine natürliche Zahl! - Bitte erneut versuchen!;";
		exit 4;
	fi
fi


#setze $5, falls noch nicht geschehen -> default: "mpg"
if [ -z "$5" ]
then
	vtype="mp4";
	echo "Benutze default-Wert 'mp4' für das Ausgabeformat!";
else
	if [ $5 == "mp4" ] || [ $5 == "flv" ] || [ $5 == "mpg" ] || [ $5 == "dv" ]
	then
		type=$5;
	else
		echo "Das Dateiformat \""$5"\" ist ungültig oder wird nicht unterstützt! - Bitte erneut versuchen!";
		exit 4;
	fi
fi


#setze $6, falls noch nicht geschehen -> default: "gleich"
if [ -z "$6" ]
then
	cutTypeH="gleich";
	echo "Benutze default-Wert 'gleich' für Zuschnitt in der Höhe!";
else
	if [ $6 == "oben" ] || [ $6 == "unten" ] || [ $6 == "gleich" ] 
	then
		cutTypeH=$6;
	else
		cutTypeH="gleich";
		echo "Ungültiger Parameter für Zuschnitt in der Höhe, benutze default-Wert 'gleich'!";
	fi
fi


#setze $7, falls noch nicht geschehen -> default: "gleich"
if [ -z "$7" ]
then
	cutTypeB="gleich";
	echo "Benutze default-Wert 'gleich' für Zuschnitt in der Breite!";
else
	if [ $7 == "links" ] || [ $7 == "rechts" ] || [ $7 == "gleich" ] 
	then
		cutTypeB=$7;
	else
		cutTypeB="gleich";
		echo "Ungültiger Parameter für Zuschnitt in der Breite, benutze default-Wert 'gleich'!";
	fi
fi


#prüfe, ob $8 gesetzt und eine gültige Zahl ist -> default: "0"
if [ -z "$8" ]
then
	margin="0";
	echo "Benutze default-Wert '0' für Freiraum zwischen den Bildern!";
else
	if  [ "$8" -eq "$8" ] 2>/dev/null
	then
		if [ $8 == 0 ]
		then
			margin=$8;
		else
			if [ $8 -gt 0 ]
			then
				margin=$8;
			else
				echo "Der Freiraum zwischen den Einzelbildern darf nicht negativ sein! - Bitte erneut versuchen!";
				exit 4;
			fi
		fi
	else
		echo "\""$8"\" ist weder eine natürliche Zahl noch null! - Bitte erneut versuchen!;";
		exit 4;
	fi
fi


#prüfen, ob $9 gesetzt ist -> default: no verbosity
if [ -z "$9" ]
then
	verbosity=false;
else
	if [ $9 == "v" ]
	then
		verbosity=true;
	elif [ $9 == "nv" ]
	then
		verbosity=false;
	else
		echo "Der Parameter \""$9"\" ist ungültig! - Bitte erneut versuchen!";
		exit 4;
	fi
fi


#prüfen, ob $10 gesetzt ist -> default: no force
if [ -z ${10} ]
then
	forcetiff=false;
else
	if [ ${10} == "force-tiff" ]
	then
		forcetiff=true;
	elif [ ${10} == "no-force-tiff" ]
	then
		forcetiff=false;
	else
		echo "Der Parameter \""${10}"\" ist ungültig! - Bitte erneut versuchen!";
		exit 4;
	fi
fi


#prüfen, ob $11 gesetzt ist und eine Zahl größer null ist -> default: 1
if [ -z ${11} ]
then
	numThreads=1;
elif [[ "${11}" -gt 0 ]]
then
	numThreads=${11};
else
	echo "Der Parameter \""${11}"\" ist ungültig! - Bitte erneut versuchen!";
	exit 4;
fi


#prüfen, ob sich das Skript "dissipateImage.sh" im Ordner befindet
if [ ! -f "./dissipateImage.sh" ]
then
	echo "Das notwendige Skript \"dissipateImage.sh\" befindet sich nicht im Ordner! - Bitte erneut versuchen!";
	exit 4;
fi


#prüfen, ob die Ordner finalv, finalp, log, meta existieren; wenn ja, löschen?
if [ -d "./finalv" ]
then
	echo "Damit das Skript richtig ausgeführt wird, muss der Ordner 'finalv' gelöscht werden!";
	echo "Fortsetzen (f) oder abbrechen (a)?"
	read confirmation3;
	if [[ $confirmation3 == "f" ]]
	then
		rm -r finalv;
	elif [[ $confirmation3 == "a" ]]
	then
		exit 4;
	else
		echo "Ungültige Eingabe! - Bitte Skript erneut starten!";
		exit 4;
	fi
fi
if [ -d "./finalp" ]
then
	echo "Damit das Skript richtig ausgeführt wird, muss der Ordner 'finalp' gelöscht werden!";
	echo "Fortsetzen (f) oder abbrechen (a)?"
	read confirmation4;
	if [[ $confirmation4 == "f" ]]
	then
		rm -r finalp;
	elif [[ $confirmation4 == "a" ]]
	then
		exit 4;
	else
		echo "Ungültige Eingabe! - Bitte Skript erneut starten!";
		exit 4;
	fi
fi
if [[ -d "./meta" ]]
then
	echo "Damit das Skript richtig ausgeführt wird, muss der Ordner 'meta' gelöscht werden!";
	echo "Fortsetzen (f) oder abbrechen (a)?"
	read confirmation6;
	if [[ $confirmation6 == "f" ]]
	then
		rm -r meta;
	elif [[ $confirmation6 == "a" ]]
	then
		exit 4;
	else
		echo "Ungültige Eingabe! - Bitte Skript erneut starten!";
		exit 4;
	fi
fi



echo "Schritt 1: Zerlege nun das Video in Einzelbilder!";


#zerlege das Video in die Einzelbilder
#TODO: Eingabeparameter für die Größe der Einzelbilder; momentan: Abmessungen genauso wie Video

#Informationen über das Video sammeln
if [[ $verbosity == true ]] #also ausgiebige Ausgaben gewünscht
then
	width=`ffprobe -show_streams -i $1 | grep -m 1 width | cut -d '=' -f 2`;
	height=`ffprobe -show_streams -i $1 | grep -m 1 height | cut -d '=' -f 2`;
else
	width=`ffprobe -loglevel quiet -show_streams -i $1 | grep -m 1 width | cut -d '=' -f 2`;
	height=`ffprobe -loglevel quiet -show_streams -i $1 | grep -m 1 height | cut -d '=' -f 2`;
fi


srate=`mediainfo $1 | grep -m 1 fps | cut -d ':' -f 2 | cut -d ' ' -f 2`;
rrate=`echo "$srate" | awk '{printf "%.0f \n", $1}'`;

#manchmal sind die Videoabmessungen anders, als es abgespielt wird; das muss geprüft werden
war=`mediainfo $1 | grep -m 1 aspect | cut -d ':' -f 2 | cut -d ' ' -f 2`;
har=`mediainfo $1 | grep -m 1 aspect | cut -d ':' -f 3`;
ar=`echo "$war $har" | awk '{printf "%.2f \n", ($1/$2)}' | cut -d ' ' -f 1`;
arprak=`echo "$width $height" | awk '{printf "%.2f \n", ($1/$2)}' | cut -d ' ' -f 1`;
if [[ $(echo "if ($ar != $arprak) 1 else 0" | bc) -eq 1 ]]
then
	echo "Laut Metainformationen im Video wird dieses im Seitenverhältnis "$ar" abgespielt!";
	echo "Das Video hat aber prinzipiell ein Seitenverhältnis von "$arprak".";
	echo "Häufig verzerrt der Videoplayer ein Video beim Abspielen absichtlich,";
	echo "um das in den Metainformationen gespeicherte Seitenverhältnis anzuzeigen.";
	echo "Soll das Video mit dem Seitenverhältnis "$war":"$har"="$ar" weiterverarbeitet werden (1),";
	echo "oder soll das originale Seitenverhältnis "$arprak" verwendet werden (2)? Bitte wählen!";
	read confirmation1;
	if [[ $confirmation1 == "1" ]]
	then
		acceptar=true;
	elif [[ $confirmation1 == "2" ]]
	then
		acceptar=false;
	else
		echo "Ungültige Eingabe! - Bitte das Skript neu starten!";
		exit 4;
	fi
fi


#erstelle Ordner für Metadaten
mkdir -p meta;


#falls sehr viele Frames erstellt werden, lieber nochmal nachfragen; nicht dass die Festplatte volläuft...
echo "Berechne die voraussichtliche Anzahl der Einzelbilder!";
frameprop=`ffprobe -v error -count_frames -select_streams v:0 -show_entries stream=nb_read_frames -of default=nokey=1:noprint_wrappers=1 $1`;

if [[ $frameprop -gt 1000 ]]
then
	echo "Soll das Video wirklich zerlegt werden?";
	echo "Dadurch entstehen "$frameprop" Einzelbilder!";
	echo "Es besteht bei einer so großen Anzahl die Gefahr, dass die Festplatte vollläuft!";
	echo "Tipp: Auf keinen Fall die Option 'force-tiff' verwenden!";
	echo "Fortfahren? (yes/no)";
	read confirmation2;
	if [[ $confirmation2 == "no" ]]
	then
		echo "Dann brechen wir lieber ab...";
		exit 4;
	elif [[ $confirmation2 != "no" ]] && [[ $confirmation2 != "yes" ]] 
	then
		echo "Ungültige Eingabe! - Bitte Skript erneut starten!";
		exit 4;
	fi
	echo "Sollen die Videoframes als TIFF-Dateien (1) oder als JPG-Dateien (2) gespeichert werden)?";
	read confirmation7;
	if [[ $confirmation7 == "1" ]]
	then
		extractAsTiff=true;
	elif [[ $confirmation7 == "2" ]] 
	then
		extractAsTiff=false;
	else
		echo "Ungültige Eingabe! - Bitte Skript erneut starten!";
		exit 4;
	fi
else
	#extractAsTiff standardmäßig auf false setzen
	extractAsTiff=false;
fi


#Video zerlegen
if [[ $extractAsTiff == true ]]
then
	if [[ $acceptar == false ]]
	then
		if [[ $verbosity == true ]]
		then
			ffmpeg -an -i $1 -s "$width"x"$height" -f image2 ./meta/meta%d.tiff;
		else
			ffmpeg -loglevel quiet -an -i $1 -s "$width"x"$height" -f image2 ./meta/meta%d.tiff;
		fi
	else
	#Annahme: Höhe ist fix, Video wird in der Breite gestreckt
		if [[ $verbosity == true ]]
		then
			ffmpeg -an -i $1 -s "$((`echo "$ar $height" | awk '{printf "%.0f \n", ($1*$2)}'`))"x"$height" -f image2 ./meta/meta%d.tiff;
		else
			ffmpeg -loglevel quiet -an -i $1 -s "$((`echo "$ar $height" | awk '{printf "%.0f \n", ($1*$2)}'`))"x"$height" -f image2 ./meta/meta%d.tiff;
		fi
	fi
else
	if [[ $acceptar == false ]]
	then
		if [[ $verbosity == true ]]
		then
			ffmpeg -an -i $1 -s "$width"x"$height" -f image2 ./meta/meta%d.jpg;
		else
			ffmpeg -loglevel quiet -an -i $1 -s "$width"x"$height" -f image2 ./meta/meta%d.jpg;
		fi
	else
	#Annahme: Höhe ist fix, Video wird in der Breite gestreckt
		if [[ $verbosity == true ]]
		then
			ffmpeg -an -i $1 -s "$((`echo "$ar $height" | awk '{printf "%.0f \n", ($1*$2)}'`))"x"$height" -f image2 ./meta/meta%d.jpg;
		else
			ffmpeg -loglevel quiet -an -i $1 -s "$((`echo "$ar $height" | awk '{printf "%.0f \n", ($1*$2)}'`))"x"$height" -f image2 ./meta/meta%d.jpg;
		fi
	fi
fi


#Anzahl der erstellen Einzelbilder herausfinden
if [[ $extractAsTiff == true ]]
then
	frameCount=`find ./meta/ -type f -iname "*.tiff" | wc -l`;
else
	frameCount=`find ./meta/ -type f -iname "*.jpg" | wc -l`;
fi




echo "Schritt 2: Zerlege nun die Einzelbilder in Teilbilder!"


#erstelle für jedes Teilvideo einen eigenen Unterordner, auch für jeden kommenden Thread
for (( ab=1; ab <= $numThreads; ab++ ))
do
	for (( a=1; a <= $numH; a++ ))
	do
		for (( b=1; b <= $numB; b++ ))
		do
			#        ..."Höhe v. oben"_"Breite v. links"
			mkdir -p ./meta/$ab/files_"$a"_"$b";
		done
	done
done


#und jetzt die Qt-Application aufrufen
#prüfen, ob die Application auch wirklich da ist
if [ ! -f "./VideoSplitter" ]
then
	echo "Die notwendige Anwendung \"VideoSplitter\" befindet sich nicht im Ordner! - Bitte erneut versuchen!";
	exit 4;
fi
#Syntax: numImages numThreads "callString for dissipateFrames.sh"
./VideoSplitter $frameCount $numThreads ./dissipateFrames.sh $numB $numH $proportion $cutTypeB $cutTypeH $margin $forcetiff $extractAsTiff #Rest ergänzt 'VideoSplitter'


echo "Multithreading beendet!";



echo "Schritt 3: Führe die Dateien der einzelnen Threads zusammen!"

#prüfen, ob was schiefgelaufen ist
#am besten anhand meta/1/files_1_1, diesen Ordner muss es ja geben
test=`find meta/1/files_1_1/ -empty`;
if [[ $test == "meta/1/files_1_1/" ]]
then
	echo "Es befinden sich keine Dateien im Ausgabeordner! Das Skript wird daher beendet!";
	exit 4;
fi


for (( a=1; a <= $numH; a++ ))
do
	for (( b=1; b <= $numB; b++ ))
	do
		#        ..."Höhe v. oben"_"Breite v. links"
		mkdir -p ./meta/files_"$a"_"$b";
	done
done


for (( cd=1; cd <= $numThreads; cd++ ))
do
	for (( j=1; j <= $(($numB*$numH)); j++ ))
	do
		if [ $(($j%$numB)) != 0 ]
		then
			nb=$(($j%$numB));
		else
			nb=$numB;
		fi
		
		nh=$(($((`echo "$(($j-1)) $numB" | awk '{printf "%.6f \n", ($1/$2)}' | cut -d '.' -f 1`))+1));
	
		if [[ $forcetiff == false ]]
		then
			mv ./meta/$cd/files_"$nh"_"$nb"/*.jpg ./meta/files_"$nh"_"$nb"/
		else
			mv ./meta/$cd/files_"$nh"_"$nb"/*.tiff ./meta/files_"$nh"_"$nb"/
		fi
	done
done


echo "Schritt 4: Erstelle nun die Einzelvideos!"

mkdir -p finalv;

c=0;

#nun das Video erstellen
for (( a=1; a <= $numH; a++))
do
	for (( b=1; b <= $numB; b++ ))
	do
		c=$(($c+1));
		echo "Fortschritt: "$c"/"$(($numB*$numH));
		#wenn forcetiff == false, dann nicht zu jpg konvertieren
		if [ $forcetiff == false ]
		then
			if [[ $verbosity == true ]]
			then
				ffmpeg -r $srate -f image2 -i ./meta/files_"$a"_"$b"/%d.jpg -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" ./finalv/finalvideo_"$a"_"$b".$type;
			else
				ffmpeg -loglevel quiet -r $srate -f image2 -i ./meta/files_"$a"_"$b"/%d.jpg -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" ./finalv/finalvideo_"$a"_"$b".$type;
			fi
		else
			if [[ $verbosity == true ]]
			then
				ffmpeg -r $srate -f image2 -i ./meta/files_"$a"_"$b"/%d.tiff -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" ./finalv/finalvideo_"$a"_"$b".$type;
			else
				ffmpeg -loglevel quiet -r $srate -f image2 -i ./meta/files_"$a"_"$b"/%d.tiff -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" ./finalv/finalvideo_"$a"_"$b".$type;
			fi
		fi
		

	done
done


rm -r meta;


echo "FERTIG!";