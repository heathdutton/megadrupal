<?php
/**
 * @file
 * Define Ctools Exportable plugin
 */
$plugin = array(
  // The name of the table as found in the schema in hook_install. This
  // must be an exportable type with the 'export' section defined.
  'schema' => 'awssdk_services',
  'access' => 'access awssdk ui',
  // These are required to provide proper strings for referring to the
  // actual type of exportable. "proper" means it will appear at the
  // beginning of a sentence.
  'title singular' => t('service'),
  'title singular proper' => t('Service'),
  'title plural' => t('services'),
  'title plural proper' => t('Services'),
  'menu' => array(
    'menu prefix' => 'admin/config/aws',
    'menu item' => 'service',
    // Title of the top level menu. Note this should not be translated,
    // as the menu system will translate it.
    'menu title' => 'AWS SDK for PHP services',
    // Description of the top level menu, which is usually needed for
    // menu items in an administration list. Will be translated
    // by the menu system.
    'menu description' => 'Administer AWS SDK for PHP services.',
  ),
  // All callbacks will accept &$form and &$form_state as arguments.
  'form' => array(
    'settings' => 'awssdk_ctools_export_ui_form',
  ),
  'handler' => array(
    'class' => 'awssdk_service_ctools_export_ui',
    'parent' => 'ctools_export_ui',
  ),
  'use wizard' => TRUE,
  'form info' => array(
    'order' => array(
      'name' => t('Service'),
      'config' => t('Config'),
    ),
    'show trail' => FALSE,
  ),
);

/**
 * Form for the access credentials and the region for a service and
 * if necessary to provide service specific configuration.
 */
function awssdk_ctools_export_ui_form(&$form, &$form_state) {
  $item = &$form_state['item'];
  if ($form_state['step'] !== 'config' && empty($item->sid)) {
    return;
  }
  ctools_include('plugins');
  $plugin = ctools_get_plugins('awssdk2', 'service', $item->name);

  $form['config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuration'),
    '#tree' => TRUE,
  );
  $form['config']['access'] = array(
    '#type' => 'fieldset',
    '#title' => t('Access credentials'),
  );
  $form['config']['access']['key'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Access key ID'),
    '#description' => t('It is a <a target=_blank href="!href">best practice</a> to create individual users to access your AWS account and never share the root account credentials with anyone else.', array('!href' => 'http://docs.aws.amazon.com/IAM/latest/UserGuide/IAMBestPractices.html')),
    '#default_value' => isset($item->config['access']['key']) ? $item->config['access']['key'] : '',
  );
  $form['config']['access']['secret'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Secret key'),
    '#default_value' => isset($item->config['access']['secret']) ? $item->config['access']['secret'] : '',
  );
  $regions = drupal_map_assoc(\Aws\Common\Enum\Region::keys());
  if ($function = ctools_plugin_get_function($plugin, 'supported_regions')) {
    $supported_regions = $function();
    if (!empty($supported_regions)) {
      $regions = array_intersect_assoc(drupal_map_assoc($supported_regions), $regions);
    }
  }
  $form['config']['region'] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('Region'),
    '#description' => t('Used region for this service.'),
    '#default_value' => isset($item->config['region']) ? $item->config['region'] : NULL,
    '#options' => $regions,
  );
  if ($function = ctools_plugin_get_function($plugin, 'service_config')) {
    $config = &$form['config'];
    $function($config, $form_state);
  }
}
