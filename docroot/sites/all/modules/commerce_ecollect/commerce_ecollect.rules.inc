<?php

/**
 * @file
 * Rules integration.
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_ecollect_rules_action_info() {
  $actions = array(
    'commerce_ecollect_set_payment_parameters' => array(
      'label' => t('Set payment parameters'),
      'group' => t('eCollect'),
      'parameter' => array(
        'entitycode' => array(
          'type' => 'text',
          'label' => t('EntityCode'),
          'description' => t('Code given by Avisor Technologies.'),
        ),
        'srvcode' => array(
          'type' => 'text',
          'label' => t('SrvCode'),
          'description' => t('Code given by Avisor Technologies.'),
        ),
        'urlresponse' => array(
          'type' => 'text',
          'label' => t('URLResponse'),
          'optional' => TRUE,
          'description' => t('URL to process your request.'),
        ),
        'urlredirect' => array(
          'type' => 'text',
          'label' => t('URLRedirect'),
          'optional' => TRUE,
          'description' => t('URL to redirect after payment process. You can use the constant COMMERCE__ORDER_ID as keyword for current order_id.'),
        ),
        'sign' => array(
          'type' => 'text',
          'label' => t('Sign'),
          'optional' => TRUE,
          'description' => t('This may be a single value to be coded as MD5 string.'),
        ),
        'signfields' => array(
          'type' => 'text',
          'label' => t('SignFields'),
          'optional' => TRUE,
          'description' => t('Fields to sign.'),
        ),
        'referencearray' => array(
          'type' => 'text',
          'label' => t('ReferenceArray'),
          'description' => t('Comma-separated extra values for this transaction. WARNING: blank spaces will be treated as it. For example, if you need 10 values, and last value is not required (blank), keep a space at end of this value, like this (without quotes): <em>"Value 1,Value 2,Value 3,Value N, , , , , , "</em>. Therefore, do not put comma for last value.'),
        ),
      ),
    ),
  );
  return $actions;
}

/**
 * Set payment parameters for this transaction.
 */
function commerce_ecollect_set_payment_parameters($entitycode, $srvcode, $urlresponse, $urlredirect, $sign, $signfields, $referencearray) {
  $_SESSION['eCollect'] = array(
    'EntityCode' => $entitycode,
    'SrvCode' => $srvcode,
    'URLResponse' => $urlresponse,
    'URLRedirect' => !empty($urlredirect) ? $urlredirect : url('ecollect/order/COMMERCE__ORDER_ID/status', array('absolute' => TRUE)),
    'Sign' => !empty($sign) ? md5($sign) : '',
    'SignFields' => $signfields,
    'ReferenceArray' => explode(',', $referencearray),
  );
}
