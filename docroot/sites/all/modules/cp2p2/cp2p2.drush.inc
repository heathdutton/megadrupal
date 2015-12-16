<?php
/**
 * @file
 * Drush integration for cp2p2.
 */

/**
 * Implements hook_drush_command().
 */
function cp2p2_drush_command() {
  $items['cp2p2-status'] = array(
    'callback' => 'drush_cp2p2_status',
    'description' => dt('Show the status of the current conversion.'),
    'aliases' => array('cp2p2'),
    'drupal dependencies' => array('cp2p2'),
  );
  $items['cp2p2-set-type'] = array(
    'callback' => 'drush_cp2p2_set_type',
    'description' => 'Control the destination profile type to be used for a given content type.',
    'drupal dependencies' => array('profile2', 'cp2p2'),
    'arguments' => array(
      'old' => 'Existing content type name.',
      'new' => 'Profile type name that will be used for the conversion. Set to "user" to copy the fields to the User entity.',
    ),
  );
  $items['cp2p2-convert'] = array(
    'callback' => 'drush_cp2p2_convert',
    'description' => 'Convert a Content Profile content type.',
    'drupal dependencies' => array('cp2p2'),
    'arguments' => array(
      'type' => 'Name of the content type to be converted.',
    ),
  );
  $items['cp2p2-migrate'] = array(
    'callback' => 'drush_cp2p2_migrate',
    'description' => 'Move data from a Content Profile content type.',
    'drupal dependencies' => array('cp2p2'),
    'arguments' => array(
      'type' => 'Name of the content type to be converted.',
    ),
  );
  $items['cp2p2-aio'] = array(
    'callback' => 'drush_cp2p2_aio',
    'description' => dt('Run the complete migration in one command.'),
    'drupal dependencies' => array('cp2p2'),
  );
  $items['cp2p2-destroy-content-profile'] = array(
    'callback' => 'drush_cp2p2_destroy_content_profile',
    'description' => dt('Remove all data and settings for a specific Content Profile type. Only run this after the data is converted as there is no going back.'),
    'drupal dependencies' => array('cp2p2'),
  );
  $items['cp2p2-destroy-converted-data'] = array(
    'callback' => 'drush_cp2p2_destroy_converted_data',
    'description' => dt('Remove all converted profile data so that the conversion can be ran again.'),
    'drupal dependencies' => array('profile2', 'cp2p2'),
  );
  return $items;
}

/**
 * Provide a status for the migration.
 */
function drush_cp2p2_status() {
  // Track whether Profile2 is installed.
  $profile2_installed = module_exists('profile2');

  // Get a list of all Content Profile content types.
  $content_types = _cp2p_content_types_list();

  // Get a list of all Profile2 profile types.
  $profile_types = $profile2_installed ? profile2_get_types() : array();

  // Compile a status table.
  $statuses = array(
    array(
      '*' . dt('Content type') . '*',
      '',
      '*' . dt('Profile type') . '*',
      '',
      '*' . dt('Structure') . '*',
      '',
      '*' . dt('Data') . '*',
    ),
  );
  // Simplify the display if not using Profile2.
  if (!$profile2_installed) {
    unset($statuses[0][1]);
    unset($statuses[0][2]);
  }

  foreach ($content_types as $content_type_name) {
    // If Profile2 is installed then a new profile type can be made.
    $profile_type_name = $profile2_installed ? variable_get('cp2p_type_' . $content_type_name, $content_type_name) : 'user';

    if (variable_get('cp2p_converted_' . $content_type_name)) {
      $structure_status = dt('Converted');
    }
    elseif ($profile2_installed || !isset($profile_types[$profile_type_name])) {
      $structure_status = dt('Not converted');
    }
    else {
      $structure_status = dt('User fields');
    }
    if (variable_get('cp2p_data_' . $content_type_name)) {
      $data_status = dt('Migrated');
    }
    else {
      $data_status = dt('Not migrated');
    }

    // Indicate if this is for the User entity.
    if ($profile_type_name == 'user') {
      $profile_type_name .= ' entity';
    }

    $status = array(
      $content_type_name,
      ':',
      $profile_type_name,
      ':',
      $structure_status,
      ':',
      $data_status,
    );
    if (!$profile2_installed) {
      unset($status[1]);
      unset($status[2]);
    }

    $statuses[] = $status;
  }

  drush_print_table($statuses, TRUE);

  if (!$profile2_installed) {
    drush_print(dt('The Profile2 module is not installed so all fields from the Content Profile content types will be copied onto the main User entity.'));
    drush_print(dt('Use the "cp2p2-convert" command to first convert content types and then the command "cp2p2-migrate" to migrate the data.'));
  }
  else {
    drush_print(dt('Use the "cp2p2-convert" command to first convert content types to a specific profile type and then the command "cp2p2-migrate" to migrate the data; the "cp2p2-set-type" command may be used to override the destination profile type.'));
  }
}

