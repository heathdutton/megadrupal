Description
===========
This module allow to override Drupal's core pager with defined preset as link or
input box for any single page. It is very useful when there are over 100 pages.

Paginate uses standard Drupal pager classes to render the pagers, so styling
is preserved.

Features:
---------
- Control whether to display links to pages.
- Direct input of the page to go on particular page.

Themes:
-------
paginate_standard     - alike standard Drupal pager theme.
paginate_istandard    - alike standard Drupal pager but additional input box to
                        go particular page.

paginate_mini         - minimal pager, providing direct page entry.
paginate_mini         - minimal pager, providing direct page entry by input box.

Instructions:
-------------
- Install and enable the module.
- Check the Configuration page to setup.
- Create and configure any number of 'preset' pagers.
- Select a preset to use as a general replacement of Drupal core pager, or
  use a preset as a pager in Views.

Installation
------------
1. Copy the entire paginate directory into the Drupal sites/all/modules or
   sites/all/modules/contrib directory.
2. Login as an administrator. Enable the module in the "Administer" -> "Modules"
3. Now replace the theme('pager') by theme('paginate_standard') or one of any
   defined theme.

An example for overriding 'tags' at a theme's level
===================================================

Themers can override pager text elements by implementing in the theme's
template.php a hook_preprocess_paginate_xxxx() function to set the 'tags'
values needed. xxxx should be replaced with the specific paginate theme
you want to address.

Example:

function mytheme_preprocess_paginate_imini(&$variables) {
  $variables['tags']['first'] = t('<<');
  $variables['tags']['previous'] = t('<');
  $variables['tags']['next'] = t('>');
  $variables['tags']['last'] = t('>>');
}
OR
function mytheme_preprocess_paginate_mini(&$variables) {
  $variables['tags']['first'] = t('<< first');
  $variables['tags']['previous'] = t('< prev');
  $variables['tags']['next'] = t('next >');
  $variables['tags']['last'] = t('last >>');
}
