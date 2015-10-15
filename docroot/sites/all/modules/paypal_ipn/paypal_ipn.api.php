<?php
/**
 * @file paypal.api.php
 * Hooks documentation
 */
/**
 * @param PaypalIpnListener $listener
 * @param $entity_type
 * @param $entity_id
 */
function hook_paypal_ipn_entity_notification(PaypalIpnListener $listener, $entity_type, $entity_id) {
  $entity = array_pop(entity_load($entity_type, array($entity_id)));
  watchdog('ipn_test', 'label = ' . entity_label($entity_type, $entity));
}

/**
 * @param PaypalIpnListener $listener
 */
function hook_paypal_ipn_notification(PaypalIpnListener $listener){
  watchdog('ipn_test', 'just report = ' . $listener->getTextReport());
}

