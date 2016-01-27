<?php

/**
 * @file
 * Provide views data and handlers for yp.module.
 */

/**
 * Implementation of hook_views_data().
 */
function yp_views_data() {
  $data['yp_stream']['table']['group'] = t('Icecast YP');
  $data['yp_stream']['table']['base'] = array(
    'field' => 'sid',
    'title' => t('Icecast YP'),
    'help' => t('Icecast streams listed by the Icecast YP directory module.'),
  );
  $schema = drupal_get_schema_unprocessed('yp', 'yp_stream');
  foreach ($schema['fields'] as $key => $field) {
    $data['yp_stream'][$key] = array(
      'title' => $field['description'],
      'help' => $field['description'] . '.',
      'field' => array(
        'click sortable' => TRUE,
      ),
    );
  }
  $data['yp_stream']['server_name']['field']['handler'] = 'views_handler_field_yp_server_name';
  $data['yp_stream']['current_song']['field']['handler'] = 'views_handler_field_yp_current_song';
  $data['yp_stream']['listen_url']['field']['handler'] = 'views_handler_field_yp_listen_url';
  $data['yp_stream']['server_type']['field']['handler'] = 'views_handler_field_yp_server_type';
  $data['yp_stream']['bitrate']['field']['handler'] = 'views_handler_field_yp_bitrate';
  $data['yp_stream']['listeners']['field']['handler'] = 'views_handler_field_numeric';
  $data['yp_stream']['max_listeners']['field']['handler'] = 'views_handler_field_numeric';
  $data['yp_stream']['last_touch']['field']['handler'] = 'views_handler_field_date';
  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
function yp_views_handlers() {
  $common = array(
    'parent' => 'views_handler_field',
    'file' => 'yp.handlers.inc',
  );
  return array(
    'handlers' => array(
      'views_handler_field_yp_server_name' => $common,
      'views_handler_field_yp_current_song' => $common,
      'views_handler_field_yp_listen_url' => $common,
      'views_handler_field_yp_server_type' => $common,
      'views_handler_field_yp_bitrate' => $common,
    ),
  );
}
