;	Drush Make file for Isotope.js

api = 2
core = 7.x

; Dependencies
projects[libraries][download][type] = 'git'
projects[libraries][download][url] = 'http://git.drupal.org/project/libraries.git'
projects[libraries][download][version] = '1.0'

; Libraries
libraries[isotope][download][type] = 'git'
libraries[isotope][download][url] = 'http://github.com/desandro/isotope.git'
libraries[isotope][download][version] = 'v1.5.18'
