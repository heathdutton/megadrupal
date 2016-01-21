<?php

/**
 * @file
 * Hooks provided by the itemsessionlock module.
 */

/**
 * Implements hook_itemsessionlock_info().
 * @return array of lock types,
 * keyed by type 'machine name'.
 */
function hook_itemsessionlock_info() {
  $locks = array();
  $locks['my_type'] = array(
    'label' => t('My type'),
    // TRUE is the default, and does not need to be specified,
    // given for reference only.
    // Setting these to FALSE will disable the automatic
    // creation of the corresponding resource.
    'view' => TRUE,
    'permission' => TRUE,
    'cron' => FALSE,
    'menu' => TRUE,
  );
  $locks['another'] = array(
    'label' => t('Another type'),
  );
  return $locks;
}
