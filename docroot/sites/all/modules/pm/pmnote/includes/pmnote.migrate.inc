<?php
/**
 * @file
 * Migration functions for the PM Note module.
 */

define('PMNOTE_MIGRATE_MAX_JOB_PER_BATCH', 5);

/**
 * Helper function for migrating between pmnote to drupal fields.
 */
function pmnote_migrate(&$sandbox) {
  pmnote_migrate_create_fields($sandbox);
  // Migrate data from pmnote node to Drupal user account.
  if (empty($sandbox['pmnote_current_vid_of_field_being_migrated'])) {
    $sandbox['pmnote_current_vid_of_field_being_migrated'] = 0;
  }
  if (pmnote_migrate_field_data($sandbox) == FALSE) {
    $sandbox['#finished'] = 0.5;
    return;
  }
  // We don't need pmnote table anymore, so dropping it.
  db_drop_table('pmnote');
  variable_del('node_options_pmnote');

  // PM Permissions default configurations.
  variable_set('pmpermission_field_parent_reference_for_pmnote', 'pmnote_parent');
  variable_set('pmpermission_node_pmnote_enabled', TRUE);

  module_load_include('inc', 'pmpermission', 'includes/pmpermission.migrate');
  pmpermission_migrate_execute('pmnote');

  $sandbox['#finished'] = 1;
  return 'PM Note nodes have been migrated to field_api based fields successfully.';
}


/**
 * Check if migration need to be performed.
 */
function pmnote_migrate_update_could_be_performed() {
  if (db_table_exists('pmnote')) {
    return TRUE;
  }
  return FALSE;
}


/**
 * Creates and attaches fields to Drupal user.
 */
function pmnote_migrate_create_fields(&$sandbox) {
  module_load_include('inc', 'pmnote', 'includes/pmnote.field_base');
  module_load_include('inc', 'pmnote', 'includes/pmnote.field_instance');
  module_load_include('inc', 'pm', 'includes/pm.field');

  $field_bases = pmnote_default_field_bases();
  pm_field_bases_create_if_required($field_bases);

  $field_instances = pmnote_default_field_instances();
  pm_field_instances_create_if_required($field_instances);
  return TRUE;
}


/**
 * Migrate pmnote node fields data to drupal user bundle.
 */
function pmnote_migrate_field_data(&$sandbox) {
  $results = db_select('pmnote', 'pmn')
    ->fields('pmn')
    ->condition('vid', $sandbox['pmnote_current_vid_of_field_being_migrated'], '>')
    ->range(0, PMNOTE_MIGRATE_MAX_JOB_PER_BATCH)
    ->execute();
  $count = 0;
  foreach ($results as $pmnote) {
    $count++;
    $sandbox['pmnote_current_vid_of_field_being_migrated'] = $pmnote->vid;
    try {
      _pmnote_migrate_migrate_single_node($pmnote->nid, $pmnote->vid, $pmnote);
    }
    catch (Exception $exc) {
      watchdog(
        'pmnote',
        'See ' . __FUNCTION__ . '() ' . $exc->getTraceAsString(),
        NULL, WATCHDOG_ERROR
      );
    }
  }
  return empty($count);
}


/**
 * Helper function to migrate single pmnote.
 */
function _pmnote_migrate_migrate_single_node($nid, $vid, $note) {
  $node = node_load($nid, $vid);

  // Ticket >> Task >> Project >> Organization.
  if ($note->task_nid) {
    $node->pmnote_parent[LANGUAGE_NONE][0]['target_id'] = $note->task_nid;
  }
  elseif ($note->project_nid) {
    $node->pmnote_parent[LANGUAGE_NONE][0]['target_id'] = $note->project_nid;
  }
  elseif ($note->organization_nid) {
    $node->pmnote_parent[LANGUAGE_NONE][0]['target_id'] = $note->organization_nid;
  }

  node_save($node);
  return TRUE;
}

/**
 * Give back the corresponding drupal uid for a pmperson id.
 */
function _pmnote_get_drupal_uid_from_pmperson_nid($nid) {
  $r = db_select('pmperson', 'p')
    ->fields('p')
    ->condition('p.nid', $nid)
    ->execute()
    ->fetchAssoc();
  $uid = empty($r) ? FALSE : $r->user_uid;
  return $uid;
}
