<?php

/**
 * @file
 * Provides a report containing all the reports in one place.
 */

/**
 * Provides a report containing where image styles are used.
 *
 * @see image_styles_mapping_menu()
 */
function image_styles_mapping_report_all() {

  $output = array();

  module_load_include('inc', 'image_styles_mapping', 'includes/image_styles_mapping.fields');
  $output[] = image_styles_mapping_fields_report();

  if (module_exists('views')) {
    module_load_include('inc', 'image_styles_mapping', 'includes/image_styles_mapping.views');
    $output[] = image_styles_mapping_views_fields_report();
  }

  if (module_exists('atom_reference') && module_exists('scald_image')) {
    module_load_include('inc', 'image_styles_mapping', 'includes/image_styles_mapping.scald_image');
    $output[] = image_styles_mapping_scald_image_report();

    if (module_exists('views')) {
      module_load_include('inc', 'image_styles_mapping', 'includes/image_styles_mapping.views_scald');
      $output[] = image_styles_mapping_views_scald_image_report();
    }
  }

  return $output;
}
