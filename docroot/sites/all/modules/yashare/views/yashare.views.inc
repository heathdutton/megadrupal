<?php
/**
 * @file
 * Implements views.inc with hook_views_data.
 */

/**
 * Implements hook_views_data().
 */
function yashare_views_data() {
  $data = array();
  foreach (entity_get_info() as $name => $type) {
    if ($name == 'node' || $name = 'taxonomy_term') {
      $data[$type['base table']]['yashare'] = array(
        'title' => t('Yandex.Share'),
        'help' => t('Yandex.Share widget.'),
        'field' => array(
          'handler' => 'yashare_handler_field_yashare',
          'click sortable' => FALSE,
        ),
      );
    }
  }
  return $data;
}
