<?php
function tb_rss_feed_generate_node($node_info, $node_id = NULL) {
  if ($node_id) {
  	$node = node_delete($node_id);
  }
  $node = new stdClass();
  $node->nid = NULL;
  $node->type = isset($node_info['node_type']) ? $node_info['node_type'] : 'article';
  node_object_prepare($node);
  $type = node_type_get_type($node);
  $node->title = isset($node_info['title']) ? $node_info['title'] : '';
  $node->revision = isset($node_info['revision']) ? $node_info['revision'] : 0;
  $node->promote = isset($node_info['promote']) ? $node_info['promote'] : 1;
  $node->created = isset($node_info['created']) && $node_info['created'] ? $node_info['created'] : time();
  $node->changed = isset($node_info['changed']) && $node_info['changed'] ? $node_info['changed'] : time();
  $node->language = isset($node_info['language']) ? $node_info['language'] : 
    (language_default('language') == 'en' ? LANGUAGE_NONE : language_default('language'));

  tb_rss_feed_generate_fields($node_info, $node, 'node', $node->type);
  node_save($node);
  return $node;
}

function tb_rss_feed_generate_fields($node_info, $object, $object_type, $bundle) {
  $field_types = field_info_field_types();
  $instances = field_info_instances($object_type, $bundle);
  foreach ($instances as $instance) {
    $field_name = $instance['field_name'];
    if (isset($node_info[$field_name])) {
      $field = field_info_field($field_name);
      if (is_array($node_info[$field_name])) {
        $field_values = $node_info[$field_name];
      }
      else {
        $field_values = array($node_info[$field_name]);
      }
  
      $module = $field_types[$field['type']]['module'];
      
      $object_field = array();
      foreach ($field_values as $field_value) {
        $generator_function = 'tb_rss_feed_' . $module . '_generate';
        if (function_exists($generator_function)) {
          $result = $generator_function($field_value, $object, $field, $instance, $bundle);
          if ($result) {
            $object_field[] = $result;
          }
        }
      }
      $object->{$field['field_name']} = array(
        $object->language => $object_field,
      );
    }
  }
}
