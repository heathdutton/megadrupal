INSTRUCTIONS
-------------
Download the module.
Download the jspdf javascript library (https://github.com/MrRio/jsPDF).
Place it under sites/all/libraries/jspdf so that the file can be found under sites/all/libraries/jspdf/dist/jspdf.min.js.
Enable the module.


This module does nothing by itself, it just provides the jspdf library so that other modules can use it.

You can load the library like this:
<?php
libraries_load('jspdf');
?>