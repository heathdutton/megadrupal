<?php

/**
 * @file
 * Settings forms
 */

/**
 * Create settings form in drupal configuration setings section for module
 */
function drupal_sync_initial_setup_admin_settings_form($form, &$form_state) {
  $node_types = node_type_get_types();
  $vocs = taxonomy_vocabulary_get_names();

  $voc_options = array();
  foreach ($vocs as $voc) {
    $voc_options[$voc->vid] = $voc->name;
  }

  $node_types = node_type_get_types();
  $node_options = array();
  foreach ($node_types as $node_type) {
    $node_options[$node_type->type] = $node_type->name;
  }

  $sites = array();
  $sync_settings = variable_get('drupal_sync_settings', array());

  $sync_settings_remote = !empty($sync_settings['drupal_sync_remote']) ? $sync_settings['drupal_sync_remote'] : array();
  $sites_options = array();
  foreach ($sync_settings_remote as $value) {
    $sites_options[$value['name']] = $value['name'];
  }

  $data = variable_get('drupal_sync_initial_setup_settings', array('node_types' => array(), 'vocabularies' => array()));


  $form['checkboxes'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
  );
  $form['checkboxes']['entities'] = array(
    '#type' => 'fieldset',
    '#title' => t('Entities'),
    '#collapsible' => FALSE,
    '#attributes' => array('class' => array('left-side')),
  );
  $form['checkboxes']['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Setup settings'),
    '#collapsible' => FALSE,
    '#attributes' => array('class' => array('right-side')),
  );

  $form['checkboxes']['settings']['compare_method'] = array(
       '#type' => 'select',
       '#title' => t('Choose a way to find the same entity'),
       '#options' => array(
         'title' => t('By title'),
         'id' => t('By entity id'),
       ),
       '#default_value' => isset($data['compare_method']) ? $data['compare_method'] : 'id',
       '#description' => t('Choose "By entity id" only if the remote sites were installed from copy of the local site database.'),
  );
  $form['checkboxes']['settings']['remote_sites'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Choose remote sites for setup relations.'),
    '#options' => $sites_options,
    '#default_value' => isset($data['remote_sites']) ? $data['remote_sites'] : array(),
  );

  $form['checkboxes']['entities']['node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Choose content types for setup relations.'),
    '#options' => $node_options,
    '#default_value' => isset($data['node_types']) ? $data['node_types'] : array(),
  );

  $form['checkboxes']['entities']['vocabularies'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Choose vocabularies for setup relations.'),
    '#options' => $voc_options,
    '#default_value' => isset($data['vocabularies']) ? $data['vocabularies'] : array(),
  );

  if (module_exists('drupal_sync_menu')) {
    $menus = menu_get_menus();
    $menu_options = array();
    foreach ($menus as $key => $value) {
      $menu_options[$key] = $value;
    }

    $form['checkboxes']['entities']['menus'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Choose menus for setup relations.'),
      '#options' => $menu_options,
      '#default_value' => isset($data['menus']) ? $data['menus'] : array(),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  $form['#attached']['css'][] = drupal_get_path('module', 'drupal_sync_initial_setup') . '/css/drupal_sync_initial_setup.css';

  if ((!empty($data['node_types']) || !empty($data['vocabularies']) || !empty($data['menus'])) && !empty($data['remote_sites'])) {
    $form['markup'] = array(
      '#type' => 'markup',
      '#markup' => "</br></br>" . l(t("Run initial setup for selected nodes and vocabularies"), 'admin/config/drupal_sync/drupal_sync_initial_setup/run'),
    );
  }

  return $form;
}


/**
 * Form submit
 */
function drupal_sync_initial_setup_admin_settings_form_submit($form, $form_state) {

  $node_types = (isset($form_state['values']['node_types'])) ? $form_state['values']['node_types'] : 0;
  $vocabularies = isset($form_state['values']['vocabularies']) ? $form_state['values']['vocabularies'] : 0;
  $remote_sites = isset($form_state['values']['remote_sites']) ? $form_state['values']['remote_sites'] : array();

  // remove empty
  foreach ($node_types as $key => $value) {
    if (empty($value)) {
      unset($node_types[$key]);
    }
  }

  foreach ($vocabularies as $key => $value) {
    if (empty($value)) {
      unset($vocabularies[$key]);
    }
  }
  foreach ($remote_sites as $key => $value) {
    if (empty($value)) {
      unset($remote_sites[$key]);
    }
  }

  $data = array(
    'node_types' => $node_types,
    'vocabularies' => $vocabularies,
    'remote_sites' => $remote_sites,
    'compare_method' => $form_state['values']['compare_method']
    );

  if (module_exists('drupal_sync_menu')) {
    $menus = isset($form_state['values']['menus']) ? $form_state['values']['menus'] : 0;

    // remove empty
    foreach ($menus as $key => $value) {
      if (empty($value)) {
        unset($menus[$key]);
      }
    }
    $data['menus'] = $menus;
  }

  variable_set('drupal_sync_initial_setup_settings', $data);
  drupal_set_message(t('The settings are saved.'), 'status');
}