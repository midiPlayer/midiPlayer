#!/bin/bash
#Author: Jonas Vollhardt

#$1 -> Bild, das zerlegt werden soll
#$2 -> Seitenverhältnis der Einzelbilder
#$3 -> Anzahl der horizontalen Teile, in die das Bild zerlegt werden soll
#$4 -> Anzahl der vertikalen Teile, in die das Bild zerlegt werden soll
#$5 -> Dateiformat der Ausgabedateien; 'png' wenn nicht anders angegeben
#$6 -> "oben", "unten" oder "gleich": gibt an, ob das Bild - falls es in der Höhe zugeschnitten werden muss - nur oben, nur unten oder oben und unten gleichmäßig abgeschnitten wird
#$7 -> "links", "rechts" oder "gleich": gibt an, ob das Bild - falls es in der Breite zugeschnitten werden muss - nur links, nur rechts oder oben und unten gleichmäßig abgeschnitten wird
#$8 -> Freiraum zwischen den einzelnen Bildern
#$9 -> "no-questions" für keine interaktiven Nachfragen; optional
#$10 -> id, falls das Skript mehrfach parallel läuft; optional


#prüfen, ob $1 gesetzt und vorhanden ist
if [[ -z "$1" ]]
then
	echo "Es wurde keine Eingabedatei festgelegt! - Bitte erneut versuchen!";
	exit 4;
elif [[ ! -f "$1" ]]
then
	echo "Die Eingabedatei \""$1"\" konnte nicht gefunden werden! - Bitte erneut versuchen!";
	exit 4
fi


#prüfen, ob $2 gesetzt ist
#TODO: prüfen ob float
if [[ -z "$2" ]]
then
	echo "Es wurde kein Seitenverhältnis festgelegt! - Bitte erneut versuchen.";
	exit 4;
else
	proportion=$2;
fi


#prüfen, ob $3 gesetzt ist
if [[ -z "$3" ]]
then
	echo "Es wurde keine Anzahl für die horizontalen Teile festgelegt! - Bitte erneut versuchen.";
	exit 4;
else
	if  [[ "$3" -eq "$3" ]] 2>/dev/null
	then
		if [[ $3 -gt 0 ]]
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
if [[ -z "$4" ]]
then
	echo "Es wurde keine Anzahl für die vertikalen Teile festgelegt! - Bitte erneut versuchen.";
	exit 4;
else
	if  [[ "$4" -eq "$4" ]] 2>/dev/null
	then
		if [[ $4 -gt 0 ]]
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


#setze $5, falls noch nicht geschehen -> default: "png"
if [[ -z "$5" ]]
then
	type="png";
	echo "Benutze default-Wert 'png' für das Ausgabeformat!";
else
	if [[ $5 == "png" ]] || [[ $5 == "jpg" ]] || [[ $5 == "jpeg" ]] || [[ $5 == "svg" ]] || [[ $5 == "bmp" ]] || [[ $5 == "gif" ]] || [[ $5 == "tiff" ]]
	then
		type=$5;
	else
		echo "Das Dateiformat \""$5"\" ist ungültig oder wird nicht unterstützt! - Bitte erneut versuchen!";
		exit 4;
	fi
fi


#setze $6, falls noch nicht geschehen -> default: "gleich"
if [[ -z "$6" ]]
then
	cutTypeH="gleich";
	echo "Benutze default-Wert 'gleich' für Zuschnitt in der Höhe!";
else
	if [[ $6 == "oben" ]] || [[ $6 == "unten" ]] || [[ $6 == "gleich" ]] 
	then
		cutTypeH=$6;
	else
		cutTypeH="gleich";
		echo "Ungültiger Parameter für Zuschnitt in der Höhe, benutze default-Wert 'gleich'!";
	fi
fi


#setze $7, falls noch nicht geschehen -> default: "gleich"
if [[ -z "$7" ]]
then
	cutTypeB="gleich";
	echo "Benutze default-Wert 'gleich' für Zuschnitt in der Breite!";
else
	if [[ $7 == "links" ]] || [[ $7 == "rechts" ]] || [[ $7 == "gleich" ]] 
	then
		cutTypeB=$7;
	else
		cutTypeB="gleich";
		echo "Ungültiger Parameter für Zuschnitt in der Breite, benutze default-Wert 'gleich'!";
	fi
