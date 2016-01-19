<?php
/**
 * @file
 * Definition of Commerce Order2pdf variables for the Variable API
 * Allows e-mail subject and body to be localized using the i18n_variable module
 */

/**
 * Implements hook_variable_info().
 */
function commerce_order2pdf_variable_info($options) {
  $variables['order2pdf_pdf_header'] = array(
    'title' => t('Commerce Order2pdf order header', array(), $options),
    'description' => t('Header to your commerce order PDF file.You can use tokens.', array(), $options),
    'type' => 'text',
    'token' => TRUE,
    'localize' => TRUE,
    'access' => 'configure order settings',
  );
  $variables['order2pdf_pdf_title'] = array(
    'title' => t('Commerce order2pdf PDF file name', array(), $options),
    'description' => t('Add your own title for generated pdf file.', array(), $options),
    'type' => 'string',
    'token' => TRUE,
    'localize' => TRUE,
    'access' => 'configure order settings',
  );
  return $variables;
}
