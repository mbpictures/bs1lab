# bs1lab
Das Repository dient der gemeinsamen Nutzung und Weiterentwicklung eines eigenen Filesystems (MyFS).
Ein Laufwerk wird mittels "Image" (in der BlockDevice-Klasse) simuliert.

## ToDo's:
 * Organisation des FS planen
 * Dateien in generiertes Image schreiben
 * Image mit MyFS formatieren und mounten
 * Methoden zum Lesen/Schreiben (im Code mit "TODO" markiert) implementieren
 * Testen

Näheres unter [Projects](https://github.com/mbpictures/bs1lab/projects). 
Eine Doc zu allen Methoden [hier](https://libfuse.github.io/doxygen/structfuse__operations.html)

## Entwurf
Um das Lesen und schreiben möglichst abstrakt zu handhaben erstellen wir zunächst einige Klassen und Structs.
Die Klasse Superblock beeinhaltet informationen über das Dateisystem (Anzahl an Blöcke (a 512 Byte), Position der DMAP, FAT). In der Klasse FAT befindet sich ein Array entries[], welches vom Typ int_16 ist. Der Array-Eintrag ist die Adresse des nächsten Speicherblocks. Gibt es keinen nächsten Speicherblock ist entries[i] null. Root-Verzeichnis ist die Klasse, in der alle Dateien und die dazugehörigen Informationen gespeichert: Dateiname (8 Byte), Dateigröße (2 Byte (FAT16)), BenutzerID (1Byte) GruppenID (1 Byte), Zugriffsrechte (3Bit + 3Bit + 3Bit),Zeitstempel (letzter Zugriff (atime), letzte Änderung (mtime), letzte Statusänderung (ctime): 3x4Byte) und den Zeiger auf den ersten Block (2 Byte für 65.536 Blöcke = 32 MiB). Das macht 18 Byte pro Eintrag, 131072 Byte für alle Einträge, was schon 256 Blöcke entspricht.
