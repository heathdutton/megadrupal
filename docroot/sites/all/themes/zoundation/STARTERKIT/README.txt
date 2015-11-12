BUILD A THEME WITH ZOUNDATION
-----------------------------
This README.txt pulls heavily from the Zen theme README.txt (http://drupal.org/project/zen)

The base Zoundation theme is built to be extended using sub-themes. 
Do not modify any of the files in the zoundation/ directory. These files will be updated by the maintainers and editing them will affect the theme upgrade.

Instead, create a sub-theme using the STARTERKIT files outside of the zoundation/ directory.

The instructions below assume that the zoundation theme and sub-theme will be installed in sites/all/themes.

If you are running a multi-site installation of Drupal, you can create a themes folder under sites/my.site.folder and put themes there that are specific to a particular site in your installation. Themes that will be shared between all sites should be placed in sites/all/themes.

1. Create the sub-theme

Copy the STARTERKIT directory and place it outside of the zoundation/ directory. Place it in sites/all/themes.

Rename STARTERKIT directory to your new sub-theme. 
IMPORTANT: The name of your sub-theme must start with an alphabetic character and can only contain lowercase letters, numbers and underscores.

For example, copy the sites/all/themes/zoundation/STARTERKIT folder and rename it as sites/all/themes/foo.

2. Setup the basic information for your sub-theme.

  In your new sub-theme folder, rename the STARTERKIT.info.txt file to include
  the name of your new sub-theme and remove the ".txt" extension. Then edit
  the .info file by editing the name and description field.

  For example, rename the foo/STARTERKIT.info file to foo/foo.info. Edit the
  foo.info file and change "name = Zoundation Sub-theme Starter Kit" to "name = Foo"
  and "description = Read..." to "description = A Zoundation sub-theme".

    Why? The .info file describes the basic things about your theme: its
    name, description, features, template regions, CSS files, and JavaScript
    files. See the Drupal 7 Theme Guide for more info:
    http://drupal.org/node/171205

  Then, visit your site's Appearance page at admin/appearance to refresh
  Drupal 7's cache of .info file data.

3. Set your website's default theme.

  Log in as an administrator on your Drupal site, go to the Appearance page at
  admin/appearance and click the "Enable and set default" link next to your
  new sub-theme.

4. Installing Zoundation Support module.

If you enable and set your sub-theme to default without installing the Zoundation Support module, you will be prompted in a Status message to do so.

Install the Zoundation Support module http://drupal.org/project/zoundation_support in sites/all/modules and then go to your modules page and enable.


5. Install and enable jquery_update 7.x-2.3-alpha1 module

Foundation requires the use of at least jQuery 1.7. 
Zoundation requires the use of the jquery_update module 7.x-2.3-alpha1 to allow for this requirement: http://drupal.org/project/jquery_update

After installing jquery_update go to admin/config/development/jquery_update and chose jQuery Version: 1.7

The Zoundation Support module checks to ensure that jquery_update >= 2.3 is installed and configured to use at least jquery 1.7. This is required for proper functioning of Foundation's javascript add-ons.


Optional steps:

6. Modify the markup in Zoundation core's template files.

  If you decide you want to modify any of the .tpl.php template files in the
  zoundation folder, copy them to your sub-theme's folder before making any changes.
  And then rebuild the theme registry.

  For example, copy zoundation/templates/page.tpl.php to foo/templates/page.tpl.php.

8. Further extend your sub-theme.

  Learn ways to further extend your sub-theme here:
  Drupal 7's Theme Guide online at:
    http://drupal.org/theme-guide

