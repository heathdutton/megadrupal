<?php

/**
 * @file
 * Registration API functions.
 */

/**
 * Returns all node registration bundles in the node_registration type.
 */
function _node_registration_bundles() {
  $bundles = _node_registration_node_types();

  $nodes = db_query('SELECT nid FROM {node_registration_node} WHERE private_fields <> 0');
  foreach ($nodes as $node) {
    $nid = $node->nid;

    $type = 'node_' . $nid;
    $name = 'Node # ' . $nid;

    $bundles[$type] = $name;
  }

  return $bundles;
}

/**
 * Returns a message for the strtotime format to show the user in admin forms.
 */
function _node_registration_strtotime_debug($setting_value, $relative_utc = 0) {
  $YMD = 'Y-m-d H:i:s';
  $relative_utc or $relative_utc = REQUEST_TIME;
  $relative_datetime = date('Y-m-d H:i:s', $relative_utc);

  $time = _node_registration_strtotime($relative_datetime, $setting_value);

  return
    date($YMD, $relative_utc) . ' ' .
    _node_registration_strtotime_format($setting_value) . ' = ' .
    date($YMD, $time);
}

/**
 * Calculates a time from a reference time and strotime offset.
 */
function _node_registration_strtotime($reference, $offset) {
  $offset = _node_registration_strtotime_format($offset);

  if (is_numeric($reference)) {
    return strtotime($offset, $reference);
  }

  return strtotime($reference . ' ' . $offset);
}

/**
 * Reformats strtotime format to all negatives (to be used in strtotime()).
 */
function _node_registration_strtotime_format($format) {
  $format = preg_replace('#(^|[^\+\d\-])(\d+)#', '$1-$2', $format);
  return $format;
}

/**
 * Helper: whether the current request is made with XHR.
 */
function _node_registration_request_is_ajax() {
  return isset($_SERVER['HTTP_X_REQUESTED_WITH']) && strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest';
}

/**
 * Reset waitinglist flags for an event.
 */
function _node_registration_reset_waitinglist($node) {
  $capacity = (int) $node->registration->capacity();

  $registration_ids = db_select('node_registration', 'nr')
    ->fields('nr', array('registration_id'))
    ->condition('nid', $node->nid)
    ->condition('cancelled', 0)
    ->orderBy('weight', 'ASC')
    ->addTag('nr_waitinglist')
    ->execute()
    ->fetchCol();
  $registrations = node_registration_load_multiple($registration_ids);

  $change = FALSE;

  $slots = 0;
  foreach ($registrations as $registration) {
    $slots += $registration->slots;

    $old_waitinglist = (bool) $registration->waitinglist;
    $new_waitinglist = $capacity && $slots > $capacity;

    // Waitinglist status changed.
    if ($old_waitinglist != $new_waitinglist) {
      $registration->waitinglist = (int) $new_waitinglist;
      $registration->save();

      // Save change for return value.
      $change = TRUE;
    }
  }

  return $change;
}

/**
 * The size of an event's waitinglist. Always >= 0.
 */
function _node_registration_waitinglist_size($node) {
  $capacity = $node->registration->capacity();
  if ($capacity) {
    $slots = db_query('SELECT SUM(slots) FROM {node_registration} WHERE nid = ? AND cancelled = 0', array($node->nid))->fetchField();

    if ($slots >= $capacity) {
      return $slots - $capacity;
    }
  }

  return 0;
}

/**
 * The current, active waitinglist of an event.
 */
function _node_registration_get_waitinglist($node) {
  $ids = db_select('node_registration', 'r')
    ->fields('r', array('registration_id'))
    ->condition('nid', $node->nid)
    ->condition('cancelled', 0)
    ->condition('waitinglist', 1)
    ->execute()
    ->fetchColumn();
  $registrations = node_registration_load_multiple($ids);
  ksort($registrations, SORT_NUMERIC);
  return $registrations;
}

