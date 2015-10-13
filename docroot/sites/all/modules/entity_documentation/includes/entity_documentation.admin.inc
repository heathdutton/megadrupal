<?php

/**
 * @file
 * Admin settings file for Entity Documentation module.
 */

/**
 * Admin setings form.
 */
function entity_documentation_settings_form($form, &$form_state) {

  // Get entity types.
  module_load_include('inc', 'entity_documentation', 'includes/entity_documentation.functions');
  $types = ed_get_entity_types();

  // Get exporters.
  $exporters = ed_get_exporters();

  ed_auto_export();

  $form = array();

  // Not available documentation.
  if (count($types) == 0 || count($exporters) == 0) {
    $form['notice'] = array(
      '#type' => 'item',
      '#markup' => t('No available documentation. Please enable at least one Entity and Exporter submodule.
          Such as <em>Entity Documentation PDF</em> and <em>Entity Documentation Node</em>.'),
    );
  }
  else {

    // Auto export container.
    $form['auto_export'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'id' => array('auto_export'),
      ),
    );

    // Header.
    $form['auto_export']['header'] = array(
      '#type' => 'html_tag',
      '#tag' => 'h3',
      '#value' => t('Auto Export'),
    );

    // Update interval.
    $update_interval = array(
      3600 => t('1 hour'),
      21600 => t('6 hours'),
      86400 => t('24 hours'),
      0 => t('Never'),
    );
    $form['auto_export']['ed_update_interval'] = array(
      '#type' => 'select',
      '#title' => t('Update interval'),
      '#options' => $update_interval,
      '#default_value' => variable_get('ed_update_interval'),
    );

    // Export path.
    $default_path = variable_get('ed_export_path',
      variable_get('file_public_path', 'sites/default/files') . '/entity_documentation');
    $form['auto_export']['ed_export_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Export path'),
      '#default_value' => $default_path,
      '#size' => 60,
      '#required' => TRUE,
    );

    foreach ($types as $entity_key => $entity) {
      // Main fieldset.
      $form['auto_export'][$entity_key] = array(
        '#type' => 'fieldset',
        '#title' => $entity['name'],
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );

      // Bundles.
      foreach ($entity['bundles'] as $bundle_key => $bundle) {

        $form['auto_export'][$entity_key][$bundle_key] = array(
          '#type' => 'html_tag',
          '#tag' => 'h4',
          '#value' => $bundle['name'],
        );

        foreach ($exporters as $exporter_key => $exporter) {
          $form['auto_export'][$entity_key][$entity_key . '_' . $bundle_key . '_' . $exporter_key] = array(
            '#type' => 'checkbox',
            '#title' => $exporter['name'],
            '#default_value' => ed_get_auto_export_enabled($entity_key, $bundle_key, $exporter_key),
          );
        }
      }
    }
  }

  // Submit button.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Settings form submit handler.
 *
 * @param $form
 * @param $form_state
 */
function entity_documentation_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];

    // Get entity types.
  module_load_include('inc', 'entity_documentation', 'includes/entity_documentation.functions');
  $types = ed_get_entity_types();

  // Get exporters.
  $exporters = ed_get_exporters();

  // Create new settings variable.
  $auto_export_settings = array();
  foreach ($types as $entity_key => $entity) {
    foreach ($entity['bundles'] as $bundle_key => $bundle) {
      foreach ($exporters as $exporter_key => $exporter) {
        $key = $entity_key . '_' . $bundle_key . '_' . $exporter_key;
        if (array_key_exists($key, $values) && $values[$key]) {
          $auto_export_settings[$key] = TRUE;
        }
      }
    }
  }

  // Save.
  variable_set('ed_auto_export', serialize($auto_export_settings));
  variable_set('ed_update_interval', $values['ed_update_interval']);
  variable_set('ed_export_path', $values['ed_export_path']);

  // Show message.
  drupal_set_message(t('Settings saved.'));
}
