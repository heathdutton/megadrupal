INSTRUCTIONS
-------------
Download the module.
Download the html2canvas javascript library (https://github.com/niklasvh/html2canvas/releases).
Place it under sites/all/libraries/html2canvas so that the file can be found under sites/all/libraries/html2canvas/html2canvas.js.
Enable the module.


This module does nothing by itself, it just provides the html2canvas library so that other modules can use it.

You can load the library like this:
<?php
libraries_load('html2canvas');
?>