<?php

/**
 * Updates specified entity fields without re-saving entire entity
 *
 * Usage example: 
 *  $node->field_name[$node->language][0]['value'] = t('John Smith');
 *  entity_save_field('field_name', $node, 'node', $node->nid);
 *
 * IMPORTANT: This function will not trigger update hooks!
 * 
 * @param  [type] $field_names [description]
 * @param  [type] $entity      [description]
 * @param  [type] $entity_type [description]
 * @param  [type] $entity_id   [description]
 * @return [type]              [description]
 */
function entity_save_field($field_names, $entity, $entity_type, $entity_id) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    if (!is_array($field_names)) {
      $field_names = array($field_names);
    }

    $field_ids = array();
    foreach ($field_names as $field_name) {
      $info = field_info_field($field_name);
      $field_ids[] = $info['id'];
    }

    field_sql_storage_field_storage_write($entity_type, $entity, 'update', $field_ids);

    cache_clear_all('field:' . $entity_type . ':' . $entity_id, 'cache_field');
  }
}