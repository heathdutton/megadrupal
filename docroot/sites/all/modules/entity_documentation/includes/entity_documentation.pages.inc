<?php

/**
 * @file
 * Page callbacks file for Entity Documentation module.
 */

/**
 * Get entity types.
 */
function entity_documentation_types_page_callback() {
  $form = array();

  // Get entity types.
  module_load_include('inc', 'entity_documentation', 'includes/entity_documentation.functions');
  $types = ed_get_entity_types();

  // Get exporters.
  $exporters = ed_get_exporters();

  // Not available documentation.
  if (count($types) == 0 || count($exporters) == 0) {
    $form['notice'] = array(
      '#type' => 'item',
      '#markup' => t('No available documentation. Please enable at least one Entity and Exporter submodule.
          Such as <em>Entity Documentation PDF</em> and <em>Entity Documentation Node</em>.'),
    );
  }
  else {
    foreach ($types as $entity_key => $entity) {
      // Main fieldset.
      $form[$entity_key] = array(
        '#type' => 'fieldset',
        '#title' => $entity['name'],
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );

      // Bundles.
      foreach ($entity['bundles'] as $bundle_key => $bundle) {

        $item = $bundle['name'];

        foreach ($exporters as $exporter_key => $exporter) {
          $item .= ' ' . l($exporter['name'],
              'ed/' . $exporter_key . '/' . $bundle['entity'] . '/' . $bundle_key);
        }

        $form[$entity_key][$bundle_key] = array(
          '#type' => 'item',
          '#markup' => $item,
        );
      }
    }
  }

  return $form;
}

/**
 * Export documentation of specific bundle.
 *
 * @param $exporter
 *   Exporter to use.
 * @param $entity
 *   Entity type.
 * @param $bundle
 *   Bundle to export.
 */
function entity_documentation_export($exporter, $entity, $bundle) {
  // Invoke module for this exporter.
  module_invoke_all('ed_documentation_export', $exporter, $entity, $bundle);
}
