BUILD A THEME WITH Beep Edition
-------------------------------

The base Beep Edition theme is designed to be easily extended by its sub-themes. You
shouldn't modify any of the CSS or PHP files in the beep_edition/ folder; but instead you
should create a sub-theme of beep_edition which is located in a folder outside of the
root beep_edition/ folder. The examples below assume beep_edition and your sub-theme will be
installed in sites/all/themes/, but any valid theme directory is acceptable
(read the sites/default/default.settings.php for more info.)


 1. Setup the location for your new sub-theme.

    Copy the STARTERKIT folder out of the beep_edition/ folder and rename it to be your
    new sub-theme. IMPORTANT: The name of your sub-theme must start with an
    alphabetic character and can only contain lowercase letters, numbers and
    underscores.

    For example, copy the sites/all/themes/beep_edition/STARTERKIT folder and rename it
    as sites/all/themes/newthemename.

      Why? Each theme should reside in its own folder. To make it easier to
      upgrade Beep Edition, sub-themes should reside in a folder separate from the base
      theme.

 2. Setup the basic information for your sub-theme.

    In your new sub-theme folder, rename the STARTERKIT.info.txt file to include
    the name of your new sub-theme and remove the ".txt" extension. Then edit
    the .info file by editing the name and description field.

    For example, rename the newthemename/STARTERKIT.info file to newthemename/newthemename.info. Edit the
    newthemename.info file and change "name = Beep Edition Sub-theme Starter Kit" to "name = NewThemeName"
    and "description = Read..." to "description = A Beep Edition sub-theme".

      Why? The .info file describes the basic things about your theme: its
      name, description, features, template regions, CSS files, and JavaScript
      files. See the Drupal 7 Theme Guide for more info:
      https://drupal.org/node/171205
    
    Then do the same for the config.rb.txt file (remove the .txt extension) and set up Compass to watch this directory for changes.

    Then, visit your site's Appearance page at admin/appearance to refresh
    Drupal 7's cache of .info file data.

 3. Edit your sub-theme to use the proper function names.

    Edit the template.php and theme-settings.php files in your sub-theme's
    folder; replace ALL occurrences of "STARTERKIT" with the name of your
    sub-theme.

    For example, edit newthemename/template.php replace
    every occurrence of "STARTERKIT" with "newthemename".

    It is recommended to use a text editing application with search and
    "replace all" functionality.

 4. Set your website's default theme.

    Log in as an administrator on your Drupal site, go to the Appearance page at
    admin/appearance and click the "Enable and set default" link next to your
    new sub-theme.


