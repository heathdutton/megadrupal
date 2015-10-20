; Fetch required API libraries and modules

core = 7.x
api = 2

; http://pear.php.net/package/PEAR
; http://pear.php.net/package/Image_GraphViz/

; You should end up with (at least) the required files 
;└── sites
;    └── all
;        └── libraries
;            └── pear
;                ├── Image
;                │   └── GraphViz.php
;                └── System.php

; Get PEAR
; Get just the required Pear core files :-}
; ... I should just try getting the full package ...
; See rdfviz-full_package.make for an explanation why we can't get the real 
; package tarball.
; From https://github.com/pear/pear-core 2012-05
libraries[pear-pear][destination] = "libraries"
libraries[pear-pear][directory_name] = "pear"
libraries[pear-pear][download][type] = "get"
libraries[pear-pear][download][url] = "https://raw.github.com/pear/pear-core/master/PEAR.php"
libraries[pear-pear5][destination] = "libraries"
libraries[pear-pear5][directory_name] = "pear"
libraries[pear-pear5][download][type] = "get"
libraries[pear-pear5][download][url] = "https://raw.github.com/pear/pear-core/master/PEAR5.php"
libraries[pear-system][destination] = "libraries"
libraries[pear-system][directory_name] = "pear"
libraries[pear-system][download][type] = "get"
libraries[pear-system][download][url] = "https://raw.github.com/pear/pear-core/master/System.php"
libraries[pear-getopt][destination] = "libraries"
libraries[pear-getopt][directory_name] = "pear/Console"
libraries[pear-getopt][download][type] = "get"
libraries[pear-getopt][download][url] = "https://raw.github.com/pear/Console_Getopt/trunk/Console/Getopt.php"


; Get PEAR Image/Graphviz - just the required file.
libraries[graphviz][destination] = "libraries"
libraries[graphviz][directory_name] = "pear/Image"
libraries[graphviz][download][type] = "get"
libraries[graphviz][download][url] = "https://raw.github.com/pear/Image_GraphViz/tags/Image_GraphViz-1.3.0/Image/GraphViz.php"

; Get ARC2
libraries[arc2][destination] = "libraries"
libraries[arc2][directory_name] = "arc"
libraries[arc2][download][type] = "get"
libraries[arc2][download][url] = "http://github.com/semsol/arc2/tarball/master"
