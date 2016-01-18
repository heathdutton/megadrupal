<?php

/**
 * @file 
 * certify.admin.inc php file
 * Configuration settings form for Drupal module Certify.
 */

/**
 * Configuration settings form.
 */
function certify_admin_settings($form, &$form_state) {
  $datefmt = explode(' ', variable_get('date_format_short', 'm/d/Y'));
  $datefmt = variable_get('certify_dateformat', $datefmt[0]);

  $form['certify_certificate_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter the path to the location where to store certificates.'),
    '#description' => t('Must be a location writable by the server, but not public. Preferable without trailing directory separator.'),
    '#default_value' => variable_get('certify_certificate_path', '/var/drupal-content/certify'),
    '#resizable' => FALSE,
  );

  $form['certify_pdftk_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to pdftk.'),
    '#description' => t('Enter the path where pdftk is installed on your server.'),
    '#default_value' => variable_get('certify_pdftk_path', '/usr/bin/pdftk'),
    '#resizable' => FALSE,
  );

  $form['certify_drm_passphrase'] = array(
    '#type' => 'textfield',
    '#title' => t('Password for Digital Rights Management.'),
    '#description' => t('Enter a password that will be needed to edit the PDF output files. If blank, certify will generate normal PDF files..'),
    '#default_value' => variable_get('certify_drm_passphrase', ''),
    '#resizable' => FALSE,
  );

  $form['certify_og_filter'] = array(
    '#type' => 'checkbox',
    '#title' => t('Filter on organic groups.'),
    '#description' => t('If this is set, and organic groups is available, available nodes will be filtered based on the current group context.'),
    '#default_value' => variable_get('certify_og_filter', TRUE),
  );
  
  $form['certify_email'] = array(
    '#type' => 'fieldset',
    '#title' => t('E-mail certificate'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['certify_email']['certify_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable'),
    '#description' => t('Sends the certificate by e-mail to the user upon completion.'),
    '#default_value' => variable_get('certify_email', FALSE),
  );
  $form['certify_email']['certify_email_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => variable_get('certify_email_subject', t('Your certificate')),
  );
  $form['certify_email']['certify_email_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => variable_get('certify_email_body', t('Congratulations with your newly achieved certification.') . "\n\n" . t('Your certificate should be attached as a PDF file to this email.')),
  );
  
  $form['certify_dateformat'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter the date format to use in the PDF certificates.'),
    '#description' => t('This is a standard PHP date format string that will be used for formatting the date in the PDF certificates.'),
    '#default_value' => $datefmt,
    '#resizable' => FALSE,
  );

  $form['certify_display_score'] = array(
    '#type' => 'select',
    '#title' => t('Score display'),
    '#description' => t('How the score should be displayed on certificates'),
    '#options' => array(
      'passed' => t('Display "Passed"'),
      'percentage' => t('Display the percentage of awarded points from the total.'),
      'points' => t('Display points as e.g. "76 of 90"'),
    ),
    '#default_value' => variable_get('certify_display_score', 'passed'),
  );
  
  $form['certify_download_certificate_preview'] = array(
    '#type' => 'item',
    '#title' => 'Certificate template',
    '#markup' => l(t('Download certificate template'), 'admin/config/system/certify/download-certificate-preview'),
  );

  return system_settings_form($form);

}

/**
 * Validate configuration settings form.
 */
function certify_admin_settings_validate($form, $form_state) {
  $certificate_path = $form_state['values']['certify_certificate_path'];
  if (!is_dir($certificate_path)) {
    form_set_error('certify_certificate_path', t('Please select an existing directory.'));
  }
  if (!is_writable($certificate_path)) {
    form_set_error('certify_certificate_path', t('The directory needs to be writable for the webserver.'));
  }

  $pdftkpath = $form_state['values']['certify_pdftk_path'];
  if (!file_exists($pdftkpath) || !is_executable($pdftkpath)) {
    form_set_error('certify_pdftk_path', t('Please enter the full path to your pdftk executable.'));
  }
}