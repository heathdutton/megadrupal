No Table Drag
=============

[See original post](https://www.commercialprogression.com/post/how-remove-tabledrag-rearranging-multiple-value-field-widgets)

Usage
-----

### UI
1. Enable the module
2. Go to your field settings for a given field
3. Check the "No Table Drag" checkbox
4. Victory!

Note that the settings are per field instance. So if you use the field in more than one place you can have the table drag on/off as necessary.

### API

1. Enable the module
2. Add the "#nodrag" option to your field widget form using hook_field_widget_form_alter
3. Victory!

Ex:
````php
/**
 * Implements hook_field_widget_form_alter().
 */
function multi_widget_remove_tabledrag_field_widget_form_alter(&$element, &$form_state, $context) {
  if (isset($element['#field_name'])) {
    switch ($element['#field_name']) {
      case 'field_long_test':
        $element['#nodrag'] = TRUE;
      default:
        break;
    }
  }
}
````