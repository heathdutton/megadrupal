<?php

/**
 * @file
 * Tools to work with forms.
 */

/**
 * Extract user input value from $form_state array.
 *
 * @param array $fs
 *   $form_state array.
 * @param string/array $element
 *   Form element to extract
 * @throws Exception
 *   For now, Element as an array raises exception and is not implemented yet.
 *
 * @return mixed
 *   Extracted value from $form_state.
 */
function _entityspice_f_values(array $fs, $element) {
  if(is_array($element)) {
    throw new Exception('Not implemented for array.');
  }

  return isset($fs['values'][$element]) ? $fs['values'][$element] : NULL;
}
