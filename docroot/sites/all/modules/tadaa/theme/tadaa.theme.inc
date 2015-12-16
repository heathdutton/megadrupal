<?php
/**
 * @file
 * Theme implementations used by the Tadaa! module.
 */

/**
 * Theme function for the module table form.
 */
function theme_tadaa_module_fieldset($variables) {
  $form = $variables['form'];

  // Build the rows.
  $rows = array();
  foreach (element_children($form) as $module) {
    $row = array();
    foreach (element_children($form[$module]) as $key) {
      $row[] = drupal_render($form[$module][$key]);
    }
    $rows[] = $row;
  }

  // Add the table.
  return theme('table', array('header' => $form['#header'], 'rows' => $rows));
}
