<?php

/**
 * @file
 * Config form.
 */

/**
 * Form definition; inline_styles_cleaner_config_form.
 *
 * @param array $form
 *   Drupal $form variable.
 * @param array $form_state
 *   Drupal $form_state variable.
 *
 * @return array $form
 *   Drupal $form variable.
 */
function inline_styles_cleaner_config_form(array $form, array &$form_state) {
  module_load_include('inc', 'inline_styles_cleaner', 'includes/inline_styles_cleaner.common');

  $form['fields_for_clean_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['fields_for_clean_fieldset']['inline_styles_cleaner_clean_text_field_summary'] = array(
    '#type' => 'checkbox',
    '#title' => t('Clean summary'),
    '#description' => t('Check it if you want clean summary from "text_with_summary" fields.'),
    '#default_value' => variable_get('inline_styles_cleaner_clean_text_field_summary', FALSE),
  );

  $form['fields_for_clean_fieldset']['inline_styles_cleaner_fields_for_clean'] = array(
    '#type' => 'checkboxes',
    '#options' => inline_styles_cleaner_common_get_text_fields_names(),
    '#title' => t('Select fields for cleaning inline styles'),
    '#default_value' => variable_get('inline_styles_cleaner_fields_for_clean', array()),
  );

  $form['generate_css_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Css generator settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['generate_css_fieldset']['inline_styles_cleaner_generate_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Generate css styles for selected fields'),
    '#description' => t("Generated css file will be contain clipped inline styles from field's values."),
    '#default_value' => variable_get('inline_styles_cleaner_generate_css', FALSE),
  );

  $form['generate_css_fieldset']['inline_styles_cleaner_update_existing_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update css rules if new inline styles will be found'),
    '#description' => t('Useful when new inline styles was added after cleaning. Css file will be contain updated css rules. If not checked then old css rules will be overriden by rules with new inline styles.'),
    '#default_value' => variable_get('inline_styles_cleaner_update_existing_css', FALSE),
    '#states' => array(
      'visible' => array(
        'input[name="inline_styles_cleaner_generate_css"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );

  $form['generate_css_fieldset']['inline_styles_cleaner_class_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Class name'),
    '#description' => t("Class name that will be applied to elements with inline styles. It's a part of full future class name. Class name will be: class_name-{entity_id}-{field_name_(value|summary)}-{element_number}"),
    '#default_value' => variable_get('inline_styles_cleaner_class_name', 'isc'),
    '#states' => array(
      'visible' => array(
        'input[name="inline_styles_cleaner_generate_css"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );

  $automatic_download_needed = variable_get('inline_styles_cleaner_automatic_download', TRUE);
  $form['generate_css_fieldset']['inline_styles_cleaner_automatic_download'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatic file download'),
    '#description' => t('Download css file after processing.'),
    '#default_value' => $automatic_download_needed,
    '#states' => array(
      'visible' => array(
        'input[name="inline_styles_cleaner_generate_css"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );

  // Show generated css file on admin form.
  $css_file_settings = variable_get('inline_styles_cleaner_css_file', array());
  if (isset($css_file_settings['fid'])) {
    $file = file_load($css_file_settings['fid']);

    if ($file) {
      $form['generate_css_fieldset']['css_file_fieldset'] = array(
        '#type' => 'fieldset',
        '#title' => t('Css rules from file'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );

      $form['generate_css_fieldset']['css_file_fieldset']['css_fid'] = array(
        '#type' => 'value',
        '#default_value' => $css_file_settings['fid'],
      );

      $form['generate_css_fieldset']['css_file_fieldset']['css_file_content'] = array(
        '#type' => 'textarea',
        '#default_value' => file_get_contents($file->uri),
        '#rows' => 8,
      );

      $form['generate_css_fieldset']['css_file_fieldset']['buttons']['download'] = array(
        '#type' => 'submit',
        '#value' => t('Download file'),
        '#submit' => array('inline_styles_cleaner_download'),
      );

      $form['generate_css_fieldset']['css_file_fieldset']['buttons']['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete file'),
        '#submit' => array('inline_styles_cleaner_delete'),
      );
    }
  }

  // Automatic download css files.
  if ($automatic_download_needed) {
    $form['#attached']['js'] = array(
      array(
        'type' => 'setting',
        'data' => array(
          'inline_styles_cleaner' => array(
            'css_file' => variable_get('inline_styles_cleaner_css_file', array(
              'fid' => NULL,
              'download_needed' => FALSE,
              'download_link' => '',
            )),
          ),
        ),
      ),
      array(
        'type' => 'file',
        'data' => drupal_get_path('module', 'inline_styles_cleaner') . '/js/inline_styles_cleaner.js',
      ),
    );
  }

  $form = system_settings_form($form);
  $form['actions']['submit']['#value'] = t('Start processing');
  $form['#submit'][] = 'inline_styles_cleaner_launch_batch';

  return $form;
}

/**
 * Custom submit to download css file.
 *
 * @param array $form
 *   Drupal $form variable.
 * @param array $form_state
 *   Drupal $form_state variable.
 */
function inline_styles_cleaner_download($form, &$form_state) {
  module_load_include('inc', 'inline_styles_cleaner', 'includes/inline_styles_cleaner.common');
  drupal_goto(INLINE_STYLES_CLEANER_DOWNLOAD_LINK . '/' . $form_state['values']['css_fid']);
}

/**
 * Custom submit to delete css file.
 *
 * @param array $form
 *   Drupal $form variable.
 * @param array $form_state
 *   Drupal $form_state variable.
 */
function inline_styles_cleaner_delete($form, &$form_state) {
  file_delete(file_load($form_state['values']['css_fid']));
  variable_del('inline_styles_cleaner_old_styles');
}

/**
 * Custom submit to launch batches.
 *
 * @param array $form
 *   Drupal $form variable.
 * @param array $form_state
 *   Drupal $form_state variable.
 */
function inline_styles_cleaner_launch_batch(array $form, array &$form_state) {
  module_load_include('inc', 'inline_styles_cleaner', 'includes/inline_styles_cleaner.common');

  $field_names = array_filter(variable_get('inline_styles_cleaner_fields_for_clean', array()));
  $generate_css_needed = variable_get('inline_styles_cleaner_generate_css', FALSE);
  $class_name = variable_get('inline_styles_cleaner_class_name', 'isc');
  $old_class_name = variable_get('inline_styles_cleaner_old_class_name', $class_name);
  $clean_summary_needed = variable_get('inline_styles_cleaner_clean_text_field_summary', FALSE);
  $update_existing_css_needed = variable_get('inline_styles_cleaner_update_existing_css', FALSE);

  // Remove saved css array from db if we don't want update existing css rules.
  if (!$update_existing_css_needed) {
    variable_del('inline_styles_cleaner_old_styles');
  }

  // We need to check $field_names with empty() function because
  // if user wouldn't select any field he should see warning message.
  if (!empty($field_names)) {
    foreach ($field_names as $field_name) {
      // Prepare batch operations.
      foreach (db_select("field_data_$field_name", $field_name)->fields($field_name)->execute() as $field) {
        $operations[] = array(
          'inline_styles_cleaner_batch_operation',
          array(
            array(
              'clean_summary_needed' => $clean_summary_needed,
              'field_settings' => array(
                'field_name' => $field_name,
                'field_instance' => $field,
              ),
              'css_settings' => array(
                'generate_css_needed' => $generate_css_needed,
                'update_existing_css_needed' => $update_existing_css_needed,
                'class_name' => $class_name,
                'old_class_name' => $old_class_name,
              ),
            ),
          ),
        );
      }
    }

    // We should check that $operation array contains at least one operation.
    // Otherwise there is no sense to launch batch.
    if (isset($operations)) {
      batch_set(array(
        'operations' => $operations,
        'finished' => 'inline_styles_cleaner_batch_finished',
        'file' => drupal_get_path('module', 'inline_styles_cleaner') . '/includes/inline_styles_cleaner.batch.inc',
      ));
    }
    else {
      drupal_set_message(t('There is now instances of selected field(s) in database.'), 'warning');
    }
  }
  else {
    drupal_set_message(t('Please select field(s) to start processing.'), 'warning');
  }
}
