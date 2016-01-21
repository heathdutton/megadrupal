<?php

/**
 * Admin forms
 *
 */

function vgwort_admin_settings() {
  $allowed_entities = array('node') ;
  $info = entity_get_info();

  $form = array();
  $form['vgwort_user'] = array(
    '#type' => 'textfield',
    '#title' => t('VGWort user name'),
    '#default_value' => variable_get('vgwort_user', '')
  );

  $form['vgwort_password'] = array(
    '#type' => 'textfield',
    '#title' => t('VGWort pass name'),
    '#default_value' => variable_get('vgwort_password', '')
  );

  $form['vgwort_index'] = array(
    '#type' => 'textfield',
    '#title' => t('VGWort index number'),
    '#default_value' => variable_get('vgwort_index', '')
  );

  $form['vgwort_threshold'] = array(
      '#type' => 'textfield',
      '#title' => t('VGWort threshold'),
      '#default_value' => variable_get('vgwort_threshold', VGWORT_THRESHOLD)
  );

  $form['vgwort_import'] = array(
      '#type' => 'textfield',
      '#title' => t('VGWort default pixel count import'),
      '#default_value' => variable_get('vgwort_import', VGWORT_IMPORT)
  );

  $form['vgwort_test'] = array(
    '#type' => 'checkbox',
    '#title' => t('VGWort test mode'),
    '#default_value' => variable_get('vgwort_test', 0),
    '#return_value' => 1
  );
  $form['vgwort_bundles'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE ,
  );

  $bundles = variable_get('vgwort_bundles', array()) ;
  $bundles = array_filter($bundles) ;

  foreach ( $info as $type => $einfo ) {
    foreach ( $einfo['bundles'] as $key => $binfo ) {
      $form['vgwort_bundles'][] = array(
        '#tree' => TRUE ,
        '#type' => 'checkbox',
        '#default_value' => (in_array($type . '|' . $key, $bundles)) ? ($type . '|' . $key) : FALSE,
        '#return_value' => $type . '|' . $key,
        '#title' => $type . ' | ' . $binfo['label']
      );
    }
  }

  return system_settings_form($form) ;
}