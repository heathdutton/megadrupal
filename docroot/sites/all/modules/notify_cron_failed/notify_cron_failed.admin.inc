<?php

/**
 * @file
 * Important administration functions for the notify cron failed module.
 */

/**
 * Implements of notify_cron_failed_form().
 */
function notify_cron_failed_form() {
  $form = array();
  $site_mail = variable_get('site_mail', '');
  $form['notify_cron_failed'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings For Notify Cron Failed'),
    '#collapsible' => FALSE,
  );
  $form['notify_cron_failed']['notify_cron_failed_email_address'] = array(
    '#type' => 'textfield',
    '#title' => t('To'),
    '#default_value' => variable_get('notify_cron_failed_email_address', $site_mail),
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('Email address to which notification is to be sent'),
    '#required' => TRUE,
  );
  $form['notify_cron_failed']['notify_cron_failed_email_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => t(variable_get('notify_cron_failed_email_subject', 'Cron run failed')),
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('A brief summary of the topic of message'),
    '#required' => TRUE,
  );
  $form['notify_cron_failed']['notify_cron_failed_email_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => t(variable_get('notify_cron_failed_email_message', 'Cron has some issues it is not running as per schedule.')),
    '#description' => t('Message to be sent to user'),
    '#required' => TRUE,
  );
  $form['notify_cron_failed']['notify_cron_failed_when'] = array(
    '#type' => 'select',
    '#title' => t('When'),
    '#options' => array(
      1 => t('1'),
      2 => t('2'),
      3 => t('3'),
      4 => t('4'),
      5 => t('5'),
      6 => t('6'),
      7 => t('7'),
      8 => t('8'),
      9 => t('9'),
      10 => t('10'),
    ),
    '#default_value' => variable_get('notify_cron_failed_when', '1'),
    '#description' => t('Set this to <em>Number of days</em> after which email notification to be sent'),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}
