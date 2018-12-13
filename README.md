# bs1lab
Das Repository dient der gemeinsamen Nutzung und Weiterentwicklung eines eigenen Filesystems (MyFS).
Ein Laufwerk wird mittels "Image" (in der BlockDevice-Klasse) simuliert.

## ToDo's:
 * Organisation des FS planen
 * Dateien in generiertes Image schreiben
 * Image mit MyFS formatieren und mounten
 * Methoden zum Lesen/Schreiben (im Code mit "TODO" markiert) implementieren
 * Testen

Näheres unter [Projects](https://github.com/mbpictures/bs1lab/projects). <br>
Eine Doc zu allen Methoden [hier](https://libfuse.github.io/doxygen/structfuse__operations.html)

## Entwurf
Um das Lesen und schreiben möglichst abstrakt zu handhaben erstellen wir zunächst einige Klassen und Structs.
Die Klasse Superblock beeinhaltet informationen über das Dateisystem (Anzahl an Blöcke (a 512 Byte), Position der DMAP, FAT). In der Klasse FAT befindet sich ein Array entries[], welches vom Typ int_16 ist. Der Array-Eintrag ist die Adresse des nächsten Speicherblocks. Gibt es keinen nächsten Speicherblock ist entries[i] null. Root-Verzeichnis ist die Klasse, in der alle Dateien und die dazugehörigen Informationen gespeichert: Dateiname (8 Byte), Dateigröße (2 Byte (FAT16)), BenutzerID (1Byte) GruppenID (1 Byte), Zugriffsrechte (3Bit + 3Bit + 3Bit),Zeitstempel (letzter Zugriff (atime), letzte Änderung (mtime), letzte Statusänderung (ctime): 3x4Byte) und den Zeiger auf den ersten Block (2 Byte für 65.536 Blöcke = 32 MiB). Das macht 18 Byte pro Eintrag, 131072 Byte für alle Einträge, was schon 256 Blöcke entspricht.

## Definitionen
DMAP ist ein boolean-Array. Freier Block = true, belegter Block = false.<br>
FAT ist ein int_16-Array. Kein Nachfolge-Block = -1, Nachfolge-Block = Adresse.<br>
Root-Verzeichnis ist Array aus Structs, welcher alle Informationen speichert.<br>
Fehler in Methode aufgetreten = 1, kein fehler = 0.<br>
Der SBlock und das RootDirectory haben eine bestimmte größe, sodass die Position des RootDirectorys und der Daten unverändert bleibt.

## Überlegungen
Attribute des SBlocks werden als struct gespeichert. Beispielcode für die serialisierung
```c++
MyStruct s;
char [] buffer = new char[sizeof(s)];
memcpy(&buffer, &s, sizeof(s));
```

## Interfaces
### Superblock
 * **int_16 findFreeBlock(void)**<br>
Finde den nächsten freien Block in der DMap und gib die Adresse dessen zurück.
 * **int_16 findNextBlock(int_16 adress)**<br>
Hat der Angegebene Block einen nachfolge Block? Falls ja gib die Adresse zurück, falls nein gib -1 zurück.
 * **void setNextBlock(int_16 blockAdress, int_16 nextBlockAdress)**<br>
Setzt den nächsten Block eines Blocks. Um den nachfolgenden Block zu entfernen muss nextBlockAdress auf -1 gesetzt wird.
 * **void markBlock(int_16 adress, boolean status)**<br>
Markiere Block als belegt (status = false) oder frei (status = true)
 * **void serialize(const char&ast; buffer)**<br>
Serialisiere den gesamten Superblock (bestehend aus DMAP, FAT und ggf. weiteren Attributen) sequentiell in den Buffer, welcher anschließend geschrieben werden kann.
 * **void deserialize(const char&ast; buffer)**<br>
Lese den Buffer ein und schreibe die einzelnen Elemente zurück in die Attribute des Superblocks.

### RootDirectory
 * **boolean addEntry(const char&ast; path, mode_t mode)**<br>
Füge neuen Eintrag in den ersten freien Index des entries-Arrays. Überprüfe, ob Filename noch nicht existiert, falls doch return 1. Filename = path, Zeiten = aktuelle Urzeit, User/Group-ID = aktueller Nutzer, Rechte = aus mode. Wenn in Array eingetragen, return 0.
 * **int removeEntry(const char&ast; path)**<br>
Suche und entferne den entry aus dem Array mit dem filename = path, dann return 0. Wenn er nicht gefunden wird return 1.
 * **void serialize(const char&ast; buffer)**<br>
Serialisiere das gesamte Entry-Array sequentiell in den Buffer, welcher anschließend geschrieben werden kann.
 * **void deserialize(const char&ast; buffer)**<br>
Lese den Buffer ein und schreibe die einzelnen Elemente als Struct-Array zurück in das Entry-Array.
