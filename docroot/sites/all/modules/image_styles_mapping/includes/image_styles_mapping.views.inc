<?php

/**
 * @file
 * Provides views fields report.
 */

/**
 * Menu callback.
 *
 * Provides views fields report.
 */
function image_styles_mapping_views_fields_report() {
  // Get the image styles.
  $image_styles = image_styles_mapping_get_image_styles();

  // Get the image fields.
  $image_fields = image_styles_mapping_get_image_fields();

  // Get the views.
  $views = views_get_all_views();

  // Check if Picture module is enabled.
  $picture = module_exists('picture');

  // Get the picture mappings.
  if ($picture) {
    $picture_mappings = image_styles_mapping_get_picture_mappings();
  }

  $header = array(
    array('data' => t('View'),          'field' => 'view'),
    array('data' => t('View display'),  'field' => 'view_display'),
    array('data' => t('Field'),         'field' => 'field'),
    image_styles_mapping_display_image_style_link(),
  );

  // Add a column for picture.
  if ($picture) {
    $header[] = image_styles_mapping_display_picture_mapping_link();
  }

  $rows = array();
  // Fetch all fieldapi fields which are used in views.
  // Therefore search in all views, displays and handler-types.
  foreach ($views as $view) {
    foreach ($view->display as $display_id => $display) {
      // Display with fields.
      if (isset($display->display_options['fields'])) {
        foreach ($display->display_options['fields'] as $field_machine_name => $field) {
          // Image field.
          if (image_styles_mapping_field_is_in_fields($field_machine_name, $image_fields)) {
            $view_display_image_styles = array();
            // Use recursive search because the structure of the
            // field_formatter is unknown.
            foreach ($image_styles as $image_style_name => $image_style_label) {
              $search_result = FALSE;
              image_styles_mapping_search_recursive($image_style_name, $field['settings'], $search_result);
              if ($search_result) {
                $view_display_image_styles[] = image_styles_mapping_display_image_style_link($image_style_label, $image_style_name);
              }
            }

            // Case empty.
            if (empty($view_display_image_styles)) {
              $view_display_image_styles[] = t('No image style used');
            }

            $view_display_image_styles = implode(', ', $view_display_image_styles);
            $row = array(
              'view' => $view->name,
              'view_display' => image_styles_mapping_view_display_link($view->name, $display_id, $display->display_title),
              'field' => $field_machine_name,
              'image_styles' => $view_display_image_styles,
            );

            // Picture support.
            if ($picture) {
              $view_mode_picture_mappings = array();
              // Use recursive search because the structure of the
              // field_formatter is unknown.
              foreach ($picture_mappings as $picture_mapping_name => $picture_mapping_label) {
                $search_result = FALSE;
                image_styles_mapping_search_recursive($picture_mapping_name, $field['settings'], $search_result);
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

  $output = array();
  $output[] = array(
    '#theme' => 'html_tag',
    '#tag' => 'h2',
    '#value' => t('View image fields'),
  );

  $output[] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => image_styles_mapping_sort_rows($header, $rows),
    '#empty' => t('No image styles have been used in any views fields yet.'),
  );

  return $output;
}
