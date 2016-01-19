<?php

/**
 * @file
 * Batch definition.
 */

/**
 * Function that cleans inline styles.
 *
 * @param array $batch_info
 *   Array with clean_summary_needed, field_settings and css_settings variables.
 * @param array $context
 *   Array with batch's metadata.
 */
function inline_styles_cleaner_batch_operation(array $batch_info, array &$context) {
  module_load_include('inc', 'inline_styles_cleaner', 'includes/inline_styles_cleaner.common');

  // Clean all inline styles from field values.
  inline_style_cleaner_clean_field_value($batch_info['field_settings']['field_name'] . '_value', $batch_info['field_settings'], $batch_info['css_settings'], $context);

  // Clean field summary if needed.
  if ($batch_info['clean_summary_needed'] && !empty($batch_info['field_settings']['field_instance']->{$batch_info['field_settings']['field_name'] . '_summary'})) {
    inline_style_cleaner_clean_field_value($batch_info['field_settings']['field_name'] . '_summary', $batch_info['field_settings'], $batch_info['css_settings'], $context);
  }

  // Let user knows what is going on.
  $context['message'] = t('Cleaning @field_name in @entity_type with id = @entity_id', array(
    '@field_name' => $batch_info['field_settings']['field_name'],
    '@entity_type' => $batch_info['field_settings']['field_instance']->entity_type,
    '@entity_id' => $batch_info['field_settings']['field_instance']->entity_id,
  ));
}

/**
 * Function that invoke when batch is finished.
 *
 * @param bool $success
 *   TRUE if batch operations finished without errors. Otherwise FALSE.
 * @param array $results
 *   Array with css classes as keys and array with
 *   property-value pairs as values.
 * @param array $operations
 *   Array with batch's operations.
 */
function inline_styles_cleaner_batch_finished($success, array $results, array $operations) {
  if ($success) {
    module_load_include('inc', 'inline_styles_cleaner', 'includes/inline_styles_cleaner.common');
    drupal_set_message(t('Completed without errors.'));

    // Update old_class_name variable.
    variable_set('inline_styles_cleaner_old_class_name', variable_get('inline_styles_cleaner_class_name', 'isc'));
    $generate_css = variable_get('inline_styles_cleaner_generate_css', FALSE);

    if ($generate_css) {
      if (count($results) > 0) {
        $file = file_save_data(inline_styles_cleaner_common_generate_css_rules(inline_styles_cleaner_update_styles_array($results)), INLINE_STYLES_CLEANER_PATH_TO_CSS_FILE, FILE_EXISTS_REPLACE);

        if ($file) {
          if (variable_get('inline_styles_cleaner_automatic_download', TRUE)) {
            drupal_set_message(t('If download has not been started click "Download file" button.'));
          }

          variable_set('inline_styles_cleaner_css_file', array(
            'fid' => $file->fid,
            'download_needed' => TRUE,
            'download_link' => INLINE_STYLES_CLEANER_DOWNLOAD_LINK,
          ));
        }
      }
    }
  }
  else {
    drupal_set_message(t('Completed with errors'), 'error');
  }
}
