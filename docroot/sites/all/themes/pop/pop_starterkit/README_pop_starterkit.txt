NOTES
======
* Drupal theme system caches template files and which theme functions should be called. What that means is if you add a new theme or preprocess function to your template.php file or add a new template (.tpl.php) file to your sub-theme, you will need to rebuild the "theme registry." See http://drupal.org/node/173880#theme-registry

* Drupal also stores a cache of the data in .info files. If you modify any lines in your sub-theme's .info file, you MUST refresh Drupal's cache by simply visiting the admin/appearance page.

* You can clear your Drupal's cache by clicking the "Clear all caches" button at admin/config/development/performance.

  Alternately, you can install one of the following modules and use their cache clearing functions:
    Administration menu module (http://drupal.org/project/admin_menu)
    Devel module (http://drupal.org/project/devel)


Create Your Own Sub-theme
=========================
The base Pop theme is designed to be easily extended by its sub-themes. Avoid modify any of the CSS or PHP files in the pop/ folder; Instead you should create a sub-theme of Pop in a folder outside of the root pop/ folder. The examples below assume Pop and your sub-theme will be installed in sites/all/themes/, but any valid theme directory is acceptable.

1. Copy the pop_starterkit folder out of the pop/ folder and rename it to be your new sub-theme.
IMPORTANT: Only lowercase letters and underscores should be used for the name of your sub-theme.

  For example, copy the sites/all/themes/pop/pop_starterkit folder and rename it as sites/all/themes/foo.

  WHY? Each theme should reside in its own folder. To make it easier to upgrade Pop, sub-themes should reside in a folder separate from their base theme.


2. In your new sub-theme folder, rename the pop_starterkit.info.txt file to include the name of your new sub-theme and remove the ".txt" extension. Then edit the .info file by editing the name and description field.

  For example, rename the foo/pop_starterkit.info.txt file to foo/foo.info. Edit the foo.info file and change "name = Pop SubTheme Starter Kit" to "name = My Foo Theme" and to "description = Foo is a Pop sub-theme".

  WHY? The .info file describes the basic things about your theme: its name, description, features, template regions, CSS files, and JavaScript files.
  See the Drupal 7 Theme Guide for more info: http://drupal.org/node/171205

  Then, visit your site's Administration > Appearance (admin/appearance) to refresh Drupal's cache of .info file data.


- By default your new Pop sub-theme is using a fixed-width layout. If you want a liquid layout for your theme, just define a new value inside your styles.css.


- If you need template.php, make sure all your hook function name start with the name of your sub-theme.

    For example, add foo/template.php and make sure all the hook implementation start with "foo", e.g. foo_theme(), foo_preprocess_page().


- Log in as an administrator on your Drupal site and go to Administration > Appearance (admin/appearance) and enable your new sub-theme.


Optional:

- MODIFYING TEMPLATE FILES:
  If you decide you want to modify any of the .tpl.php template files in the pop folder, copy them to your subtheme's folder before making any changes. Then rebuild the theme registry.

    For example, copy pop/templates/page.tpl.php to foo/templates/page.tpl.php


CSS Files
=========
- Sub-theme should make use of style.css to add sub-theme specific styling.

- custom.css is meant for end-user usage to customize CSS related to their site, end user MUST backup the custom.css if they are used during theme upgrade.

- Sub-theme can override Pop base theme CSS by duplicating the CSS files in pop/css folder.
IMPORTANT: The file should be included in your sub-theme info file.

    For example, if you override the layout.css, add to your foo/foo.info
      stylesheets[all][]  = css/layout.css

