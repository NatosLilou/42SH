Titre du projet:
42SH

Projet de 3ème année d'école d'ingénieur informatique (EPITA), qui consiste à créer un shell posix en respectant la SCL associée https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18.


Démarrage :
autoreconf --install
./configure
make

Usage :
./42sh [OPTION] [SCRIPT] [ARGUMENTS...]
./42sh -c [SCRIPT] instead of reading the script from a file, directly interpret the argument as a shell script

Fabriqué avec :
Vim (éditeur de texte)
C (language)
shell (language pour les testsuites)
Autoreconf (build system)

Auteurs :

Elena Villepreux
Thomas Galateau
Nathan Sue
Hugo Saint-Raymond
