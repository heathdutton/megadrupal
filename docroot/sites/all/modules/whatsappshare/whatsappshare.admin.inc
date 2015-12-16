<?php
/**
 * @file
 * Provides the Whatsappshare' administrative interface.
 */

/**
 * Creating form which save whatsapp share data.
 *
 * @param array $form
 *   Array form will contain information about whatsappshare_form data.
 * @param array $form_submit
 *   Array get form values form submissions.
 */
function whatsappshare_form($form, &$form_submit) {
  $form['button_text'] = array(
    '#title' => t('Button Text'),
    '#type' => 'textfield',
    '#description' => t('Insert your Whatsapp share button text.'),
    '#default_value' => variable_get('whatsappshare_button_text', t('Share on whatsapp')),
    '#required' => TRUE,
  );
  $form['button_size'] = array(
    '#title' => t('Button Size'),
    '#type' => 'select',
    '#description' => t('Select the Button Size.'),
    '#options' => array(
      'wa_btn_s' => t('Small'),
      'wa_btn_m' => t('Medium'),
      'wa_btn_l' => t('Large'),
    ),
    '#default_value' => variable_get('whatsappshare_button_size', t('Small')),
  );
  $form['sharing_text'] = array(
    '#title' => t('Sharing text'),
    '#type' => 'textarea',
    '#description' => t('Insert Sharing text.'),
    '#required' => TRUE,
    '#cols' => 60,
    '#rows' => 5,
    '#default_value' => variable_get('whatsappshare_sharing_text', t('Your sharing text will come here.')),
  );
  $form['sharing_location'] = array(
    '#title' => t('Sharing location'),
    '#type' => 'textfield',
    '#description' => t('Insert Sharing location using a jQuery selector. For example: #page-title to place it after the page title.'),
    '#required' => TRUE,
    '#default_value' => variable_get('whatsappshare_sharing_location', '#page-title'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  return $form;
}

/**
 * Function whatsappshare_form_submit() will contain submission logic.
 *
 * @param array $form_state
 *   Array which set form values in database after form submissions.
 */
function whatsappshare_form_submit($form, &$form_state) {
  variable_set('whatsappshare_button_text', $form_state['values']['button_text']);
  variable_set('whatsappshare_button_size', $form_state['values']['button_size']);
  variable_set('whatsappshare_sharing_text', $form_state['values']['sharing_text']);
  variable_set('whatsappshare_sharing_location', $form_state['values']['sharing_location']);
  drupal_set_message(t('Your Whatsapp Share configuration has been saved.'));
}
