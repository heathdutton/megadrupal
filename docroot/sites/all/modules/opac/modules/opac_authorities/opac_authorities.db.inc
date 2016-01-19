<?php
/**
 * @file
 * Database function call for opac authorities module.
 */

/**
 * Get all authority ids.
 *
 * @return array
 *   A 2-levels associative array that contains authority ids
 *   1st-level keys are server ids.
 *   2nd-level keys are server authids.
 *   Example:
 *     $authorities = array(
 *       'server_id1' => array('
 *         'server_authid1' => 42,
 *         // ...
 *       ),
 *     );
 */
function opac_authorities_get_ids_assoc() {
  $rows = db_select('opac_authority', 'oa')
    ->fields('oa')
    ->execute()
    ->fetchAll();
  $auths = array();
  foreach ($rows as $row) {
    $auths[$row->server_id][$row->server_authid] = $row->authid;
  }
  return $auths;
}

/**
 * Add an authority-type for an ILS server.
 *
 * @param array $values
 *   A array containing authority type values.
 */
function opac_authorities_server_type_add($values) {
  $result = db_insert('opac_authorities_server_types')
    ->fields(array(
      'serv_id' => $values['serv_id'],
      'authtype' => $values['authtype'],
      'matching_field' => $values['matching_field'],
      'value' => $values['value'],
      'weight' => $values['weight'],
    ))
    ->execute();
}

/**
 * Edit a authority-type for an ILS server.
 *
 * @param array $values
 *   A array containing content type values.
 */
function opac_authorities_server_type_edit($values) {
  $result = db_update('opac_authorities_server_types')
    ->fields(array(
      'matching_field' => $values['matching_field'],
      'value' => $values['value'],
      'weight' => $values['weight'],
    ))
    ->condition('serv_id', $values['serv_id'])
    ->condition('authtype', $values['authtype'])
    ->execute();
}

/**
 * Get machine names of all authority types used by ILS servers.
 *
 * @parma string $serv_id
 *   ILS server identifier.
 *
 * @parma string $authtype
 *   Machine name of the auhtority type.
 *
 * @parma string $fieldname
 *   Node field name.
 *
 * @return array
 *   machine names of authority types
 */
function opac_authorities_get_server_types($serv_id, $authtype = NULL, $fieldname = NULL) {
  $query = db_select('opac_authorities_server_types', 'at')
    ->fields('at')
    ->orderBy('weight', 'ASC')
    ->condition('serv_id', $serv_id);

  if ($authtype) {
    $query->condition('authtype', $authtype);
  }

  if ($fieldname) {
    $query->condition('matching_field', $fieldname);
  }

  $result = $query->execute();

  $server_types = array();
  while ($type = $result->fetchAssoc()) {
    $server_types[$type['authtype']] = $type;
  }
  return $server_types;
}

/**
 * Delete a authority-type for an ILS server.
 *
 * @param array $values
 *   A array containing authority type values.
 */
function opac_authorities_server_type_delete($values) {
  $mapping = opac_authorities_build_fields_mapping($values['serv_id'], $values['authtype']);
  $result = db_delete('opac_authorities_server_types')
    ->condition('serv_id', $values['serv_id'])
    ->condition('authtype', $values['authtype'])
    ->execute();
  foreach ($mapping['mapped'] as $values) {
    opac_authorities_mapping_del($values);
  }
}

/**
 * Build a fields mapping for a given authority type/ILS server.
 *
 * Arrange fields in mapped or unmapped list.
 *
 * @param string $serv_id
 *   ILS server identifier.
 *
 * @param string $authority_type_name
 *   Authority type name.
 *
 * @return array
 *   Fields mapping.
 */
function opac_authorities_build_fields_mapping($serv_id, $authority_type_name = NULL) {
  $cache_name = 'opac_authorities_fields_mapping_' . $serv_id . '_' . $authority_type_name;
  $sorted_fields;

  if ($cache = cache_get($cache_name)) {
    $sorted_fields = $cache->data;
  }
  else {
    $authority_types = opac_authorities_get_server_types($serv_id, $authority_type_name);

    $sorted_fields = array('unmapped' => array(), 'mapped' => array());
    foreach ($authority_types as $authority_type) {
      // Get the fields attached to this to the content_type.
      $entity_fields = field_info_instances('opac_authority', $authority_type['authtype']);
      $entity_fields['title'] = array(
        'label' => t('Authority title'),
        'field_name' => 'title',
        'bundle' => $authority_type['authtype'],
      );

      // Create a 'mapped' and 'unmapped' list.
      // Mapped and unmapped field will not be displayed at the same place.
      foreach ($entity_fields as $name => $field) {
        // Retrieves the ILS/connector fields mapped with $name.
        // The $name var is the node field name.
        // The $mapped_field var is the ILS/connector field name.
        $mapped_field = opac_authorities_get_mapping($serv_id, $authority_type['authtype'], $name);
        // The field is already mapped.
        // Adds operations link for this mapping.
        if ($mapped_field) {
          $mapped_field['operations'] = l(t('edit'),
            "admin/config/opac/server/$serv_id/authority-mapping/" . $authority_type['authtype']
            . "/$name/edit")
            . "&nbsp;" . l(t('delete'),
            "admin/config/opac/server/$serv_id/authority-mapping/" . $authority_type['authtype']
            . "/$name/delete");
          $sorted_fields['mapped'][$name] = $mapped_field;
        }
        // Remaining unmapped fields.
        else {
          $field['authtype'] = $authority_type['authtype'];
          $sorted_fields['unmapped'][$name] = $field;
        }
      }
    }
    cache_set($cache_name, $sorted_fields, 'cache');
  }
  return $sorted_fields;
}

