<?php

/**
 * @file
 * This file holds functions for handling sn_quick_multisite_action_form.
 */

// Including sn_quick_multisite.inc (to populate existing sites list etc.)
module_load_include('inc', 'sn_quick_multisite', 'sn_quick_multisite');

/**
 * Form builder; to create multisite.
 *
 * @ingroup forms
 */
function sn_quick_multisite_action_form($form, &$form_state) {

  $hints_text[] = t('Before creating new site, you should make a backup to restore; if unfortunately something go wrong.');

  $list = theme('item_list', array('items' => $hints_text));

  $form['list'] = array(
    '#prefix' => '<div class="messages warning" style="font-weight: bold;">',
    '#suffix' => '</div>',
    '#markup' => $list,
    '#type' => 'markup',
  );

  $form['site_values'] = array(
    '#type' => 'fieldset',
    '#title' => t('SiteName Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="hosts-div">',
    '#suffix' => '</div>',
  );

  $form['site_values']['sitename'] = array(
    '#type' => 'textfield',
    '#title' => t('Name of subdomain'),
    '#size' => 50,
    '#maxlength' => 20,
    '#required' => TRUE,
    '#description' => t('The name of subdomain, for multisite. For e.g. to create  site "abc.example.com" on domain example.com, enter "abc" only!!'),
  );

  $form['site_values']['is_prod'] = array(
    '#title' => t('Is this domain registered?'),
    '#type' => 'radios',
    '#maxlength' => 3,
    '#default_value' => 'no',
    '#options' => array(
      'yes' => t('Yes'),
      'no' => t('No')),
    '#ajax' => array(
      'callback' => 'sn_quick_multisite_is_prod_callback',
      'effect' => 'fade',
      'wrapper' => 'hosts-div',
      'method' => 'replace',
    ),
    '#description' => t("If your new site is a production site (i.e. domain name is already registered or mapped with some server's IP), then hosts settings are not requied. But if you are going to create a development site only, then you will require to know about the path of your server's hosts file and make sure it is writable."),
  );
  // Enable/disable host settings.
  if (!isset($form_state['values']['is_prod']) || $form_state['values']['is_prod'] == 'no') {
    $form['site_values']['$hostfile_values'] = array(
      '#type' => 'fieldset',
      '#title' => t('Hosts Settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#prefix' => '<div id="hosts-required-div">',
      '#suffix' => '</div>',
    );

    $form['site_values']['$hostfile_values']['is_host_made'] = array(
      '#type' => 'checkbox',
      '#title' => t('Configured host?'),
      '#required' => FALSE,
      '#ajax' => array(
        'callback' => 'sn_quick_multisite_hosts_required_callback',
        'effect' => 'fade',
        'wrapper' => 'hosts-required-div',
        'method' => 'replace',
      ),
      '#description' => t('If you have already made the hosts entry in hosts file or would like to do host entry your self, then below host file path is not required just choose this option.'),
    );
    // Check if, user has opted for doing hosts settings
    // on his own.
    if (!isset($form_state['values']['is_host_made']) || $form_state['values']['is_host_made'] == '0') {
      $form['site_values']['$hostfile_values']['hosts_filepath'] = array(
        '#type' => 'textfield',
        '#title' => t('Hosts file path'),
        '#size' => 100,
        '#required' => TRUE,
        '#description' => t("Enter the host file location. For e.g. usually on Linux severs we have a hosts file located at '/etc/hosts' location."),
      );
    }
    elseif (isset($form_state['values']['is_host_made']) && $form_state['values']['is_host_made'] == '1') {
      unset($form['site_values']['$hostfile_values']['hosts_filepath']);
    }
  }
  elseif (isset($form_state['values']['is_prod']) && $form_state['values']['is_prod'] == 'yes') {
    unset($form['site_values']['$hostfile_values']);
  }

  $form['cloning_values'] = array(
    '#type' => 'fieldset',
    '#title' => t('Site Cloning'),
    '#description' => t('Do you want to clone an existing site?'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="replace-div">',
    '#suffix' => '</div>',
  );

  $form['cloning_values']['cloning_options'] = array(
    '#type' => 'radios',
    '#maxlength' => 3,
    '#default_value' => 'no',
    '#options' => array(
      'yes' => t('Yes'),
      'no' => t('No')),
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'sn_quick_multisite_replace_callback',
      'wrapper' => 'replace-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  // Populate existing sites, if user opted for cloning.
  if (isset($form_state['values']['cloning_options']) && $form_state['values']['cloning_options'] == 'yes') {
    $form['cloning_values']['site_list'] = array(
      '#title' => t("Sites List"),
      '#type' => 'select',
      '#options' => sn_quick_multisite_get_sites_list(),
      '#description' => t('This is where we get the existing sites'),
    );
  }
  elseif (isset($form_state['values']['cloning_options']) && $form_state['values']['cloning_options'] == 'no') {
    unset($form['cloning_values']['site_list']);
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save and Continue'),
  );
  return $form;
}

/**
 * Form validation handler for sn_quick_multisite_action_form .
 */
function sn_quick_multisite_action_form_validate($form, &$form_state) {

  // Validating sitename.
  sn_quick_multisite_form_validate_sitename($form, $form_state);
  // Validating if hosts file is writable or not.
  // But only if, user has given hosts file path
  // and want system to do hosts entry.
  if (isset($form_state['values']['is_prod']) && $form_state['values']['is_prod'] == 'no' && $form_state['values']['is_host_made'] == '0') {
    sn_quick_multisite_form_hostfilevalidate($form, $form_state);
  }

  // Some more required validation checks.
  // Checking if drupal sites directory is writabe or not.
  $sites_dir_path = sn_quick_multisite_sites_folder_path();
  if (!is_writable($sites_dir_path)) {
    form_set_error(t('Directory " %sites_dir_path" is not writable, please make it writable to proceed.', array('%sites_dir_path' => $sites_dir_path)));
  }
}

/**
 * Form submission handler for sn_quick_multisite_action_form .
 *
 * @param array $form
 *   Form values in array format.
 * @param array $form_state
 *   Form State in array format.
 */
function sn_quick_multisite_action_form_submit($form, &$form_state) {

  $sitename = $form_state['values']['sitename'];
  // Site directory name e.g.,for site name 'testsite' on example.com,
  // It will be - testsite.example.com.
  $host_details = sn_quick_multisite_get_host_details();
  $site_dir = $sitename . '.' . $host_details['domain_name'];
  $is_clone = $form_state['values']['cloning_options'];
  $clone_site = ($is_clone == 'yes' && isset($form_state['values']['site_list'])) ? $form_state['values']['site_list'] : FALSE;
  $is_prod = ($form_state['values']['is_prod'] == 'yes') ? TRUE : FALSE;
  $is_host_made = ($is_prod && $form_state['values']['is_host_made'] == '1') ? TRUE : FALSE;
  $hosts_filepath = (!$is_host_made && isset($form_state['values']['hosts_filepath'])) ? $form_state['values']['hosts_filepath'] : FALSE;
  // Preparing batch.
  // Will execute all the methods in batch to avoid PHP timeout.
  sn_quick_multisite_prepare_batch($site_dir, $sitename, $is_clone, $hosts_filepath, $clone_site);
}

/**
 * Function to enable dropdown on select of radio buttons.
 * 
 * If user has opted for clone an existing site,
 * then populate the exisiting sites
 *
 * @param array $form
 *   Form values in array format.
 * @param array $form_state
 *   Form State in array format.
 *
 * @return array
 *   $form['cloning_values'] option to render dropdown.
 */
function sn_quick_multisite_replace_callback($form, &$form_state) {
  return $form['cloning_values'];
}

/**
 * To enable/disable Hosts settings.
 * 
 * If user has selected the option of production site
 * then hide host settings ELSE enable them
 *
 * @param array $form
 *   Form values in array format.
 * @param array $form_state
 *   Form State in array format.
 *
 * @return array
 *   $form['site_values']  to show/hide hosts settings.
 */
function sn_quick_multisite_is_prod_callback($form, &$form_state) {
  return $form['site_values'];
}

/**
 * To enable/disable hosts file path input field.
 * 
 * If user want to do hosts seetings on his own
 * or he has made hosts settings his own
 * then, he may or may not opt to provide host file path
 * 
 * @param array $form
 *   Form values in array format.
 * @param array $form_state
 *   Form State in array format.
 *
 * @return array
 *   $form['site_values']['$hostfile_values'] to show/hide 
 *   hosts input field.
 */
function sn_quick_multisite_hosts_required_callback($form, &$form_state) {
  return $form['site_values']['$hostfile_values'];
}

/**
 * Function to validate the entered sitename for valid sitename.
 *
 * @param array $form
 *   Form values in array format.
 * @param array $form_state
 *   Form State in array format.
 */
function sn_quick_multisite_form_validate_sitename($form, &$form_state) {

  $sitename = $form_state['values']['sitename'];
  if (preg_match('/[^a-z0-9A-Z]/', $sitename)) {
    form_set_error('sitename', t('Spaces and special characters are not allowed in sitename.'));
  }
  elseif (!empty($sitename)) {
    $host_details = sn_quick_multisite_get_host_details();
    $site_dir_name = $sitename . '.' . $host_details['domain_name'];
    $drupal_site_folder_path = sn_quick_multisite_sites_folder_path();
    $new_site_path = $drupal_site_folder_path . $site_dir_name;

    if (is_dir($new_site_path)) {
      form_set_error('sitename', t('"%sitename" subdomain site already exists.', array('%sitename' => $sitename)));
    }
  }
}

/**
 * Function to validate Host File to check whether it is Writable/Read-only.
 *
 * @param array $form
 *   Form values in array format.
 * @param array $form_state
 *   Form State in array format.
 */
function sn_quick_multisite_form_hostfilevalidate($form, &$form_state) {
  $hosts_filepath = $form_state['values']['hosts_filepath'];
  if (!is_file($hosts_filepath)) {
    form_set_error('hosts_filepath', t('File "%hosts_filepath" does not exists or not a file.', array('%hosts_filepath' => $hosts_filepath)));
  }
  elseif (!is_writable($hosts_filepath)) {
    form_set_error('hosts_filepath', t('File "%hosts_filepath" is read only, please make it writable', array('%hosts_filepath' => $hosts_filepath)));
  }
}
