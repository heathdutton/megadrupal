<?php

/**
 * @file Contains the administration UI forms and submission handlers.
 */

// Load helpers
include_once drupal_get_path('module', 'clicktale') . '/includes/clicktale.helpers.inc';

/**
 * Implementation of the administration form.
 *
 * @return string $form
 */
function clicktale_admin_settings_form() {
  $form = array();

  // Check that the PHP integration library is installed.
  if (!clicktale_get_library_path($msg)) {
    drupal_set_message($msg, 'error');
    return;
  }

  // Check that the PHP integration library config.php file is writable.
  $config = clicktale_get_library_path() . '/config.php';
  if (!clicktale_is_config_writable($config)) {
    // This is done this way, as is_writable do not test if the file is writable
    // by the web-server and touch dies without return value.
    $msg = t('The PHP integration library config.php file (%path) is *not* writable.', array('%path' => $config));
    drupal_set_message($msg, 'error');
    return;
  }

  // The top and bottom javascript textares
  $form['clicktale'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tracking code (javascript)'),
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
    '#description' => t('Insert the tracking code in the fields below. The code can be obtained by going to clicktale.com. Select your project and go to the "Help" menu and click "Get Tracking Code in the sub-menu.'),
    '#tree' => TRUE,
  );

  // Load defaults values
  $defaults = variable_get('clicktale', array());

  // Build field set elements
  $form['clicktale']['top'] = array(
    '#type' => 'textarea',
    '#title' => t('Top code'),
    '#default_value' => isset($defaults['top']) ? $defaults['top'] : '',
    '#required' => TRUE,
  );

  $form['clicktale']['bottom'] = array(
    '#type' => 'textarea',
    '#title' => t('Bottom code'),
    '#rows' => 12,
    '#default_value' => isset($defaults['bottom']) ? $defaults['bottom'] : '',
    '#required' => TRUE,
  );

  // Load advanced settings defaults
  $defaults = variable_get('clicktale_advanced', array());

  $form['clicktale_advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced options'),
    '#description' => t('Advanced options can be used to finetune the Clicktale tracking.'),
    '#tree' => TRUE,
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
  );

  $form['clicktale_advanced']['method'] = array(
    '#type' => 'select',
    '#title' => t('Cache method'),
    '#default_value' => isset($defaults['method']) ? $defaults['method'] : array('filesystem' => 'filesystem'),
    '#options' => array(
        'FileSystem' => t('Filesystem'),
        'APC' => t('APC'),
      ),
    '#description' => t('Different cache metods can be used to save clicktale informations'),
  );

  $form['clicktale_advanced']['delete'] = array(
    '#type' => 'radios',
    '#title' => t('Delete cache after pull'),
    '#default_value' => isset($defaults['delete']) ? $defaults['delete'] :'yes',
    '#options' => array('yes' => t('Yes'), 'no' => t('No')),
    '#description' => t('If true, the cached page is deleted after clicktale fetches it from your site.'),
  );

  $form['clicktale_advanced']['size'] = array(
    '#type' => 'textfield',
    '#title' => t('Max folder size in Mb (filesystem only)'),
    '#default_value' => isset($defaults['size']) ? $defaults['size'] :'50',
    '#description' => t('The size of the cache dir in MegaBytes. If the cache gets larger than that, stale caches are removed.'),
  );

  $form['clicktale_advanced']['ttl'] = array(
    '#type' => 'textfield',
    '#title' => t('Time to live (APC only)'),
    '#default_value' => isset($defaults['ttl']) ? $defaults['ttl'] : '60',
    '#description' => t('Will only keep pages in cache for up to MaxCachedSeconds seconds. This will make sure that your cache storage will only take a small amount of memory.'),
  );

  $form['clicktale_advanced']['keylog'] = array(
    '#type' => 'radios',
    '#title' => t('Log keyboard'),
    '#default_value' => isset($defaults['keylog']) ? $defaults['keylog'] : 'no',
    '#options' => array('yes' => t('Yes'), 'no' => t('No')),
    '#description' => t('If true, the information enter in input fields will be logged (password are how ever <strong>never</strong> logged)'),
  );

  // Get the system settings form elements
  $form = system_settings_form($form);

  // Add extra submit handler (change PHP integration library settings)
  $form['#submit'][] = 'clicktale_admin_settings_form_submit';

  // Add validtor function
  $form['#validate'][] = 'clicktale_admin_settings_form_validate';

  return $form;
}

