<?php
/**
 * @file
 * Basic REST API to set and delete current user's shared Instagram user ID.
 */


/**
 * Menu callback for instagram_profile/user_id, an AJAX/REST API endpoint.
 */
function instagram_profile_ajax() {
  // Get the input variables.  E.g. $_POST for POST method.
  parse_str(file_get_contents('php://input'), $input);

  // Check access.
  if (!user_access('share own instagram feed')) {
    user_is_logged_in()
      ? _instagram_profile_http_status(403, 'Forbidden')
      : _instagram_profile_http_status(401, 'Please log in');
  }
  // Validate the CSRF token.
  else if (empty($input['csrf_token'])) {
    _instagram_profile_http_status(400, 'Token is required');
  }
  else if ($input['csrf_token'] !== $_SESSION['instagram_csrf_token']) {
    _instagram_profile_http_status(403, 'Token is invalid');
  }
  // Validation passed, process the inputs.
  else {
    $status = _instagram_profile_ajax($input);
    if (is_array($status)) {
      call_user_func_array('_instagram_profile_http_status', $status);
    }
    else {
      // Handle other unexpected failures.
      _instagram_profile_http_status(500, 'Internal server error');
    }
  }
}

/**
 * Process input data for both POST (save) and DELETE methods.
 */
function _instagram_profile_ajax($input) {
  $method = $_SERVER['REQUEST_METHOD'];
  if ('DELETE' === $method) {
    if (!instagram_profile_has_instagram($GLOBALS['user']->uid)) {
      return array(404, 'Current user already has no instagram_user_id');
    }
    // There is no additional validation to do.  Process the value.
    else if (instagram_profile_delete_user_id($id)) {
      // Success!
      return array(204, 'Deleted');
    }
  }
  else if ('POST' === $method) {
    // Validate the input Instagram user ID.
    if (empty($input['instagram_user_id'])) {
      return array(400, 'instagram_user_id is required');
    }
    else if (!_instagram_profile_validate_user_id($input['instagram_user_id'])) {
      return array(400, 'instagram_user_id must be a positive integer');
    }
    // Validation passed.  Save the new value.
    else if (instagram_profile_save_user_id($input['instagram_user_id'])) {
      // Success!
      return array(201, 'Saved');
    }
  }
}

/**
 * Saves the currently logged in user's Instagram user ID.
 *
 * @param $id Integer
 *    The (new) Instagram user ID.
 *
 * @return Boolean
 *   TRUE on success.
 */
function instagram_profile_save_user_id($id) {
  // A db_merge is also known as an UPSERT query.
  // https://drupal.org/node/310085
  $success = db_merge('instagram_profile')
    ->key(array('drupal' => $GLOBALS['user']->uid))
    ->fields(array('instagram' => $id))
    ->execute();

  return in_array($success, array(MergeQuery::STATUS_UPDATE, MergeQuery::STATUS_INSERT));
}

/**
 * Deletes the currently logged in user's Instagram user ID.
 *
 * @return Boolean
 *   TRUE on success.
 */
function instagram_profile_delete_user_id() {
  return db_delete('instagram_profile')
    ->condition('drupal', $GLOBALS['user']->uid)
    ->execute();
}

// Sets the HTTP response status code and message.
function _instagram_profile_http_status($code, $status) {
  drupal_add_http_header('Status', "$code $status");
}
