<?php

/**
 * @file
 * Eloqua Helper functions and constants
 */

// Eloqua statuses.
define('ELOQUA_STATUS_NEW', 'new');
define('ELOQUA_STATUS_FAILED', 'failed');
define('ELOQUA_STATUS_UPLOADED', 'uploaded');

// Default Eloqua posting interval.
define('ELOQUA_POST_INTERVAL_DEFAULT', 86400);

/**
 * Loads webform settings from the database.
 *
 * @param int $nid
 *   The webform node ID.
 *
 * @return object
 *   The webform.
 */
function eloqua_webform_load($nid) {
  // Get the raw result from the DB.
  if (!is_numeric($nid)) {
    $type = gettype($nid);
    watchdog('eloqua', 'Invalid argument sent to !module_name (!type).',
      array('!module_name' => __FUNCTION__, '!type' => $type), WATCHDOG_DEBUG);
    return NULL;
  }

  // Load webform.
  $result = db_select('eloqua_webform')
    ->fields('eloqua_webform')
    ->condition('nid', $nid)
    ->execute()
    ->fetchAll(PDO::FETCH_OBJ);
  $result = _eloqua_unserialize_data_column($result);
  $webform = array_shift($result);

  if (empty($webform)) {
    return NULL;
  }

  // Send the object around to all of its friends.
  foreach (module_implements('eloqua_form_load') as $module_name) {
    $method = $module_name . '_eloqua_form_load';
    $method($webform);
  }

  return $webform;
}

/**
 * Creates webform settings from the database.
 *
 * @hook eloqua_form_create
 *
 * @param object $webform
 *   The webform.
 *
 * @return bool
 *   The result of the update.
 */
function eloqua_webform_create($webform) {
  if (!is_object($webform)) {
    $type = gettype($webform);
    watchdog('eloqua', 'Invalid argument sent to !module_name (!type).',
      array('!module_name' => __FUNCTION__, '!type' => $type));
    return FALSE;
  }

  $index = array(
    'nid' => $webform->{'nid'},
  );
  $fields = array(
    'form_name' => $webform->{'form_name'},
    'is_active' => $webform->{'is_active'},
    'data' => serialize($webform->{'data'}),
  );

  $result = _eloqua_db_insert_update($index, $fields);

  // If unable to create the payment plan, end.
  if (!$result) {
    return FALSE;
  }

  foreach (module_implements('eloqua_form_create') as $module_name) {
    $method = $module_name . '_eloqua_form_create';
    $method($webform);
  }

  _eloqua_webform_update($webform);

  return $result;
}

/**
 * Updates a webform settings from the database.
 *
 * @hook eloqua_form_update
 *
 * @param object $webform
 *   The webform.
 *
 * @return bool
 *   The result of the update.
 */
function eloqua_webform_update($webform) {
  if (!is_object($webform)) {
    $type = gettype($webform);
    watchdog('eloqua', 'Invalid argument sent to !module_name (!type).',
      array('!module_name' => __FUNCTION__, '!type' => $type), WATCHDOG_DEBUG);
    return FALSE;
  }

  // Can't update the obvious invalid ppid of '0'...
  if (empty($webform->nid)) {
    return FALSE;
  }

  foreach (module_implements('eloqua_form_update') as $module_name) {
    $method = $module_name . '_eloqua_form_update';
    $method($webform);
  }

  // Save the result to the database and return the result.
  return _eloqua_webform_update($webform);
}

/**
 * Deletes a webform settings object from the database.
 *
 * @param int $nid
 *   The node ID.
 *
 * @return bool
 *   The result of the deletion.
 */
