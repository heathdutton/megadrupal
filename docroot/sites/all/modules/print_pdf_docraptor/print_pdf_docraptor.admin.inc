<?php

/**
 * @file
 * Contains the administrative functions of the print_pdf_docraptor module.
 *
 * This file is included by the print_pdf_docraptor module, and includes the
 * settings form.
 *
 * @ingroup print
 */

/**
 * Form constructor for the docraptor options settings form.
 *
 * @ingroup forms
 */
function print_pdf_docraptor_settings() {
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Docraptor options'),
  );

  $form['settings']['print_pdf_docraptor_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#required' => TRUE,
    '#default_value' => variable_get('print_pdf_docraptor_api_key', ''),
    '#size' => 60,
    '#maxlength' => 250,
    '#description' => t('Enter your API key found in your Docraptor user account settings'),
  );
  $form['settings']['print_pdf_docraptor_test'] = array(
    '#type' => 'checkbox',
    '#title' => t("Use docraptor in testing Mode"),
    '#default_value' => variable_get('print_pdf_docraptor_test', FALSE),
    '#description' => t('Test mode documents do not count against your monthly document quota - this way you can play with styles until you get a good looking document without wasting any of your allotted documents.'),
  );
  $form['settings']['print_pdf_docraptor_filename'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Filename'),
    '#default_value' => variable_get('print_pdf_docraptor_filename', ''),
    '#description' => t('A name for the document. This can be any string that you find meaningful to describe this document - it is just used for identification purposes on the Docraptor account dashboard. Tokens may be used in these options (see list below).'),
  );
  if (module_exists('token')) {
    $form['settings']['print_pdf_filename_patterns'] = array(
      '#type' => 'fieldset',
      '#title' => t('Replacement patterns'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['settings']['print_pdf_filename_patterns']['descriptions'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('node'),
    );
  }

  return system_settings_form($form);
}
