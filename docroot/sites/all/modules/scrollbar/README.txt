*Scrollbar Module*

About:
------

Scrollbar is a very simple Drupal module to implement the jScrollPane javascript
functionality to your Drupal and make the css selectors get a custom jquery 
scrollbar.
jScrollPane is a cross-browser jQuery plugin by Kelvin Luck 
(http://jscrollpane.kelvinluck.com) which converts a browser's default 
scrollbars (on elements with a relevant overflow property) into an HTML 
structure which can be easily skinned with CSS.

Installation:
-------------

A) Donwload the module and extract it to the modules directory.
B) Go to the jScrollPane download page at 
http://jscrollpane.kelvinluck.com/index.html#download

UPDATE: You can download all the required and complementary files from my 
github repo at https://github.com/tplcom/jscrollpane/archive/master.zip 
and upload them into libraries/jscrollpane folder (see below).

[Required files]

1. Download jquery.jscrollpane.min.js and place it into 
libraries/jscrollpane folder.
2. Download jquery.jscrollpane.css and place it into 
libraries/jscrollpane folder.

[Optional files]

3. Download jquery.mousewheel.js and place it into 
libraries/jscrollpane folder.
4. Download mwheelIntent.js and place it into 
libraries/jscrollpane folder.

C) On your theme css add one or more styles for the element you want to get the 
custom jquery scrollbar.
For example, if you want to apply the .jScrollPane() function to the 
.field-name-body element just add this CSS

.field-name-body {
  height: 200px;
  overflow: auto;
}

For more examples please refer to 
http://jscrollpane.kelvinluck.com/index.html#examples

Configuration:
--------------

A) Go to admin/config/user-interface/scrollbar and configure as you want.
For more information on how to use the jScrollPane() parameters please refer to
the jScrollPage settings page (http://jscrollpane.kelvinluck.com/settings.html).

Credits:
--------

Many thanks to Kelvin Luck (http://kelvinluck.com) for this excellant 
jquery plugin.
