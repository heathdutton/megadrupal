<?php

/**
 * @file
 * Contains ajax commands for Entity Abuse module.
 */

/**
 * AJAX command that openes jQuery UI dialog with provided data.
 */
function entity_abuse_ajax_command_open_dialog($data, $settings = array()) {

  $settings += array(
    'draggable' => FALSE,
    'modal' => TRUE,
    'resizable' => FALSE,
    'title' => t('Submit complain'),
  );

  drupal_add_library('system', 'ui.dialog');
  drupal_add_js(drupal_get_path('module', 'entity_abuse') . '/js/entity_abuse.js');

  return array(
    'command' => 'open_dialog',
    'data' => $data,
    'settings' => $settings,
  );
}
