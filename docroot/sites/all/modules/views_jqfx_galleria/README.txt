Views jQFX: Galleria
----------------------------
This module is a Views jQFX addon that integrates the Galleria plugin with
views. The Views jQFX module is a dependency. Galleria is a highly themable,
versatile, and cusomizable javascript image gallery plugin.

Installation
----------------------------
1) If you have not already done so, get and install the
   Views jQFX module from the drupal project page.
2) Place this module in your modules directory and enable it at admin/modules.
   It will appear in the views section.
3) Download the Galleria plugin and place it in
   your sites/all/libraries directory.
   To get the plugin to go the project page (http://galleria.aino.se).
   The final directory structure should look like:

   '/sites/all/libraries/galleria/galleria.js' (Rename this file. See below.)
   '/sites/all/libraries/galleria/galleria.min.js' (Rename this file. See below.)
   '/sites/all/libraries/galleria/plugins/' (Your download may not come with plugins.)
   '/sites/all/libraries/galleria/themes/'
   '/sites/all/libraries/galleria/themes/classic/galleria.classic.js'
     etc...

   Note that the galleria plugin javascript files
   may have a version number and need to be renamed.
   Example: galleria-1.2.2.js would be renamed to galleria.js
   Example: galleria-1.2.2.min.js would be renamed to galleria.min.js
4) Create or edit your content type.
   Captions are created from the alt and title tag attributes
   of the images in the content type you wish to display.
   If you want to display captions you must have the ability
   to add these attributes to you image fields.
   To enable image attributes navigate to admin->structure->content types
   Select 'manage fields' in the content type(s)
   that will be displayed in the Galleria (or create a new content type).
   Edit the image field that you want to display
   and be sure that the 'alt' and 'title' boxes are checked.
   This will provide a textfield for adding information
   to uploaded images when creating or editing a node.
   If this is a new content type an image field will need to be created for it.
   You can add multiple image fields per node. Each image will become a slide.
5) Create the node and/or nodes that you wish to display
   under admin->content->add
6) Create a node view.
   If you are new to views you may want to find a tutorial for it.
   There are many out there.
   Only the images in the node view will be displayed in the Galleria.
   Navigate to admin->structure->views->add
   Name your view (ie. jQFX Galleria).
   Check the 'Create a page' and/or 'Create a block' view
   with display format 'jQFX' of 'fields'.
   Enter the maximum number of items to display. 30 is usually safe.
   I recommend not using the pager. Then hit 'Continue & edit'.
   Next to the fields, click 'add'.
   Check any image fields you wish to apply (ie. Content: Image)
   then click 'Add and configure fields'.
   Select Formatter 'Image', Image style 'thumbnail',
   and Link image to 'file'. Then click 'Apply all displays'.
   Galleria will load the full size images using the links.
   You do not need anything but thumbnail size images in your view.
   Add your filters, arguments, etc for the content.
7) Next to 'Format: jQFX' click 'settings'.
   Under jQFX type->FX settings, select 'Galleria'.
   This will open the fieldset for your Galleria settings options.
   Choose your settings. They should be pretty self explainatory.
   Then click 'Apply all displays' and save your view.

Notes:
----------------------------
These instructions will provide a very basic install.
Flexibility is provided by the power of views.
Combined with arguments, filters, panels, node overrides etc
the sky is the limit. Experiment.

For further information on Galleria options check out the project page.
http://galleria.aino.se/docs/1.2/

Adding themes
----------------------------
    Galleria 1.2 has only the classic theme prepackaged in the download.
    Place additional themes in the plugin themes directory.
    The directory structure should look like:
       '/sites/all/libraries/galleria/themes/dots/galleria.dots.js'
       '/sites/all/libraries/galleria/themes/fullscreen/galleria.fullscreen.js'
       '/sites/all/libraries/galleria/themes/miniml/galleria.miniml.js'
       '/sites/all/libraries/galleria/themes/twelve/galleria.twelve.js'

    Themes listed in the dropdown menu can be selected via that route.
    If you build your own custom theme you will need to specify the path.

Please post any comments, questions, or bugs in the issues queue of the Views
jQFX: Galleria project page on Drupal at
http://drupal.org/sandbox/jamesbenison/1076658
