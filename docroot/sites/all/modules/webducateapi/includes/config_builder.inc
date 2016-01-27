<?php
/**
 * @file
 * Configuration builder module integration.
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function webducateapi_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "config_builder" && $api == "config_builder") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_config_builder_defaults().
 */
function webducateapi_config_builder_defaults() {
  $export = array();

  $config_builder = new stdClass();
  $config_builder->disabled = FALSE; /* Edit this to true to make a default config_builder disabled initially */
  $config_builder->api_version = 1;
  $config_builder->name = 'webducateapi';
  $config_builder->label = 'WebducateAPI';
  $config_builder->description = '';
  $config_builder->path = 'admin/config/services/webducate';
  $config_builder->access = array(
    'type' => 'user_access',
    'user_access' => 'access administration menu',
  );
  $config_builder->fapi = array(
    'webducateapi_settings' => array(
      '#title' => 'API settings',
      '#type' => 'fieldset',
      '#collapsible' => 0,
      '#collapsed' => 0,
      '#form_builder' => array(
        'element_id' => 'webducateapi_settings',
        'element_type' => 'fieldset',
        'configurable' => TRUE,
        'removable' => TRUE,
      ),
      '#key' => 'webducateapi_settings',
      '#weight' => '0',
      '#tree' => 1,
      'url' => array(
        '#title' => 'URL',
        '#type' => 'textfield',
        '#form_builder' => array(
          'element_id' => 'url',
          'element_type' => 'textfield',
          'configurable' => TRUE,
          'removable' => TRUE,
        ),
        '#key' => 'url',
        '#weight' => '0',
        '#required' => 1,
        '#default_value' => 'http://portal.webducate.com.au/services/xmlrpc',
      ),
      'key' => array(
        '#title' => 'Key',
        '#type' => 'textfield',
        '#form_builder' => array(
          'element_id' => 'key',
          'element_type' => 'textfield',
          'configurable' => TRUE,
          'removable' => TRUE,
        ),
        '#key' => 'key',
        '#weight' => '1',
        '#required' => 1,
      ),
      'domain' => array(
        '#title' => 'Domain',
        '#type' => 'textfield',
        '#form_builder' => array(
          'element_id' => 'domain',
          'element_type' => 'textfield',
          'configurable' => TRUE,
          'removable' => TRUE,
        ),
        '#key' => 'domain',
        '#weight' => '2',
        '#required' => 1,
      ),
      'username' => array(
        '#title' => 'Username',
        '#type' => 'textfield',
        '#form_builder' => array(
          'element_id' => 'username',
          'element_type' => 'textfield',
          'configurable' => TRUE,
          'removable' => TRUE,
        ),
        '#key' => 'username',
        '#weight' => '3',
        '#required' => 1,
      ),
      'password' => array(
        '#title' => 'Password',
        '#type' => 'textfield',
        '#form_builder' => array(
          'element_id' => 'password',
          'element_type' => 'textfield',
          'configurable' => TRUE,
          'removable' => TRUE,
        ),
        '#key' => 'password',
        '#weight' => '4',
        '#required' => 1,
      ),
    ),
  );
  $export['webducateapi'] = $config_builder;

  return $export;
}
