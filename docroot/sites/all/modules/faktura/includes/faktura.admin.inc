<?php

function faktura_settings_export_form() {
  $form = array();
  
  $form['faktura_pdf_method_pdftemplate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create PDF files with the www.pdftemplate.eu API'),
    '#description' => t('Sign up for a free account at <a href="http://www.pdftemplate.eu">www.pdftemplate.eu</a>.'),
    '#default_value' => variable_get('faktura_pdf_method_pdftemplate', 1),
  );
  
  $form['faktura_pdftemplate_user'] = array(
    '#type' => 'textfield',
    '#title' => t('PDFTemplate.eu user name'),
    '#default_value' => variable_get('faktura_pdftemplate_user', ''),
  );
  
  $form = system_settings_form($form);
  
  return $form;
}