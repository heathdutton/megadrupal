<?php
/**
 * @file
 * Migration functions for the PM Organization module.
 */

define('PMORGANIZATION_MIGRATE_MAX_JOB_PER_BATCH', 5);

/**
 * Helper function for migrating between pmorganization to drupal fields.
 */
function pmorganization_migrate(&$sandbox) {
  pmorganization_migrate_create_fields($sandbox);

  // Migrate data from pmorganization node to Drupal user account.
  if (empty($sandbox['pmorganization_current_vid_of_field_being_migrated'])) {
    $sandbox['pmorganization_current_vid_of_field_being_migrated'] = 0;
  }
  if (pmorganization_migrate_field_data($sandbox) == FALSE) {
    $sandbox['#finished'] = 0.5;
    return;
  }

  // We don't need pmorganization table anymore, so dropping it.
  db_drop_table('pmorganization');
  variable_del('node_options_pmorganization');

  variable_set('pmpermission_field_org_member_reference', 'pmorganization_members');
  variable_set('pmpermission_node_pmorganization_enabled', TRUE);

  module_load_include('inc', 'pmpermission', 'includes/pmpermission.migrate');
  pmpermission_migrate_execute('pmorganization');

  $sandbox['#finished'] = 1;
  return 'PM Organization nodes have been migrated to field_api based fields successfully.';
}


/**
 * Check if migration need to be performed.
 */
function pmorganization_migrate_update_could_be_performed() {
  if (db_table_exists('pmorganization')) {
    return TRUE;
  }
  return FALSE;
}


/**
 * Creates and attaches fields to Drupal user.
 */
function pmorganization_migrate_create_fields(&$sandbox) {
  module_load_include('inc', 'pmorganization', 'includes/pmorganization.field_base');
  module_load_include('inc', 'pmorganization', 'includes/pmorganization.field_instance');
  module_load_include('inc', 'pm', 'includes/pm.field');

  $field_bases = pmorganization_default_field_bases();
  pm_field_bases_create_if_required($field_bases);

  $field_instances = pmorganization_default_field_instances();
  pm_field_instances_create_if_required($field_instances);
  return TRUE;
}


/**
 * Migrate pmorganization node fields data to drupal user bundle.
 */
function pmorganization_migrate_field_data(&$sandbox) {
  $results = db_select('pmorganization', 'pmorg')
    ->fields('pmorg')
    ->condition('vid', $sandbox['pmorganization_current_vid_of_field_being_migrated'], '>')
    ->range(0, PMORGANIZATION_MIGRATE_MAX_JOB_PER_BATCH)
    ->execute();
  $count = 0;
  foreach ($results as $pmorganization) {
    $count++;
    $sandbox['pmorganization_current_vid_of_field_being_migrated'] = $pmorganization->vid;
    try {
      _pmorganization_migrate_migrate_single_node($pmorganization->nid, $pmorganization->vid, $pmorganization);
    }
    catch (Exception $exc) {
      watchdog(
        'pmorganization',
        'See ' . __FUNCTION__ . '() ' . $exc->getTraceAsString(),
        NULL, WATCHDOG_ERROR
      );
    }
  }
  return empty($count);
}

/**
 * Helper function to migrate single pmorganization.
 */
function _pmorganization_migrate_migrate_single_node($nid, $vid, $org) {
  $node = node_load($nid, $vid);

  if ($org->currency) {
    $node->pm_currency[LANGUAGE_NONE][0]['value'] = $org->currency;
  }

  if ($org->orglanguage) {
    $node->pm_orglanguage[LANGUAGE_NONE][0]['value'] = $org->orglanguage;
  }

  if ($org->pricemode) {
    $node->pm_pricemode[LANGUAGE_NONE][0]['value'] = $org->pricemode;
  }

  if ($org->price) {
    $node->pm_price[LANGUAGE_NONE][0]['value'] = $org->price;
  }

  if ($org->email) {
    $node->pm_mail[LANGUAGE_NONE][0]['email'] = $org->email;
  }

  if ($org->www) {
    $node->pm_www[LANGUAGE_NONE][0]['url'] = $org->www;
  }

  if ($org->taxid) {
    $node->pm_taxid[LANGUAGE_NONE][0]['value'] = $org->taxid;
  }

  if ($org->phone) {
    $node->pm_phone[LANGUAGE_NONE][0]['value'] = $org->phone;
  }

  $address = array();
  if ($org->address) {
    $address['premise'] = $org->address;
  }
  if ($org->city) {
    $address['locality'] = $org->city;
  }
  if ($org->provstate) {
    $address['thoroughfare'] = $org->provstate;
  }
  if ($org->country) {
    $address['country'] = $org->country;
  }
  if ($org->zip) {
    $address['postal_code'] = $org->zip;
  }

  $type = array();
  $type['customer'] = $org->iscustomer;
  $type['provider'] = $org->isprovider;

  $i = 0;
  foreach ($type as $key => $value) {
    if ($value) {
      $node->pmorganization_type[LANGUAGE_NONE][$i]['value'] = $key;
      ++$i;
    }
  }

  if ($org->isactive) {
    $node->pm_isactive[LANGUAGE_NONE][0]['value'] = '1';
  }

  $results = db_select('pmperson', 'pmp')
    ->fields('pmp', array('user_uid'))
    ->condition('pmp.organization_nid', $nid)
    ->execute();
  foreach ($results as $pmperson) {
    $node->pmorganization_members[LANGUAGE_NONE][] = array('target_id' => $pmperson->user_uid);
  }

  node_save($node);

  if ($address) {
    $wrapper = entity_metadata_wrapper('node', $node);
    foreach ($address as $key => $value) {
      $wrapper->pm_address->$key = $value;
    }
    $wrapper->save();
  }
  return TRUE;
}
