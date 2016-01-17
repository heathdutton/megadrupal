<?php

/**
 * @file
 * XML-RPC callback, these will be loaded only when someone hits the xmlrpc.php
 */

/**
 * Callback to return node information.
 */
function sentry_client_nodeinfo_xmlc($key) {
  if (sentry_client_validate($key)) {
    if ($nodeinfo = _sentry_client_nodeinfo()) {
      return $nodeinfo;
    }
    else {
      return array('error' => t('No node info available.'));
    }
  }
}

/**
 * Get encrypted and serialised data for passing back to sentry server.
 *
 * @global string $base_url
 *   Site base url
 * @global type $base_path
 *   Site base path
 *
 * @return string
 *   Encrypted and serialised array of nodeinfo
 */
function _sentry_client_nodeinfo() {

  // New nodeinfo array.
  $nodeinfo = array();

  // Get content types list.
  module_load_include('module', 'node');
  $content_types = array();
  foreach (node_type_get_types() as $type_data) {
    $content_types[$type_data->type] = $type_data;
  }

  // Entity => table.
  $entities = array(
    'nodes' => 'node',
    'comments' => 'comment',
    'users' => 'users',
  );

  foreach ($entities as $entity => $table) {

    // Total.
    $sql = 'SELECT ';
    $sql .= ($entity == 'nodes') ? 'type, ' : '';
    $sql .= 'COUNT(' . $table[0] . 'id) AS total FROM {' . $table . '} ';
    $sql .= 'WHERE status = :status ';
    $sql .= ($entity == 'nodes') ? 'GROUP BY type ORDER BY type' : '';

    $result = db_query($sql, array(':status' => 1));

    if ($entity == 'nodes') {
      foreach ($result as $record) {
        $type_alias = ($content_types[$record->type]->name != '') ? $content_types[$record->type]->name : $record->type;
        $nodeinfo[$entity]['total'][$type_alias] = $record->total;
      }
    }
    else {
      $nodeinfo[$entity]['total'] = $result->fetchObject()->total;
    }

    // Total created in last 24 hours.
    if ($entity == 'nodes') {
      $sql = '
        SELECT a.type, COUNT(b.type) AS total
        FROM {' . $table . '} AS a
        LEFT OUTER JOIN {' . $table . '} AS b
        ON a.nid = b.nid AND b.created >= :time AND b.status = :status
        GROUP BY a.type
        ORDER BY a.type
      ';
    }
    else {
      $sql = '
        SELECT COUNT(' . $table[0] . 'id) AS total
        FROM {' . $table . '}
        WHERE created >= :time AND status = :status
      ';
    }
    $result = db_query($sql, array(':time' => (REQUEST_TIME - 86400), ':status' => 1));

    if ($entity == 'nodes') {
      foreach ($result as $record) {
        $type_alias = ($content_types[$record->type]->name != '') ? $content_types[$record->type]->name : $record->type;
        $nodeinfo[$entity]['total_last_day'][$type_alias] = $record->total;
      }
    }
    else {
      $nodeinfo[$entity]['total_last_day'] = $result->fetchObject()->total;
    }

    // Last 5 created.
    $result = db_query('
      SELECT *
      FROM {' . $table . '}
      WHERE status = :status
      ORDER BY created DESC
      LIMIT 0, 5',
      array(':status' => 1)
    );
    foreach ($result as $i => $record) {
      // Removing sensible data from user.
      if ($entity == 'users') {
        unset($record->pass);
        unset($record->init);
        $table = 'user';
      }

      // Adding url alias.
      global $base_url, $base_path;
      $r = get_object_vars($record);
      $record->url_alias = $base_url . $base_path . drupal_get_path_alias($table . '/' . $r[$table[0] . 'id']);

      // Adding type alias to nodes.
      if ($entity == 'nodes') {
        $record->type_alias = ($content_types[$record->type]->name != '') ? $content_types[$record->type]->name : $record->type;
      }

      if ($entity == 'comments') {
        $record->url_alias .= '#comment-' . $r[$table[0] . 'id'];
      }

      $nodeinfo[$entity]['last_5'][$i] = $record;
    }
  }

  // Stats encryption.
  $data = array();
  $key = variable_get('sentry_client_key', NULL);
  $encryption = _sentry_client_nodeinfo_encrypt($nodeinfo, $key);
  $data['stats'] = $encryption['data'];
  $data['encryption'] = $encryption['encryption'];

  return $data;

}


/**
 * Helper function to encrypt data.
 *
 * @param array $data
 *   Unencrypted nodeinfo array.
 * @param string $key
 *   User's identification key.
 *
 * @return string
 *   Encrypted data
 */
function _sentry_client_nodeinfo_encrypt($data, $key) {
  if (is_array($data)) {
    $data = serialize($data);
  }
  if (extension_loaded('mcrypt')) {
    // Use mcrypt.
    $encrypted_data['data'] = base64_encode(mcrypt_encrypt(MCRYPT_RIJNDAEL_256, md5($key), $data, MCRYPT_MODE_CBC, md5(md5($key))));
    $encrypted_data['encryption'] = 'mcrypt';
  }
  else {
    // Use custom encryption.
    $encrypted_data['data'] = _sentry_client_nodeinfo_custom_encrypt($data, $key);
    $encrypted_data['encryption'] = 'custom';
  }
  return $encrypted_data;
}


/**
 * Helper function to encrypt in case mcrypt is not enabled.
 *
 * @param array $data
 *   Unencrypted nodeinfo array.
 * @param string $key
 *   User's identification key.
 *
 * @return string
 *   Encrypted data
 */
function _sentry_client_nodeinfo_custom_encrypt($data, $key) {
  $result = '';
  for ($i = 0; $i < drupal_strlen($data); $i++) {
    $char = drupal_substr($data, $i, 1);
    $keychar = drupal_substr($key, ($i % drupal_strlen($key)) - 1, 1);
    $char = chr(ord($char) + ord($keychar));
    $result .= $char;
  }
  return base64_encode($result);
}
