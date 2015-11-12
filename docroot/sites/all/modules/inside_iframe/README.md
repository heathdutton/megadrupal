Sometimes a website needs to be displayed inside an iFrame, as well as being displayed in a full window. In the case when it is displayed inside an iFrame, it should be modified a bit to better fit this display, for example some parts can be hidden, the margin of the page can be lowered or removed, etc. These modifications can be done by including an extra CSS file that contains the necessary customizations.

The challenge for the application is to detect and know when it is being displayed inside an iFrame, and when on a full window.
This module solves this problem by appending to the URL the query parameter `?display=iframe`. The function `inside_iframe()` provided by the module returns TRUE when such a query parameter is present. This can be used by the application to customize the logic of the UI. Then the module takes care to alter all the url-s and goto-s of the application by appending automatically the query parameter `?display=iframe` , when the application is inside an iFrame.

The module also provides the hook `hook_inside_iframe()`, which can be used by the application to include extra CSS files, like this:
```
/**
 * Implements hook_inside_iframe().
 */
function MODULE_inside_iframe() {
  $path = drupal_get_path('module', 'MODULE');
  drupal_add_css($path . '/inside_iframe.css');
}
```
