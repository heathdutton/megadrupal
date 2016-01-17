<?php

/**
 * Implementation of hook_views_data().
 */
function signup_sms_views_data() {
  $data = array();

  // Expose signup sms codes to views.
  $data['signup_sms_codes']['table']['group'] = t('Signup SMS');
  $data['signup_sms_codes']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );
  $data['signup_sms_codes']['nid'] = array(
    'title' => 'Signup SMS Codes',
    'help' => 'Pull Signup SMS information for the node',
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => 'Signup SMS Codes label',
    ),
  );

  $data['signup_sms_codes']['code'] = array(
    'title' => 'Code',
    'help' => 'SMS Code',
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );
  return $data;
}