fi


#prüfe, ob $8 gesetzt und eine gültige Zahl ist -> default: "0"
if [[ -z "$8" ]]
then
	margin="0";
	echo "Benutze default-Wert '0' für Freiraum zwischen den Bildern!";
else
	if  [[ "$8" -eq "$8" ]] 2>/dev/null
	then
		if [[ $8 == 0 ]]
		then
			margin=$8;
		else
			if [[ $8 -gt 0 ]]
			then
				margin=$8;
			else
				echo "Der Freiraum zwischen den Einzelbildern darf nicht negativ sein! - Bitte erneut versuchen!";
				exit 4
			fi
		fi
	else
		echo "\""$8"\" ist weder eine natürliche Zahl noch null! - Bitte erneut versuchen!;";
		exit 4
	fi
fi


#prüfen, ob $9 korrekt gesetzt -> default: Nachfragen erlaubt
if [[ -z "$9" ]]
then
	questions=true;
elif [[ $9 == "no-questions" ]]
then
	questions=false;
elif [[ $9 == "questions" ]]
then
	questions=true;
else
	echo "Der Parameter \""$9"\" ist ungültig! - Bitte erneut versuchen!";
	exit 4;
fi


#prüfen, ob $10 gesetzt ist -> default: "1" (damit kann das Skript aber nicht mehrfach parallel laufen)
if [[ -z ${10} ]]
then
	pid=1;
elif [[ ${10} -gt 0 ]]
then
	pid=${10};
else
	echo "Der Parameter \""${10}"\" ist ungültig! - Bitte erneut versuchen!";
	exit 4;
fi


#lese die Höhe des Bildes aus
width=`identify ./$1 | cut -f 3 -d " " | sed s/x.*//`

#lese die Breite des Bildes aus
height=`identify ./$1 | cut -f 3 -d " " | sed s/.*x//`



#nachfragen, ob das Bild korrekt gedreht ist
if [[ $questions == true ]]
then
	svinput=`echo "$width $height" | awk '{printf "%.2f \n", ($1/$2)}'`;
	if [[ $(echo "if ($svinput >= 1.00) 1 else 0" | bc) -eq 1 ]]
	then
		 echo "Ist es korrekt, dass das es sich um ein Bild im Querformat handelt? (j/n)";
	elif [[ $(echo "if ($svinput < 1.00) 1 else 0" | bc) -eq 1 ]]
	then
		echo "Ist es korrekt, dass das es sich um ein Bild im Hochformat handelt? (j/n)";
	else
		echo "Fataler Fehler! - Bitte erneut versuchen!";
		exit 4;
	fi

	read confirmation1;

	if [[ $confirmation1 == "n" ]]
	then
		echo "Soll das Bild gedreht werden? (j/n)";
		read confirmation2;
		if [[ $confirmation2 == "j" ]]
		then
			echo "Möglichkeit 1: 90° nach rechts";
			echo "Möglichkeit 2: 90° nach links";
			echo "Bitte wählen! (1/2)";
			read confirmation3;
			if [[ $confirmation3 == "1" ]]
			then
				echo "Drehe das Bild um 90° nach rechts und mache weiter!";
				convert $1 -rotate 90 temp.$type;
				mv temp.$type $1;
			elif [[ $confirmation3 == "2" ]]
			then
				echo "Drehe das Bild um 90° nach links und mache weiter!";
				convert $1 -rotate -90 temp.$type;
				mv temp.$type $1;
			else
				echo "Ungültige Eingabe! - Bitte das Skript erneut starten!";
				exit 4;
			fi
		elif [[ $confirmation2 == "n" ]]
		then
			echo "Dann brechen wir lieber ab...";
			exit 4;
		else
			echo "Ungültige Eingabe! - Bitte das Skript erneut starten!";
			exit 4;
		fi
	elif [[ $confirmation1 != "j" ]] && [[ $confirmation1 != "n" ]]
	then
		echo "Ungültige Eingabe! - Bitte das Skript erneut starten!";
		exit 4;
	fi
fi