function eloqua_webform_delete($nid) {
  if (!is_numeric($nid)) {
    $type = gettype($nid);
    watchdog('eloqua', 'Invalid argument sent to !module_name (!type).',
      array('!module_name' => __FUNCTION__, '!type' => $type), WATCHDOG_DEBUG);
    return FALSE;
  }

  // Can't update the obvious invalid ppid of '0'...
  if (empty($nid)) {
    return FALSE;
  }

  foreach (module_implements('eloqua_form_delete') as $module_name) {
    $method = $module_name . '_eloqua_form_delete';
    $method($nid);
  }

  $result = db_delete('eloqua_webform')
    ->condition('nid', $nid)
    ->execute();

  return (is_numeric($result)) ? ($result > 0) : FALSE;
}

/**
 * Updates the webform object from the database.
 *
 * @param object $webform
 *   The webform.
 *
 * @return bool
 *   The result of the update.
 */
function _eloqua_webform_update($webform) {
  $fields = array(
    'form_name' => $webform->{'form_name'},
    'is_active' => $webform->{'is_active'},
    'data' => serialize($webform->{'data'}),
  );
  $result = db_update('eloqua_webform')
    ->fields($fields)
    ->condition('nid', $webform->{'nid'})
    ->execute();

  return (is_numeric($result)) ? ($result > 0) : FALSE;
}

/**
 * Loads a post form the database.
 *
 * @hook eloqua_post_load ($post)
 *
 * @param int $post_id
 *   The post ID.
 *
 * @return object
 *   The post object.
 */
function eloqua_post_load($post_id) {
  // Get the raw result from the DB.
  if (!is_numeric($post_id)) {
    $type = gettype($post_id);
    watchdog('eloqua', 'Invalid argument sent to !module_name (!type).',
      array('!module_name' => __FUNCTION__, '!type' => $type), WATCHDOG_DEBUG);
    return NULL;
  }

  $result_set = db_select('eloqua_saved_posts')
    ->condition('post_id', $post_id)
    ->execute()
    ->fetchAll();
  $result = _eloqua_unserialize_data_column($result_set);
  $post = array_shift($result);

  if (empty($post)) {
    return NULL;
  }

  // Send the object around to all of its friends.
  foreach (module_implements('eloqua_post_load') as $module_name) {
    $method = $module_name . '_eloqua_post_load';
    $method($post);
  }

  return $post;
}

/**
 * Creates a post into the database.
 *
 * @hook eloqua_post_create
 *
 * @param object $post
 *   The post object.
 *
 * @return int
 *   The post ID.
 */
function eloqua_post_create($post) {
  if (!is_object($post)) {
    $type = gettype($post);
    watchdog('eloqua', 'Invalid argument sent to !module_name (!type).',
      array('!module_name' => __FUNCTION__, '!type' => $type));
    return FALSE;
  }

  $fields = array(
    'form_id' => $post->{'form_id'},
    'post_time' => $post->{'post_time'},
    'status' => $post->{'status'},
    'data' => serialize($post->{'data'}),
  );
  $result = db_insert('eloqua_saved_posts')
    ->fields($fields)
    ->execute();
  // The result must be the post_id or we'll end up with duplicate records in
  // the eloqua_saved_posts table.
  if ($result === FALSE || $result === NULL) {
    $result = FALSE;
  }

  if (!$result) {
    // If unable to create the payment plan, end.
    return FALSE;
  }

  $post->{'post_id'} = $result;

  foreach (module_implements('eloqua_post_create') as $module_name) {
    $method = $module_name . '_eloqua_post_create';
    $method($post);
  }

  _eloqua_post_update($post);

  return $post;
}

/**
 * Updates a post from the database.
 *
 * @hook eloqua_post_update
 *
 * @param object $post
 *   The post object.
 *
 * @return bool
 *   The result of the update.
 */
function eloqua_post_update($post) {
  if (!is_object($post)) {
    $type = gettype($post);
    watchdog('eloqua', 'Invalid argument sent to !module_name (!type).',
      array('!module_name' => __FUNCTION__, '!type' => $type), WATCHDOG_DEBUG);
    return FALSE;
  }

  // Can't update the obvious invalid ppid of '0'...
  if (empty($post->{'post_id'})) {
    return FALSE;
  }

  foreach (module_implements('eloqua_post_update') as $module_name) {
    $method = $module_name . '_eloqua_post_update';
    $method($post);
  }

  // Save the result to the database and return the result.
  return _eloqua_post_update($post);
}

