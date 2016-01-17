Views jQFX: Supersized
----------------------------
This module is a Views jQFX addon that integrates the Supersized plugin with
views.

Dependencies:
Views, Views jQFX module, Libraries module and Supersized 3.2.7.

Supersized is available at:
http://www.buildinternet.com/project/supersized/download.html

Installation
----------------------------
1) If you have not already done so, get and install the Views jQFX module from
   the drupal project page.

2) Place this module in your modules directory and enable it at admin/modules.
   It will appear in the views section.

3) Download the Supersized 3.2.7 library using the url above. Extract it into
   /sites/all/libraries/supersized. Supersized comes with a couple of
   implementations, but only the "slideshow" folder is required.
   The path to the library should be:
   /sites/all/libraries/supersized/slideshow/js/supersized.3.2.7.js.

4) Create or edit your content type.
   Captions are created from the title tag attributes of the images in the
   content type you wish to display.
   If you want to display captions you must have the ability to add this
   attribute to you image fields.
   To enable image attributes navigate to admin->structure->content types
   Select 'manage fields' in the content type(s) that will be displayed in the
   Supersized (or create a new content type).
   Edit the image field that you want to display and be sure that the 'title'
   box is checked.
   This will provide a textfield for adding information to uploaded images when
   creating or editing a node.
   If this is a new content type an image field will need to be created for it.
   Creating an image style on /admin/config/media/image-styles is recommended
   to keep all images consistent, but not required.

5) Create the nodes that you wish to display under admin->content->add
   Upload ONE image per node and fill in the title tag for the caption.
   Add your image link in the link field.

6) Create a node view.
   If you are new to views you may want to find a tutorial for it.
   There are many out there.
   Only the images in the node view will be displayed in Supersized.
   Navigate to admin->structure->views->add
   Name your view (ie. jQFX Supersized).
   Check the 'Create a page' and/or 'Create a block' view
   with display format 'jQFX' of 'fields'.
   Enter the maximum number of items to display.
   Next to the fields, click 'add'.
   Check any image fields you wish to apply (ie. Content: Image)
   then click 'Add and configure fields'.
   Select Formatter 'Image', chose a style.
   Then click 'Apply all displays'.
   Add your filters, arguments, etc for the content.

7) Next to 'Format: jQFX' click 'settings'.
   Under jQFX type->FX settings, select 'Supersized'.
   This will open the fieldset for your Supersized settings options.
   Choose your settings. They should be pretty self explainatory.
   Then click 'Apply all displays' and save your view.

Theming notes
----------------------------
  The Supersized plugin version 3.2.7 (tested) comes with two
  prepackaged themes.
  They are Default and "Shutter"

TODO: The Full Supersized Options List


Please post any comments, questions, or bugs in the issues queue of the
Views jQFX: Supersized project page on Drupal:
http://drupal.org/sandbox/thechanceg/1358600
