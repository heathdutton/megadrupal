<?php

/**
 * @file
 * Provides image fields report.
 */

/**
 * Menu callback.
 *
 * Provides image fields report.
 */
function image_styles_mapping_fields_report() {
  // Get the image styles.
  $image_styles = image_styles_mapping_get_image_styles();

  // Get the image fields.
  $image_fields = image_styles_mapping_get_image_fields();

  // Get the instances.
  $instances_info = field_info_instances();

  // Check if Picture module is enabled.
  $picture = module_exists('picture');

  // Get the picture mappings.
  if ($picture) {
    $picture_mappings = image_styles_mapping_get_picture_mappings();
  }

  $header = array(
    array('data' => t('Entity'),    'field' => 'entity_type'),
    array('data' => t('Bundle'),    'field' => 'bundle'),
    array('data' => t('View mode'), 'field' => 'view_mode'),
    array('data' => t('Field'),     'field' => 'field'),
    image_styles_mapping_display_image_style_link(),
  );

  // Add a column for picture.
  if ($picture) {
    $header[] = image_styles_mapping_display_picture_mapping_link();
  }

  $rows = array();
  // Entities.
  foreach ($instances_info as $entity_machine_name => $bundles) {
    // Bundles.
    foreach ($bundles as $bundle => $bundle_fields) {
      // Fields.
      foreach ($bundle_fields as $field_machine_name => $field) {
        // Image field.
        if (in_array($field_machine_name, $image_fields)) {
          // View modes.
          foreach ($field['display'] as $view_mode_machine_name => $view_mode) {
            // Field enabled.
            if ($view_mode['type'] != 'hidden') {
              $view_mode_image_styles = array();
              // Use recursive search because the structure of the
              // field_formatter is unknown.
              foreach ($image_styles as $image_style_name => $image_style_label) {
                $search_result = FALSE;
                image_styles_mapping_search_recursive($image_style_name, $view_mode, $search_result);
                if ($search_result) {
                  $view_mode_image_styles[] = image_styles_mapping_display_image_style_link($image_style_label, $image_style_name);
                }
              }

              // Case empty.
              if (empty($view_mode_image_styles)) {
                $view_mode_image_styles[] = t('No image style used');
              }

              $view_mode_image_styles = implode(', ', $view_mode_image_styles);
              $row = array(
                'entity_type' => $entity_machine_name,
                'bundle' => $bundle,
                'view_mode' => image_styles_mapping_display_view_mode_link($entity_machine_name, $bundle, $view_mode_machine_name),
                'field' => $field_machine_name,
                'image_styles' => $view_mode_image_styles,
              );

              // Picture support.
              if ($picture) {
                $view_mode_picture_mappings = array();
                // Use recursive search because the structure of the
                // field_formatter is unknown.
                foreach ($picture_mappings as $picture_mapping_name => $picture_mapping_label) {
                  $search_result = FALSE;
                  image_styles_mapping_search_recursive($picture_mapping_name, $view_mode, $search_result);
                  if ($search_result) {
                    $view_mode_picture_mappings[] = image_styles_mapping_display_picture_mapping_link($picture_mapping_label, $picture_mapping_name);
                  }
                }

                // Case empty.
                if (empty($view_mode_picture_mappings)) {
                  $view_mode_picture_mappings[] = t('No picture mapping used');
                }

                $view_mode_picture_mappings = implode(', ', $view_mode_picture_mappings);
                // Add the data to the last row.
                $row['picture_mappings'] = $view_mode_picture_mappings;
              }

              $rows[] = $row;
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
    '#value' => t('Image fields'),
  );

  $output[] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => image_styles_mapping_sort_rows($header, $rows),
    '#empty' => t('No image styles have been used in any image fields yet.'),
  );

  return $output;
}
