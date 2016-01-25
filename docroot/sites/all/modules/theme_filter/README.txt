====================================
theme_filter
====================================

theme_filter module that will add an input filter which will convert the string
or a token type string.
[theme:THEME_NAME] converts to /themes/THEME_NAME or sites/themes/THEME_NAME
[theme:THEME_NAME:absolute] converts to absolute theme path like
http://site-url/themes/THEME_NAME OR http://site-url/sites/all/themes/THEME_NAME
to be used to render the images from theme path.

====================================
settings
====================================

1) Enable the module.
2) go to - Text formats - admin/config/content/formats
3) Click on Configure for which text formats you want to enable the filter.
4) Then in "Enabled filters" section check the checkbox - "Replace Theme Path"
5) save that. and You are ready to use the string.!
(use the "1st way" mentioned below in "USE" section)


NOTE: this module also supports token, you can use the token_filter module, you
      just need to enable this module, and then in the 4th step Instead of
      "Replace Theme Path" you can select only "Replace tokens"
      and use the "2nd way" mentioned below,

====================================
USE
====================================

1st way  (without token_filter)::
---------
Select the text format for which you have enabled the filters, add this text to
body part where You want to use the theme path. Available tokens are
[theme:THEME_NAME] =>
      [theme:bartik] converts to /themes/bartik,
[theme:THEME_NAME:absolute] =>
      [theme:bartik:absolute] converts to http://sitename.com/themes/bartik),
[path-to-theme] =>
      [path-to-theme] converts to /themes/bartik,
[path-to-theme:absolute]  =>
      [path-to-theme:absolute] converts to http://sitename.com/themes/bartik.

2nd way  (with token_filter)::
--------
This module provides token to be used anywhere in the site with token module.
Here are the available tokens,

[theme-filter:theme:THEME_NAME]
[theme-filter:theme:THEME_NAME:absolute]
[theme-filter:path-to-theme]
[theme-filter:path-to-theme:absolute]

====================================
Current maintainers:
====================================
 * Mitesh Patel (developermitesh) - https://www.drupal.org/u/developermitesh