/**
 * Deletes a post object from the database.
 *
 * @param int $post_id
 *   The post ID.
 *
 * @return bool
 *   The result of the deletion.
 */
function eloqua_post_delete($post_id) {
  if (!is_numeric($post_id)) {
    $type = gettype($post_id);
    watchdog('eloqua', 'Invalid argument sent to !module_name (!type).',
      array('!module_name' => __FUNCTION__, '!type' => $type), WATCHDOG_DEBUG);
    return FALSE;
  }

  // Can't update the obvious invalid ppid of '0'...
  if (empty($post_id)) {
    return FALSE;
  }

  foreach (module_implements('eloqua_post_delete') as $module_name) {
    $method = $module_name . '_eloqua_post_delete';
    $method($post_id);
  }

  $result = db_delete('eloqua_saved_posts')
    ->condition('post_id', $post_id)
    ->execute();

  return (is_numeric($result)) ? ($result > 0) : FALSE;
}

/**
 * Updates the post object from the database.
 *
 * @param object $post
 *   The post object.
 *
 * @return bool
 *   The result of the update.
 */
function _eloqua_post_update($post) {
  $fields = array(
    'form_id' => $post->{'form_id'},
    'post_time' => $post->{'post_time'},
    'status' => $post->{'status'},
    'data' => serialize($post->{'data'}),
  );
  $result = db_update('eloqua_saved_posts')
    ->fields($fields)
    ->condition('post_id', $post->{'post_id'})
    ->execute();

  return (is_numeric($result)) ? ($result > 0) : FALSE;
}

/**
 * Goes through the result set and unserialises any 'data' columns.
 *
 * @param array $result_set
 *   The result set.
 *
 * @return array
 *   The result set with unserialized data.
 */
function _eloqua_unserialize_data_column($result_set) {
  $result = array();
  if (!is_array($result_set)) {
    $result_set = array();
  }

  foreach ($result_set as $row) {
    $row->data = unserialize($row->data);
    $result[] = $row;
  }

  return $result;
}

/**
 * Query the Database and mimic a INSERT ... ON DUPLICATE KEY UPDATE.
 *
 * Because there isn't a "Drupal" way to do an INSERT ... ON DUPLICATE KEY
 * UPDATE using the abstraction layer they provided.
 *
 * Also, when using InnoDB tables, you cannot actually retrieve the
 * last-insert-id reliably. So, all in all, this just returns a TRUE/FALSE now,
 * as opposed to the Drupal 6 Version.
 *
 * @param array $index
 *   The node ID to update.
 * @param array $fields
 *   The fields to update.
 *
 * @return bool
 *   The result of the update.
 */
function _eloqua_db_insert_update($index, $fields) {
  $insert_fields = $index + $fields;
  $do_update = FALSE;
  $result = FALSE;
  try {
    $result = db_insert('eloqua_webform')
      ->fields($insert_fields)
      ->execute();

    // No longer able to return the last inserted ID.
    if ($result !== FALSE) {
      $result = TRUE;
    }
  }
  catch (PDOException $e) {
    // Constraint error.
    if ($e->getCode() == '23000') {
      $do_update = TRUE;
    }
  }

  // Attempt update.
  if ($do_update) {
    $query = db_update('eloqua_webform')
      ->fields($fields);
    foreach ($index as $key => $value) {
      $query->condition($key, $value);
    }

    try {
      $result = $query->execute();
    }
    catch (PDOException $e) {
      return FALSE;
    }

    // Rows updated.
    if (is_numeric($result)) {
      $result = (bool) $result;
    }
    else {
      // Probably failed.
      $result = FALSE;
    }
  }

  return $result;
}
