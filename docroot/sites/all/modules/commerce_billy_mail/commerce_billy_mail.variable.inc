<?php
/**
 * @file
 * Definition of Commerce Billy Mail variables for the Variable API
 * Allows e-mail subject and body to be localized using the i18n_variable module
 */

/**
 * Implements hook_variable_info().
 */
function commerce_billy_mail_variable_info($options) {
  $variable['commerce_billy_mail_from'] = array(
    'title' => t('E-mail Sender Address (FROM)', array(), $options),
    'description' => t('The e-mail sender address (FROM). Defaults to the site e-mail address.', array(), $options),
    'type' => 'mail_address',
    'default' => ini_get('sendmail_from'),
    'access' => 'administer site configuration',
  );
  $variable['commerce_billy_mail_cc'] = array(
    'title' => t('E-mail recipient CC address', array(), $options),
    'description' => t('Carbon Copy (CC) recipient for the invoice e-mails.
    	Multiple addresses can be separated by comma (,) without whitespace. Leave empty if not used.', array(), $options),
    'type' => 'mail_address',
    'access' => 'administer site configuration',
  );
  $variable['commerce_billy_mail_bcc'] = array(
    'title' => t('E-mail recipient BCC address', array(), $options),
    'description' => t('Blind Carbon Copy (BCC) recipient for the invoice e-mails. Multiple addresses can be separated by comma (,) without whitespace. Leave empty if not used.',	array(), $options),
    'type' => 'mail_address',
    'access' => 'administer site configuration',
  );
  $variable['commerce_billy_mail_subject'] = array(
    'title' => t('Commerce Billy Mail Subject', array(), $options),
    'description' => t('Subject for the invoice e-mails. You may use tokens.', array(), $options),
    'type' => 'string',
    'token' => TRUE,
    'localize' => TRUE,
    'access' => 'administer site configuration',
  );
  $variable['commerce_billy_mail_body'] = array(
    'title' => t('Commerce Billy Mail Body', array(), $options),
    'description' => t('Body for the invoice e-mails. You may use tokens.', array(), $options),
    'type' => 'text',
    'token' => TRUE,
    'localize' => TRUE,
    'access' => 'administer site configuration',
  );
  $variable['commerce_billy_mail_attach_pdf_invoice'] = array(
    'title' => t('Attach PDF invoice', array(), $options),
    'description' => t('Send the Billy PDF invoice attached.', array(), $options),
    'type' => 'boolean',
    'default' => 1,
    'access' => 'administer site configuration',
  );
  $variable['commerce_billy_mail_plaintext'] = array(
    'title' => t('Send e-mails as plain text.', array(), $options),
    'description' => t('Send the invoice e-mails as plain text (no HTML body).', array(), $options),
    'type' => 'boolean',
    'default' => 0,
    'access' => 'administer site configuration',
  );
 
  return $variable;
}