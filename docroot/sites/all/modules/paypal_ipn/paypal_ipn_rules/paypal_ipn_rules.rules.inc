<?php
/**
 * @files
 * Rules Hooks
 */
/**
 * Implements hook_rules_event_info().
 */
function paypal_ipn_rules_rules_event_info() {
  $items = array(

    'paypal_ipn' => array(
      'label' => t('PayPal IPN Notification'),
      'group' => t('PayPal'),
      'variables' => array(
          'paypal_ipn_notification' => array('type' => 'paypal_ipn', 'label' => t('PayPal IPN Post Back')),
        ),
    ),
    'paypal_ipn_entity' => array(
      'label' => t('PayPal IPN Notification for Entity'),
      'group' => t('PayPal'),
      'variables' => array(
        'paypal_ipn_notification' => array('type' => 'paypal_ipn', 'label' => t('PayPal IPN Post Back')),
        'entity' => array(
          'type' => 'entity',
          'label' =>  t('Entity'),
          'wrapped' => TRUE,
          'restriction' => 'selector',
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implements hook_rules_data_info().
 */
function paypal_ipn_rules_rules_data_info(){
  return array(
    'paypal_ipn' => array(
      'label' => t('PayPal IPN Post Back'),
      'wrap' => TRUE,
      'property info' => _paypal_ipn_rules_notification_info(),
    ),
  );
}

/**
 * Paypal IPN Info array.
 * @return array
 */
function _paypal_ipn_rules_notification_info() {
  $post_fields = &drupal_static(__FUNCTION__);
  if (!isset($post_fields)) {
    if ($cache = cache_get('paypalipn_post_fields')) {
      $post_fields = $cache->data;
    }
    else {
      $post_fields = _paypal_ipn_rules_known_fields_info();
      drupal_alter('paypal_ipn_rules_fields_info', $post_fields);
      cache_set('paypal_ipn_post_fields', $post_fields, 'cache');
    }
  }
  return $post_fields;
}