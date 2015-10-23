<?php
/**
 * @file
 * Migration functions for the PM Team module.
 */

define('PMTEAM_MIGRATE_MAX_JOB_PER_BATCH', 5);

/**
 * Helper function for migrating between PMTeam to drupal fields.
 */
function pmteam_migrate(&$sandbox) {
  pmteam_migrate_create_fields($sandbox);

  // Migrate data from pmteam node to Drupal user account.
  if (empty($sandbox['pmteam_current_vid_of_field_being_migrated'])) {
    $sandbox['pmteam_current_vid_of_field_being_migrated'] = 0;
  }

  if (pmteam_migrate_field_data($sandbox) == FALSE) {
    $sandbox['#finished'] = 0.5;
    return;
  }

  // We don't need pmteam table anymore, so dropping it.
  db_drop_table('pmteam');
  variable_del('node_options_pmteam');

  module_load_include('inc', 'pmpermission', 'includes/pmpermission.migrate');
  pmpermission_migrate_execute('pmteam');

  $sandbox['#finished'] = 1;
  return 'PM Team nodes have been migrated to field_api based fields successfully.';
}


/**
 * Check if migration need to be performed.
 */
function pmteam_migrate_update_could_be_performed() {
  if (db_table_exists('pmteam')) {
    return TRUE;
  }
  return FALSE;
}


/**
 * Creates and attaches fields to Drupal user.
 */
function pmteam_migrate_create_fields(&$sandbox) {
  module_load_include('inc', 'pmteam', 'includes/pmteam.field_base');
  module_load_include('inc', 'pmteam', 'includes/pmteam.field_instance');
  module_load_include('inc', 'pm', 'includes/pm.field');

  $field_bases = pmteam_default_field_bases();
  pm_field_bases_create_if_required($field_bases);

  $field_instances = pmteam_default_field_instances();
  pm_field_instances_create_if_required($field_instances);
  return TRUE;
}


/**
 * Migrate pmteam node fields data to drupal user bundle.
 */
function pmteam_migrate_field_data(&$sandbox) {
  $results = db_select('pmteam', 'pt')
    ->fields('pt')
    ->condition('vid', $sandbox['pmteam_current_vid_of_field_being_migrated'], '>')
    ->groupBy('pt.vid')
    ->range(0, PMTEAM_MIGRATE_MAX_JOB_PER_BATCH)
    ->execute();
  $count = 0;
  foreach ($results as $pmteam) {
    $count++;
    $sandbox['pmteam_current_vid_of_field_being_migrated'] = $pmteam->vid;
    _pmteam_migrate_field_data($pmteam);
  }
  return empty($count);
}

/**
 * Helper function to migrate single node, given any pmteam row.
 *
 * @see pmperson_migrate_field_data()
 */
function _pmteam_migrate_field_data($pmteam) {
  $member_ids = array();
  try {
    $results = db_select('pmteam', 'pt')
      ->fields('pt')
      ->condition('vid', $pmteam->vid)
      ->execute();
    foreach ($results as $pt) {
      $member_ids[] = $pt->mnid;
    }
    if (!empty($member_ids)) {
      _pmteam_migrate_migrate_single_node($pmteam->nid, $pmteam->vid, $member_ids);
    }
  }
  catch (Exception $exc) {
    watchdog(
      'pmteam',
      'See ' . __FUNCTION__ . '() ' . $exc->getTraceAsString(),
      NULL, WATCHDOG_ERROR
    );
  }
}

/**
 * Helper function to migrate single pmteam.
 */
function _pmteam_migrate_migrate_single_node($nid, $vid, $members) {

  $update_needed_flag = FALSE;
  $users = array();
  $pmorganizations = array();
  foreach ($members as $mnid) {
    if ($pmperson = _pmteam_migrate_load_data('pmperson', $mnid)) {
      $users[] = $pmperson->user_uid;
    }
    elseif (_pmteam_migrate_load_data('pmorganization', $mnid)) {
      $pmorganizations[] = $mnid;
    }
  }
  $node = node_load($nid, $vid);
  if (!empty($users)) {
    foreach ($users as $uid) {
      $node->pmteam_user[LANGUAGE_NONE][] = array(
        'target_id' => $uid,
      );
      $update_needed_flag = TRUE;
    }
  }
  if (!empty($pmorganizations)) {
    foreach ($pmorganizations as $node_id) {
      $node->pmteam_organization[LANGUAGE_NONE][] = array(
        'target_id' => $node_id,
      );
      $update_needed_flag = TRUE;
    }
  }
  if ($update_needed_flag) {
    node_save($node);
  }
  return TRUE;
}

/**
 * Helper function to load either pmperson or pmorganization data.
 */
function _pmteam_migrate_load_data($type, $nid) {
  $result = db_select($type, 'row')
    ->fields('row')
    ->condition('nid', $nid)
    ->execute()
    ->fetchObject();
  return $result;
}
