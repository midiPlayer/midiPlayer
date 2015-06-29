#include "mainwindow.h"
#include <QApplication>
/*
 * Dies ist ein Programm zur Steuerung der Lichttechnik.
 * Es ist eine vollständige Neuetwicklung auf der Basis des MidiPlayers, der erstmals für die Theateraufführung
 * "Ein Sommernachtstraum" (drg11/12 Hr. Krenz) im Februar 2015 von Jakob Braun programmiert wurde.
 * Entstanden ist es im Rahmen des Informatikunterrichts im Sommer 2015 (und dank Unterstützung durch
 * Jakob Braun).
 * Die Urheber sind im Einzelnen: Matthias Sauer, Torge Mewes, Dominique Stahl, Leon Mayrhofer, Jonas Vollhardt
 * und Jakob Braun.
 * Grundlegend ist dabei der zweiteilige Aufbau:
 * Auf der einen Seite gibt es den LightServer, der auf dem vorkonfigurierten Stand-PC gestartet wird.
 * Gegenstück ist die Benutzeroberfläche (GUI, hier LightClient), die von beinahe jedem Endgerät gestartet
 * werden kann.
 * Einschränkungen gibt es dabei hinsichtlich des Betriebssystems: Der Client ist momentan nur für Linux
 * und Android kompiliert, kann aber mithilfe des Qt-Creators (mit dem das Programm auch entwickelt wurde)
 * auch für jedes andere OS erstellt werden (beispielsweise Windows oder Apples iOS).
 * Der Client muss dabei im WLAN-Netz "light", Standardpasswort "ymca" (vom Server per AcessPoint ausgestrahlt)
 * angemeldet sein.
 * Server und Client kommunizieren hierbei über das sogenannte WebSocket-Protokoll:
 * Das WebSocket-Protokoll ist ein auf TCP basierendes Netzwerkprotokoll, das entworfen wurde, um eine
 * bidirektionale Verbindung zwischen einer Webanwendung und einem WebSocket-Server bzw. einem Webserver,
 * der auch WebSockets unterstützt, herzustellen.
 * (Anmerkung: Das light-Netz ist nur für die Kommunikation zwischen Server und Client gedacht, und hat deshalb
 * auch keinen Internetzugriff.)
 *
 *
 *
 *
 *
 *


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
