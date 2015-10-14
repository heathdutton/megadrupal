<?php

/**
 * Returns HTML for a submit button form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #attributes, #button_type, #name, #value.
 *
 * @ingroup themeable
 */
function twbs_submit($variables) {
  return theme('button', $variables['element']);
}
