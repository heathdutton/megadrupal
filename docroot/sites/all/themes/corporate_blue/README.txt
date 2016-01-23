WHAT IS Corporate Blue?
------------------------

Corporate blue theme is a super clean professional theme with a div based, fixed width, 3 column layout.

FEATURES
--------

 * Corporate Blue has an in build slideshow feature

 * To enable/configure slideshow do the following steps
   
   * Goto appearance/settings/corporate_blue.
   * Select Banner management and upload the slideshow images and click 'Save configuration'.

 * In Corporate Blue theme a fixed size can be set for content images so that content alignment won't be affected
 * To do this use following steps

   * Goto configuration -> image style (path: admin/config/media/image-styles) and see which all styles available.
   * You can add a new style using Add Style Option
     * Once you added the style goto structure -> content type(path admin/structure/types)
     * For required content type click manage display tab
       * For default image (path for content type article image : admin/structure/types/manage/article/display)
       * For teaser image (path for content type article teaser : admin/structure/types/manage/article/display/teaser)
     * click on image setting and select which image style you need and click save.
     * You can set this for both default image and teaser image

 * Now in content page how big image you uploaded only a fixed size image is displayed.
 
INSTALLATION
------------

 1. Download Corporate Blue from http://drupal.org/project/corporate_blue

 2. Unpack the downloaded files, take the folders and place them in your
    Drupal installation under one of the following locations:
      sites/all/themes
        making it available to the default Drupal site and to all Drupal sites
        in a multi-site configuration
      sites/default/themes
        making it available to only the default Drupal site
      sites/example.com/themes
        making it available to only the example.com site if there is a
        sites/example.com/settings.php configuration file

    Note: you will need to create the "themes" folder under "sites/all/"
    or "sites/default/".

FURTHER READING
---------------

Full documentation on using Corporate Blue:
  http://www.freedrupalthemes.net/docs/corporate_blue

Drupal theming documentation in the Theme Guide:
  http://drupal.org/theme-guide
  
Corporate Blue demo site
  http://d7.freedrupalthemes.net/t/corporate_blue
  
NOTE
----

* This theme looks good only when content with images are promoted to front page.

