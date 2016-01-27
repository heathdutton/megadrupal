<?php

/**
 * Implements hook_form_FORM_ID_alter() for visual_select_file_settings_form().
 */
function vsf_upload_form_visual_select_file_settings_form_alter(&$form, &$form_state) {
  $form['vsf_upload'] = array(
    '#type' => 'fieldset',
    '#title' => t('VSF Upload'),
  );

  $states = array();
  if (module_exists('file_entity')) {
    $form['vsf_upload']['vsf_file_entity'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use file entity for uploads in the file browser'),
      '#default_value' => variable_get('vsf_file_entity', TRUE),
    );

    $states = array(
      '#states' => array(
        'invisible' => array(':input[name="vsf_file_entity"]' => array('checked' => TRUE)),
      )
    );
  }

  $scheme_options = array();
  foreach (file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE) as $scheme => $stream_wrapper) {
    $scheme_options[$scheme] = $stream_wrapper['name'];
  }

  if (count($scheme_options) > 1) {
    $form['vsf_upload']['vsf_upload_scheme'] = array(
      '#type' => 'radios',
      '#title' => t('Upload destination'),
      '#options' => $scheme_options,
      '#default_value' => variable_get('vsf_upload_scheme', VSF_DEFAULT_UPLOAD_SCHEME),
      '#description' => t('Select where the final files should be stored. Private file storage has significantly more overhead than public files, but allows restricted access to files within this field.'),
    ) + $states;
  }
  else {
    $keys = array_keys($scheme_options);
    $form['vsf_upload']['vsf_upload_scheme'] = array(
      '#type' => 'value',
      '#value' => current($keys),
    );
  }

  form_load_include($form_state, 'inc', 'file', 'file.field');

  $form['vsf_upload']['vsf_upload_location'] = array(
    '#type' => 'textfield',
    '#title' => t('File directory'),
    '#default_value' => variable_get('vsf_upload_location', VSF_DEFAULT_UPLOAD_LOCATION),
    '#description' => t('Optional subdirectory within the upload destination where files will be stored. Do not include leading or trailing slashes.'),
    '#element_validate' => array('_file_generic_settings_file_directory_validate'),
  ) + $states;

  // Make the extension list a little more human-friendly by comma-separation.
  $form['vsf_upload']['vsf_upload_extensions'] = array(
    '#type' => 'textfield',
    '#title' => t('Allowed file extensions'),
    '#default_value' => variable_get('vsf_upload_extensions', VSF_DEFAULT_UPLOAD_EXTENSIONS),
    '#description' => t('Separate extensions with a space or comma and do not include the leading dot.'),
    '#element_validate' => array('_file_generic_settings_extensions'),
    '#required' => TRUE,
    '#maxlength' => 9999, // system_element_info() weirdly ALWAYS adds 128 to ALL 'textfield' elements...
  ) + $states;

  $form['vsf_upload']['vsf_upload_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum upload size'),
    '#default_value' => variable_get('vsf_upload_size', VSF_DEFAULT_UPLOAD_FILE_SIZE),
    '#description' => t('Enter a value like "512" (bytes), "80 KB" (kilobytes) or "50 MB" (megabytes) in order to restrict the allowed file size.'),
    '#size' => 10,
    '#element_validate' => array('_file_generic_settings_max_filesize'),
    '#required' => TRUE,
  ) + $states;
}
