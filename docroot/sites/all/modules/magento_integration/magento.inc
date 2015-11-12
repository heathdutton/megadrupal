<?php

/**
 * @file
 * Helper functions for Drupal Magento integration.
 */

/**
 * Test the connection to the Magento Core API.
 *
 * @return
 *   True for a successful login into the Magento API and false otherwise.
 */
function magneto_test_api_connection() {
  module_load_include('inc', 'magento', 'magento.class');
  $magento_connection = new MagentoConnection();
  $magento = new Magento($magento_connection);

  if (!empty($magento->connected)) {
    drupal_set_message(t('Successfully connected to Magneto Core API.'), 'status');
    return TRUE;
  }
  else {
    drupal_set_message(t('Unable to connect to the Magento Core API. Check the Drupal log for more information.'), 'error');
    return FALSE;
  }
}
