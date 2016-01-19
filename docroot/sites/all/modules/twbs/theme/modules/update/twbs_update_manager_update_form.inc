<?php

/**
 * Returns HTML for the first page in the process of updating projects.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function twbs_update_manager_update_form($variables) {
  $form = $variables['form'];
  $last = variable_get('update_last_check', 0);
  $output = theme('update_last_check', array('last' => $last));
  $output .= drupal_render_children($form);
  return $output;
}
