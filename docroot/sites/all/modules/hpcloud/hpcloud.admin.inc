<?php
/**
 * @file
 * Part of the hpcloud package.
 */


/**
 * Settings admin form.
 */
function hpcloud_admin_settings() {
  $form = array();

  $manage_links = array(
    '!manage' => l('Console', 'https://console.hpcloud.com', array('absolute' => TRUE)),
  );

  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('Account Information'),
    '#description' => t('Configure Drupal to connect to your HPCloud account. You must supply an endpoint URL, a tenant ID, and either account/secret or login/password. All of this information can be found on the !manage.', $manage_links),
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
  );

  $form['account']['hpcloud_account_endpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Endpoint URL'),
    '#description' => t("The URL to HPCloud's Identity Services endpoint. Find it at the !manage.", $manage_links),
    '#size' => 60,
    '#maxlength' => 128,
    '#default_value' => variable_get('hpcloud_account_endpoint', 'https://region-a.geo-1.identity.hpcloudsvc.com:35357/v2.0/'),
    '#required' => TRUE,
  );

  // Tenant ID.
  $form['account']['hpcloud_account_tenantid'] = array(
    '#type' => 'textfield',
    '#title' => t('Tenant ID'),
    '#description' => t('The alphanumeric Tenant ID number assigned by HPCloud. Find it at the !manage.', $manage_links),
    '#size' => 60,
    '#maxlength' => 128,
    '#default_value' => variable_get('hpcloud_account_tenantid', ''),
    '#required' => TRUE,
  );

  // Tenant Name.
  $form['account']['hpcloud_account_tenantname'] = array(
    '#type' => 'textfield',
    '#title' => t('Tenant Name'),
    '#description' => t('The Tenant Name assigned by HPCloud. Find it at the !manage.', $manage_links),
    '#size' => 60,
    '#maxlength' => 128,
    '#default_value' => variable_get('hpcloud_account_tenantname', ''),
    '#required' => TRUE,
  );
  $form['account']['identifier'] = array(
    '#type' => 'fieldset',
    '#title' => t('API keys'),
    '#description' => t('Option 1: Supply API keys to log in. You can find your API keys in the !manage.', $manage_links),
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
  );

  $form['account']['identifier']['hpcloud_account_account'] = array(
    '#type' => 'textfield',
    '#title' => t('Access Key'),
    '#description' => t('The alphanumeric account ID issued by HPCloud.'),
    '#size' => 60,
    '#maxlength' => 128,
    '#default_value' => variable_get('hpcloud_account_account', ''),
  );


  $form['account']['identifier']['hpcloud_account_secret'] = array(
    '#type' => 'password',
    '#title' => t('Secret Key'),
    '#description' => t('The secret key matched with the account ID. This is used to authenticate.'),
    '#size' => 60,
    '#maxlength' => 128,
    '#default_value' => variable_get('hpcloud_account_secret', ''),
  );
  $form['account']['login'] = array(
    '#type' => 'fieldset',
    '#title' => t('Login Information'),
    '#description' => t('Option 2: Use your login and password. Typically this is the account you use to log in to the !manage.', $manage_links),
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
  );

  $form['account']['login']['hpcloud_account_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#description' => t('The account username.'),
    '#size' => 60,
    '#maxlength' => 128,
    '#default_value' => variable_get('hpcloud_account_username', ''),
  );
  $form['account']['login']['password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#description' => t('The account password for the given username.'),
    '#size' => 60,
    '#maxlength' => 128,
    '#default_value' => variable_get('hpcloud_account_password', ''),
  );

  // Advanced
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
  );

  $options = array(
    'phpstream' => "PHP Stream Wrapper",
  );
  //  'curl' => "CURL library (requires libcurl support)",
  if (function_exists('curl_init')) {
    $options['curl'] = "CURL library";
  }
  else {
    drupal_set_message('CURL is not installed.');
    //$options[''] = "CURL library (Unavailable)";
  }


  $form['advanced']['hpcloud_transport_class'] = array(
    '#type' => 'select',
    '#title' => t('HTTP Transport Layer'),
    '#description' => t('HP Cloud can use different HTTP transports. If you can, use CURL. Otherwise, use PHP\'s built-in HTTP support.'),
    '#options' => $options,
    '#default_value' => variable_get('hpcloud_transport_class', 'phpstream'),
  );

  return system_settings_form($form);
}