/**
 * Helper to keep URI strings DRY.
 */
function _node_registration_type_to_uri($type) {
  return $type;
}

/**
 * Helper to keep TYPE strings DRY.
 */
function _node_registration_uri_to_type($uri) {
  return $uri;
}

/**
 * Saves or retrieves with events have been sent reminders.
 */
function _node_registration_reminders_sent($nids = array()) {
  $sent = variable_get('node_registration_reminders_sent', array());

  if ($nids) {
    $sent = array_merge($sent, (array) $nids);
    variable_set('node_registration_reminders_sent', $sent);
  }

  return $sent;
}

/**
 * Returns all event node objects (for certrain node types).
 */
function _node_registration_event_nodes($types = array()) {
  $types or $types = array_keys(_node_registration_node_types());

  $nodes = node_load_multiple(FALSE, array('type' => (array) $types));

  return $nodes;
}

/**
 * Default e-mail header From name.
 */
function _node_registration_default_from_name() {
  return variable_get('site_name');
}

/**
 * Send an email to all registrations for a given node.
 */
function node_registration_send_broadcast($node, $subject, $message, $registrations = array(), $options = array()) {
  // Registrations.
  $registrations or $registrations = node_registration_load_multiple(FALSE, array('nid' => $node->nid, 'cancelled' => 0));

  // Options.
  $notify = isset($options['notify']) ? $options['notify'] : FALSE;

  if ($registrations) {
    // Send e-mails.
    $sent_to = 0;
    foreach ($registrations as $registration) {
      $registration->node = $node;

      $token_data = array(
        'node' => $node,
        'node-registration' => $registration,
      );

      if (isset($options['alter'])) {
        $context = array(
          'node' => $node,
          'registration' => $registration,
        );
        $type = array(
          'node_registration_email',
          'node_registration_email_' . $options['alter'],
        );
        drupal_alter($type, $options, $context);
      }

      $result = _node_registration_send_email($registration->email, $subject, $message, $token_data, $options);
      if ($result) {
        $sent_to++;
      }
      else {
        // Log failure.
        watchdog('node_registration', 'Failed to send registration broadcast e-mail to %email.', array('%email' => $registration->email), WATCHDOG_ERROR);
      }
    }

    if ($sent_to) {
      $params = array('@total' => count($registrations), '@sent_to' => $sent_to);
      if ($notify) {
        // Notify user of success.
        drupal_set_message(t('The message has been sent to @sent_to / @total registrees.', $params));
      }

      // Log success.
      watchdog('node_registration', 'Registration e-mail sent to @sent_to / @total registrants.', $params);
    }
    else {
      if ($notify) {
        // Notify user of failure.
        drupal_set_message(t('There was an error sending the message.'), 'error');
      }
    }
  }
  else {
    if ($notify) {
      drupal_set_message(t('There are no participants registered for this %type.', array('%type' => $node->type)), 'warning');
    }
  }
}

/**
 * Internal send e-mail function.
 */
