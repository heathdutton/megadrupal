;	Drush Make file for views_isotope

api = 2
core = 7.x

; Isotope library.
libraries[isotope][download][type] = 'get'
libraries[isotope][download][url] = 'http://isotope.metafizzy.co/isotope.pkgd.min.js'
libraries[isotope][download][directory_name] = 'isotope'
libraries[isotope][download][destination] = 'libraries'

; ImagesLoaded library.
libraries[imagesLoaded][download][type] = 'get'
libraries[imagesLoaded][download][url] = 'http://imagesloaded.desandro.com/imagesloaded.pkgd.min.js'
libraries[imagesLoaded][download][directory_name] = 'imagesLoaded'
libraries[imagesLoaded][download][destination] = 'libraries'
