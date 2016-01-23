<?php
/**
 * @file
 */
/**
 * Implements hook_form().
 * TowTruct admin settings form callback
 */
function towtruck_admin_settings_form($form, &$form_state) {
  $form['towtruck_main_js'] = array(
    '#type' => 'textfield',
    '#title' => t('TownTruck main js file location'),
    '#default_value' => variable_get('towtruck_main_js', TowTruckMainJS)    
  );
  $form['TowTruckConfig_enableAnalytics'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable analytics'),
    '#default_value' => variable_get('TowTruckConfig_enableAnalytics', TowTruckEnableAnalytics)
  );
  $form['TowTruckConfig_analyticsCode'] = array(
    '#type' => 'textfield',
    '#title' => t('Analytics code'),
    '#default_value' => variable_get('TowTruckConfig_analyticsCode', TowTruckAnalyticsCode)
  );
  $form['TowTruckConfig_cloneClicks'] = array(
    '#type' => 'checkbox',
    '#title' => t('Clone clicks'),
    '#default_value' => variable_get('TowTruckConfig_cloneClicks', TowTruckCloneClicks)
  );
  $form['TowTruckConfig_hubBase'] = array(
    '#type' => 'textfield',
    '#title' => t('Clone clicks'),
    '#default_value' => variable_get('TowTruckConfig_hubBase', TowTruckHubBase)
  );
  return system_settings_form($form);
}
