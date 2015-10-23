<?php

/**
 * @file
 * Hooks provided by Commerce So Colissimo.
 */

/**
 * React when a So Colissimo shipping service status is created.
 *
 * @param array $shipping_service_status
 *   Saved status of the shipping service.
 */
function hook_commerce_socolissimo_service_status_insert($shipping_service_status) {
  if ($shipping_service_status['enabled']) {
    $shipping_service = commerce_shipping_service_load($shipping_service_status['name']);
    drupal_set_message('A new shipping service is available %shipping_service.', array('%shipping_service' => $shipping_service['title']));
  }
}

/**
 * React when a So Colissimo shipping service status is updated.
 *
 * @param array $shipping_service_status
 *   Saved status of the shipping service.
 */
function hook_commerce_socolissimo_service_status_update($shipping_service_status) {
  $shipping_service = commerce_shipping_service_load($shipping_service_status['name']);
  $status = $shipping_service_status['enabled'] ? t('enabled') : t('disabled');
  drupal_set_message('Shipping service %shipping_service has been %status', array('%shipping_service' => $shipping_service['title'], '%status' => $status));
}

/**
 * React when a So Colissimo shipping service status is deleted.
 *
 * @param array $shipping_service_status
 *   Saved status of the shipping service.
 */
function hook_commerce_socolissimo_service_status_delete($shipping_service_status) {
  $shipping_service = commerce_shipping_service_load($shipping_service_status['name']);
  $status = $shipping_service_status['enabled'] ? t('enabled') : t('disabled');
  drupal_set_message("Shipping service %shipping_service is enabled. It's status has been deleted.", array('%shipping_service' => $shipping_service['title']));
}