function _node_registration_send_email($recipient, $subject, $message, $token_data = array(), $options = array()) {
  global $language;

  // Set up from. Fill in later.
  $from_name = '';
  $from_email = '';

  // Auto complete token objects.
  if (isset($token_data['node-registration'])) {
    if (!empty($token_data['node-registration']->author_uid) && empty($token_data['node-registration']->author)) {
      $token_data['node-registration']->author = user_load($token_data['node-registration']->author_uid);
    }
    if (!empty($token_data['node-registration']->uid) && empty($token_data['node-registration']->user)) {
      $token_data['node-registration']->user = user_load($token_data['node-registration']->uid);
    }
    if (!empty($token_data['node-registration']->node) && empty($token_data['node'])) {
      $token_data['node'] = $token_data['node-registration']->node;
    }
  }

  // From name + e-mail.
  if (isset($token_data['from'])) {
    $from_name = $token_data['from']['from_name'];
    $from_email = $token_data['from']['from_email'];
  }
  // From node settings.
  elseif (isset($token_data['node'])) {
    $settings = $token_data['node']->registration;
    $settings->sender_name && $from_name = token_replace($settings->sender_name, $token_data, array('clear' => TRUE));
    $settings->sender_mail && $from_email = token_replace($settings->sender_mail, $token_data, array('clear' => TRUE));
  }

  // Add defaults and combine.
  $from_name or $from_name = _node_registration_default_from_name();
  $from_email or $from_email = variable_get('site_mail');
  $from = $from_name . ' <' . $from_email . '>';

  // Token data.
  if ($token_data) {
    $subject = token_replace($subject, $token_data, array('clear' => TRUE));
    $message = token_replace($message, $token_data, array('clear' => TRUE));
  }

  // Mail params.
  $params = array(
    'subject' => $subject,
    'message' => $message,
  ) + $options;
  $type = isset($options['alter']) ? $options['alter'] : 'broadcast';

  $result = drupal_mail('node_registration', $type, $recipient, $language, $params, $from);

  return $result['result'];
}

/**
 * Implements hook_node_access().
 *
 * Ignores existing node access permissions and creates a few new.
 */
function node_registration_node_access($node, $op, $account = NULL, &$reason = NULL) {
  if (!in_array($op, array('register', 'register others', 'registered', 'registration settings'))) {
    return;
  }

  global $user;
  $account or $account = $user;

  if (!is_object($node)) {
    $node = node_load((string) $node);
    if (!is_object($node)) {
      return;
    }
  }

  $nid = empty($node->nid) ? 0 : $node->nid;
  $nid_key = $nid ?: $node->type;

  $reasons = &drupal_static(__FUNCTION__ . ':reasons');

  $cache_key = __FUNCTION__ . ':' . $nid_key . ':' . $op . ':' . $account->uid;
  $access = _node_registration_cache($cache_key, function() use ($cache_key, $node, $nid, $op, $account, &$reason) {

    switch ($op) {
      case 'register':
        $registration = entity_get_controller('node_registration')->create(array(
          'nid' => $nid,
        ));
        return node_registration_access($registration, 'add', $account, $reason);
        break;

      case 'register others':
        return user_access('other node registration', $account);
        break;

      case 'registered':
        return ($registration = _node_registration_user_registered($node, $account)) && node_registration_access($registration, 'view', $account, $reason);
        break;

      case 'registration settings':
        return _node_registration_node_type_enabled($node->type) && user_access('administer node registration', $account);
        break;
    }

    return FALSE;

  }, function($result, $cache_key) use ($node, $op, $account, &$reason) {

    // Allow other modules to override and change this result.
    $reason or ($reason = $result ? '' : '?');
    $alt_access = module_invoke_all('node_registration_node_access', $node, $op, $account, $reason);
    if (in_array(!$result, $alt_access, TRUE)) {
      return !$result;
    }

    return $result;

  });

  if ($reason) {
    $reasons[$cache_key] = $reason;
  }
  else {
    $reason = @$reasons[$cache_key];
  }

  return $access;
}

/**
 * All Registration access callbacks. Just like node.module has node_access.
 */
