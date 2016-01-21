<?php
/**
 * @file
 * Rules configurations for Stamps.com shipping modes.
 */

/**
 * Implements hook_default_rules_configuration().
 *
 * This creates a condition for every shipping mode.  This is called
 * after each new shipping mode is created by calling entity_flush_caches().
 */
function uc_stamps_default_rules_configuration() {
  $configs = array();
  $modes = db_select('uc_stamps_modes', 'm')
           ->fields('m', array('mid', 'name'))
           ->execute();
  foreach ($modes as $mode) {
    $set = rules_and(array(
      'order' => array('type' => 'uc_order', 'label' => t('Order')),
    ));
    $set->label = t('@modename conditions', array('@modename' => $mode->name));
    $configs['uc_stamps_mode_condition_' . $mode->mid] = $set;
  }
  return $configs;
}
