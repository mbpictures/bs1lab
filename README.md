# bs1lab
## Info
This fs was developed as a university project. Doc in German, Comments in English.


Das Repository dient der gemeinsamen Nutzung und Weiterentwicklung eines eigenen Filesystems (MyFS).
Ein Laufwerk wird mittels "Image" (in der BlockDevice-Klasse) simuliert. [Anleitung zur Installation](#installation-und-testen)

## ToDo's:
 * Organisation des FS planen
 * Dateien in generiertes Image schreiben
 * Image mit MyFS formatieren und mounten
 * Methoden zum Lesen/Schreiben (im Code mit "TODO" markiert) implementieren
 * Testen
 * Ausführlich dokumentieren

Näheres unter [Projects](https://github.com/mbpictures/bs1lab/projects). <br>
Eine Doc zu allen Methoden [hier](https://libfuse.github.io/doxygen/structfuse__operations.html)

## Entwurf
Um das Lesen und schreiben möglichst abstrakt zu handhaben erstellen wir zunächst einige Klassen und Strukturen.
Die Klasse Superblock beeinhaltet Informationen über das Dateisystem (Anzahl an Blöcke (a 512 Byte), DMAP, FAT und die Adresse des ersten Datenblocks). Die FAT ist ein Array entries[], welches vom Typ u_int_16 ist. Der Array-Eintrag ist die Adresse des nächsten Speicherblocks. Gibt es keinen nächsten Speicherblock ist entries[i] 0. Root-Verzeichnis ist die Klasse, in der alle Dateien und die dazugehörigen Informationen gespeichert: Dateiname (8 Byte), Dateigröße (2 Byte (FAT16)), BenutzerID (1Byte) GruppenID (1 Byte), Zugriffsrechte (3x4 Bit),Zeitstempel (letzter Zugriff (atime), letzte Änderung (mtime), letzte Statusänderung (ctime): 3x4Byte) und den Zeiger auf den ersten Block (2 Byte für 65.536 Blöcke = 32 MiB). Das macht 18 Byte pro Eintrag, 131072 Byte für alle Einträge, was schon 256 Blöcke entspricht.

## Definitionen
DMAP ist ein Boolean-Array. Freier Block = true, belegter Block = false.<br>
FAT ist ein u_int_16-Array. Kein Nachfolge-Block = 0, Nachfolge-Block = Adresse.<br>
Root-Verzeichnis ist Array aus Structs, welcher alle Informationen speichert.<br>
Fehler in Methode aufgetreten = 1, kein Fehler = 0.<br>
Der SBlock und das RootDirectory haben eine bestimmte Größe, sodass die Position des Root-Directorys und der Daten unverändert bleibt.

## Überlegungen
Attribute des SBlocks werden als Struct gespeichert. Beispielcode für die Serialisierung
```c++
MyStruct s;
char [] buffer = new char[sizeof(s)];
memcpy(&buffer, &s, sizeof(s));
```

Gemountete Datenträger werden in /etc/fstab eingetragen. Diese Datei wird immer beim Boot gelesen und bei Ausführung des *mount* Command. Typischer Eintrag:
```
# device-spec   mount-point     fs-type      options                                          dump pass
# mounting tmpfs
tmpfs           /mnt/tmpfschk   tmpfs        defaults                                           0 0
```

Das initialisieren größerer Datenstrukturen kann mit memset vereinfacht werden.
```c++
bool myArr[500];
memset(myArr, 1, 500);
```


## Interfaces
### Superblock
 * **int_16 findFreeBlock(void)**<br>
Finde den nächsten freien Block in der DMap und gib die Adresse dessen zurück.
 * **int_16 findNextBlock(int_16 address)**<br>
Hat der Angegebene Block einen nachfolge Block? Falls ja gib die Adresse zurück, falls nein gib -1 zurück.
 * **void setNextBlock(int_16 blockAddress, int_16 nextBlockAddress)**<br>
Setzt den nächsten Block eines Blocks. Um den nachfolgenden Block zu entfernen muss nextBlockAdress auf -1 gesetzt wird.
 * **void markBlock(int_16 address, boolean status)**<br>
Markiere Block als belegt (status = false) oder frei (status = true)
 * **void serialize(const char&ast; buffer)**<br>
Serialisiere den gesamten Superblock (bestehend aus DMAP, FAT und ggf. weiteren Attributen) sequentiell in den Buffer, welcher anschließend geschrieben werden kann.
 * **void deserialize(const char&ast; buffer)**<br>
Lese den Buffer ein und schreibe die einzelnen Elemente zurück in die Attribute des Superblocks.

### RootDirectory
 * **int addEntry(const char&ast; path, uint16_t firstBlock, uint32_t sizeOfFile, mode_t mode, uint8_t uid, uint8_t gid)**<br>
Füge neuen Eintrag in den ersten freien Index des entries-Arrays. Überprüfe, ob Filename noch nicht existiert, falls doch return -(ENTEXITST). Filename = path, Zeiten = aktuelle Urzeit, User/Group-ID = Parameter, Rechte = aus mode. Wenn in Array eingetragen, return 0.
 * **int removeEntry(const char&ast; path)** (write only)<br>
Suche und entferne den entry aus dem Array mit dem filename = path, dann return 0. Wenn er nicht gefunden wird return 1.
 * **int searchEntry(const char&ast; path, uint8_t uid, uint8_t gid)**<br>
Suche aus Array filename = path und return index des entsprechenden FileEntries. Wenn er nicht gefunden wird return -NOENTRY.
 * **FileEntry getEntry(int index)**<br>
Gib den FileEntry am enstsprechenden index zurück.
 * **void setFileSize(uint16_t)** (write only)
Setzt die Dateigröße auf die aktuell geschriebenen Bytes.
 * **void serialize(const char&ast; buffer)**<br>
Serialisiere das gesamte Entry-Array sequentiell in den Buffer, welcher anschließend geschrieben werden kann.
 * **void deserialize(const char&ast; buffer)**<br>
Lese den Buffer ein und schreibe die einzelnen Elemente als Struct-Array zurück in das Entry-Array.

## Aufgabe 1 - Read Only
## Forderungen
* Dateien sollen die üblichen Attribute unterstützen (Name, Größe, Rechte, Zeitstempel etc...)
* Es gibt nur ein Verzeichnis: Das Root-Verzeichnis
* Einbindung in den Verzeichnisbaum des Betriebssystems (/mnt/)
* Initiales Kopieren von Daten beim Erstellen des Dateisystems
* Lesen der initial kopierten Daten

## Hinweise
* Keine Unterverzeichnisse
* Beim Kopieren von Daten werden Pfadnamen entfernt
* Bei Duplikaten (gleicher Name) soll ein Fehler geworfen werden
* Zugriffsrechte sind immer Read-Only (444)

## Installation und Testen
Um dieses Projekt nutzen zu können wird eine Linux umgebung benötigt. Zunächst muss das Repository geklont werden. Anschließend müssen (falls noch nicht geschehen) folgende Pakete nachinstalliert werden: g++, gcc, fuse, libfuse-dev, make. Anschließend kann im Root Verzeichnis die Kommandos "make all" bzw "make mkfs.myfs" oder "make mount.myfs" ausgeführt werden. Um eine Container Datei zu erstellen wird folgendes Kommando benötigt: "./mkfs.myfs fs/container.bin DATEIEN_ZUM_KOPIEREN..." (erstellt die container.bin Datei im Verzeichnis fs des Repositories). Um das Dateisystem einzuhängen: "./mount.myfs fs/container.bin logfile mountpoint".
