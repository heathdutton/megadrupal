OVERVIEW
--------

The Media Nivo Slider module provides Media Gallery integration for the Nivo Slider jQuery plugin (http://nivo.dev7studios.com/).
This module provides the ability to take any media gallery and expose it's images as a Nivo Slider slideshow which is contained in a Drupal block.

These Nivo Slider blocks allow you to easily add a slideshow to various pages and in various regions of your site. 

The Nivo Slider blocks are individually customizable so each of your slideshows can have different transition effects, speeds, etc. 
In addition, since the Nivo Slider blocks are sourced from a media gallery, the ordering of images in the slideshow can easily be changed by simply re-ordering the media in the gallery.

This also allows for strictly curated slideshows and the ability to easily have the same image be a part of multiple slideshows if desired.

INSTALLATION
------------

1. Download and unpack the Media Nivo Slider module into your modules directory.

2. Download and unpack the module dependencies.
  a. Media Gallery Module (7.x-1.0-beta7) [http://drupal.org/project/media_gallery] - Download the media gallery module and its dependencies and unpack it into you modules directory.
  b. Libraries Module (7.x-1.0) [http://drupal.org/project/libraries] - Download the libraries module and unpack it into your modules directory.
  c. Nivo Slider jQuery plugin (2.7.1) [http://nivo.dev7studios.com/] - Download the Nivo Slider jQuery plugin and unpack it to your libraries folder, typically 'sites/all/libraries'. If you have Drush installed on your site you can use the following command to have Drush download and unpack the Nivo Slider plugin for you: 'media-nivo-slider-plugin'

3. Enable the modules.
   a. Visit your site's Administration > Modules page.
   b. Enable Media Nivo Slider. This will automatically enable all required modules.

GETTING STARTED
---------------

To get started with the Media Nivo Slider module you'll need to create a media gallery that will be the source of your slider images:

1. Go to Add content, and create a gallery.
2. When creating the gallery expand the 'Media Nivo Slider' fieldset and check the box to enable a nivo slider for this gallery.
3. Select 'default' from the 'Nivo Slider Preset' dropdown. This will tell the slider to use the default nivo slider options.
4. The gallery will be empty by default, so use the "Add media" link to quickly upload a few images from your computer. 
5. The Nivo Slider will be follow the image ordering of the gallery so you can use drag-and-drop to rearrange the images to your liking.
6. Go to the Blocks administrative page.
7. Find and activate the Media Nivo Slider block for your gallery by adding it to a page region.
8. View the slider by navigating to a page that contains your block!

CREATING CONFIGURATION PRESETS
------------------------------

If you want to customize the Nivo Slider options you will need to create new configurations that can be selected via the 'Media Nivo SLider'
options when creating/editing a gallery node.

To create a new configuration do the following:
1. Navigate to the Media Nivo Slider admin page (admin/config/media/media-nivo-slider).
2. Click the 'Create a Nivo Slider Configuration' link.
3. Configure the Nivo Slider and presentation options to your liking and submit the form.
4. Clear the Drupal cache to rebuild the preset select options.

The newly created configuration will now be available for selection in the 'Media Nivo Slider' options when editing/creating a gallery node.

ADDING IMAGE CAPTIONS/LINKS
---------------------------

Captions and images can be added to the slides via two fields that have been added to the image entities. 'Media Nivo Slider Caption' and 'Media Nivo Slider Link'.
Both of these fields can be found by either editing an image entity directly or using the 'Edit Media' tab on the gallery page.

USING IMAGE STYLES
------------------

The Media Nivo Slider module leverages the image styles functionality to allow you to easily resize, crop, saturate, etc the images in the slider. 
You can configure which image style, if any, a slider should use via the 'Image Style' field under the 'Media Nivo Slider' fieldset on the gallery edit form.

ADDING NEW/CUSTOM THEMES
------------------------

You can add additional themes either from the web your own custom theme by following the theme structure in the standard Nivo Slider library download. All themes should reside within the themes folder of the downloaded library. When adding or removing themes, be sure to clear your Drupal cache to make sure the theme select field has an updated list of themes.

For some tips on writing custom Nivo Slider themes see: http://nivo.dev7studios.com/support/advanced-tutorials/creating-custom-themes-for-the-nivo-slider/

UPGRADING FROM v1
-----------------

If you have previously been using any of the 7.x-1 releases of the Media Nivo Slider module be sure to either visit update.php or run drush's
`updatedb` command after you have replaced the v1 module code with v2. Doing so will trigger an upgrade update that will transition your existing Media Nivo Slider blocks to using the new configuration preset format.

The upgrade update will take the settings for each of your existing Media Nivo Slider blocks and create a new configuration preset, after which it will assign the newly created configuration preset to the node. This should ensure that you experience no changes in the presentation of your existing Media Nivo Slider blocks.

Once the upgrade is complete you may want to edit, and/or consolidate the configuration presets if you were using the same settings across multiple Media Nivo Slider blocks. 

If you add or remove any configuration presets, be sure to clear the Drupal cache to ensure the 'Nivo Slider Preset' selection list is up to date.

KNOWN ISSUES
------------
1. You may encounter an error similar to the following when installing the module via drush:
  - Module media_nivo_slider cannot be enabled because it depends on media  (7.x-1.0-beta5) but 7.x-1.0-beta5 is available
  - Module media_nivo_slider cannot be enabled because it depends on multiform  (1.0-beta2) but 7.x-1.0-beta2 is available
  This is due to the version dependencies set in the media_gallery.info file. See http://drupal.org/node/1180672 for more information.
  The current work around is to use the Drupal UI to enable the module.
