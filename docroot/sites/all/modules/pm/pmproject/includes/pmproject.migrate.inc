<?php
/**
 * @file
 * Migration functions for the PM Project module.
 */

define('PMPROJECT_MIGRATE_MAX_JOB_PER_BATCH', 5);

/**
 * Helper function for migrating between pmproject to drupal fields.
 */
function pmproject_migrate(&$sandbox) {
  pmproject_migrate_create_fields($sandbox);
  // Migrate data from pmproject node to Drupal user account.
  if (empty($sandbox['pmproject_current_vid_of_field_being_migrated'])) {
    $sandbox['pmproject_current_vid_of_field_being_migrated'] = 0;
  }
  if (pmproject_migrate_field_data($sandbox) == FALSE) {
    $sandbox['#finished'] = 0.5;
    return;
  }

  // We don't need pmproject table anymore, so dropping it.
  db_drop_table('pmproject');
  variable_del('node_options_pmproject');

  // PM Permissions default configurations.
  variable_set('pmpermission_field_assigned_reference', 'pm_assigned');
  variable_set('pmpermission_field_pm_reference', 'pm_manager');
  variable_set('pmpermission_field_parent_reference_for_pmproject', 'pmproject_parent');
  variable_set('pmpermission_node_pmproject_enabled', TRUE);

  module_load_include('inc', 'pmpermission', 'includes/pmpermission.migrate');
  pmpermission_migrate_execute('pmproject');

  $sandbox['#finished'] = 1;
  return 'PM Project nodes have been migrated to field_api based fields successfully.';
}

/**
 * Check if migration needs to be performed.
 */
function pmproject_migrate_update_could_be_performed() {
  if (db_table_exists('pmproject')) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Creates and attaches fields to Drupal user.
 */
function pmproject_migrate_create_fields(&$sandbox) {
  module_load_include('inc', 'pmproject', 'includes/pmproject.field_base');
  module_load_include('inc', 'pmproject', 'includes/pmproject.field_instance');
  module_load_include('inc', 'pm', 'includes/pm.field');

  $field_bases = pmproject_default_field_bases();
  pm_field_bases_create_if_required($field_bases);

  $field_instances = pmproject_default_field_instances();
  pm_field_instances_create_if_required($field_instances);
  return TRUE;
}

/**
 * Migrate pmproject node fields data to drupal user bundle.
 */
function pmproject_migrate_field_data(&$sandbox) {
  $results = db_select('pmproject', 'pmprj')
    ->fields('pmprj')
    ->condition('vid', $sandbox['pmproject_current_vid_of_field_being_migrated'], '>')
    ->range(0, PMPROJECT_MIGRATE_MAX_JOB_PER_BATCH)
    ->execute();
  $count = 0;
  foreach ($results as $pmproject) {
    $count++;
    $sandbox['pmproject_current_vid_of_field_being_migrated'] = $pmproject->vid;
    try {
      _pmproject_migrate_migrate_single_node($pmproject->nid, $pmproject->vid, $pmproject);
    }
    catch (Exception $exc) {
      watchdog(
        'pmproject',
        'See ' . __FUNCTION__ . '() ' . $exc->getTraceAsString(),
        NULL, WATCHDOG_ERROR
      );
    }
  }
  return empty($count);
}

/**
 * Helper function to migrate single pmproject.
 */
function _pmproject_migrate_migrate_single_node($nid, $vid, $prj) {
  $node = node_load($nid, $vid);

  $assigned = _pmproject_get_drupal_uid_from_pmperson_nid($prj->assigned_nid);
  if ($assigned) {
    $node->pm_assigned[LANGUAGE_NONE][0]['target_id'] = $assigned;
  }

  $manager = _pmproject_get_drupal_uid_from_pmperson_nid($prj->manager_nid);
  if ($manager) {
    $node->pm_manager[LANGUAGE_NONE][0]['target_id'] = $manager;
  }

  if ($prj->duration) {
    $node->pm_duration[LANGUAGE_NONE][0]['value'] = $prj->duration;
  }

  if ($prj->datebegin) {
    $node->pm_date[LANGUAGE_NONE][0]['value'] = $prj->datebegin;
  }

  if ($prj->dateend) {
    $node->pm_date[LANGUAGE_NONE][0]['value2'] = $prj->dateend;
  }

  if ($prj->billed) {
    $node->pm_billing_status[LANGUAGE_NONE][0]['value'] = 'Billed';
  }
  elseif ($prj->billable) {
    $node->pm_billing_status[LANGUAGE_NONE][0]['value'] = 'Billable';
  }

  if ($prj->durationunit) {
    $node->pm_durationunit[LANGUAGE_NONE][0]['value'] = $prj->durationunit;
  }

  if ($prj->projectcategory) {
    $node->pm_projectcategory[LANGUAGE_NONE][0]['value'] = $prj->projectcategory;
  }

  if ($prj->projectpriority) {
    $node->pm_projectpriority[LANGUAGE_NONE][0]['value'] = $prj->projectpriority;
  }

  if ($prj->projectstatus) {
    $node->pm_projectstatus[LANGUAGE_NONE][0]['value'] = $prj->projectstatus;
  }

  if ($prj->organization_nid) {
    $node->pmproject_parent[LANGUAGE_NONE][0]['target_id'] = $prj->organization_nid;
  }

  if ($prj->currency) {
    $node->pm_currency[LANGUAGE_NONE][0]['value'] = $prj->currency;
  }

  if ($prj->price) {
    $node->pm_price[LANGUAGE_NONE][0]['value'] = $prj->price;
  }

  if ($prj->pricemode) {
    $node->pm_pricemode[LANGUAGE_NONE][0]['value'] = $prj->pricemode;
  }

  node_save($node);
  return TRUE;
}

/**
 * Give back the corresponding drupal uid for a pmperson id.
 */
function _pmproject_get_drupal_uid_from_pmperson_nid($nid) {
  $r = db_select('pmperson', 'p')
    ->fields('p')
    ->condition('p.nid', $nid)
    ->execute()
    ->fetchAssoc();
  $uid = empty($r) ? FALSE : $r->user_uid;
  return $uid;
}
