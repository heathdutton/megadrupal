<?php
/**
 * @file
 * Party general hook include.
 */

/**
 * Implements hook_party_access().
 *
 * Handles basic access to parties:
 *  - user permissions
 *  - data set cardinality settings
 */
function party_party_access($op, $party = NULL, $data_set = NULL, $account = NULL) {
  // Set the data_set_name
  if (isset($data_set)) {
    $data_set_name = $data_set['set_name'];
  }

  // If we're looking at permission for a particular data set we check these.
  if (isset($data_set_name)) {
    // Determine what the Core permissions system has to say about this.
    switch ($op) {
      case 'view':
        $permission_string = 'view party attached ' . $data_set_name;
        break;
      case 'edit':
        $permission_string = 'edit party attached ' . $data_set_name;
        break;
      case 'detach':
        $permission_string = 'detach party attached ' . $data_set_name;
        break;
      case 'attach':
      case 'add':
        $permission_string = 'attach party ' . $data_set_name;
        break;
    }

    if (isset($permission_string)) {
      $permission_access = user_access($permission_string, $account);
    }

    // Determine what data set info has to say about this.
    // Data set actions in hook_party_data_set_info() are defined to match with
    // values of $op here.

    // Build an array of forced allowed actions.
    // @TODO: remove this when we re-work permissions.
    $allowed_ops = array('view', 'edit', 'detach');

    // Other ops depend on the data set defining the action.
    if (isset($data_set['actions'][$op]) || in_array($op, $allowed_ops)) {
      $data_access = TRUE;

      // TEMPORARY until we handle all actions here.
      // see http://drupal.org/node/1673608, http://drupal.org/node/1673606
      if ($op == 'add' || $op == 'attach') {
        // Operations 'add' and 'attach' additionally need a check on data set
        // cardinality.
        // TODO: store the information about which ops need this elsewhere?
        if (isset($data_set['max cardinality'])) {
          $ids = $party->getDataSetController($data_set_name)->getEntityIds();
          if (count($ids) >= $data_set['max cardinality']) {
            $data_access = FALSE;
          }
        }
      }
    }
    else {
      $data_access = FALSE;
    }
  }
  else {
    // If we're not being asked about attachments, just use plain permissions.
    switch ($op) {
      case 'view':
        $permission_string = 'view parties';
        break;
      case 'edit':
        $permission_string = 'edit parties';
        break;
      case 'archive':
        $permission_string = 'archive parties';
        break;
      case 'delete':
        $permission_string = 'delete parties';
        break;
    }

    if (isset($permission_string)) {
      $permission_access = user_access($permission_string, $account);
    }
  }

  // A data access deny is global.
  if (isset($data_access) && $data_access == FALSE) {
    return FALSE;
  }

  // Otherwise, permission access returns allow or ignore.
  return !empty($permission_access) ? TRUE: NULL;
}

/**
 * Implements hook_party_operations.
 */
function party_party_operations() {
  $operations = array(
    'merge' => array(
      'label' => t('Merge parties'),
      'callback' => 'party_party_operations_merge',
    ),
  );
  return $operations;
}

/**
 * Implements hook_party_primary_fields_alter().
 */
function party_party_primary_fields_alter(Party $party, &$needs_store) {
  // Check we are not creating a duplicate primary email address.
  if ($needs_store && variable_get('party_ensure_no_dup_emails', FALSE)) {
    // See if we have an email address to check for duplicates.
    if (!empty($party->email)) {
      $query = party_query();
      $query->fields('party');
      $query->range(0, 1);
      $query->condition('email', $party->email);

      // If already saved,
      if (!empty($party->pid)) {
        $query->condition('pid', $party->pid, '!=');
      }

      // If the email is already used, clear it.
      if ($match = $query->execute()->fetchAssoc()) {
        // Log a message.
        $args = array(
          '@party_email' => $party->email,
          '@matched_party_label' => $match['label'],
          '@matched_party_id' => $match['pid'],
        );
        $message = 'Prevented saving party primary email address @party_email
          as the address is already used by @matched_party_label
          (@matched_party_id). The party has been saved with the email left
          empty.';
        drupal_set_message(t($message, $args), 'warning', FALSE);

        watchdog('party', $message, $args);

        // Clear the email.
        $party->email = NULL;
      }
    }
  }
}

/**
 * Primary field callback for converting PID to 'Party PID' label.
 *
 * @see party_party_primary_fields_sources_alter()
 */
function party_primary_fields_callback_party_label_default($value) {
  return isset($value) ? t('Party @pid', array('@pid' => $value)) : NULL;
}

/**
 * Primary field callback for converting a Name field to a label.
 *
 * @see party_party_primary_fields_sources_alter()
 */
function party_primary_fields_callback_party_name_field_label($value, $target, $target_info, $source_info) {
  if (isset($value)) {
    // Get hold of the name format.
    $data_set = party_get_data_set_info($source_info['data_set']);
    $instance = field_info_instance($data_set['entity type'], $source_info['property'], $data_set['entity bundle']);
    $format = name_get_format_by_machine_name($instance['settings']['override_format']);
    if (empty($format)) {
      $format = name_get_format_by_machine_name('default');
    }

    return name_format($value, $format);
  }
}
