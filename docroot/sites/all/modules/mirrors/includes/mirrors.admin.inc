<?php

/**
 * @file
 * Creates an administration form where mirrors can be enabled/disabled per
 * bundle.
 */

/**
 * Page callback for mirrors settings page.
 */
function mirrors_admin_page() {
  $form = drupal_get_form('mirrors_admin_form');
  return $form;
}

/**
 * Mirrors administration form.
 */
function mirrors_admin_form($form, &$form_state) {

  $form = array();

  $form['#prefix'] = t("Import and Export entities with Feeds and Views.<br/>If enabled you get 2 links to the respective importer and exporter UI.<br/><br/>");

  // Set the table header.
  $header = array(
    'entity_type' => t('Entity Type'),
    'bundle' => t('Bundle'),
    'status' => t('Status'),
    'importer' => t('Importer'),
    'exporter' => t('Exporter'),
  );

  // Load saved values from database and set as default values.
  $stored_values = variable_get('mirrors_settings');

  $default_values = array();
  if (isset($stored_values)) {
    foreach ($stored_values as $stored_value_key => $stored_value) {
      if ($stored_value_key === $stored_value) {
        $default_values[$stored_value_key] = TRUE;
      }
    }
  }

  // Load available bundles.
  $mirrors_bundles = mirrors_bundles();

  // Create option for each available bundle.
  $options = array();
  foreach ($mirrors_bundles as $entity_type_key => $entity_type) {
    foreach ($entity_type as $bundle_key => $bundle) {
      $key = $entity_type_key . '__' . $bundle_key;

      // Check if enabled, set some helper links.
      if (isset($stored_values[$key]) && $stored_values[$key] !== 0) {
        $saved_setting = t('Enabled');
        $export_links = l(t('Views UI'), 'admin/structure/views/view/mirrors_' . $key . '/edit');
        $import_links = l(t('Feeds UI'), 'admin/structure/feeds/mirrors_' . $key);
      }
      else {
        $saved_setting = t('Disabled');
        $import_links = '';
        $export_links = '';
      }

      $options[$key] = array(
        'entity_type' => $entity_type_key,
        'bundle' => $bundle_key,
        'status' => $saved_setting,
        'importer' => $import_links,
        'exporter' => $export_links,
      );
    }
  }

  // Create the table form.
  $form['mirrors_bundle_settings'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No content available.'),
    '#default_value' => $default_values,
  );

  // Form submit.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  $form['#submit'][] = '_mirrors_admin_form_submit';

  return $form;
}

/**
 * Mirrors administraton form submit callback.
 */
function _mirrors_admin_form_submit($form, &$form_state) {
  $data = array();

  // Loop through submitted values.
  foreach ($form_state['values']['mirrors_bundle_settings'] as $submitted_key => $submitted_value) {
    $data[$submitted_key] = $submitted_value;
  }

  // Save results to database.
  variable_set('mirrors_settings', $data);

  // Rebuild views cache, otherwise changes won't show up.
  views_invalidate_cache();
  drupal_set_message(t("Mirrors settings has been saved."));
}