/**
 * Override the profile type.
 */
function drush_cp2p2_set_type($old, $new) {
  if (empty($old) || empty($new)) {
    return drush_set_error('MISSING_ARGUMENTS', dt("The command's arguments were not provided, see 'drush help !command' for further details.", array('!command' => 'cp2p2-set-type')));
  }

  // Track whether Profile2 is installed.
  $profile2_installed = module_exists('profile2');

  // Load all content type specs.
  $content_types = node_type_get_types();

  // Work out what the profile type is.
  $profile_type_name = $profile2_installed ? variable_get('cp2p_type_' . $old, $old) : 'user';

  // Ensure the requested content type actually exists.
  if (!isset($content_types[$old])) {
    return drush_set_error('CONTENT_TYPE_DOESNT_EXIST', dt('The requested content type, "!type", doesn\'t exist, please confirm the name via the "cp2p2-status" command.', array('!type' => $old)));
  }

  // Some extra steps if the profile type is actually changed.
  if ($profile_type_name != $new) {
    // It might not be best to change the profile type if the migration has already
    // ran.
    if (variable_get('cp2p_data_' . $old)) {
      if (!drush_confirm(dt('The data for the "!old" content type has already been migrated, are you sure you want to change the profile type?', array('!old' => $old)))) {
        return drush_user_abort();
      }
    }

    // It might not be best to change the profile type if the conversion has
    // already ran.
    elseif (variable_get('cp2p_converted_' . $old)) {
      if (!drush_confirm(dt('The "!old" content type has already been converted, are you sure you want to change the profile type?', array('!old' => $old)))) {
        return drush_user_abort();
      }
    }

    // Delete the variables saying the conversion was done.
    variable_get('cp2p_converted_' . $old);
    variable_get('cp2p_data_' . $old);
  }

  // Update the setting.
  variable_set('cp2p_type_' . $old, $new);

  if ($new == 'user') {
    drush_print(dt('Fields from the "!old" content type will be copied to the User entity.', array('!old' => $old)));
  }
  else {
    drush_print(dt('The "!old" content type will be converted to the "!new" profile type.', array('!old' => $old, '!new' => $new)));
  }
}

/**
 * Convert a content type to a profile type.
 */
