qTip Form API
-------------------------

Author:
Joshua Ellinger (westwesterson)

Development sponsored by Fanlala, inc.

What is qTip Form API?
-------------------------
qTip Form API is a development tool which allows you to create tooltips in code via the form api.
It uses the wonderful qTip plugin (version 2 or compatible).
Link: http://craigsworks.com/projects/qtip2/

How to install
-------------------------
1. Install the libraries module (required dependency)
2. Download the qTip2 library: http://qtip2.com/download
3. Rename the qTip folder and upload the qtip library inside your libraries folder,
     the result will be something like /sites/all/libraries/qtip
     the qtip js library should be at the path /sites/all/libraries/qtip/qtip.min.js
     the qtip css library should be at /sites/all/libraries/qtip/qtip.min.css
4. Now that the library is installed; Install the module as normal


API Documentation
-------------------------

This module works by enabling an extra parameter to each form element.
This parameter contains an array of configuration which is then used to configure the qtip module.

To add a tooltip add the following to any form element:
'#qtip' => array(/*add your configuration overrides here*/),

inside the array you should place all your overrides.

Override settings use the same syntax as listed in the default settings
They are passed in as a php array instead of Javascript.

Settings documentation:
https://github.com/Craga89/qTip2/wiki/Core
https://github.com/Craga89/qTip2/wiki/Content
https://github.com/Craga89/qTip2/wiki/Position
https://github.com/Craga89/qTip2/wiki/Show
https://github.com/Craga89/qTip2/wiki/Hide
https://github.com/Craga89/qTip2/wiki/Style

Extended syntax:
In order to make adding of description text easier, you can use this extended syntax
instead of the default overrides for the content title and content text fields.

  'title' => array(
    'type' => '',
    'value' => '',
  ),
  'text' => array(
    'type' => '',
    'value' => '',
  ),

valid types for title and text:
  string 'description'
    Moves the contents of the description field of the form element inside the tooltip.
    the value field is ignored.
  string 'markup'
    enter html markup or a text string directly into the 'value' field.

there is one more extended extended syntax:
  'selector' => ''
this allows you to override the default jquery selector (which is by default the form field id)



Example use of qtip in a form array.
See the included qtip_fapi_example module for a working example.
-------------------------

$form['myfield'] = array(
  '#type' => 'textfield',
  '#title' => 'My Field',
  '#description' => 'Title of myfield',
  '#required' => TRUE,
  '#qtip' => array(
    'title' => array(
      'type' => 'description',
    ),
    'text' => array(
      'type' => 'markup',
      'value' => '<strong>Tooltip</strong> content of textfield.',
    ),
    'position' => array(
      'my' => 'top left', // Use the corner...
      'at' => 'bottom right', // ...and opposite corner
    ),
    'style' => array(
      'classes' => 'ui-tooltip-shadow ui-tooltip-tipsy',
    ),
    'selector' => '#edit-myfield',
  ),
);

Defaults
--------------------------
It is possible to define your own defaults in a custom module using the following:

/**
 * Implements hook_element_info_alter().
 *
 * Set my_module qtip defaults.
 */
function my_module_element_info_alter(&$type) {
  foreach ($type as $element => $values) {
    $type[$element]['#qtip'] = array(
      // Define default overrides.
    );
  }
  return;
}
