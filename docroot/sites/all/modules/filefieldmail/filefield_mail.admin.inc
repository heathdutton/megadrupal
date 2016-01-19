<?php
/**
 * @file
 * filefield_mail module settings UI.
 */

/**
 * Form builder; Configure the filefield_mail settings.
 *
 * @see system_settings_form()
 */
function filefield_mail_admin_settings() {
  $form = array();
  // filefield_mail configuration settings.
  // the admin can write the content of the mail with the download link.
  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('content of the mail with the download link'),
  );
  $form['display']['filefield_mail_subject'] = array(
    '#type' => 'textfield',
    '#size' => 80,
    '#maxlength' => 80,
    '#title' => t('Email subject'),
    '#default_value' => variable_get('filefield_mail_subject', ''),
  );
  $form['display']['filefield_mail_text_before_link'] = array(
    '#type' => 'textarea',
    '#cols' => 60,
    '#rows' => 10,
    '#title' => t('Email text placed before the link'),
    '#default_value' => variable_get('filefield_mail_text_before_link', ''),
  );
  $form['display']['filefield_mail_text_after_link'] = array(
    '#type' => 'textarea',
    '#cols' => 60,
    '#rows' => 10,
    '#title' => t('Email text placed after the link'),
    '#default_value' => variable_get('filefield_mail_text_after_link', ''),
  );
  return system_settings_form($form);
}
