<?php
/**
 * Author: Ted Bowman
 * Date: 10/17/14
 * Time: 9:18 PM
 */
/**
 * @return PaypalIpnListener
 * @throws Exception
 */
function paypal_ipn_processor() {
  libraries_load('php_paypal_ipn');
  $listener = new PaypalIpnListener();

  $verified = $listener->processIpn();
  $report = $listener->getTextReport();

  if ($verified) {
    watchdog('paypal_ipn',"Report: $report");
  } else {
    watchdog('paypal_ipn',"Not verified. Report: $report", NULL, WATCHDOG_CRITICAL);
  }
  return $listener;

}
function paypal_ipn_listen_entity($entity_type, $entity_id) {
  $listener = paypal_ipn_processor();
  if ($listener->isVerified()) {
    module_invoke_all('paypal_ipn_entity_notification', $listener, $entity_type, $entity_id);
  }

}
function paypal_ipn_listen() {
  $listener = paypal_ipn_processor();
  if ($listener->isVerified()) {
    module_invoke_all('paypal_ipn_notification', $listener);
  }

}