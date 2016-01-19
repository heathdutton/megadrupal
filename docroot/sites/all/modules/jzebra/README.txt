What does jZebra do?
--------------------
This module is a wrapper that conveniently integrates the jZebra project's code
into your Drupal website, enabling users to easily print ZPL/EPL documents. The
actual jZebra code, which includes the Java applet and JavaScript controllers,
is licensed and distributed independently of this module. More details about the
jZebra project are available at its homepage: http://code.google.com/p/jzebra/

Note that jZebra is *not* a generic web print backend: it is designed with the
intent of sending raw ZPL/EPL document data to a thermal barcode printer.


Installation
------------
As mentioned earlier, this module is a wrapper which integrates the jZebra code
into a Drupal installation. Thus, installation is a two-part process (the Drupal
jZebra module and the jZebra library).

1. Extract the module files and copy them to the site/all/modules folder in
   your Drupal installation.

2. Download the latest version of jZebra here:
   http://code.google.com/p/jzebra/downloads/list

   Extract the jZebra library to sites/all/libraries/jzebra (you may need to
   create this folder if it does not exist). When finished, the 'zebra.jar' and
   related files should reside in the sites/all/libraries/jzebra folder.

3. Visit "Administer > Site Building > Modules" and enable the jZebra module.


Configuration
-------------
When printing a document, jZebra will automatically retrieve a list of computers
connected to the user's machine and let the user select which printer to use.

It is recommended that users setup a printer device that uses the RAW driver so
that jZebra can send the print commands directly to the printer. The following
tutorials on how to do so are available at the jZebra project website:
* Windows XP: http://code.google.com/p/jzebra/wiki/TutorialRawXP
* Mac OS X: http://code.google.com/p/jzebra/wiki/TutorialRawOSX
* Linux (Ubuntu): http://code.google.com/p/jzebra/wiki/TutorialRawUbuntu


Sending a document to the printer
---------------------------------
After configuring your printer, sending a document is very straightforward.
Simply save a ZPL/EPL file somewhere in your Drupal filesystem (either public or
private) and then point your browser to the following URL:
http://www.example.com/jzebra/scheme://path/to/file

If your file was stored as sites/default/files/some_barcode.zpl, then you would
use: http://www.example.com/jzebra/public://some_barcode.zpl

The private filesystem is also supported, and if your file is located at
sites/default/files/private/some_barcode.zpl then you would use the following:
http://www.example.com/jzebra/private://some_barcode.zpl


Caveats and known bugs
----------------------
* Users must have JavaScript enabled and a recent version of Java in order to
  print with jZebra. Java can be installed or updated at http://www.java.com.


Credits and Sponsors
--------------------
Development of this module was performed by Grindflow Management LLC,
http://www.grindflow.com.

Special thanks to the jZebra team for their excellent work on the jZebra
library!