/**
 * Get mapping for a given ILS server authtype and field.
 *
 * @param string $serv_id
 *   ILS server machine name.
 *
 * @param string $authority_type_name
 *   An authority type machine name.
 *
 * @param string $fieldname
 *   A field machine name.
 *
 * @return array
 *   mapping for field that match the conditions.
 */
function opac_authorities_get_mapping($serv_id, $authority_type_name, $fieldname) {
  $mappings = opac_authorities_get_mappings(array(
    'servers' => $serv_id,
    'authority_types' => $authority_type_name,
    'fields' => $fieldname,
  ));
  return count($mappings) ? $mappings[0] : '';
}

/**
 * Search authorities mappings.
 *
 * @param array $filters
 *   An associative array which contains filters.
 *   Each filter can be given as an array or a simple string.
 *   Available filters are:
 *   - servers
 *   - authority_types
 *   - fields
 *
 * @return array
 *   List of mappings.
 */
function opac_authorities_get_mappings($filters = array()) {
  $sql_names = array(
    'servers' => 'serv_id',
    'authority_types' => 'authtype',
    'fields' => 'authority_field_name',
  );
  $query = db_select('opac_authority_mapping', 'am')->fields('am');
  foreach ($sql_names as $filters_key => $sql_name) {
    if (isset($filters[$filters_key])) {
      $filter = $filters[$filters_key];
      if (!is_array($filter)) {
        $filter = array($filter);
      }
      $query->condition($sql_name, $filter, 'IN');
    }
  }
  return $query->execute()->fetchAll(PDO::FETCH_ASSOC);
}

/**
 * Add a mapping for an ILS server / authtype.
 *
 * @param array $mapping
 *   A array containing mapping values whose server identifier.
 */
function opac_authorities_mapping_add($mapping) {
  $serv_id = db_insert('opac_authority_mapping')
    ->fields(array(
      'serv_id' => $mapping['serv_id'],
      'authtype' => $mapping['authtype'],
      'authority_field_name' => $mapping['authority_field_name'],
      'authority_field_label' => $mapping['authority_field_label'],
      'mapped_with' => $mapping['mapped_with'],
      'vid' => $mapping['vid'],
      'vocabulary_machine_name' => $mapping['vocabulary_machine_name'],
      'vocabulary_name' => $mapping['vocabulary_name'],
      'term_field' => $mapping['term_field'],
      'term_field_label' => $mapping['term_field_label'],
      'nomatch_rule' => $mapping['nomatch_rule'],
    ))
    ->execute();
}

/**
 * Perform a mapping modification.
 *
 * @param array $mapping
 *   A array containing mapping values whose server identifier.
 */
function opac_authorities_mapping_mod($mapping) {
  $serv_id = db_update('opac_authority_mapping')
    ->fields(array(
      'mapped_with' => $mapping['mapped_with'],
      'vid' => $mapping['vid'],
      'vocabulary_machine_name' => $mapping['vocabulary_machine_name'],
      'vocabulary_name' => $mapping['vocabulary_name'],
      'term_field' => $mapping['term_field'],
      'term_field_label' => $mapping['term_field_label'],
      'nomatch_rule' => $mapping['nomatch_rule'],
    ))
    ->condition('serv_id', $mapping['serv_id'], '=')
    ->condition('authtype', $mapping['authtype'], '=')
    ->condition('authority_field_name', $mapping['authority_field_name'], '=')
    ->execute();
}

/**
 * Delete a mapping.
 *
 * @param array $mapping
 *   A array containing mapping values whose server identifier.
 */
function opac_authorities_mapping_del($mapping) {
  $num_deleted = db_delete('opac_authority_mapping')
    ->condition('serv_id', $mapping['serv_id'], '=')
    ->condition('authtype', $mapping['authtype'], '=')
    ->condition('authority_field_name', $mapping['authority_field_name'], '=')
    ->execute();
}
