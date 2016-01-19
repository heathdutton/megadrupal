<?php

/**
 * @file
 * Admin page callbacks.
 */




/**
 * Admin settings form
 */
function ebooks_settings() {
  $form = array();
  /*$form['ebooks_settings_vendors'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Available Vendors'),
    '#description' => t('Which eBooks vendors are visitors allowed to search?'),
    '#options' => array(
      '3m' => '3M',
      'overdrive' => 'Overdrive'
    ),
    '#default_value' => variable_get('ebooks_settings_vendors', array('3m', 'overdrive')),
    '#required' => TRUE
  );*/
  $form['ebooks_3m_url'] = array(
    '#type' => 'textfield',
    '#title' => t('3M URL'),
    '#description' => t('e.g. http://ebook.3m.com/library/EXAMPLE'),
    '#default_value' => variable_get('ebooks_3m_url')
  );
  $form['ebooks_overdrive_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Overdrive URL'),
    '#description' => t('e.g. http://example.lib.overdrive.com'),
    '#default_value' => variable_get('ebooks_overdrive_url')
  );
  $form['ebooks_overdrive_client_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Overdrive Client Key'),
    '#description' => t('Can be acquired on https://developer.overdrive.com'),
    '#default_value' => variable_get('ebooks_overdrive_client_key')
  );
  $form['ebooks_overdrive_client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Overdrive Client Secret'),
    '#description' => t('Can be acquired on https://developer.overdrive.com'),
    '#default_value' => variable_get('ebooks_overdrive_client_secret')
  );
  $form['ebooks_overdrive_account_id_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Overdrive Account ID Number'),
    '#description' => t('Can be found on https://developer.overdrive.com in the "Library Search". Look for a 4 or 5-digit number.'),
    '#default_value' => variable_get('ebooks_overdrive_account_id_number'),
    '#size' => 5
  );
  $form['ebooks_overview'] = array(
    '#type' => 'textarea',
    '#title' => t('Overview Paragraph'),
    '#description' => t('If you want an introductory paragraph to appear at the top of the form, put that text here.'),
    '#default_value' => variable_get('ebooks_overview')
  );

  $form = system_settings_form($form);
  return $form;
}
