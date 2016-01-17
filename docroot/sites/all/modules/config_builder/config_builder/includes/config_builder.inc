<?php
/**
 * @file
 * Configuration builder module integration.
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function config_builder_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "config_builder" && $api == "config_builder") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_config_builder_defaults().
 */
function config_builder_config_builder_defaults() {
  $export = array();

  $config_builder = new stdClass();
  $config_builder->disabled = TRUE;
  $config_builder->api_version = 1;
  $config_builder->name = 'site_information';
  $config_builder->label = 'Site information';
  $config_builder->description = 'Replacement form for the system \'Site information\' form.';
  $config_builder->path = 'admin/config/system/site-information';
  $config_builder->access = array(
    'type' => 'user_access',
    'user_access' => 'administer site configuration',
  );
  $config_builder->fapi = array(
    'site_information' => array(
      '#weight' => '0',
      '#collapsed' => '0',
      '#collapsible' => '0',
      '#title' => 'Site details',
      '#type' => 'fieldset',
      'site_name' => array(
        '#required' => '1',
        '#title' => 'Site name',
        '#type' => 'textfield',
        '#weight' => '0',
        '#key' => 'site_name',
        '#form_builder' => array(
          'element_id' => 'site_name',
          'element_type' => 'textfield',
          'configurable' => TRUE,
          'removable' => TRUE,
        ),
      ),
      'site_slogan' => array(
        '#required' => '0',
        '#type' => 'textfield',
        '#title' => 'Slogan',
        '#description' => 'How this is used depends on your site\'s theme.',
        '#weight' => '1',
        '#key' => 'site_slogan',
        '#form_builder' => array(
          'element_id' => 'site_slogan',
          'element_type' => 'textfield',
          'configurable' => TRUE,
          'removable' => TRUE,
        ),
      ),
      'site_mail' => array(
        '#required' => '1',
        '#type' => 'textfield',
        '#title' => 'E-mail address',
        '#description' => 'The <em>From</em> address in automated e-mails sent during registration and new password requests, and other notifications. (Use an address ending in your site\'s domain to help prevent this e-mail being flagged as spam.)',
        '#weight' => '2',
        '#key' => 'site_mail',
        '#form_builder' => array(
          'element_id' => 'site_mail',
          'element_type' => 'textfield',
          'configurable' => TRUE,
          'removable' => TRUE,
        ),
      ),
      '#key' => 'site_information',
      '#form_builder' => array(
        'element_id' => 'site_information',
        'element_type' => 'fieldset',
        'configurable' => TRUE,
        'removable' => TRUE,
      ),
    ),
    'front_page' => array(
      '#weight' => '2',
      '#collapsed' => '0',
      '#type' => 'fieldset',
      '#collapsible' => '0',
      '#title' => 'Front page',
      'default_nodes_main' => array(
        '#multiple_toggle' => '1',
        '#title' => 'Number of posts on front page ',
        '#type' => 'select',
        '#options' => array(
          1 => '1',
          2 => '2',
          3 => '3',
          4 => '4',
          5 => '5',
          6 => '6',
          7 => '7',
          8 => '8',
          9 => '9',
          10 => '10',
          11 => '15',
          12 => '20',
          13 => '25',
          14 => '30',
        ),
        '#multiple' => '0',
        '#description' => 'The maximum number of posts displayed on overview pages such as the front page.',
        '#key_type_toggled' => '0',
        '#weight' => '0',
        '#required' => '0',
        '#key' => 'default_nodes_main',
        '#form_builder' => array(
          'element_id' => 'default_nodes_main',
          'element_type' => 'select',
          'configurable' => TRUE,
          'removable' => TRUE,
        ),
      ),
      'site_frontpage' => array(
        '#description' => 'Optionally, specify a relative URL to display as the front page.  Leave blank to display the default content feed.',
        '#type' => 'textfield',
        '#title' => 'Default front page',
        '#weight' => '1',
        '#size' => '40',
        '#required' => '0',
        '#field_prefix' => url('', array('absolute' => TRUE)),
        '#key' => 'site_frontpage',
        '#form_builder' => array(
          'element_id' => 'site_frontpage',
          'element_type' => 'textfield',
          'configurable' => TRUE,
          'removable' => TRUE,
        ),
      ),
      '#key' => 'front_page',
      '#form_builder' => array(
        'element_id' => 'front_page',
        'element_type' => 'fieldset',
        'configurable' => TRUE,
        'removable' => TRUE,
      ),
    ),
    'error_page' => array(
      '#weight' => '3',
      '#collapsed' => '0',
      '#type' => 'fieldset',
      '#collapsible' => '0',
      '#title' => 'Error pages',
      'site_403' => array(
        '#title' => 'Default 403 (access denied) page',
        '#description' => 'This page is displayed when the requested document is denied to the current user. Leave blank to display a generic "access denied" page.',
        '#weight' => '0',
        '#size' => '40',
        '#type' => 'textfield',
        '#required' => '0',
        '#field_prefix' => url('', array('absolute' => TRUE)),
        '#key' => 'site_403',
        '#form_builder' => array(
          'element_id' => 'site_403',
          'element_type' => 'textfield',
          'configurable' => TRUE,
          'removable' => TRUE,
        ),
      ),
      'site_404' => array(
        '#title' => 'Default 404 (not found) page',
        '#description' => 'This page is displayed when no other content matches the requested document. Leave blank to display a generic "page not found" page.',
        '#weight' => '1',
        '#size' => '40',
        '#type' => 'textfield',
        '#required' => '0',
        '#field_prefix' => url('', array('absolute' => TRUE)),
        '#key' => 'site_404',
        '#form_builder' => array(
          'element_id' => 'site_404',
          'element_type' => 'textfield',
          'configurable' => TRUE,
          'removable' => TRUE,
        ),
      ),
      '#key' => 'error_page',
      '#form_builder' => array(
        'element_id' => 'error_page',
        'element_type' => 'fieldset',
        'configurable' => TRUE,
        'removable' => TRUE,
      ),
    ),
  );
  $export['site_information'] = $config_builder;

  return $export;
}
