-- SUMMARY --

DrawingField provides a field type to create HTML5 based drawing.

 Features:
 
 * Basic Drawing tools
 * Exporting drawings to PNG
 * Saving and loading JSON
 * Constant-size or infinite canvases

-- REQUIREMENTS --

* Libraries API http://drupal.org/project/libraries
* Jquery Update http://drupal.org/project/jquery_update

-- BROWSER SUPPORT --
IE10+, Firefox 3+, Chrome, Safari 1.4+ and Opera 9.5+

-- INSTALLATION --

 * Download literallycanvas API from:
   https://www.versioneye.com/javascript/literallycanvas:literallycanvas/0.4.0
   extract contents to libraries/literallycanvas directory.
 * Download REACT.js library from:
   http://cdnjs.cloudflare.com/ajax/libs/react/0.10.0/react-with-addons.js
   and rename as react.js, add to libraries/reactjs directory.
 * Install and enable Libraries API module.
 * Install and enable Jquery Update module and configure it with jquery1.7+
   version.
 * Install and enable DrawingField module.
 * Create a content type, entity form with a drawingfield widget.
 * Customize the canvas background color, width and height the
   drawingfield instance to your needs
 * Start creating drawings

-- CONFIGURATION --

* Create a content type, Entityform with a drawingfield and
  configure settings per field.

-- CONTACT --

Current maintainers:
* Mohd Rashid (rashid_786) - http://www.drupal.org/user/679418
