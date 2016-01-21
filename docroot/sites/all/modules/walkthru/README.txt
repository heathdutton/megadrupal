Requirements
=============
Libraries http://drupal.org/project/libraries
My fork of the Guiders-JS library https://github.com/redndahead/Guiders-JS

Installation
============

1) Download Libraries module to sites/all/modules
2) Create a libraries directory in sites/all so you have sites/all/libraries
3) Make a guiders-redndahead directory in the libraries directory
4) Download my fork of the Guiders-JS library
5) Extract guiders-1.2.0.js, guiders-1.2.0.css into sites/all/libraries/guiders-redndahead
6) Rename guiders-1.2.0.js to guiders.js
7) Rename guiders-1.2.0.css to guiders.css
8) Download/Install this module into sites/all/modules

Usage
=====

1) Go to admin/structure/walkthru
2) Create your first walkthru
3) Click List next to the walkthru you created
4) Add your steps

You can then add a link on your website to start your walkthru. The class of the
link needs to be "walkthru-link" and the name of the link needs to be the
machine name of your walkthru.

ex. <a href="#" class="walkthru-link" name="first-walkthru">First Walkthru</a>