function drush_cp2p2_convert($content_type_name) {
  if (empty($content_type_name)) {
    return drush_set_error('MISSING_ARGUMENTS', dt("The command's arguments were not provided, see 'drush help !command' for further details.", array('!command' => 'cp2p2-convert')));
  }

  // Track whether Profile2 is installed.
  $profile2_installed = module_exists('profile2');

  // List of all content types.
  $content_types = node_type_get_types();

  // Ensure the requested content type actually exists.
  if (!isset($content_types[$content_type_name])) {
    return drush_set_error('CONTENT_TYPE_DOESNT_EXIST', dt('The requested content type, "!type", doesn\'t exist, please confirm the name via the "cp2p2-status" command.', array('!type' => $content_type_name)));
  }
  $content_type = $content_types[$content_type_name];

  // Get a list of all Profile2 profile types.
  $profile_types = $profile2_installed ? profile2_get_types() : array();

  // Identify the destination profile type.
  $profile_type_name = $profile2_installed ? variable_get('cp2p_type_' . $content_type_name, $content_type_name) : 'user';

  // If the destination profile type is 'user' then we're not using Profile2
  // even if it is installed.
  if ($profile_type_name == 'user') {
    $profile2_installed = FALSE;
  }

  // The destination entity type.
  $new_entity_type = $profile2_installed ? 'profile2' : 'user';

  // Can't re-create the same profile type.
  if ($profile2_installed && isset($profile_types[$profile_type_name])) {
    return drush_set_error('PROFILE_TYPE_ALREADY_EXISTS', dt('The requested profile type, "!type", already exists and will not be replaced.', array('!type' => $profile_type_name)));
  }

  // Get the field specs for this entity/bundle combination.
  $params = array(
    'entity_type' => $new_entity_type,
    'bundle' => $profile_type_name,
  );
  $all_fields = field_read_instances($params);

  // Step 1: Create the profile type (if applicable).
  if ($profile2_installed) {
    $params = array(
      'userCategory' => TRUE,
      'userView' => TRUE,
      'type' => $profile_type_name,
      'label' => $content_type->name,
      'weight' => 0,
      'is_new' => TRUE,
    );
    $profile_type = new ProfileType($params);
    profile2_type_save($profile_type);
  }

  // Step 2: Attach the fields to the profile type.
  $params = array(
    'entity_type' => 'node',
    'bundle' => $content_type_name,
  );
  foreach (field_read_instances($params) as $ctr => $field) {
    $field['entity_type'] = $new_entity_type;
    $field['bundle'] = $profile_type_name;

    // The main 'body' field can't be converted, it's a node-only thing, so
    // create a replacement.
    if ($field['field_name'] == 'body') {
      // The new field's name will have the profile type name as a prefix.
      $field['field_name'] = "field_{$profile_type_name}_body";

      // If the field doesn't already exist, create it.
      if (!isset($all_fields[$field['field_name']])) {
        // The new field needs to work the same as the old one.
        $field['type'] = 'text_with_summary';

        // Remove the IDs.
        unset($field['id']);
        unset($field['field_id']);
        unset($field['deleted']);

        // Create the new field.
        field_create_field($field);
      }
    }

    // Save the instance. This seems safe to do for fields just created with
    // field_create_field().
    field_create_instance($field);
  }

  // All done!
  variable_set('cp2p_converted_' . $content_type_name, TRUE);
  if ($profile2_installed) {
    drush_print(dt('A new "!profile_type_name" profile type has be created that matches the old "!content_type_name" content type.', array('!content_type_name' => $content_type_name, '!profile_type_name' => $profile_type_name)));
  }
  else {
    drush_print(dt('Fields from the old "!content_type_name" content type have been converted to the User entity.', array('!content_type_name' => $content_type_name)));
  }
}

/**
 * Migrate the data from the old fields.
 */