function node_registration_access($registration, $op, $account = NULL, &$reason = NULL) {
  global $user;
  $account or $account = $user;

  $reasons = &drupal_static(__FUNCTION__ . ':reasons');

  $cache_key = implode(':', array(__FUNCTION__, $account->uid, $registration->nid, $registration->registration_id, $op));
  $access = _node_registration_cache($cache_key, function() use ($cache_key, $registration, $op, $account, &$reason) {

    // There's definitely a node involved.
    $node = $registration->node ? $registration->node : ($registration->node = node_load($registration->nid));
    $type = $node->type;
    $settings = $node->registration;
    $enabled = $settings->enabled();

    $debug = isset($_GET['debug']);
    $reason = '';

    $is_admin = user_access('administer node registration', $account);

    switch ($op) {
      case 'delete':
        // Must be cancelled.
        if (!$registration->cancelled) {
          return FALSE;
        }

        // Admins: always.
        if ($is_admin) {
          return TRUE;
        }

        // Others, only with the right permission.
        if (user_access('delete own ' . $type . ' node registration', $account)) {
          return TRUE;
        }
        break;

      case 'view':
        // Admins: always.
        if ($is_admin) {
          return TRUE;
        }

        // Never if: not enabled, cancelled or not verified.
        if (!$enabled || $registration->cancelled) {
          return FALSE;
        }

        // Semi view admins: no matter the owner.
        if (user_access('view ' . $type . ' node registration', $account)) {
          return TRUE;
        }

        // The rest: only if they're the owner.
        if (user_access('view own ' . $type . ' node registration', $account)) {
          return _node_registration_secret_access($registration, $account);
        }
        break;

      case 'edit':
      case 'update':
        // Admins: always.
        if ($is_admin) {
          return TRUE;
        }

        // If enabled, cancellable and access: only if they're the owner.
        $cancellable = $settings->max_cancel_time_passed();
        if ($enabled && !$cancellable && user_access('edit own ' . $type . ' node registration', $account)) {
          return _node_registration_secret_access($registration, $account);
        }
        break;

      case 'cancel':
        // Must not be cancelled.
        if ($registration->cancelled) {
          return FALSE;
        }

        // Admins: always.
        if ($is_admin) {
          return TRUE;
        }

        // Not if past cancel time.
        if ($settings->max_cancel_time_passed()) {
          return FALSE;
        }

        // If enabled and access: only if they're owner.
        if ($enabled && user_access('cancel own ' . $type . ' node registration', $account)) {
          return _node_registration_secret_access($registration, $account);
        }
        break;

      case 'add':
      case 'create':
        // Check: enabled.
        if ($enabled) {

          // Registration min date.
          $allow_min_date = $settings->min_registration_time_passed($disallow_min_date_reason);

          // Registration max date.
          $allow_max_date = !$settings->max_registration_time_passed($disallow_max_date_reason);

          // Check: start and end date.
          if ($allow_min_date && $allow_max_date) {

            // Check: capacity.
            if (_node_registration_event_has_room($node) || $settings->allow_exceeding_capacity) {

              // Fetch existing registration for current user.
              $registration = _node_registration_user_registered($node, $account);

              // Check: already registered.
              if (!$registration || node_registration_node_access($node, 'register others', $account)) {

                // The following are indirect settings from registration type and node.
                $user_access = user_access('add ' . $type . ' node registration', $account);
                $registration_access = $account->uid ? $settings->allow_authenticated : $settings->allow_anonymous;

                // Check: user access.
                if ($is_admin || ($user_access && $registration_access)) {

                  return TRUE;

                }
                else {
                  // Fail: user access.
                  $debug && watchdog('node_registration', 'user access insufficient');
                  $reason = 'access';
                }

              }
              else {
                // Fail: already registered.
                $debug && watchdog('node_registration', 'user already registered');
                $reason = 'registered';
              }

            }
            else {
              // Fail: capacity.
              $debug && watchdog('node_registration', 'no room left and no exceeding capacity');
              $reason = 'capacity';
            }

          }
          else {
            // Fail: invalid start or end date.
            $debug && watchdog('node_registration', 'registration date invalid: start date (@min_date) or end date (@max_date)', array(
              '@min_date' => $disallow_min_date_reason,
              '@max_date' => $disallow_max_date_reason,
            ));
            $reason = 'date';
          }

        }
        else {
          // Fail: registration not enabled.
          $debug && watchdog('node_registration', 'event registration not enabled');
          $reason = 'disabled';
        }
        break;
    }

    return FALSE;

  }, function($result, $cache_key) use ($registration, $op, $account, &$reason) {

    // Allow other modules to override and change this result.
    $reason or ($reason = $result ? '' : '?');
    $alt_access = module_invoke_all('node_registration_access', $registration, $op, $account, $reason);
    if (in_array(!$result, $alt_access, TRUE)) {
      return !$result;
    }

    return $result;

  });

  if ($reason) {
    $reasons[$cache_key] = $reason;
  }
  else {
    $reason = @$reasons[$cache_key];
  }

  return $access;
}

