<?php

/**
 * @file
 * Settings display in admin section.
 */

require_once dirname(__FILE__) . '/magento_api.inc';

/**
 * Implements hook_form().
 */
function magento_api_settings($form, &$form_state) {

  $form['providers'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Providers'),
  );

  $header = array(
    'code'      => t('Code'),
    'name'      => t('Name'),
    'url'       => t('Url'),
    'api_user'  => t('Api User'),
    'api_key'   => t('Api Key'),
    'available' => t('Available'),
  );

  $rows      = array();
  $providers = magento_api_provider_list();
  $debug     = variable_get('magento_debug', TRUE);

  foreach ($providers as $code => $provider) {
    $row = array(
      'code'      => $code,
      'name'      => $provider['name'],
      'url'       => l($provider['url'], $provider['url'], array('attributes' => array('target' => '_blank'))),
      'api_user'  => $provider['api_user'],
      'api_key'   => '[' . (empty($row['api_key']) ? t('Provided') : t('Missing')) . ']',
      'available' => t('Down'),
    );

    // Create remote adapter which wrap soapclient.
    $adapter = magento_api_provider_get($code, $debug);

    if ($adapter && $adapter->login()) {
      $time = microtime(TRUE);
      $adapter->ping();
      $duration         = microtime(TRUE) - $time;
      $row['available'] = t('Up') . '<br/><em>(' . t(
          '!duration ms',
          array('!duration' => round($duration * 1000))
        ) . ')*</em>';
    }

    $rows[] = $row;
  }

  $form['providers']['table'] = array(
    '#theme'  => 'table',
    '#header' => $header,
    '#rows'   => $rows,
    '#empty'  => t('No content available.'),
    '#suffix' => '<p><em>Providers stored in "settings.php" file.<br/>' . "* : represents duration for 'ping' method, 'login' action is previously done.</em></p>",
  );

  return $form;
}