function hpcloud_admin_settings_validate(&$form, &$form_state) {
  $endpoint = $form_state['values']['hpcloud_account_endpoint'];
  $parts = parse_url($endpoint);
  if (!empty($parts['path'])) {
    $new_url = $parts['scheme'] . '://' . $parts['host'];
    if (!empty($parts['port'])) {
      $new_url .= ':' . $parts['port'];
    }
    $form_state['values']['hpcloud_account_endpoint'] = $new_url;
    drupal_set_message(t('The endpoint URL has been normalized to %url', array('%url' => $new_url)), 'status');
  }

  if (!empty($form_state['values']['hpcloud_account_account'])
     && empty($form_state['values']['hpcloud_account_secret'])) {

    form_set_error('account][identifier][hpcloud_account_secret', 'Secret key is required.');
  }
}

/**
 * Select which stream wrappers to override.
 */
function hpcloud_admin_wrapper_form($form, &$form_state) {
  $form = array();
  $scheme_map = variable_get('hpcloud_enabled_schemes', array());

  $containers = _hpcloud_list_containers();
  $options = array('' => '-- No Container --');
  foreach ($containers as $name => $container) {
    $label = $name;
    if ($container->acl()->isPublic()) {
      $label .= ' (Public)';
    }
    $options[$name] = $label;
  }

  $default_compatibility = t('unknown');
  $compatibility = array(
    'public' => 'compatible',
    'private' => 'compatible',
    'temporary' => 'compatible, but not recommended',
    'hpcloud' => 'compatible',
  );

  $wrappers = module_invoke_all('stream_wrappers');

  $form['containers'] = array(
    '#type' => 'value',
    '#value' => array_keys($containers),
  );

  foreach ($wrappers as $name => $info) {
    $note = isset($compatibility[$name]) ? $compatibility[$name] : $default_compatibility;
    $default = '';
    if (!empty($scheme_map[$name])) {
      $default = $scheme_map[$name];
    }

    $form['wrapper_' . $name ] = array(
      '#type' => 'fieldset',
      '#title' => $info['name'],
      '#description' => $info['description'],
      '#collapsed' => FALSE,
      '#collapsible' => FALSE,
    );

    $form['wrapper_' . $name]['compatibility'] = array(
      '#markup' => '<p><b>' . t('Compatibility: ') . '</b>' . $note . '</p>',
    );

    $form['wrapper_' . $name]['stream_' . $name] = array(
      '#type' => 'select',
      '#title' => t('Map to Container'),
      '#description' => t('Link this stream wrapper to a container on your HPCloud object storage. This will store files in the object storage.'),
      '#options' => $options,
      '#default_value' => $default,
    );
  }


  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Submit handler for wrapper form.
 */
function hpcloud_admin_wrapper_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $containers = $values['containers'];
  $wrappers = module_invoke_all('stream_wrappers');
  $mapping = array();

  $scheme_map = array();
  foreach ($wrappers as $name => $info) {
    $key = 'stream_' . $name;
    if (!empty($values[$key]) && in_array($values[$key], $containers)) {
      $msg = t('Mapping "@name://" to the %cont container.', array('@name' => $name, '%cont' => $values[$key]));
      drupal_set_message($msg, 'status');
      $scheme_map[$name] = $values[$key];
    }
  }

  variable_set('hpcloud_enabled_schemes', $scheme_map);
}
/**
 * Test authentication.
 */
function hpcloud_admin_test_form($form_state) {
  $form = array();

  $form['do_test'] = array(
    '#type' => 'submit',
    '#value' => t('Test Credentials'),
  );

  return $form;
}

/**
 * Submit handler for test form.
 */
function hpcloud_admin_test_form_submit(&$form_state, $form) {
  $id = _hpcloud_login(FALSE);
  if (!empty($id)) {
    drupal_set_message('Authentication was successful.', 'status');
  }
  else {
    drupal_set_message('Authentication to HPCloud failed.', 'error');
  }
}

/**
 * List the containers for object storage.
 *
 * This used to be a general purpose page, but is now just
 * for object storage.
 */
function hpcloud_admin_service_details_page() {
  $page = array(
    'admin_service_details_page' => array(
      '#markup' => theme_table(hpcloud_admin_container_list()),
    ),
  );
  return $page;
}

function hpcloud_admin_container_list() {
  $containers = _hpcloud_list_containers();

  $table = array(
    '#theme' => 'table',
    'header' => array('Name', 'Container URL', 'Size in Bytes', '# of Objects','Actions'),
    'caption' => NULL,
    'rows' => array(),
    'colgroups' => array(),
    'sticky' => FALSE,
    'empty' => t('No service endpoints. Check your credentials and tenant ID.'),
    'attributes' => array(),
  );
  foreach ($containers as $container) {
    $table['rows'][] = array( 'data' => array(
      array('data' => check_plain($container->name())),
      array('data' => check_plain($container->url())),
      array('data' => _hpcloud_smart_bytes((int) $container->bytes())),
      array('data' => check_plain($container->count())),
      array('data' => l('view details', 'admin/config/services/hpcloud-object/container/' . $container->name())),
    ));
  }

  return $table;
}

