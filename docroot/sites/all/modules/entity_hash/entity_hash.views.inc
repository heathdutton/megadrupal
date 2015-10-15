<?php
/**
 * @file
 * Views Implementation for entity hash
 */

/**
 * Implements hook_views_data_alter().
 */
function entity_hash_views_data_alter(&$data) {
  foreach (entity_get_info() as $info) {
    if (isset($info['hash']) && $info['hash'] == TRUE && !empty($info['entity keys']['hash'])) {
      $table = $info['base table'];
      $field = $info['entity keys']['hash'];
      $data[$table][$field] = array(
        'title' => t('@type hash', array('@type' => $info['label'])),
        'help' => t('The hash of the @type.', array('@type' => $info['label'])),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
          'allow empty' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_string',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      );
      
      if (!empty($info['revision table'])) {
        $revision_table = $info['revision table'];
        $revision_field = $info['entity keys']['hash'];

        $data[$revision_table][$revision_field] = $data[$table][$field];
        $data[$revision_table][$revision_field]['title'] = t('@type revision hash', array('@type' => $info['label']));
        $data[$revision_table][$revision_field]['help'] = t('The revision hash of the @type.', array('@type' => $info['label']));
      }
    }
  }
}
