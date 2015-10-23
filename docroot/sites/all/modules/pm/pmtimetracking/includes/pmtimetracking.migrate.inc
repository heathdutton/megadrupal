<?php
/**
 * @file
 * Migration functions for the PM Timetracking module.
 */

define('PMTIMETRACKING_MIGRATE_MAX_JOB_PER_BATCH', 5);

/**
 * Helper function for migrating between pmtimetracking to drupal fields.
 */
function pmtimetracking_migrate(&$sandbox) {
  pmtimetracking_migrate_create_fields($sandbox);
  // Migrate data from pmtimetracking node to Drupal user account.
  if (empty($sandbox['pmtimetracking_current_vid_of_field_being_migrated'])) {
    $sandbox['pmtimetracking_current_vid_of_field_being_migrated'] = 0;
  }
  if (pmtimetracking_migrate_field_data($sandbox) == FALSE) {
    $sandbox['#finished'] = 0.5;
    return;
  }
  // We don't need pmtimetracking table anymore, so dropping it.
  db_drop_table('pmtimetracking');
  variable_del('node_options_pmtimetracking');

  // PM Permissions default configurations.
  variable_set('pmpermission_field_parent_reference_for_pmtimetracking', 'pmtimetracking_parent');
  variable_set('pmpermission_node_pmtimetracking_enabled', TRUE);

  module_load_include('inc', 'pmpermission', 'includes/pmpermission.migrate');
  pmpermission_migrate_execute('pmtimetracking');

  $sandbox['#finished'] = 1;
  return 'PM Timetracking nodes have been migrated to field_api based fields successfully.';
}


/**
 * Check if migration need to be performed.
 */
function pmtimetracking_migrate_update_could_be_performed() {
  if (db_table_exists('pmtimetracking')) {
    return TRUE;
  }
  return FALSE;
}


/**
 * Creates and attaches fields to Drupal user.
 */
function pmtimetracking_migrate_create_fields(&$sandbox) {
  module_load_include('inc', 'pmtimetracking', 'includes/pmtimetracking.field_base');
  module_load_include('inc', 'pmtimetracking', 'includes/pmtimetracking.field_instance');
  module_load_include('inc', 'pm', 'includes/pm.field');

  $field_bases = pmtimetracking_default_field_bases();
  pm_field_bases_create_if_required($field_bases);

  $field_instances = pmtimetracking_default_field_instances();
  pm_field_instances_create_if_required($field_instances);
  return TRUE;
}


/**
 * Migrate pmtimetracking node fields data to drupal user bundle.
 */
function pmtimetracking_migrate_field_data(&$sandbox) {
  $results = db_select('pmtimetracking', 'pmt')
    ->fields('pmt')
    ->condition('vid', $sandbox['pmtimetracking_current_vid_of_field_being_migrated'], '>')
    ->range(0, PMTIMETRACKING_MIGRATE_MAX_JOB_PER_BATCH)
    ->execute();
  $count = 0;
  foreach ($results as $pmtimetracking) {
    $count++;
    $sandbox['pmtimetracking_current_vid_of_field_being_migrated'] = $pmtimetracking->vid;
    try {
      _pmtimetracking_migrate_migrate_single_node($pmtimetracking->nid, $pmtimetracking->vid, $pmtimetracking);
    }
    catch (Exception $exc) {
      watchdog(
        'pmtimetracking',
        'See ' . __FUNCTION__ . '() ' . $exc->getTraceAsString(),
        NULL, WATCHDOG_ERROR
      );
    }
  }
  return empty($count);
}


/**
 * Helper function to migrate single pmtimetracking.
 */
function _pmtimetracking_migrate_migrate_single_node($nid, $vid, $timetracking) {
  $node = node_load($nid, $vid);

  if (isset($timetracking->datebegin) AND !empty($timetracking->datebegin)) {
    $node->pm_date[LANGUAGE_NONE][0]['value'] = $timetracking->datebegin;
    if (isset($timetracking->dateend) AND !empty($timetracking->dateend)) {
      $node->pm_date[LANGUAGE_NONE][0]['value2'] = $timetracking->dateend;
    }
  }

  // Ticket >> Task >> Project >> Organization.
  if ($timetracking->ticket_nid) {
    $node->pmtimetracking_parent[LANGUAGE_NONE][0]['target_id'] = $timetracking->ticket_nid;
  }
  elseif ($timetracking->task_nid) {
    $node->pmtimetracking_parent[LANGUAGE_NONE][0]['target_id'] = $timetracking->task_nid;
  }
  elseif ($timetracking->project_nid) {
    $node->pmtimetracking_parent[LANGUAGE_NONE][0]['target_id'] = $timetracking->project_nid;
  }
  elseif ($timetracking->organization_nid) {
    $node->pmtimetracking_parent[LANGUAGE_NONE][0]['target_id'] = $timetracking->organization_nid;
  }

  if ($timetracking->duration) {
    $node->pm_duration[LANGUAGE_NONE][0]['value'] = $timetracking->duration;
  }

  $node->pm_durationunit[LANGUAGE_NONE][0]['value'] = 'hour';

  if ($timetracking->billing_duration) {
    $node->pm_billing_duration[LANGUAGE_NONE][0]['value'] = $timetracking->billing_duration;
  }

  node_save($node);
  return TRUE;
}

/**
 * Give back the corresponding drupal uid for a pmperson id.
 */
function _pmtimetracking_get_drupal_uid_from_pmperson_nid($nid) {
  $r = db_select('pmperson', 'p')
    ->fields('p')
    ->condition('p.nid', $nid)
    ->execute()
    ->fetchAssoc();
  $uid = empty($r) ? FALSE : $r->user_uid;
  return $uid;
}
