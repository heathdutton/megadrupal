<?php
/**
 * @file
 * Migration functions for the PM Ticket module.
 */

define('PMTICKET_MIGRATE_MAX_JOB_PER_BATCH', 5);

/**
 * Helper function for migrating between pmticket to drupal fields.
 */
function pmticket_migrate(&$sandbox) {
  pmticket_migrate_create_fields($sandbox);
  // Migrate data from pmticket node to Drupal user account.
  if (empty($sandbox['pmticket_current_vid_of_field_being_migrated'])) {
    $sandbox['pmticket_current_vid_of_field_being_migrated'] = 0;
  }
  if (pmticket_migrate_field_data($sandbox) == FALSE) {
    $sandbox['#finished'] = 0.5;
    return;
  }

  // We don't need pmticket table anymore, so dropping it.
  db_drop_table('pmticket');
  variable_del('node_options_pmticket');

  // PM Permissions default configurations.
  variable_set('pmpermission_field_assigned_reference', 'pm_assigned');
  variable_set('pmpermission_field_pm_reference', 'pm_manager');
  variable_set('pmpermission_field_parent_reference_for_pmticket', 'pmticket_parent');
  variable_set('pmpermission_node_pmticket_enabled', TRUE);

  module_load_include('inc', 'pmpermission', 'includes/pmpermission.migrate');
  pmpermission_migrate_execute('pmticket');

  $sandbox['#finished'] = 1;
  return 'PM Ticket nodes have been migrated to field_api based fields successfully.';
}

/**
 * Check if migration need to be performed.
 */
function pmticket_migrate_update_could_be_performed() {
  if (db_table_exists('pmticket')) {
    return TRUE;
  }
  return FALSE;
}


/**
 * Creates and attaches fields to Drupal user.
 */
function pmticket_migrate_create_fields(&$sandbox) {
  module_load_include('inc', 'pmticket', 'includes/pmticket.field_base');
  module_load_include('inc', 'pmticket', 'includes/pmticket.field_instance');
  module_load_include('inc', 'pm', 'includes/pm.field');

  $field_bases = pmticket_default_field_bases();
  pm_field_bases_create_if_required($field_bases);

  $field_instances = pmticket_default_field_instances();
  pm_field_instances_create_if_required($field_instances);
  return TRUE;
}

/**
 * Migrate pmticket node fields data to drupal user bundle.
 */
function pmticket_migrate_field_data(&$sandbox) {
  $results = db_select('pmticket', 'pmt')
    ->fields('pmt')
    ->condition('vid', $sandbox['pmticket_current_vid_of_field_being_migrated'], '>')
    ->range(0, PMTICKET_MIGRATE_MAX_JOB_PER_BATCH)
    ->execute();
  $count = 0;
  foreach ($results as $pmticket) {
    $count++;
    $sandbox['pmticket_current_vid_of_field_being_migrated'] = $pmticket->vid;
    try {
      _pmticket_migrate_migrate_single_node($pmticket->nid, $pmticket->vid, $pmticket);
    }
    catch (Exception $exc) {
      watchdog(
        'pmticket',
        'See ' . __FUNCTION__ . '() ' . $exc->getTraceAsString(),
        NULL, WATCHDOG_ERROR
      );
    }
  }
  return empty($count);
}

/**
 * Helper function to migrate single pmticket.
 */
function _pmticket_migrate_migrate_single_node($nid, $vid, $ticket) {
  $node = node_load($nid, $vid);

  $assigned = _pmticket_get_drupal_uid_from_pmperson_nid($ticket->assigned_nid);
  if ($assigned) {
    $node->pm_assigned[LANGUAGE_NONE][0]['target_id'] = $assigned;
  }

  if ($ticket->ticketcategory) {
    $node->pm_category[LANGUAGE_NONE][0]['value'] = $ticket->ticketcategory;
  }

  if ($ticket->duration) {
    $node->pm_duration[LANGUAGE_NONE][0]['value'] = $ticket->duration;
  }

  if ($ticket->durationunit) {
    $node->pm_durationunit[LANGUAGE_NONE][0]['value'] = $ticket->durationunit;
  }

  if ($ticket->pricemode) {
    $node->pm_pricemode[LANGUAGE_NONE][0]['value'] = $ticket->pricemode;
  }

  if ($ticket->price) {
    $node->pm_price[LANGUAGE_NONE][0]['value'] = $ticket->price;
  }

  if ($ticket->currency) {
    $node->pm_currency[LANGUAGE_NONE][0]['value'] = $ticket->currency;
  }

  if ($ticket->ticketpriority) {
    $node->pm_priority[LANGUAGE_NONE][0]['value'] = $ticket->ticketpriority;
  }

  if ($ticket->ticketstatus) {
    $node->pm_status[LANGUAGE_NONE][0]['value'] = $ticket->ticketstatus;
  }

  if (isset($ticket->billed) AND isset($ticket->billable)) {
    $new_value = $ticket->billed ? 'Billed' : ($ticket->billable ? 'Billable' : 'Not billable');
    $node->pm_billing_status[LANGUAGE_NONE][0]['value'] = $new_value;
  }

  if (isset($ticket->datebegin) AND !empty($ticket->datebegin)) {

    $node->pm_date[LANGUAGE_NONE][0]['value'] = $ticket->datebegin;

    if (isset($ticket->dateend) AND !empty($ticket->dateend)) {
      $node->pm_date[LANGUAGE_NONE][0]['value2'] = $ticket->dateend;
    }
  }

  if ($ticket->parent_nid) {
    $node->pmticket_parent[LANGUAGE_NONE][0]['target_id'] = $ticket->parent_nid;
  }
  elseif ($ticket->project_nid) {
    $node->pmticket_parent[LANGUAGE_NONE][0]['target_id'] = $ticket->project_nid;
  }

  node_save($node);
  return TRUE;
}

/**
 * Give back the corresponding drupal uid for a pmperson id.
 */
function _pmticket_get_drupal_uid_from_pmperson_nid($nid) {
  $r = db_select('pmperson', 'p')
    ->fields('p')
    ->condition('p.nid', $nid)
    ->execute()
    ->fetchAssoc();
  $uid = empty($r) ? FALSE : $r->user_uid;
  return $uid;
}
