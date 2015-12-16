<?php

/**
 * @file
 * Hook definitions for the Webform Hints module.
 */

/**
 * @defgroup webform_hints_hooks Webform Hints Module Hooks
 * @{
 * Webform Hints' hooks allow other modules to provide hints for custom webform
 * components and to customize form elements after hints are applied.
 */

/**
 * Alter fieldtypes supported by webform hints.
 *
 * The fieldtypes are the names of the form elements which are supported by
 * Webform hints. You can add and remove hint support by altering the
 * $fieldtypes array.
 *
 * Simple components may just work after being added to this array, but more
 * complex components may need to use hook_webform_hints_element_alter() to
 * modify the element to properly support the addition of hints.
 *
 * @param $fieldtypes array
 *   Strings which represent webform components that hints are applied to.
 * @param $element array
 *   The current webform component element.
 *
 * @see webform_hints_add_title()
 * @see hook_webform_hints_element_alter()
 *
 */
function hook_webform_hints_fieldtypes_alter(&$fieldtypes, &$element) {
  // Add my_custom_element_name to the list of supported components. Check
  // against the element type first or all elements will be modified.
  if ($element['#type'] == 'my_custom_element_name') {
    $fieldtypes[] = 'my_custom_element_name';
  }
}

/**
 * Alter a webform component element after applying webform hints.
 *
 * This can be used to add further support to custom components added with
 * hook_webform_hints_fieldtypes_alter(). It can also be used to customize
 * elements that Webform Hints already supports after their hints have been
 * added.
 *
 * @param $element array
 *   The current webform component element.
 * @param $required_label string
 *   Contains the required label text to be added to the hint.
 *
 * @see webform_hints_add_title()
 * @see hook_webform_hints_fieldtypes_alter()
 */
function hook_webform_hints_element_alter(&$element, &$required_label) {
  if ($element['#type'] == 'textfield') {
    // Example: Textfield components should use the description for the hint
    // text rather than the label. The label should still be shown.
    $element['#attributes']['placeholder'] = $element['#description'];
    $element['#description'] = '';
    $element['#title_display'] = 'before';
  }
}

/**
 * @}
 */
