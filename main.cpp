#include "mainwindow.h"
#include <QApplication>
/*

 Dazu können verschiedene "Regler" als MIDI-Ausgang verwendet werden. Dadurch kann das Programm sowohl zur
 Steuerung eines Mischpults (per MIDI-Adapter) verwendet werden als auch zum Ansteuern von einer DMX Software wie z.B. qlc+.
 Für den DMX-Ausgang werden (neben dem Adapter, der im PC-Gehäuse verbaut sein sollte,) folgende libs benötigt: ola, libola0, libola-devel
Midiausgang wir hier jack-midi verwendet. Daher musst du vor dem Start des Programms den jack-Server starten. Am besten mit qjackcrl.
Falls dein Midiadapter nicht in jack eingebunden wird sondern in alsa-MIDI kannst du das Programm a2jmidid verwenden.
Prinzipiell wird die Steuerung durch scenen gestaltet. Jede Szene giebt eine Liste mit verwendeten Lampen an und dazu dieren Aktuelle Helligkeit.
 Die Methode zur Rückgabe der Helligkeit wir regelmäßig aufgerufen, so dass Szenen das licht auch dynamisch verändern können.
Szenen können so z.B. auch in Echtzeit auf Musik oder andere Eingabequellen (z.B midiregler um die Effekte zu beeinflussen) reagieren.

Alles Szenen müssen die Klasse Szene implementieren. Alle weiteren Szenen könenn als Beispielimplementationen verstanden werden.

main.c:
Hier wird eigentlich nur das mainwindow instanziiert.

 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
