IMAGE STYLES BY THEMES
======================

Super simple utility module which allows themes to:

  1. Declare new image styles,
  2. Alter existing image styles.


USAGE
-----

  1. Install this module

  2. Ensure your theme is enabled,

  3. On the settings page for your theme, ensure the "Allow this theme to
     declare image styles" and/or "Allow this theme to alter image styles" box
     is ticked, as your needs require.

  4. Create a file of the following name at the ROOT of your theme directory:

      THEMENAME.image_styles,inc

  5. Implement whichever of the following two hooks you need:

      THEMENAME_theme_image_default_styles()

      THEMENAME_theme_image_styles_alter()

  These hooks correspond with the usual image module hooks, with a slight name
  change to avoid conflicts. See image_styles_by_themes.api.php for more info.

  6. Clear caches!


WARNING
-------

As soon as you disable your theme, all image styles it declared will be removed
from the system. This can lead to seriously broken content. Be warned!
