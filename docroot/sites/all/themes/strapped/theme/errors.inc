<?php
/**
 * @file
 * Error related theme functions.
 */


/**
 * Because the form errors are included in the main form element wrapper
 * this will just return nothing.
 */
function strapped_ife_form_element($variables) {
  $output = '';
  $output = $variables['element']['#children'];
  return $output;
}
