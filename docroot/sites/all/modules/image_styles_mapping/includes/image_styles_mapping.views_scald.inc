<?php

/**
 * @file
 * Provides scald image in views report.
 */

/**
 * Menu callback.
 *
 * Provides scald image in views report.
 */
function image_styles_mapping_views_scald_image_report() {
  // Get the atom reference fields.
  $atom_reference_fields = image_styles_mapping_get_atom_reference_fields();

  // Get the views.
  $views = views_get_all_views();

  $header = array(
    array('data' => t('View'),          'field' => 'view'),
    array('data' => t('View display'),  'field' => 'view_display'),
    array('data' => t('Field'),         'field' => 'field'),
    image_styles_mapping_display_scald_image_context_link(),
    image_styles_mapping_display_image_style_link(),
  );
  $rows = array();
  // Fetch all fieldapi fields which are used in views.
  // Therefore search in all views, displays and handler-types.
  foreach ($views as $view) {
    foreach ($view->display as $display_id => $display) {
      // Display with fields.
      if (isset($display->display_options['fields'])) {
        foreach ($display->display_options['fields'] as $field_machine_name => $field) {
          // Atom reference field.
          if (image_styles_mapping_field_is_in_fields($field_machine_name, $atom_reference_fields)) {
            $scald_context = $field['type'];

            $view_display_image_styles = image_styles_mapping_get_scald_context_image_styles($scald_context);

            $rows[] = array(
              'view' => $view->name,
              'view_display' => image_styles_mapping_view_display_link($view->name, $display_id, $display->display_title),
              'field' => $field_machine_name,
              'scald_context' => $scald_context,
              'image_styles' => $view_display_image_styles,
            );
          }
        }
      }
    }
  }

  $output = array();
  $output[] = array(
    '#theme' => 'html_tag',
    '#tag' => 'h2',
    '#value' => t('View scald atom reference image fields'),
  );

  $output[] = array(
    '#theme' => 'html_tag',
    '#tag' => 'p',
    '#value' => t('Warning : there is no warranty that the atom reference fields
      can use image atoms as you can display different bundles in the same display
      and the field settings may be different for each bundles.'),
  );

  $output[] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => image_styles_mapping_sort_rows($header, $rows),
    '#empty' => t('No image atoms have been allowed in any view fields yet.'),
  );

  return $output;
}
