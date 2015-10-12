<?php
/**
 * @file
 * Declare basic info to Variable API module.
 * NOTE: Only basic declarations are provided to avoid dependency.
 */

 
/**
 * Implements hook_variable_info().
 */
function quick_feedback_variable_info($options) {
  $variables[SETTINGS_PREFIX . 'link_mail'] = array(
    'title' => t('Quick Feedback Email', array(), $options),
    'description' => t('Who to send quick feedback email to.', array(), $options),
    'type' => 'string',
    'localize' => TRUE,
    'default' => variable_get('site_mail', ini_get('sendmail_from')),
  );
  $variables[SETTINGS_PREFIX . 'link_text'] = array(
    'title' => t('Quick Feedback Link', array(), $options),
    'description' => t('Form link text.', array(), $options),
    'type' => 'string',
    'localize' => TRUE,
    'default' => EMAIL_FEEDBACK_LINK_TEXT,
  );
  $variables[SETTINGS_PREFIX . 'mail_subject'] = array(
    'title' => t('Quick Feedback Subject', array(), $options),
    'description' => t('Email subject line.', array(), $options),
    'type' => 'string',
    'localize' => TRUE,
    'default' => EMAIL_FEEDBACK_MAIL_SUBJECT,
  );
  $variables[SETTINGS_PREFIX . 'mail_body'] = array(
    'title' => t('Quick Feedback Body', array(), $options),
    'description' => t('Email body content.', array(), $options),
    'type' => 'string',
    'localize' => TRUE,
    'default' => EMAIL_FEEDBACK_MAIL_BODY,
  );

  return $variables;
}
