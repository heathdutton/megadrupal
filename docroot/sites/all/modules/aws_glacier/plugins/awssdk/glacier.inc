<?php
$plugin = array(
  'title' => 'Glacier',
  'supported_regions' => 'aws_glacier_supported_regions',
  'service_config' => 'aws_glacier_plugin_config_form',
);

/**
 * Returns all supported regions for aws glacier for the awssdk services configuration.
 */
function aws_glacier_supported_regions() {
  $supported = array('NORTHERN_VIRGINIA', 'OREGON', 'NORTHERN_CALIFORNIA',
  'IRELAND', 'FRANKFURT', 'SYDNEY', 'TOKYO');
  return $supported;
}

/**
 * Config form for this awssdk service plugin.
 */
function aws_glacier_plugin_config_form(&$config, $form_state) {
  $item = &$form_state['item'];
  $config['debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug'),
    '#default_value' => isset($item->config['debug']) ? $item->config['debug'] : 0,
    '#description' => t('When enabled, all invoked commands will be logged to watchdog.')
  );
}