#prüfen, ob Freiraum klein genug, dass auch noch ein Bild übrigbleibt
if [[ 0 -gt $(($width-$(($(($numB-1))*$margin)))) ]] || [[ 0 -gt $(($height-$(($(($numH-1))*$margin)))) ]]
then
	echo "Der Freiraum zwischen den Einzelbildern ist so groß gewählt, dass es keine Bilder mehr gibt! - Bitte erneut versuchen!";
	exit 4;
fi


#theoretisches Seitenverhältnis der Einzelbilder ohne Zuschnitt des Originalbildes: SVtheor = [(B-nb)/anzB]/[(H-nh)/anzH]
#                      B-nb                           anzB                        H-nh             anzH
svTheor=`echo "$(($width-$(($(($numB-1))*$margin)))) $numB $(($height-$(($(($numH-1))*$margin)))) $numH" | awk '{printf "%.4f \n", ($1/$2)/($3/$4)}'`;




#finde heraus, ob das Bild, um das geforderte Seitenverhältnis zu erreichen, in der Höhe oder der Breite gekürzt werden muss
if [[ $(echo $svTheor'<'$proportion | bc -l) == 1 ]]
then
	
	echo "Die Höhe des Bildes muss reduziert werden.";

	
	#berechne die Pixelbreite eines Teilbildes: [Bges - (anzB-1)*margin]/anzB = Bteil
	#                        Bges         anzB-1    margin       anzB
	widthOfOnePart=`echo "$(($width-$(($(($numB-1))*$margin )))) $numB" | awk '{printf "%.6f \n", ($1/$2)}'`;

	
	#schließe auf die abzuschneidenden Pixel in der Höhe: Hges - (widthOfOnePart/proportion)*anzH - (anzH-1)*margin
 	pixToCutH=$(($height-$((`echo "$widthOfOnePart $proportion $numH" | awk '{printf "%.0f \n", (($1/$2)*$3)}'`))-$(($(($numH-1))*$margin))))
	
	
	echo "Es müssen zunächst vom Originalbild "$pixToCutH" Pixel in der Höhe abgeschnitten werden.";

	
	if (( $pixToCutH >= 0 ))
	then
		if [[ $cutTypeH == "gleich" ]]
		then
			#schneide das Bild in der Höhe zu, und zwar zu gleichen Teilen oben und unten
			echo "Das Originalbild wird zu gleichen Teilen in der Höhe abgeschnitten.";
			convert $1 -crop 100%x+0+$(($pixToCutH/2)) +repage out_"$pid"_1.$type;
			convert out_"$pid"_1.$type -gravity South -crop 100%x+0+$(($pixToCutH-($pixToCutH/2))) +repage out_"$pid"_2.$type;
			rm out_"$pid"_1.$type;
			
		else
			if [[ $cutTypeH == "oben" ]]
			then
				#schneide das Bild in der Höhe zu, und zwar nur oben
				echo "Das Originalbild wird in der Höhe nur oben abgeschnitten.";
				convert $1 -crop 100%x+0+$pixToCutH +repage out_"$pid"_2.$type;
			else
				#schneide das Bild in der Höhe zu, und zwar nur unten
				echo "Das Originalbild wird in der Höhe nur unten abgeschnitten.";
				convert $1 -gravity South -crop 100%x+0+$pixToCutH +repage out_"$pid"_2.$type;
			fi
		fi
	else
		echo "Es wird eine negative Pixelanzahl abgeschnitten! Dieser Fehler dürfte nicht auftreten - bitte erneut versuchen!";
		exit 4;
	fi
	
	
