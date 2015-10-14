<?php
/**
 *
 * @file
 *  The file include the module services callback function implementations.
 */

/**
 * @param $id
 *
 * @return string
 */
function _field_resource_resource_retrieve($id = NULL) {
  $output = NULL;
  $resources = &drupal_static(__FUNCTION__);

  if (isset($resources[$id])) {
    return $resources[$id];
  }

  if ($cache = cache_get("field_resource_$$id")) {
    return $cache->data;
  }

  $entities = _field_resource_field_infos();
  foreach ($entities as $entity_type => $info) {
    foreach ($info['fields'] as $field_name) {
      $query = new EntityFieldQuery();
      $query
        ->entityCondition('entity_type', $entity_type)
        ->fieldCondition($field_name, 'field_resource_machine_name', $id);
      $result = $query->execute();

      if ($result) {
        $entity = entity_load($entity_type, array_keys($result[$entity_type]));
        $entity_id = key($entity);
        $field_value = field_get_items($entity_type, $entity[$entity_id], $field_name);
        $field_content = token_replace($field_value[0]['field_resource_content_value'], array($entity_type => $entity[$entity_id]));
        $output = check_markup($field_content, $field_value[0]['field_resource_content_format']);
        cache_set("field_resource_$id", $output, 'cache', CACHE_TEMPORARY);
      }
    }
  }

  return $output;
}
