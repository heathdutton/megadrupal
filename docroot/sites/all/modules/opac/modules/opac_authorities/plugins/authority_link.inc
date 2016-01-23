<?php

/**
 * @file
 * Authority link plugin.
 *
 * This plugin allows to link biblios with authorities.
 *
 * It only works with entityreference fields.
 */

/**
 * Callback 'available' for Authority link plugin.
 */
function opac_authorities_plugin_authority_link_available($server, $field) {
  if ($field['type'] == 'entityreference'
  && $field['settings']['target_type'] == 'opac_authority') {
    return TRUE;
  }
  return FALSE;
}

/**
 * Callback 'value' for Authority link plugin.
 */
function opac_authorities_plugin_authority_link($record, $mapping) {
  $remotefield = $mapping['mapped_with'];
  $field = array('und' => array());

  if (array_key_exists($remotefield, $record)) {
    $field_info = field_info_field($mapping['node_field_name']);
    if ($field_info['settings']['target_type'] == 'opac_authority') {
      $target_bundles = array_keys($field_info['settings']['handler_settings']['target_bundles']);
      $query = "
        SELECT authid
        FROM {opac_authority}
        WHERE server_id = :server_id
          AND server_authid = :server_authid
      ";
      $bind_params = array();
      if (count($target_bundles) > 0) {
        $query .= "AND authtype IN (:bundles)";
        $bind_params[':bundles'] = $target_bundles;
      }

      foreach ($record[$remotefield] as $remoteid) {
        $bind_params = array_merge($bind_params, array(
          ':server_id' => $mapping['serv_id'],
          ':server_authid' => $remoteid,
        ));
        $result = db_query($query, $bind_params);
        $count = $result->rowCount();
        if ($count == 1) {
          $auth = $result->fetch();
          $field['und'][] = array('target_id' => $auth->authid);
        }
        elseif ($count > 1) {
          // This should never happen.
          error_log("Found more than 1 authority (server_id: "
            . $mapping['serv_id'] . ", server_authid: $remoteid)");
        }
      }
    }
  }

  return $field;
}
