<?php
// $Id$

/**
 * @file
 * OM Maps Admin Configuration
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * Admin Form - Enabling the Maps
 *
 */
function om_maps_admin() {
    
  drupal_set_title(t('OM Maps Settings'));

  // checking on OM Maximenu variable if the maps are loaded
  $codes = array_keys(om_maps_loaded_maps_get());

  // getting available maps, codes only
  $maps = om_maps_get();

  $form = array();
  $form['om_maps'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#description' => t('Click load button to upload map/s. These Map/s are automatically added to OM Maximenu.<br />
      (Once enabled, it can only be disabled by deleting it using the OM Maximenu settings.)
    '),
  );  
  $maps_array = array();  
  foreach ($maps as $file_name => $map) { 
    $enabled = in_array($map['code'], $codes) ? 1: 0;
    $title   = $enabled ? om_string_name($map['map_name'], FALSE) . ' (Enabled)': om_string_name($map['map_name'], FALSE);
    $maps_array[$file_name]['enabled'] = array(
      '#type'          => 'checkbox',
      '#title'         => $title,
      '#disabled'      => $enabled,
      '#default_value' => $enabled,
    ); 
   $form['om_maps'] += $maps_array;
  }
   
  $form['Load'] = array( '#type' => 'submit', '#value' => t('Load'), );
  
  return $form;
}


/**
 * 1 Submit for all settings
 *
 */
function om_maps_admin_submit($form, $form_state) {
  global $_om_maximenu_variable;
  $maximenu = $_om_maximenu_variable;

  $maps = $form_state['values']['om_maps'];
  
  // gets the highest key value of existing menus
  $count = !empty($maximenu) ? max(array_keys($maximenu)) + 1: 1;
  
  $i = 0;  
  foreach ($maps as $file_name => $enabled) {
    if ($enabled['enabled'] == 1) {
      $count =  $count + $i;
      $maximenu[$count] = om_maps_content_get($file_name, FALSE);
      $i++;
    } 
  }

  // Save all settings in 1 variable
  variable_set('om_maximenu', $maximenu);
  
  drupal_set_message(t('Your map/s have been loaded.'));
} 

