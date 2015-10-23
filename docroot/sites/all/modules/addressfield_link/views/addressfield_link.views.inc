<?php

/**
 * Implements hook_field_views_data_alter().
 */
function addressfield_link_field_views_data_alter(&$result, $field, $module) {
  if ($module == 'addressfield') {
    foreach ($result as $table_name => $table) {
      $field_name = $field['field_name'];
      if (isset($result[$table_name][$field_name])) {
        $field_title = $result[$table_name][$field_name]['title'];
        $group = $result[$table_name][$field_name]['group'];
        $title = $field_title . ' with link';
        $help = $result[$table_name][$field_name . '_data']['help'] . '. Address field link from ' . $field_name . ' field.';
        $result[$table_name]['table']['group'] = t('Address links');
        $result[$table_name][$field_name . '_data'] = array(
          'group' => $group,
          'title' => $title,
          'help' => $help,
          'field' => array(
            'handler' => 'addressfield_link_handler_field_address_link',
            'click sortable' => TRUE,
          )
        );
      }
    }
  }
}
