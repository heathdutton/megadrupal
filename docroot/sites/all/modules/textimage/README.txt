
Textimage adds text to image functionality using GD2 and Freetype, enabling
users to create crisp images on the fly for use as theme objects, headings or
limitless other possibilities.

This module requires clean URLs be enabled. Without clean URL support Textimage
is unable to cache images, which can cause serious performance problems.


Textimage was originally written by Fabiano Sant'Ana (wundo).
- http://chuva-inc.com

Co-maintained by:
- Stuart Clark (Deciphered) http://stuar.tc/lark
- Mondrake http://drupal.org/user/1307444



Features
--------

* Support for TrueType fonts and OpenType fonts.
* Rotate text at any angle.
* Automatic text wrapping when using maximum width.
* Configurable opacity in text color.
* Backgrounds:
  * Define a color or simply have a transparent background.
  * Use a pre-made image to integrate directly with your theme.
  * Use another Textimage preset to achieve a multi-layered image
    (see image above).
* CCK and Views formatter integration.
  * CCK Textfield widget support.
  * Email module widget support.
* Support for non-alphanumeric characters.


Requirements
------------

* GD2
* FreeType


Install instructions
--------------------

1. Make sure your server supports the needed PHP extensions, if you're in a
   shared environment you may need help from your host.

2. Upload fonts files you want to use for Textimages to the 'fonts' directory
   inside textimage directory. If you want you can change this directory to
   anywhere accessible by Drupal.


Usage
------------

1. via theme_textimage_image():

   Use the theme_textimage_image() function at the theme/module level with the
   following format:

   theme('textimage_image', array(
    'preset' => 'Preset',
    'text'   => 'Text',
    'additional_text' => array('Additional', 'Text'),
    'format'    => 'png',
    // Don't include the file extension!
    'file_path' => 'public://myimages/sub_folder/image-filename'
   ));

2. via Field/Views formatter:

   Select a Textimage preset in a text field display options.


3. via URL:

   Create an image with the URL in following format:
   /[files directory]/textimage/[Preset](/Additional/Text)/[Text].[extension]

   Notes:
   a) this method can only be used by users with the 'create textimages'
   permission. This is to prevent Anonymous users from creating random images.
   b) this method only works if the "Default download method" on the
   admin/config/media/file-system page is set to a local file system (in fact,
   to a local stream wrapper).

   If you need dynamically created Textimages, it is strongly advised you use
   one of the methods detailed above.


Updating
------------

* Always run update.php on your Drupal site after updating Textimage.
* Due to certain changes in the Textimage module, some of your presets may
  require alterations after updating.
