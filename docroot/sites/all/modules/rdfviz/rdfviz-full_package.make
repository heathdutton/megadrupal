; Fetch required API libraries and modules
; DO NOT USE THIS.
; It attempts to download PEAR packages, but 
; A: Pear packages come with unwelcome version-numbered subfolders
; B: Downloading several things into the same folder like Pear wants to causes 
;    overwrites in drush make. So it just doesn't fit.
; See the rdfviz.make for a cheap work-around.

core = 6.x
api = 2

/ I want to download

;http://pear.php.net/package/PEAR
;http://pear.php.net/package/Image_GraphViz/

;You should end up with (at least) the required files 
;sites/all/libraries/pear/System.php
;sites/all/libraries/pear/Image/Graphviz.php

; Get PEAR (fail)
;libraries[pear][directory_name] = "pear"
;libraries[pear][destination] = "libraries"
;libraries[pear][download][type] = "get"
;libraries[pear][download][url] = "http://download.pear.php.net/package/PEAR-1.9.4.tgz"
; A direct download doesn't unpack itself flat. It adds a versioned subdir.
; And I can't find an option in drush-make for that.

; clone PEAR (awful)
; Checkout from github instead and avoid the package manager. 
; However this is huge.
;libraries[pear][directory_name] = "pear"
;libraries[pear][destination] = "libraries"
;libraries[pear][download][type] = "git"
;libraries[pear][download][url] = "https://github.com/pear/pear-core.git"
;libraries[pear][download][revision] = 'master'


; Get PEAR Image/Graphviz (fail)
;libraries[graphviz][destination] = "libraries"
;libraries[graphviz][directory_name] = "pear/Image"
;libraries[graphviz][download][type] = "get"
;libraries[graphviz][download][url] = "http://download.pear.php.net/package/Image_GraphViz-1.3.0.tgz"
