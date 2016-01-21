<?php
/**
 * @file
 * Sample hooks demonstrating usage of Raven.
 */

/**
 * Provide user information for logging.
 *
 * @param array $user_info
 *   A reference to array of user account info.
 */
function hook_raven_user_alter(array &$user_info) {
  global $user;
  if (user_is_logged_in()) {
    $user_info['id'] = $user->uid;
    $user_info['name'] = $user->name;
    $user_info['email'] = $user->mail;
    $user_info['roles'] = implode(', ', $user->roles);
  }
}

/**
 * Provide tags for logging.
 *
 * @param array $tags
 *   A reference to array of sentry tags.
 */
function hook_raven_tags_alter(array &$tags) {
  $tags['foo_version'] = get_foo_version();
}

/**
 * Provide extra information for logging.
 *
 * @param array $extra
 *   A reference to array of extra error info.
 */
function hook_raven_extra_alter(array &$extra) {
  $extra['foo'] = 'bar';
}

/**
 * Filter known errors so do not log them to Sentry again and again.
 *
 * @param array $error
 *   A reference to array containing error info.
 */
function hook_raven_error_filter_alter(array &$error) {
  $known_errors = array();

  drupal_alter('raven_known_php_errors', $known_errors);

  // Filter known errors to prevent spamming the Sentry server.
  foreach ($known_errors as $known_error) {
    $check = TRUE;

    foreach ($known_error as $key => $value) {
      if ($error[$key] != $value) {
        $check = FALSE;
        break;
      }
    }

    if ($check) {
      $error['process'] = FALSE;
      break;
    }
  }
}

/**
 * Provide the list of known PHP errors.
 *
 * @param array $known_errors
 *   A reference to array containing PHP errors info.
 */
function hook_raven_known_php_errors_alter(array &$known_errors) {
  $known_errors[] = array(
    'code' => E_NOTICE,
    'message' => 'Array to string conversion',
    'file' => DRUPAL_ROOT . '/sites/all/modules/views/plugins/views_plugin_cache.inc',
    'line' => 206,
  );

  $known_errors[] = array(
    'code' => E_NOTICE,
    'message' => 'Undefined index: width',
    'file' => DRUPAL_ROOT . '/sites/all/modules/flexslider/flexslider_fields/flexslider_fields.module',
    'line' => 140,
  );

  $known_errors[] = array(
    'code' => E_NOTICE,
    'message' => 'Undefined index: height',
    'file' => DRUPAL_ROOT . '/sites/all/modules/flexslider/flexslider_fields/flexslider_fields.module',
    'line' => 141,
  );
}

/**
 * Filter known watchdog entries so do not log them to Sentry again and again.
 *
 * @param array $filter
 *   A reference to array containing log entry info.
 */
function hook_raven_watchdog_filter_alter(array &$filter) {
  $log_entry = $filter['log_entry'];
  if ($log_entry['type'] === 'foo') {
    $filter['process'] = FALSE;
  }
}

/**
 * Alter the array of fields that should be sanitized.
 *
 * The field will be used in a regular expression, so you may need to run
 * preg_quote($field, '/') on the field name.
 *
 * @param array $fields
 *   A reference to array containing the field names.
 */
function hook_raven_sanitize_fields_alter(array &$fields) {
  $fields[] = 'data';
}
