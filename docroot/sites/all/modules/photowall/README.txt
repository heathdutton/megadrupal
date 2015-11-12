--- README  -------------------------------------------------------------

Photowall

Provides a Photowall format for displaying Image field, using the JQuery Photowall plugin.

The Photowall plugin was originally developed from https://github.com/creotiv


--- INSTALLATION --------------------------------------------------------

1 - Extract the module into /sites/all/modules directory.

2 - Download the zip containing JQuery Photowall plugin here : https://github.com/tanmayk/jquery-photowall

3 - Put jquery-photowall folder into /sites/all/libraries folder. Photowall plugin file should be located at
    /sites/all/libraries/jquery-photowall/jquery-photowall.js.

--- USAGE ---------------------------------------------------------------

1 - Enable Photowall at /admin/modules, (Image, File, Field, Field SQL storage, Libraries required as well)

2 - Create or edit a content type at /admin/structure/types and include an Image field. Edit this image field to make it so that multiple image files may be added ("Number of values" setting at admin/structure/types/manage/{content type}/fields/{field_image/field_media}).

3 - Go to "Manage display" for your content type (/admin/structure/types/manage/{content type}/display) and switch the format of your multiple image field from Image to Photowall.

4 - Click the settings wheel in the slideshow-formatted multiple image/media field to edit advanced settings.

5 - Save! and here you go.


--- AVAILABLE OPTIONS ---------------------------------------------------

Zoom Factor : Set zoom factor between 1.3 to 1.6 for better results.


Written by: Tanmay Khedekar