/**
 * Checks whether the given or global user has access to a registration by
 * checking ownership or the registration's secret.
 */
function _node_registration_secret_access($registration, $account = NULL) {
  global $user;
  $account or $account = $user;

  // Authenticated.
  if ($account->uid) {
    // User is author.
    if ($registration->uid == $account->uid) {
      return TRUE;
    }
  }
  // Anonymous.
  else {
    // Anonymous registration with matching secret.
    if (!$registration->uid && isset($_GET['secret']) && $_GET['secret'] == $registration->secret) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * Creates a secret (to save in the registration to identify anonymous registrations).
 */
function _node_registration_secret() {
  $secret = drupal_hash_base64(uniqid(mt_rand(), TRUE) . drupal_random_bytes(55));
  $secret = str_replace(array('-', '_'), '', $secret);
  $secret = substr($secret, 0, 40);

  return $secret;
}

/**
 * Retrieves existing registration for a certain user (in a certain event).
 */
function _node_registration_user_registered($node, $account = NULL) {
  global $user;

  $account or $account = $user;

  if ($account->uid) {
    $registrations = $registrations = node_registration_load_multiple(FALSE, array(
      'uid' => $account->uid,
      'nid' => $node->nid,
      'cancelled' => 0,
    ));
    if ($registrations) {
      return reset($registrations);
    }
  }
}

/**
 * Retrieves existing registration for a certain e-mail address (in a certain event).
 */
function _node_registration_email_registered($node, $email) {
  $registrations = node_registration_load_multiple(FALSE, array(
    'email' => $email,
    'nid' => $node->nid,
    'cancelled' => 0,
  ));
  if ($registrations) {
    $registration = reset($registrations);
    return $registration;
  }
}

/**
 * Return the number of registrations for a given node.
 */
function node_registration_event_count($node) {
  $nid = $node->nid;
  $count = db_query("SELECT sum(slots) FROM {node_registration} WHERE nid = ? AND cancelled = 0", array($nid))->fetchField();
  return (int) $count;
}

/**
 * Helper to determine if a node has any slots left.
 */
function _node_registration_event_has_room($node) {
  if ($node->registration->enabled()) {
    $capacity = $node->registration->capacity();
    $registrations = node_registration_event_count($node);
    return !$capacity || $capacity > $registrations;
  }

  return FALSE;
}

/**
 * Node types for which registration is enabled.
 */
function _node_registration_node_types() {
  $types = array();

  foreach (node_type_get_names() as $type => $name) {
    if (_node_registration_node_type_enabled($type)) {
      $types[$type] = $name;
    }
  }

  return $types;
}

/**
 * Change base registration settings for a given node/registration type.
 *
 * Only for `enabled` and date and capacity fields.
 */
function _node_registration_node_type_enable($type, $status, $settings) {
  $settings['status'] = (int) $status;
  return _node_registration_node_type_settings($type, $settings);
}

/**
 * Whether an event's registration is enabled.
 */
function _node_registration_node_type_enabled($type) {
  if ($enabled = _node_registration_node_type_settings($type)) {
    return $enabled->status;
  }

  return 0;
}

/**
 * Saves or retrieves registration type settings.
 */
function _node_registration_node_type_settings($type, $settings = array()) {
  $var_name = 'node_registration_type_settings_' . $type;

  if ($settings) {
    $settings = (array) $settings;
    $settings += variable_get($var_name, array());
    return variable_set($var_name, $settings);
  }

  $settings = variable_get($var_name, array()) + _node_registration_defaults();
  return (object) $settings;
}
