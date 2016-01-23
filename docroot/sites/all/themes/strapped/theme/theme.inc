<?php
/**
 * @file
 * theme.inc
 */



/**
 * Implements hook_theme().
 *
 * @return array
 */
function strapped_theme() {
  return array(
    // @see wrappers.inc
    'column_wrapper' => array(
      'render element'  => 'element',
    ),
    // @see wrappers.inc
    'row_wrapper' => array(
      'render element'  => 'element',
    ),
    // @see wrappers.inc
    'checkbox_wrapper' => array(
      'render element'  => 'element',
    ),
    // @see wrappers.inc
    'radio_wrapper' => array(
      'render element'  => 'element',
    ),
    // @see wrappers.inc
    'form_type_wrapper' => array(
      'render element'  => 'element',
    ),
    // @see wrappers.inc
    'form_group_wrapper' => array(
      'render element'  => 'element',
    ),
    // @see wrappers.inc
    'form_field_wrapper' => array(
      'render element'  => 'element',
    ),
    // @see wrappers.inc
    'form_control_static_wrapper' => array(
      'render element'  => 'element',
    ),
    'tooltip' => array(
      'variables' => array(
        'icon' => 'glyphicon-question-sign',
        'html' => 'true',
        'toggle' => 'tooltip',
        'position' => 'right',
        'title' => ''
      ),
      'template' => 'tooltip',
      'path' => drupal_get_path('theme', 'strapped') . '/templates/bootstrap',
    ),
    'help' => array(
      'variables' => array(
        'description' => '',
      ),
      'template' => 'help',
      'path' => drupal_get_path('theme', 'strapped') . '/templates/bootstrap',
    ),
    'error' => array(
      'variables' => array(
        'message' => '',
      ),
      'template' => 'error',
      'path' => drupal_get_path('theme', 'strapped') . '/templates/bootstrap',
    ),
  );
}



