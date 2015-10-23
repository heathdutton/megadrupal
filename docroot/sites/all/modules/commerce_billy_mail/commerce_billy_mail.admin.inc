<?php

/**
 * @file
 * Commercy billy mail administration
 */

/**
 * commerce_billy_mail_settings_form.admin settings form.
 */
function commerce_billy_mail_settings_form() {
  $module_path = drupal_get_path('module', 'commerce_billy_mail');
  $commercy_billy_pdf_enabled = module_exists('commerce_billy_pdf invoice');

  $site_mail = variable_get('site_mail', ini_get('sendmail_from'));
  $mail_from = variable_get('commerce_billy_mail_from', '');
  $form['commerce_billy_mail_from'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail Sender Address (FROM)'),
    '#description' => t("The E-mail sender (from) address. Defaults to the site mail address."),
    '#default_value' => empty($mail_from) ? $site_mail : $mail_from,
    '#required' => TRUE,
  );

  $form['commerce_billy_mail_cc'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail recipient CC address'),
    '#description' => t("Carbon Copy (CC) recipient for the invoice mails. Multiple addresses can be separated by comma (,) without whitespace. Leave empty if not used."),
    '#required' => FALSE,
    '#default_value' => variable_get('commerce_billy_mail_cc', ''));

  $form['commerce_billy_mail_bcc'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail recipient BCC address'),
    '#description' => t("Blind Carbon Copy (BCC) recipient for the invoice mails. Multiple addresses can be separated by comma (,) without whitespace. Leave empty if not used."),
    '#required' => FALSE,
    '#default_value' => variable_get('commerce_billy_mail_bcc', ''));

  $form['commerce_billy_mail_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#description' => t("Subject for the invoice mails. You may use tokens."),
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_billy_mail_subject', ''));

  $form['commerce_billy_mail_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#description' => t("Body for the invoice mails. You may use tokens."),
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_billy_mail_body', ''));

  $form['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('commerce-order', 'user'), // The token types that have specific context. Can be multiple token types like 'term' and/or 'user'
    '#global_types' => TRUE, // A boolean TRUE or FALSE whether to include 'global' context tokens like [current-user:*] or [site:*]. Defaults to TRUE.
    '#click_insert' => TRUE, // A boolean whether to include the 'Click this token to insert in into the the focused textfield' JavaScript functionality. Defaults to TRUE.
    '#dialog' => TRUE,
  );

  $form['commerce_billy_mail_plaintext'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send E-mails as plain text.'),
    '#description' => t("Send the invoice E-mails as plain text (no HTML body)."),
    '#default_value' => variable_get('commerce_billy_mail_plaintext', FALSE));

  $form['commerce_billy_mail_attach_pdf_invoice'] = array(
    '#type' => 'checkbox',
    '#title' => t('Attach PDF invoice'),
    '#description' => t("Send the Billy PDF invoice attached.<br /><strong>Important:</strong> You need to enable commerce_billy_pdf and check your status page to see if you meet the requirements for an attachment handling email library like !mimemail.", array('!mimemail' => l('Mimemail', 'https://www.drupal.org/project/mimemail'))),
    '#default_value' => variable_get('commerce_billy_mail_attach_pdf_invoice', TRUE) && module_exists('commerce_billy_pdf'),
    '#disabled' => !module_exists('commerce_billy_pdf'),
  );

  $is_theme_registry_workaround_available = _commerce_billy_mail_is_theme_registry_workaround_available();
  if ($is_theme_registry_workaround_available) {
    $form['commerce_billy_mail_use_theme_registry_workaround_theme_items'] = array(
      '#type' => 'select',
      '#title' => t('PDF invoice: Use theme registry admin theme workaround (Advanced)'),
      '#description' => t('This workaround is <strong>only recommended and available if</strong> you 1. use an administration theme AND have overridden .tpl.php template files of the commerce_billy_pdf module in your default theme: @default_theme The reason is a limitation in Drupals theme detection when using an administration theme. <strong>Warning:</strong> If you select a file that isn\'t overridden in your default template directory, no template file will be found!', array('@default_theme' => commerce_billy_mail_custom_theme())),
      '#default_value' => variable_get('commerce_billy_mail_use_theme_registry_workaround_theme_items', array()),
      '#options' => _commerce_billy_mail_get_pdf_theme_items(),
      '#disabled' => !$is_theme_registry_workaround_available,
      '#multiple' => TRUE,
      '#states' => array(
        // Only show if PDF invoice is enabled
        'visible' => array(
          ':input[name="commerce_billy_mail_attach_pdf_invoice"]' => array('checked' => TRUE),
        ),
      ),
    );
  }

  return system_settings_form($form);
}
