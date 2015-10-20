<?php
/**
 * @file
 * Migration functions for the PM Task module.
 */

define('PMTASK_MIGRATE_MAX_JOB_PER_BATCH', 5);

/**
 * Helper function for migrating between pmtask to drupal fields.
 */
function pmtask_migrate(&$sandbox) {
  pmtask_migrate_create_fields($sandbox);
  // Migrate data from pmtask node to Drupal user account.
  if (empty($sandbox['pmtask_current_vid_of_field_being_migrated'])) {
    $sandbox['pmtask_current_vid_of_field_being_migrated'] = 0;
  }
  if (pmtask_migrate_field_data($sandbox) == FALSE) {
    $sandbox['#finished'] = 0.5;
    return;
  }

  // We don't need pmtask table anymore, so dropping it.
  db_drop_table('pmtask');
  variable_del('node_options_pmtask');

  // PM Permissions default configurations.
  variable_set('pmpermission_field_assigned_reference', 'pm_assigned');
  variable_set('pmpermission_field_pm_reference', 'pm_manager');
  variable_set('pmpermission_field_parent_reference_for_pmtask', 'pmtask_parent');
  variable_set('pmpermission_node_pmtask_enabled', TRUE);

  module_load_include('inc', 'pmpermission', 'includes/pmpermission.migrate');
  pmpermission_migrate_execute('pmtask');

  $sandbox['#finished'] = 1;
  return 'PM Task nodes have been migrated to field_api based fields successfully.';
}

/**
 * Check if migration needs to be performed.
 */
function pmtask_migrate_update_could_be_performed() {
  if (db_table_exists('pmtask')) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Creates and attaches fields to Drupal user.
 */
function pmtask_migrate_create_fields(&$sandbox) {
  module_load_include('inc', 'pmtask', 'includes/pmtask.field_base');
  module_load_include('inc', 'pmtask', 'includes/pmtask.field_instance');
  module_load_include('inc', 'pm', 'includes/pm.field');

  $field_bases = pmtask_default_field_bases();
  pm_field_bases_create_if_required($field_bases);

  $field_instances = pmtask_default_field_instances();
  pm_field_instances_create_if_required($field_instances);
  return TRUE;
}

/**
 * Migrate pmtask node fields data to drupal user bundle.
 */
function pmtask_migrate_field_data(&$sandbox) {
  $results = db_select('pmtask', 'pmt')
    ->fields('pmt')
    ->condition('vid', $sandbox['pmtask_current_vid_of_field_being_migrated'], '>')
    ->range(0, PMTASK_MIGRATE_MAX_JOB_PER_BATCH)
    ->execute();
  $count = 0;
  foreach ($results as $pmtask) {
    $count++;
    $sandbox['pmtask_current_vid_of_field_being_migrated'] = $pmtask->vid;
    try {
      _pmtask_migrate_migrate_single_node($pmtask->nid, $pmtask->vid, $pmtask);
    }
    catch (Exception $exc) {
      watchdog(
        'pmtask',
        'See ' . __FUNCTION__ . '() ' . $exc->getTraceAsString(),
        NULL, WATCHDOG_ERROR
      );
    }
  }
  return empty($count);
}

/**
 * Helper function to migrate single pmtask.
 */
function _pmtask_migrate_migrate_single_node($nid, $vid, $task) {
  $node = node_load($nid, $vid);

  $assigned = _pmtask_get_drupal_uid_from_pmperson_nid($task->assigned_nid);
  if ($assigned) {
    $node->pm_assigned[LANGUAGE_NONE][0]['target_id'] = $assigned;
  }

  if ($task->taskcategory) {
    $node->pm_category[LANGUAGE_NONE][0]['value'] = $task->taskcategory;
  }

  if ($task->duration) {
    $node->pm_duration[LANGUAGE_NONE][0]['value'] = $task->duration;
  }

  if ($task->durationunit) {
    $node->pm_durationunit[LANGUAGE_NONE][0]['value'] = $task->durationunit;
  }

  if ($task->pricemode) {
    $node->pm_pricemode[LANGUAGE_NONE][0]['value'] = $task->pricemode;
  }

  if ($task->price) {
    $node->pm_price[LANGUAGE_NONE][0]['value'] = $task->price;
  }

  if ($task->currency) {
    $node->pm_currency[LANGUAGE_NONE][0]['value'] = $task->currency;
  }

  if ($task->taskpriority) {
    $node->pm_priority[LANGUAGE_NONE][0]['value'] = $task->taskpriority;
  }

  if ($task->taskstatus) {
    $node->pm_status[LANGUAGE_NONE][0]['value'] = $task->taskstatus;
  }
  if ($task->stepno) {
    $node->pm_step_number[LANGUAGE_NONE][0]['value'] = $task->stepno;
  }
  if ($task->weight) {
    $node->pm_weight[LANGUAGE_NONE][0]['value'] = $task->weight;
  }

  if (isset($task->billed) AND isset($task->billable)) {
    $new_value = $task->billed ? 'Billed' : ($task->billable ? 'Billable' : 'Not
billable');
    $node->pm_billing_status[LANGUAGE_NONE][0]['value'] = $new_value;
  }

  if (isset($task->datebegin) AND !empty($task->datebegin)) {

    $node->pm_date[LANGUAGE_NONE][0]['value'] = $task->datebegin;

    if (isset($task->dateend) AND !empty($task->dateend)) {
      $node->pm_date[LANGUAGE_NONE][0]['value2'] = $task->dateend;
    }
  }

  if ($task->parent_nid) {
    $node->pmtask_parent[LANGUAGE_NONE][0]['target_id'] = $task->parent_nid;
  }
  elseif ($task->project_nid) {
    $node->pmtask_parent[LANGUAGE_NONE][0]['target_id'] = $task->project_nid;
  }

  node_save($node);
  return TRUE;
}

/**
 * Give back the corresponding drupal uid for a pmperson id.
 */
function _pmtask_get_drupal_uid_from_pmperson_nid($nid) {
  $r = db_select('pmperson', 'p')
    ->fields('p')
    ->condition('p.nid', $nid)
    ->execute()
    ->fetchAssoc();
  $uid = empty($r) ? FALSE : $r->user_uid;
  return $uid;
}
