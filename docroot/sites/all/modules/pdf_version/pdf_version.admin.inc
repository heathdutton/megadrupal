<?php

/**
 * Form builder for the PDF version admin settings form.
 */
function pdf_version_admin_settings_form($form, &$form_state) {
  // The location of PhantomJS.
  $form['pdf_version_phantomjs_binary'] = array(
    '#title' => t('PhantomJS binary path'),
    '#type' => 'textfield',
    '#default_value' => pdf_version_variable_get('phantomjs_binary'),
    '#description' => t('Set this to the full path of the phantomjs binary, eg <em>/usr/bin/phantomjs</em>.'),
    '#element_validate' => array('pdf_version_validate_phantomjs_binary_path'),
    '#required' => TRUE,
  );

  // Should SSL errors be ignored by PhantomJS?
  $form['pdf_version_ignore_ssl_errors'] = array(
    '#title' => t('Ignore SSL errors?'),
    '#type' => 'checkbox',
    '#default_value' => pdf_version_variable_get('ignore_ssl_errors'),
    '#description' => t('If your site uses SSL (https), tick this if the PDFs are just a blank page.'),
  );

  return system_settings_form($form);
}

/**
 * Validation function for the PhantomJS binary path setting.
 */
function pdf_version_validate_phantomjs_binary_path($element) {
  $value = $element['#value'];
  if (!empty($value) && !is_executable($value)) {
    form_error($element, t('The specified @title does not point to a valid executable binary.', array('@title' => $element['#title'])));
  }
}