function hpcloud_admin_container_details_page($name) {
  try {
    $container = _hpcloud_object_storage()->container($name);
  } catch (\HPCloud\Exception $e) {
    watchdog_exception('HP Cloud', $e);
    drupal_set_message('Object storage is not activated for the project or the container cannot be found.', 'error');
    drupal_goto('admin/config/services/hpcloud-object/containers');
    exit;
  }

  if (strlen($container->name()) == 0) {
    drupal_set_message('The container cannot be found.', 'error');
    drupal_goto('admin/config/services/hpcloud-object/containers');
    exit;
  }

  drupal_set_title($container->name());
  $table = array(
    '#theme' => 'table',
    'header' => array(),
    'caption' => NULL,
    'rows' => array(
      array('data' => array(
        array('data' => t('Name')),
        array('data' => check_plain($container->name())),
      )),
      array('data' => array(
        array('data' => t('Storage Space')),
        array('data' => _hpcloud_smart_bytes($container->bytes())),
      )),
      array('data' => array(
        array('data' => t('Metadata')),
        array('data' => check_plain(json_encode($container->metadata()))),
      )),
      array('data' => array(
        array('data' => t('Number of Objects')),
        array('data' => check_plain($container->count())),
      )),
      array('data' => array(
        array('data' => t('Container URL')),
        array('data' => check_plain($container->url())),
      )),
      array('data' => array(
        array('data' => t('CDN URL')),
        array('data' => check_plain($container->cdnUrl(FALSE))),
      )),
      array('data' => array(
        array('data' => t('CDN SSL URL')),
        array('data' => check_plain($container->cdnUrl(TRUE))),
      )),
      array('data' => array(
        array('data' => t('Access Category')),
        array('data' => $container->acl()->isPublic() ? t('Public') : t('Private')),
      )),
      array('data' => array(
        array('data' => t('Access Control List')),
        array('data' => check_plain((string) $container->acl())),
      )),
    ),
    'colgroups' => array(),
    'sticky' => FALSE,
    'empty' => t('No service endpoints. Check your credentials and tenant ID.'),
    'attributes' => array(),
  );
  $page = array(
    'admin_service_details_page' => array(
      '#markup' => theme_table($table),
    ),
  );
  return $page;
}
/**
 * Given a number in bytes, try to make a pretty number.
 */
function _hpcloud_smart_bytes($bytes) {
  if ($bytes > 1073741824) {
    return sprintf('%d G', $bytes / 1073741824);
    // GiB
  }
  elseif ($bytes > 1048576) {
    return sprintf('%d M', $bytes / 1048576);
    // MiB
  }
  elseif ($bytes > 1024) {
    // KiB
    return sprintf('%d K', $bytes / 1024);
  }
  return $bytes . ' B';

}

/**
 * Display the service catalog.
 */
function hpcloud_admin_service_catalog_page() {
  $tables = array();
  $id = _hpcloud_login(FALSE);

  if (empty($id)) {
    drupal_set_message('Login to HP Cloud failed.', 'error');
    drupal_goto('admin/config/services/hpcloud');
    exit;

  }
  $table = array(
    '#theme' => 'table',
    'header' => array(),
    'caption' => NULL,
    'rows' => array(),
    'colgroups' => array(),
    'sticky' => FALSE,
    'empty' => t('No service endpoints. Check your credentials and tenant ID.'),
    'attributes' => array(),
  );

  $catalog = $id->serviceCatalog();
  if (!empty($catalog)) {
    foreach ($catalog as $service) {
      hpcloud_admin_service_catalog_item_table($service, $table);
    }
  }

  $page = array(
    'admin_service_catalog_page' => array(
      // Why does this not work? (I am not a themer.)
      //'#markup' => theme('table', $table),
      '#markup' => theme_table($table),
    ),
  );
  return $page;
}

/**
 * Build a table data structure for a service catalog item.
 */
function hpcloud_admin_service_catalog_item_table($item, &$table) {
  $table['rows'][] = array(array(
    'data' => t('%name (%type)', array('%name' => $item['name'], '%type' => $item['type'])),
    'colspan' => 2,
    'header' => TRUE,
  ));

  foreach ($item['endpoints'] as $endpoint) {
    foreach ($endpoint as $header => $value) {
      $row = array('data' => array(
        array('data' => check_plain($header)),
        array('data' => check_plain($value)),
      ));
      $table['rows'][] = $row;
    }
  }
}
