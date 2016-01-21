WHERE TO START
--------------
This is a theme for themers and includes the building blocks for your HTML5 theme. 
This theme is designed to be a base theme which you use to develop a custom sub-theme on top of it.

It is recommended not to alter any of the jBase5 files and do all your custom development within
your subtheme. This allows for easier updates to the jBase5 theme files without altering your custom
subtheme.


INSTALLATION
------------

 1. Download jBase5 from http://drupal.org/project/jbase5

 2. Unpack the downloaded file, take the folders and place them in your
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

 3. Create a new folder in your theme's directory (next to, not in jbase5) for your subtheme.

    Example: mytheme

 4. Create a new .info file for your theme defining "jbase5" as the "base theme".

    Example: mytheme.info

    ; $Id$

    ; Theme Info -----------------------------------------------------------------
    ; required information about the theme
    
    name = My Theme
    description = My subtheme using jBase5 as the base theme
    core = 7.x
    base theme = jbase5
    engine = phptemplate

 5. You should now have a custom folder next to "jbase5" and a .info file in there which is set
    up to use jBase5 as the base theme.

 6. Go to your site and enable your newly created subtheme and begin developing!



FURTHER READING
---------------

Drupal theming documentation in the Theme Guide:
  http://drupal.org/theme-guide



SPECIAL THANKS
---------------

This project is sponsored by Eternalistic Designs (www.eternalistic.net) and OpenSesame (www.opensesame.com)

Special thanks to Jay Wolf for creating the first jBase (https://github.com/jaywolf/jBase) and all the help refining this version.
