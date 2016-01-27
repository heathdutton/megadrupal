<?php

/**
 * Display node fields from the node.
 */
class node_field_handler_field_node_field extends views_handler_field {

  function render($values) {
    $node_fields_content = '';
    $params = array(
      'nid' => $values->nid,
    );
    $node_fields = node_field_db_field_select($params);
    if ($node_fields) {
      $to_render = node_field_node_content_add_node_fields($node_fields);
      $node_fields_content = drupal_render($to_render);
    }
    return $node_fields_content;
  }
}
