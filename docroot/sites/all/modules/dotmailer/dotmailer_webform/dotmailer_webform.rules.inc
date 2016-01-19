<?php

/**
 * @file
 * This file provides rules API callbacks
 */

/**
 * Implements hook_rules_condition_info().
 */
function dotmailer_webform_rules_condition_info() {
  $items = array();
  $items['dotmailer_webform_rule_has_subscribed_dotmailer'] = array(
    'label' => t('Subscribed to dotMailer'),
    'parameter' => array(
      'mailing_list' => array(
        'type' => 'list<integer>',
        'label' => t('The submitted webform node'),
        'options list' => 'dotmailer_active_address_books_list',
      ),
      'data' => array(
        'type' => 'webform',
        'label' => t('The submitted webform data.'),
      ),
    ),
    'help' => t('Returns TRUE if the user subscribes to the specified dotMailer mailing list using a webform.'),
    'group' => t('dotMailer'),
  );
  return $items;
}


function dotmailer_webform_rule_has_subscribed_dotmailer($mailing_lists, $submission, $more_args_here) {
  foreach ($submission['components'] as $component) {
    if ($component['component']['type'] == 'dotmailer') {
      $value = $component['value'][0];
      if ($value && isset($mailing_lists[$value])) {
        return TRUE;
      }
    }
  }
  return FALSE;
}