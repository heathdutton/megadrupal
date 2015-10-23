LC3_Clean theme for Drupal
===========================


1. General info
----------------

This Drupal theme is developed by Creative Development for their Drupal
distribution, including the connector module for LiteCommerce 3 e-commerce
solution.

Features:
- Clean and smooth design
- Supports LC Connector module, styles module's pages and blocks correctly
- Built-in drop-down menu
- Sign-in form in a popup layer
- Optional Facebook and Twitter links in the footer
- 3 columns
- Tableless layout (a few places where tables remain for compatibility with
  older browsers)
- Compatible with IE6+, FF3.5+ and Chrome 10+


2. Usage
---------

It is highly recommended to customize the theme by creating a subtheme (see
http://drupal.org/node/225125), so that you would not lose your changes when
upgrading the theme to a newer version.

To use a subtheme, select it on the "Appearance" page (be sure to keep "Clean
LC3 theme" enabled).


3. Installation
----------------

1) Download the theme package.

2) Unpack it to the "sites/all/themes" subdirectory of your Drupal 7
installation.

3) The lc3_clean theme requires the libraries module be installed
(http://drupal.org/project/libraries).

You will now need to create a sites/all/libraries folder if you don't already
have the libraries module installed. The following javascript file then need to
be retrieved and saved to the sites/all/libraries folder:
   > http://jquery.malsup.com/block/#download  - save file as jquery.blockUI.js 

4) Log in as administrator to your Drupal site, go to the "Appearance" page,
enable and make "Clean LC3 theme" the default Drupal theme.


4. Configuration 
-----------------

1) Log in as administrator to your Drupal site, go to the "Appearance" page and
follow "Settings" link for "Clean LC3 theme".

2) If you have a Facebook page or Twitter account, you can specify their names
in the "Social links" section. Leave the fields empty if you don't have these
accounts.

3) Choose which elements (logo, slogan, menu, ...) are to be displayed.

4) Upload your logo and favicon images.

5) Save changes.

6) To use the drop-down menu for the primary links, create a menu with the
links, set all the menu items to "expanded", go to the "Structure" -> "Menus"
-> "Settings" tab and then select your menu in "Source for the Main links".


5. Contacting us 
-----------------

Please use the Drupal issue tracker for sending your feedback to us:
http://drupal.org/project/issues/


Thank you for using the "Clean LC3 theme"!

