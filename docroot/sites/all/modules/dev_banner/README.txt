
Development Banner
------------------
When developing Drupal sites, we are often switching between development 
versions and a production site. Since they normally look identical, it is easy 
to get confused as to which host we are on. This utility displays a small banner 
to indicate the host of a site, as a reminder to where you are.

A common development methodology is to create a number of versions of a site, 
which may include a production site, a staging site (a copy of the production 
site used for testing new features), a test site (for testing more radical 
changes), and a development site (where new modules and features are developed).

You may specify a host name to be associated with the devel, stage, and test 
types.

You may select which banners to use from five default sets, or add your own 
custom set of banners.

You may select a position on the screen for the banner, and configure it to stay 
in a fixed position if the screen is scrolled.


Installation
------------
Expand the archive and copy the dev_banner tree to sites/all/modules, and enable 
the module.

If you are updating from an earlier version, you may need to refresh your menu 
cache.


Configuration
-------------
Go to admin/settings/dev_banner to set configuration options:
  1) General settings:
       * click the checkbox to enable dev_banner; if not checked,
         the display of development banners will be inhibited.

       * select a placement position for the banner
     
  2) Select images: select a banner image set.
  
  3) URL mapping: for each server type, enter the host name from the URL of the 
  site. Don't enter the protocol part (http://) or a slash at the end,  i.e.,
  for the site "http://staging.mysite.com", you would enter "staging.mysite.com"

  If the site includes a port number, include it; for 
  "http://staging.anothersite.com:8080", you would enter 
  "staging.anothersite.com:8080"

  If the site's base URL is in a subdirectory, include it (but not the following 
  slash); for "http://www.testsite.com/stage", you would enter 
  "www.testsite.com/stage"


If you would like to always turn off dev_banner for a particular host (i.e. your 
production server), insert this line in the $conf array for the host in its 
settings.php file:

  'dev_banner_enabled' => 1,

Similarly, set it to zero for hosts that you will always want dev_banner to be 
enabled on. This step is not necessary, but will save you from having to update 
the "enable" configuration settings after importing a database from one host to 
another.


Adding your own custom banners
------------------------------
It's easy to add your own custom images. They should be PNG format (to allow for 
alpha transparency) and measure 72px by 72px. Create a subdirectory under your 
files directory called dev_banner, and create subdirectories for banner 
positions, and place your images there. They will automatically be detected when 
you refresh the configuration screen.

The files must follow the naming convention of 
dev_banner/custom/POSITION/custom_xxxx.png, where "xxxx" is the banner name 
(devel, stage, or test). The POSITION folder correstonds to the four placement 
positions:
  ne = upper right
  nw = upper left
  se = lower right
  sw = lower left

If you create the three banners for the set, they will be named like this:

  files/dev_banner/custom/ne/custom_devel.png
  files/dev_banner/custom/nw/custom_devel.png
  files/dev_banner/custom/se/custom_devel.png
  files/dev_banner/custom/sw/custom_devel.png
  files/dev_banner/custom/ne/custom_stage.png
  files/dev_banner/custom/nw/custom_stage.png
  files/dev_banner/custom/se/custom_stage.png
  files/dev_banner/custom/sw/custom_stage.png
  files/dev_banner/custom/ne/custom_test.png
  files/dev_banner/custom/nw/custom_test.png
  files/dev_banner/custom/se/custom_test.png
  files/dev_banner/custom/sw/custom_test.png


Customizing styles
------------------

There are two CSS files in the css subdirectory:
  dev_banner.css controls the look on regular site pages
  dev_banner.admin.css controls the look on the configuration page

You may override these styles, by adding them to you theme's local style sheet, 
or copying them to your theme directory and adding directives in your theme's 
.info file to include them. It is not advised to modify the style sheets in the 
module directory, as they might change with future versions.


Interactions with other modules
-------------------------------
Vertical Tabs
If you have the Vertical Tabs (http://drupal.org/project/vertical_tabs) module 
installed, the configuration form will be displayed in an easier to read fashion.

Administration Menu
If you are using the Administration Menu module 
(http://drupal.org/project/admin_menu) and displaying it at the top of the page, 
the left- (or right) most part of it will be obscured by Development Banner. 
Included in dev_banner.css is a directive to add a left or right offset to the 
admin menu (depending on position). You may override this setting in your 
theme's style sheet.

Admin module
If you are using the Admin module (http://drupal.org/project/admin), the 
default position for the trigger icon is upper left, which will be obscured by 
Development Banner. Use the Admin settings to select another trigger position, 
or place Development Banner to the right position.