/**
 * Check that the PHP integration library config files exists.
 *
 * @param array $form
 * @param array $form_state
 */
function clicktale_admin_settings_form_validate($form, &$form_state) {
  // Check that the javascript code is set
  if ($form_state['values']['clicktale']['top'] && $form_state['values']['clicktale']['bottom']) {
    // Check that paths exists, if not create paths
    if (!clicktale_create_paths()) {
      form_set_error('clicktale', t('The clicktale folders could not be create in the files folder, so configuration have not been saved. Is the files folder writable!'));
    }
  }
}

/**
 * Form handler that updates clicktale PHP integration library configuration.
 *
 * @param array $form
 * @param array $form_state
 */
function clicktale_admin_settings_form_submit($form, &$form_state) {
  // Include the XML writer (first need on submit)
  include_once drupal_get_path('module', 'clicktale') . '/includes/clicktale.xml.inc';

  // Add extra linje to bottom script for cache callback
  $append_str = "\nClickTaleFetchFrom=\"%FetchFromUrl%\";";
  $bottom = $form_state['values']['clicktale']['bottom'];

  // Disable keylogging
  if ($form_state['values']['clicktale_advanced']['keylog'] == 'no') {
    $append_str .= "\nClickTaleEventsMask-=4;";
  }

  // Insert the append_str into the bottom code (there may be a better way)
  $search = '<script type="text/javascript">';
  $start_pos = strrpos($bottom, $search) + strlen($search);
  $bottom = substr($bottom, 0, $start_pos) . $append_str . substr($bottom, $start_pos);

  // Build XML file for PHP intergration module
  $xml = clicktale_xml_create($form_state['values']['clicktale']['top'], $bottom);

  // Write the xml
  clicktale_xml_write($xml);

  // Opdate PHP integration module config
  clicktale_configuration_update_php($form_state['values']['clicktale_advanced']);
}

/**
 * Build configuration form.
 *
 * @return array $form
 */
function clicktale_admin_configuration_form() {
  $form = array();

  // Enable
  $form['clicktale_configuration'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enable configuration'),
    '#options' => array('enable' => t('Enable')),
    '#description' => t('If checked the settings here will be used to limit the insertion of the tracking code.'),
    '#default_value' => variable_get('clicktale_configuration', array()),
  );

  // Rolles
  $default = variable_get('clicktale_roles', array());
  $form['clicktale_roles'] = array(
    '#type' => 'fieldset',
    '#title' => 'Roles',
    '#description' => t('Tracking by user roles (Only checked user roles will be tracked).'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  $form['clicktale_roles']['roles'] = array(
    '#type' => 'checkboxes',
    '#options' => user_roles(),
    '#default_value' => isset($default['roles']) ? $default['roles'] : array(1 => 1),
  );

  // Path (taken from the block module)
  $options = array(t('Insert on every page except the listed pages.'), t('Insert only on the listed pages.'));
  $default = variable_get('clicktale_paths', array());
  $form['clicktale_paths'] = array(
    '#type' => 'fieldset',
    '#title' => 'Paths',
    '#description' => t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );
  
  $form['clicktale_paths']['visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Insert tracking code on specific pages'),
    '#options' => $options,
    '#default_value' => isset($default['visibility']) ? $default['visibility'] : '0',
  );

  $form['clicktale_paths']['pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#default_value' => isset($default['pages']) ? $default['pages'] : '',
  );

  // Ip
  $options = array(t('Whitelist IP\'s'), t('Black list IP\'s'));
  $default = variable_get('clicktale_ip', array());
  $form['clicktale_ip'] = array(
    '#type' => 'fieldset',
    '#title' => 'IP adresses',
    '#description' => t('Filter tracking based on IP addresses.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  $form['clicktale_ip']['visibility'] = array(
    '#type' => 'radios',
    '#title' => t('IP\'s'),
    '#options' => $options,
    '#default_value' => isset($default['visibility']) ? $default['visibility'] : '1',
  );
  
  $form['clicktale_ip']['list'] = array(
    '#type' => 'textfield',
    '#title' => t('IP\'s'),
    '#default_value' => isset($default['list']) ? $default['list'] : '',
    '#description' => t('Enter a list of comma spereated IP\'s'),
  );

  // System setting look
  $form = system_settings_form($form);

  return $form;
}
