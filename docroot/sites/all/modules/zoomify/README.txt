Installation:
=============
-/ Install and enable the Libraries API module (http://drupal.org/project/library).
-/ Enable the Zoomify module and at least one handler module (default this would be "Zoomify handler for Image").
-/ Configure the module at admin/config/zoomify
-/ Get the ZoomifyViewer.swf file, which is available from the Express package at http://www.zoomify.com/express.htm
-/ Add the file to your libraries folder at sites/<all or sitename>/libraries/zoomify, so it can be found at e.g. sites/all/libraries/zoomify/ZoomifyViewer.swf
-/ When using the "Zoomify handler for Image" module, set the field formatter for images you want to use the Zoomify Viewer for.
   Use "Image in Zoomify viewer" for adding the Zoomify viewer in place, or "Image linked to Zoomify viewer page" for having an image (with image style) linked to the Zoomify tab.

Optional:
If you want to use the Python backend, download the ZoomifyImage package from http://sourceforge.net/projects/zoomifyimage/
and unpack it in your libraries folder under the zoomify folder. So the structure becomes sites/<all or sitename>/libraries/zoomify/ZoomifyImage

Please refer to the module's page at http://drupal.org/project/zoomify for more instructions.