function drush_cp2p2_migrate($content_type_name) {
  if (empty($content_type_name)) {
    return drush_set_error('MISSING_ARGUMENTS', dt("The command's arguments were not provided, see 'drush help !command' for further details.", array('!command' => 'cp2p2-migrate')));
  }

  // Load all content type specs.
  $content_types = node_type_get_types();

  // Ensure the requested content type actually exists.
  if (!isset($content_types[$content_type_name])) {
    return drush_set_error('CONTENT_TYPE_DOESNT_EXIST', dt('The requested content type, "!type", doesn\'t exist, please confirm the name via "!command".', array('!type' => $content_type_name, '!command' => 'cp2p2-status')));
  }

  // Track whether Profile2 is installed.
  $profile2_installed = module_exists('profile2');

  // Identify the destination profile type.
  $profile_type_name = $profile2_installed ? variable_get('cp2p_type_' . $content_type_name, $content_type_name) : 'user';

  // If the destination profile type is 'user' then we're not using Profile2
  // even if it is installed.
  if ($profile_type_name == 'user') {
    $profile2_installed = FALSE;
  }

  // Load all profile type definitions.
  $profile_types = $profile2_installed ? profile2_get_types() : array();

  // The destination entity type.
  $new_entity_type = $profile2_installed ? 'profile2' : 'user';

  // Ensure the requested profile type actually exists.
  if ($profile2_installed && !isset($profile_types[$profile_type_name])) {
    return drush_set_error('PROFILE_TYPE_DOESNT_EXIST', dt('The requested profile type, "!type", doesn\'t exist, please confirm via the "cp2p2-status" command that the content type has been converted.', array('!type' => $profile_type_name)));
  }

  // Load all field specs.
  $params = array(
    'entity_type' => $new_entity_type,
    'bundle' => $profile_type_name,
  );
  $fields = array();
  foreach (field_read_instances($params) as $ctr => $field) {
    $fields[$field['field_name']] = $field;
  }

  // Loop over each node.
  $records = db_query("SELECT nid FROM {node} WHERE type=:type AND uid > 0", array(':type' => $content_type_name));

  foreach ($records as $record) {
    // Load the profile node.
    $node = node_load($record->nid);

    // Load the user account.
    $account = user_load($node->uid);
    // This shouldn't happen, but catch accounts that failed to load.
    if (empty($account) || empty($account->name)) {
      drush_set_error('CP2P2', dt("Problem loading user !user!", array('!user' => $node->uid)));
      continue;
    }

    // Build the new profile object.
    $profile = array();
    $profile['type'] = $profile_type_name;
    $profile['uid'] = $node->uid;
    $profile['created'] = $node->created;

    // Copy each field.
    foreach (get_object_vars($node) as $key => $val) {
      // Only copy items with a name starting in 'field_'.
      if (strpos($key, 'field_') === 0) {
        // Optional alter the field data.
        $function = '_cp2p2_' . $fields[$key]['widget_type'] . '_alter';
        if (function_exists($function)) {
          $function($val);
        }
        $profile[$key] = $val;
      }
    }
    // Copy the body field.
    if (isset($node->body) && is_array($node->body)) {
      // The destination field.
      $field_name = "field_{$profile_type_name}_body";
      // Loop over each language.
      foreach ($node->body as $lang_name => $lang_data) {
        // Proceed if there's a value for this language.
        if (!empty($lang_data) && is_array($lang_data) && isset($lang_data[0]['value'])) {
          $profile[$field_name] = array(
            $lang_name => array(
              array(
                'value' => $lang_data[0]['value'],
                'format' => $lang_data[0]['format'],
                'summary' => $lang_data[0]['summary'],
              ),
            ),
          );
        }
      }
    }

    // Save the Profile2 record.
    if ($profile2_installed) {
      $new_profile = profile2_create($profile);
      $new_profile->save();
    }
    // No Profile2, so save the user fields.
    else {
      $fields_ids = array();
      foreach ($profile as $field_name => $field_data) {
        if (strpos($field_name, 'field_') === 0) {
          $account->$field_name = $field_data;
        }
      }
      user_save($account, $profile);
    }

    variable_set('cp2p_data_' . $content_type_name, TRUE);
    drush_print(dt('Converted the "!content_type_name" profile type for !user.', array('!content_type_name' => $content_type_name, '!user' => $node->name)));
  }
}

/**
 * Run all of the necessary steps thus migrating all fields in one go.
 */
function drush_cp2p2_aio() {
  foreach (_cp2p_content_types_list() as $type) {
    drush_cp2p2_convert($type);
    drush_cp2p2_migrate($type);
  }

  drush_print(dt('All done.'));
}

/**
 * Cleanup the current installation by removing the old content types & data.
 */
