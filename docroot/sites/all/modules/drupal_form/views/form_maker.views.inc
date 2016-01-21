<?php

/**
 * @file
 * Views hooks implemented for the Form maker module.
 */

/**
 * Implements hook_views_data_alter().
 */
function form_maker_views_data_alter(&$data) {
  $data['node']['form_maker_form_view'] = array(
    'group' => t('Form maker'),
    'field' => array(
      'title' => t('Form'),
      'help' => t('The Form display for this node.'),
      'handler' => 'form_maker_handler_field_form_view',
    ),
  );
}

/**
 * Implements hook_views_handlers().
 */
function form_maker_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'form_maker') . '/views',
    ),
    'handlers' => array(
      'form_maker_handler_field_form_view' => array(
        'parent' => 'views_handler_field',
        'file' => 'form_maker_handler_field_form_view.inc',
      ),
    ),
  );
}
