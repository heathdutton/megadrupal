<?php

/**
 * @file
 * Provides scald image report.
 */

/**
 * Menu callback.
 *
 * Provides scald image report.
 */
function image_styles_mapping_scald_image_report() {
  // Get the atom reference fields.
  $atom_reference_fields = image_styles_mapping_get_atom_reference_fields();

  // Get the instances.
  $instances_info = field_info_instances();

  $header = array(
    array('data' => t('Entity'),    'field' => 'entity_type'),
    array('data' => t('Bundle'),    'field' => 'bundle'),
    array('data' => t('View mode'), 'field' => 'view_mode'),
    array('data' => t('Field'),     'field' => 'field'),
    image_styles_mapping_display_scald_image_context_link(),
    image_styles_mapping_display_image_style_link(),
  );
  $rows = array();
  // Entities.
  foreach ($instances_info as $entity_machine_name => $bundles) {
    // Bundles.
    foreach ($bundles as $bundle => $bundle_fields) {
      // Fields.
      foreach ($bundle_fields as $field_machine_name => $field) {
        // Atom reference field.
        if (in_array($field_machine_name, $atom_reference_fields)) {
          // Field can reference image atom.
          if (in_array('image', $field['settings']['referencable_types'])) {
            // View modes.
            foreach ($field['display'] as $view_mode_machine_name => $view_mode) {
              // Field enabled.
              if ($view_mode['type'] != 'hidden') {
                $scald_context = $view_mode['type'];

                $view_mode_image_styles = image_styles_mapping_get_scald_context_image_styles($scald_context);

                $rows[] = array(
                  'entity_type' => $entity_machine_name,
                  'bundle' => $bundle,
                  'view_mode' => image_styles_mapping_display_view_mode_link($entity_machine_name, $bundle, $view_mode_machine_name),
                  'field' => $field_machine_name,
                  'scald_context' => $scald_context,
                  'image_styles' => $view_mode_image_styles,
                );
              }
            }
          }
        }
      }
    }
  }

  $output = array();
  $output[] = array(
    '#theme' => 'html_tag',
    '#tag' => 'h2',
    '#value' => t('Scald atom reference image'),
  );

  $output[] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => image_styles_mapping_sort_rows($header, $rows),
    '#empty' => t('No image atoms have been allowed in any atom reference fields yet. Or such fields are not used.'),
  );

  return $output;
}