function drush_cp2p2_destroy_content_profile() {
  if (!drush_confirm(dt('Do you wish to remove all Content Profile content types, nodes and settings? There is no way to recover the data other than restoring a backup.'))) {
    return drush_user_abort();
  }
  if (!drush_confirm(dt('Are you really sure? This *will* delete all of the content profile data!'))) {
    return drush_user_abort();
  }

  // Check each content type.
  foreach (node_type_get_types() as $type_name => $type_data) {
    // Delete the content type.
    if (variable_get("content_profile_use_{$type_name}", FALSE)) {
      // Delete the nodes.
      $records = db_query("SELECT nid FROM {node} WHERE type=:type AND uid > 0", array(':type' => $type_name));
      foreach ($records as $record) {
        node_delete($record->nid);
      }
      // Delete the content type.
      node_type_delete($type_name);

      watchdog('node', 'Deleted content type %name and all of its nodes.', array('%name' => $type_name), WATCHDOG_NOTICE);
      node_types_rebuild();

      // Delete the two possible variables.
      variable_del("content_profile_{$type_name}");
      variable_del("content_profile_use_{$type_name}");
    }
  }
  menu_rebuild();
}

/**
 * Remove all of the converted data so that the conversion can be ran again.
 */
function drush_cp2p2_destroy_converted_data() {
  if (!drush_confirm(dt('Do you wish to remove all converted user profiles and settings? There is no way to recover the data other than restoring a backup.'))) {
    return drush_user_abort();
  }
  if (!drush_confirm(dt('Are you really sure? This *will* delete all of the converted data!'))) {
    return drush_user_abort();
  }

  // Load all profile types.
  $profile_types = entity_load_multiple_by_name('profile2_type', FALSE);

  // Remove all user profiles.
  foreach (_cp2p_content_types_list() as $content_type_name) {
    // Work out the profile type being used.
    $profile_type_name = variable_get('cp2p_type_' . $content_type_name, $content_type_name);

    // Get a list of all profile IDs.
    $ids = db_query("SELECT pid FROM {profile} WHERE type=:type", array(':type' => $profile_type_name))->fetchCol();
    drush_print(dt('Purging all !profile_type_name records...', array('!profile_type_name' => $profile_type_name)));

    // Delete the data.
    profile2_delete_multiple($ids);
    drush_print(dt('All !profile_type_name records purged.', array('!profile_type_name' => $profile_type_name)));

    // Remove the profile type itself.
    $profile_types[$profile_type_name]->delete();
    drush_print(dt('The "!profile_type_name" profile type has been removed.', array('!profile_type_name' => $profile_type_name)));

    // Remove any extra variables.
    variable_del('cp2p_converted_' . $content_type_name);
    variable_del('cp2p_data_' . $content_type_name);
    variable_del('cp2p_type_' . $content_type_name);
    drush_print(dt('The "!profile_type_name" CP2P2 settings have been removed.', array('!profile_type_name' => $profile_type_name)));
  }

  drush_print(dt('All done.'));
}

/**
 * Get a list of all content types that have been enabled for use with the
 * Content Profile module.
 *
 * @return
 *   Simple array ($type=>$type) of all supported content types.
 */
function _cp2p_content_types_list() {
  $content_types = &drupal_static(__FUNCTION__);

  // If the list wasn't compiled before, do so now.
  if (!isset($content_types)) {
    // Check each content type in turn.
    foreach (node_type_get_types() as $type_name => $type_data) {
      // Content Profile used a variable in format "content_profile_use_[type]"
      // to control whether the content type is used.
      if (variable_get("content_profile_use_{$type_name}", FALSE)) {
        $content_types[$type_name] = $type_name;
      }
    }
  }

  return $content_types;
}

/**
 * Callback for altering a location field prior to saving.
 */
function _cp2p2_location_alter(&$val) {
  // Remove the 'lid' value so that it will be saved as a new record.
  if (isset($val[LANGUAGE_NONE][0]['lid'])) {
    unset($val[LANGUAGE_NONE][0]['lid']);
  }
}
