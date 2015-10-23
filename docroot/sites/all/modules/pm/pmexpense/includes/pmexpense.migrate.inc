<?php
/**
 * @file
 * Migration functions for the PM Expense module.
 */

define('PMEXPENSE_MIGRATE_MAX_JOB_PER_BATCH', 5);

/**
 * Helper function for migrating between pmexpense to drupal fields.
 */
function pmexpense_migrate(&$sandbox) {

  pmexpense_migrate_create_fields($sandbox);
  // Migrate data from pmexpense node to Drupal user account.
  if (empty($sandbox['pmexpense_current_vid_of_field_being_migrated'])) {
    $sandbox['pmexpense_current_vid_of_field_being_migrated'] = 0;
  }
  if (pmexpense_migrate_field_data($sandbox) == FALSE) {
    $sandbox['#finished'] = 0.5;
    return;
  }
  // We don't need pmexpense table anymore, so dropping it.
  db_drop_table('pmexpense');
  variable_del('node_options_pmexpense');

  // PM Permissions default configurations.
  variable_set('pmpermission_field_parent_reference_for_pmexpense', 'pmexpense_parent');
  variable_set('pmpermission_node_pmexpense_enabled', TRUE);

  module_load_include('inc', 'pmpermission', 'includes/pmpermission.migrate');
  pmpermission_migrate_execute('pmexpense');

  $sandbox['#finished'] = 1;
  return 'PM Note nodes have been migrated to field_api based fields successfully.';
}


/**
 * Check if migration need to be performed.
 */
function pmexpense_migrate_update_could_be_performed() {
  if (db_table_exists('pmexpense')) {
    return TRUE;
  }
  return FALSE;
}


/**
 * Creates and attaches fields to Drupal user.
 */
function pmexpense_migrate_create_fields(&$sandbox) {
  module_load_include('inc', 'pmexpense', 'includes/pmexpense.field_base');
  module_load_include('inc', 'pmexpense', 'includes/pmexpense.field_instance');
  module_load_include('inc', 'pm', 'includes/pm.field');

  $field_bases = pmexpense_default_field_bases();
  pm_field_bases_create_if_required($field_bases);

  $field_instances = pmexpense_default_field_instances();
  pm_field_instances_create_if_required($field_instances);
  return TRUE;
}


/**
 * Migrate pmexpense node fields data to drupal user bundle.
 */
function pmexpense_migrate_field_data(&$sandbox) {
  $results = db_select('pmexpense', 'pmx')
    ->fields('pmx')
    ->condition('vid', $sandbox['pmexpense_current_vid_of_field_being_migrated'], '>')
    ->range(0, PMEXPENSE_MIGRATE_MAX_JOB_PER_BATCH)
    ->execute();
  $count = 0;
  foreach ($results as $pmexpense) {
    $count++;
    $sandbox['pmexpense_current_vid_of_field_being_migrated'] = $pmexpense->vid;
    try {
      _pmexpense_migrate_migrate_single_node($pmexpense->nid, $pmexpense->vid, $pmexpense);
    }
    catch (Exception $exc) {
      watchdog(
        'pmexpense',
        'See ' . __FUNCTION__ . '() ' . $exc->getTraceAsString(),
        NULL, WATCHDOG_ERROR
      );
    }
  }
  return empty($count);
}


/**
 * Helper function to migrate single pmexpense.
 */
function _pmexpense_migrate_migrate_single_node($nid, $vid, $expense) {
  $node = node_load($nid, $vid);

  // Ticket >> Task >> Project >> Organization.
  if ($expense->ticket_nid) {
    $node->pmexpense_parent[LANGUAGE_NONE][0]['target_id'] = $expense->ticket_nid;
  }
  elseif ($expense->task_nid) {
    $node->pmexpense_parent[LANGUAGE_NONE][0]['target_id'] = $expense->task_nid;
  }
  elseif ($expense->project_nid) {
    $node->pmexpense_parent[LANGUAGE_NONE][0]['target_id'] = $expense->project_nid;
  }
  elseif ($expense->organization_nid) {
    $node->pmexpense_parent[LANGUAGE_NONE][0]['target_id'] = $expense->organization_nid;
  }

  $node->pm_amount[LANGUAGE_NONE][0]['value'] = $expense->amount;
  if ($expense->provider_nid) {
    $node->pm_provider[LANGUAGE_NONE][0]['value'] = $expense->provider_nid;
  }

  node_save($node);
  return TRUE;
}
