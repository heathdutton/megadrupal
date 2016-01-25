<?php
/**
 * @file
 * db call functions for opac users module.
 */

/**
 * Get fields mapping for a given ILS server.
 *
 * @param string $serv_id
 *   ILS server identifier.
 *
 * @return array
 *   mapping values.
 */
function opac_users_get_fields_mapping($serv_id) {
  $select = db_select('opac_users_mapping', 'om')
    ->fields('om')
    ->condition('serv_id', $serv_id, '=');

  $result = $select->execute();
  $mapping = $result->fetchAllAssoc('patron_field', PDO::FETCH_ASSOC);

  return $mapping;
}

/**
 * Get field mapping for a given ILS server and field.
 *
 * @param string $serv_id
 *   ILS server identifier.
 *
 * @param string $patron_field
 *   Field name.
 *
 * @return array
 *   mapping values.
 */
function opac_users_get_field_mapping($serv_id, $patron_field) {
  $select = db_select('opac_users_mapping', 'om')
    ->fields('om')
    ->condition('serv_id', $serv_id, '=')
    ->condition('patron_field', $patron_field, '=');

  $result = $select->execute();
  return $result->fetchAssoc();
}

/**
 * Modify field mapping.
 *
 * @param array $mapping
 *   Mapping values.
 */
function opac_users_field_mapping_mod($mapping) {
  $row = db_select('opac_users_mapping', 'om')
    ->fields('om')
    ->condition('serv_id', $mapping['serv_id'], '=')
    ->condition('patron_field', $mapping['patron_field'], '=')
    ->execute();

  if ($row->rowCount()) {
    db_update('opac_users_mapping')
      ->fields(array(
        'label_display' => $mapping['label_display'],
      ))
      ->condition('serv_id', $mapping['serv_id'], '=')
      ->condition('patron_field', $mapping['patron_field'], '=')
      ->execute();
  }
  else {
    db_insert('opac_users_mapping')
      ->fields(array(
        'serv_id' => $mapping['serv_id'],
        'patron_field' => $mapping['patron_field'],
        'label_display' => $mapping['label_display'],
      ))
      ->execute();
  }
}