else

	echo "Die Breite des Bildes muss reduziert werden.";
	
	#berechne die Pixelhöhe eines Teilbildes: [Hges - (anzH-1)*margin]/anzH = Hteil
	#                          Hges         anzH-1    margin       anzH
	heightOfOnePart=`echo "$(($height-$(($(($numH-1))*$margin )))) $numH" | awk '{printf "%.6f \n", ($1/$2)}'`;
	

	
	#schließe auf die abzuschneidenden Pixel in der Breite: Bges - heightOfOnePart*proportion*anzB - (anzB-1)*margin
 	pixToCutB=$(($width-$((`echo "$heightOfOnePart $proportion $numB" | awk '{printf "%.0f \n", ($1*$2*$3)}'`))-$(($(($numB-1))*$margin))))
	
	
	echo "Es müssen zunächst vom Originalbild "$pixToCutB" Pixel in der Breite abgeschnitten werden.";
	
	if (($pixToCutB >= 0))
	then
		if [[ $cutTypeB == "gleich" ]]
		then
			#schneide das Bild in der Breite zu, und zwar zu gleichen Teilen links und rechts
			echo "Das Originalbild wird zu gleichen Teilen in der Breite zugeschnitten.";
			convert $1 -gravity West -crop 100%x+$(($pixToCutB/2)) +repage out_"$pid"_1.$type;
			convert out_"$pid"_1.$type -gravity East -crop 100%x+$(($pixToCutB-($pixToCutB/2))) +repage out_"$pid"_2.$type;
			rm out_"$pid"_1.$type;
		else
			if [[ $cutTypeB == "links" ]]
			then
				#schneide das Bild in der Breite zu, und zwar nur links
				echo "Das Originalbild wird in der Breite nur links zugeschnitten.";
				convert $1 -gravity West -crop 100%x+$pixToCutB +repage out_"$pid"_2.$type;
			else
				#schneide das Bild in der Breite zu, und zwar nur rechts
				echo "Das Originalbild wird in der Breite nur rechts zugeschnitten.";
				convert $1 -gravity East -crop 100%x+$pixToCutB +repage out_"$pid"_2.$type;
			fi
		fi
	else
		echo "Es wird eine negative Pixelanzahl abgeschnitten! Dieser Fehler dürfte nicht auftreten - bitte erneut versuchen!";
		exit 4;
	fi
	
	
fi


#erst noch Breite und Höhe der Einzelbilder berechnen (bisher ist nur eine der beiden ausgerechnet worden)
if [[ -z "$widthOfOnePart" ]] && [[ $heightOfOnePart > 0 ]]
then
	#die schon ausgerechnete Größe noch runden
	heightOfOnePart=`echo "$heightOfOnePart" | awk '{printf "%.0f \n", $1}'`;
	widthOfOnePart=`echo "$heightOfOnePart $proportion" | awk '{printf "%.0f \n", ($1*$2)}'`;
else
	if [[ -z "$heightOfOnePart" ]] && [[ $widthOfOnePart > 0 ]]
	then
		#die schon ausgerechnete Größe noch runden
		widthOfOnePart=`echo "$widthOfOnePart" | awk '{printf "%.0f \n", $1}'`;
		heightOfOnePart=`echo "$widthOfOnePart $proportion" | awk '{printf "%.0f \n", ($1/$2)}'`;
	else
		echo "Dieser Fehler ist fatal, und dürfte nicht passieren! - Bitte erneut versuchen!";
		exit 4;
	fi
fi


#jetzt muss das Bild gehackstückelt werden
#gehe vertikal, also von oben nach unten durch, und teile währenddessen die horizontalen Reihen von links nach rechts auf

mkdir -p finalp_"$pid";

for (( i=1; i <= $numH; ++i ))
do
	for (( l=1; l <= $numB; ++l ))
	do
		echo "Erzeuge Bild für Reihe "$i", Spalte "$l;
		ii=$(($i-1));
		ll=$(($l-1));
		anumH=$(($numH-$i))
		anumB=$(($numB-$l))

		#erst links und oben abschneiden
		convert out_"$pid"_2.$type -crop +"$(($(($ll*$widthOfOnePart))+$(($ll*$margin))))"+"$(($(($ii*$heightOfOnePart))+$(($ii*$margin))))" +repage meta-"$pid"-_"$i"_"$l".$type;
		#dann rechts und unten
		convert meta-"$pid"-_"$i"_"$l".$type -crop -"$(($(($anumB*$widthOfOnePart))+$(($anumB*$margin))))"-"$(($(($anumH*$heightOfOnePart))+$(($anumH*$margin))))" +repage ./finalp_"$pid"/final-"$pid"-_"$i"_"$l".$type;
		#meta-Datei löschen
		rm meta-"$pid"-_"$i"_"$l".$type;
	done
done


rm out_"$pid"_2.$type;
echo "lösche";


echo "FERTIG!";