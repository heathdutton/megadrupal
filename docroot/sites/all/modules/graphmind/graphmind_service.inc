<?php

/**
 * Save a GraphMind map. (FreeMind XML format.)
 * 
 * @param integer $entity_id
 * @param integer $entity_vid
 * @param integer $delta
 * @param string $field_name
 * @param string $entity_type
 * @param string $mm
 * @return string 
 */
function graphmind_service_save_graphmind($entity_id, $entity_vid, $delta, $field_name, $entity_type, $mm) {
  // Update data in both the data and revision tables.
  // @TODO check if there is a better way to do it - with the Field API.
  foreach (array('field_revision_', 'field_data_') as $table_prefix) {
    db_query("
      UPDATE {" . db_escape_table($table_prefix . $field_name). "}
      SET " . db_escape_field($field_name . '_map') . " = :mm
      WHERE revision_id = :revision_id AND delta = :delta
    ", array(
      ':mm' => $mm,
      ':revision_id' => $entity_vid,
      ':delta' => $delta,
    ));
  }
  
  // Invalidate field cache.
  cache_clear_all("field:$entity_type:$entity_id", 'cache_field');
  
  return '1';
}

/**
 * Services callback - get a preview of a node.
 *
 * @param integer $nid
 * @return string
 */
function graphmind_service_tooltip_get_view($nid) {
  $node = node_load((int)$nid);
  if (!$node) {
    return '';
  }

  $out = $node->title;
  
  if (isset($node->body[$node->language])) {
    $view = $node->body[$node->language][0]['value'];
    $view = preg_replace("/<[^>]*>/", '', $view);
    $view = preg_replace("/\s{1,}/", ' ', $view);
    $view = preg_replace("/\n{1,}/", "\n", $view);
    $out .= "\n\n" . $view;
  }
  
  return $out;
}

/**
 * Implementation of the getMap Services resource.
 * 
 * @param integer $entity_id
 * @param integer $delta
 * @return string 
 */
function graphmind_service_get_map($entity_vid, $delta, $field_name) {
  $result = db_query("
    SELECT " . db_escape_field($field_name . '_map') . "
    FROM {" . db_escape_table('field_revision_' . $field_name) . "} 
    WHERE revision_id = :revision_id AND delta = :delta
  ", array(
    ':revision_id' => $entity_vid,
    ':delta' => $delta,
  ));
  
  return $result->fetchColumn();
}
