<?php
/**
 * @file
 * db call functions for opac items module.
 */

/**
 * Get fields mapping for a given ILS server.
 *
 * @param string $serv_id
 *   ILS server machine name.
 *
 * @return array
 *   Mapped fields for the server.
 */
function opac_items_get_fields_mapping($serv_id) {
  $select = db_select('opac_items_mapping', 'im')
    ->fields('im')
    ->condition('serv_id', $serv_id, '=')
    ->orderBy('weight', 'ASC');

  $mapping = array();
  $result = $select->execute();

  while ($row = $result->fetchAssoc()) {
    $mapping[] = $row;
  }
  return $mapping;
}

/**
 * Get a field mapping for a given ILS server and field name.
 *
 * @param string $serv_id
 *   ILS server machine name.
 *
 * @param string $item_field
 *   Field name for which get the mapping.
 *
 * @return array
 *   field mapping.
 */
function opac_items_get_field_mapping($serv_id, $item_field) {
  $select = db_select('opac_items_mapping', 'im')
    ->fields('im')
    ->condition('serv_id', $serv_id, '=')
    ->condition('item_field', $item_field, '=')
    ->orderBy('weight', 'ASC');

  $result = $select->execute();
  return $result->fetchAssoc();
}

/**
 * Add a mapping.
 *
 * @param array $mapping
 *   Mapping values.
 */
function opac_items_field_mapping_add($mapping) {
  $mid = db_insert('opac_items_mapping')
    ->fields(array(
      'serv_id' => $mapping['serv_id'],
      'item_field' => $mapping['item_field'],
      'display_header' => $mapping['display_header'],
      'vocabulary_machine_name' => $mapping['vocabulary_machine_name'],
      'vocabulary_name' => $mapping['vocabulary_name'],
      'term_field' => $mapping['term_field'],
      'term_field_label' => $mapping['term_field_label'],
      'nomatch_rule' => $mapping['nomatch_rule'],
      'show_as_link' => $mapping['show_as_link'],
      'weight' => $mapping['weight'],
    ))
    ->execute();
}

/**
 * Modify an existing mapping.
 *
 * @param array $mapping
 *   Mapping values.
 */
function opac_items_field_mapping_mod($mapping) {
  $mid = db_update('opac_items_mapping')
    ->fields(array(
      'display_header' => $mapping['display_header'],
      'weight' => $mapping['weight'],
      'vocabulary_machine_name' => $mapping['vocabulary_machine_name'],
      'vocabulary_name' => $mapping['vocabulary_name'],
      'term_field' => $mapping['term_field'],
      'term_field_label' => $mapping['term_field_label'],
      'nomatch_rule' => $mapping['nomatch_rule'],
      'show_as_link' => $mapping['show_as_link'],
    ))
    ->condition('serv_id', $mapping['serv_id'], '=')
    ->condition('item_field', $mapping['item_field'], '=')
    ->execute();
}

/**
 * Modify weight an existing mapped item field.
 *
 * @param string $serv_id
 *   ILS server identifier.
 *
 * @param string $item_field
 *   The name of the item field to be changed.
 *
 * @param int $weight
 *   The new weight to be applied for the item field.
 */
function opac_items_field_mapping_mod_weight($serv_id, $item_field, $weight) {
  $mid = db_update('opac_items_mapping')
    ->fields(array(
      'weight' => $weight,
    ))
    ->condition('serv_id', $serv_id, '=')
    ->condition('item_field', $item_field, '=')
    ->execute();
}

/**
 * Delete an existing mapping.
 *
 * @param string $serv_id
 *   ILS server identifier.
 *
 * @param string $item_field
 *   The name of the item field for which delete the mapping.
 */
function opac_items_field_mapping_del($serv_id, $item_field) {
  $num_deleted = db_delete('opac_items_mapping')
    ->condition('serv_id', $serv_id, '=')
    ->condition('item_field', $item_field, '=')
    ->execute();
}
