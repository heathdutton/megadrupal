<?php

/**
 * @file
 * Drush commands for Profile Lite module.
 */

/**
 * Implementation of hook_drush_help().
 */
function profile_lite_drush_help($section) {
  switch ($section) {
    case 'drush:content-profile-migrate':
      return dt('Migrate Drupal 6 Content Profile nodes to user fields. Using ' .
                'this command on a large set of profiles is not recommended. '  .
                'Instead, use Migrate module.');
  }
}

/**
 * Implementation of hook_drush_command().
 */
function profile_lite_drush_command() {
  $items['content-profile-migrate'] = array(
    'description' => 'Migrate Drupal 6 Content Profile nodes to user fields.',
    'drupal dependencies' => array('field_tools', 'content_migrate'),
  );

  return $items;
}

/**
 * Command callback for content-profile-migrate.
 *
 * TODO: This command really needs to use Queue API, but for smaller sets of
 * data it works fine.
 */
function drush_profile_lite_content_profile_migrate() {
  module_load_include('admin.inc', 'field_tools');
  module_load_include('inc', 'content_migrate', 'includes/content_migrate.admin');

  // Load up content migrate options, so we can make sure that all data has been
  // migrated from CCK to Drupal 7 fields.
  $content_migrate_fields = content_migrate_get_options();
  if (empty($content_migrate_fields['converted'])) {
    drush_log(dt("It appears that CCK's Content Migrate module has not been run " .
                 'yet. Please run drush content-migrate-fields prior to running ' .
                 'this script.'), 'error');
    return;
  }

  // Ask some questions up front so the user can set it and forget it.
  $msg = 'Would you like to overwrite any exiting profile data that might be on ' .
         'your users with the data from the content profile nodes? If you have '  .
         'never run this script before and are sure the fields from the '         .
         'content_profile node types have never existed on the user entity '      .
         'before, it is probably safe to respond yes here. If you are NOT sure, ' .
         'respond no.';
  $overwrite = drush_confirm(dt($msg));
  $delete = drush_confirm(dt('When users have been migrated, would you like to '  .
                             'delete the old content_profile nodes?'));

  // Load all the node type names.
  $types = _node_types_build()->names;

  // Array of totals for all profile types, used in dt().
  $totals = array(
    '!updated' => 0,
    '!total' => 0,
    '!deleted' => 0,
    '!failed' => 0,
  );

  // Iterate over each type, checking to see if any of them are content_profiles.
  foreach ($types as $type => $name) {
    // Don't process node types that weren't from content_profile.
    if (!variable_get('content_profile_use_' . $type, FALSE)) {
      unset($types[$type]);
      continue;
    }

    // Set up some dt() placeholders.
    $dt_args = array(
      '!updated' => 0,
      '!total' => 0,
      '!deleted' => 0,
      '!failed'  => 0,
      '@type' => "$name ($type)",
    );

    drush_log(dt('Migrating field data from node type @type to the user entity.', $dt_args), 'status');
    // Get a fresh copy of the user fields for each type.
    $user_fields = field_info_instances('user', 'user');
    $instances = field_info_instances('node', $type);

    // Check for fields that have not been migrated by Content Migrate module.
    if ($unmigrated = array_diff_key($instances, $content_migrate_fields['converted'])) {
      $dt_args['@unmigrated'] = implode(', ', array_keys($unmigrated));
      drush_log(dt('The @unmigrated field(s) on node type @type have not been ' .
                   'migrated by Content Migrate module. This must be resolved ' .
                   'before the node type can be migrated to the User entity.', $dt_args), 'error');
      continue;
    }

    // Check to see if we need to create the profile_lite category.
    $question = 'There does not appear to be a profile category for @type. Would you like to create it?';
    if (module_exists('profile_lite') && !_profile_lite_category_exists($type) && drush_confirm(dt($question, $dt_args))) {
      $category = new stdClass();
      $category->name = $type;
      $category->label = $name;
      foreach ($instances as $field_name => $instance) {
        $category->fields[$field_name] = array('weight' => $instance['weight']);
      }
      profile_lite_category_save($category);
    }

    // Add the node type field instances to the user entity.
    if ($missing_instances = array_diff_key($instances, $user_fields)) {
      drush_log(dt('Saving field instances from @type to the user entity.', $dt_args), 'status');
      foreach ($missing_instances as $instance) {
        _field_tools_add_instance_to_bundles($instance, array('user' => array('user')));
      }
    }
    else {
      drush_log(dt('The user entity already contains all field instances from type @type. Continuing.', $dt_args), 'status');
    }

    // Load up the nodes of this type.
    $node_query = new EntityFieldQuery();
    $node_query->entityCondition('entity_type', 'node')
               ->entityCondition('bundle', $type);
    $result = $node_query->execute();
    // Well drat, we need the uid so we can't use this. Have to load the full nodes.
    //field_attach_load('node', $result['node']);
    drush_log(dt('Loading all nodes of type @type. This may take a while.', $dt_args), 'status');

    // If we don't have any nodes, no reason to continue. Move to the next type.
    if (empty($result['node'])) {
      drush_log(dt('There are not any nodes of this type. Continuing...'), 'status');
      continue;
    }

    // Now load up the nodes. If there are a lot of nodes, this will probably die,
    // but it works for smaller sets for now.
    $nodes = node_load_multiple(array_keys($result['node']));
    $totals['!total'] += $dt_args['!total'] = count($nodes);

    drush_log(dt('There are !total total nodes of type @type to migrate to user entities.', $dt_args), 'status');
    if (!drush_confirm(dt('Would you like to continue migrating the @type data to user entities?', $dt_args))) {
      continue;
    }

    foreach ($nodes as $node) {
      $account = user_load($node->uid);
      $dt_args['@name'] = $account->name;
      $dt_args['@uid']  = $account->uid;
      // Remove everything but the profile fields from the node object.
      $profile_data = array_intersect_key((array) $node, $instances);
      // If we are preserving existing data, check for it.
      if (!$overwrite) {
        foreach (array_keys($instances) as $field_name) {
          if (!empty($account->$field_name)) {
            unset($profile_data[$field_name]);
          }
        }
      }
      if (empty($profile_data)) {
        drush_log(dt('No profile data to save for user @name (uid @uid).', $dt_args), 'status');
        continue;
      }
      drush_log(dt('Saving profile data for user @name (uid @uid).', $dt_args));
      if (user_save($account, $profile_data)) {
        drush_log(dt('Saved profile data for user @name (uid @uid).', $dt_args), 'success');
        $dt_args['!updated']++;
        $nodes_to_delete[] = $node->nid;
      }
      else {
        drush_log(dt('Failed to save profile data for user @name (uid @uid).', $dt_args), 'error');
        $dt_args['!failed']++;
      }
    }

    drush_log(dt('Migrated !updated of !total @type nodes.', $dt_args), 'success');
    if (!empty($dt_args['!failed'])) {
      drush_log(dt('Failed to migrate !failed nodes.', $dt_args), 'warning');
    }

    // Now delete all the nodes in one fell swoop.
    $dt_args['!deleted'] = count($nodes_to_delete);
    drush_log(dt('Deleting !deleted node(s). This may take a while.', $dt_args), 'status');
    if ($delete && $nodes_to_delete) {
      node_delete_multiple($nodes_to_delete);
      drush_log(dt('Deleted !deleted node(s).', $dt_args), 'success');
    }
    $totals['!updated'] += $dt_args['!updated'];
    $totals['!failed']  += $dt_args['!failed'];
    $totals['!deleted'] += $dt_args['!deleted'];
  }

  // Finished! Tally the score.
  $totals['!types-count'] = count($types);
  $totals['@types'] = implode(',', $types);
  $msg = 'Migrated !updated of !total node(s) from !types-count content_profile ' .
         'node type(s). !failed failed. Deleted !deleted node(s).';
  drush_log(dt($msg, $totals), 'status');
}
