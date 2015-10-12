<?php
/**
 * @file
 * Define admin settings form
 */

/**
 * Implements module settings.
 *
 * Includes general settings and those specified in the selected driver.
 */
function ilsauthen_admin_settings() {

  // Get any requirement info specified by current driver.
  $driver = variable_get('ilsauthen_driver', 'sample');
  module_load_include('inc', 'ilsauthen', '/drivers/' . $driver);
  $driver_meta = ilsauthen_driver_meta();
  if (array_key_exists('driver_requirements', $driver_meta)) {
    $path = realpath(".") . '/'  . drupal_get_path('module', 'ilsauthen') . '/' . 'drivers/';
    $name = $driver_meta['driver_requirements']['php_class_filename'];
    $type = $driver_meta['driver_requirements']['php_class_filetype'];
    $url = $driver_meta['driver_requirements']['php_class_download_url'];
    $name_type = $name . '.' . $type;
    $absolute = $path . $name_type;
    if (!file_exists($absolute)) {
      drupal_set_message(t('%name_type is not installed. You can download it <a href="@url">here</a>. Save %name_type in %path', array(
        '@url' => url($url),
        '%name_type' => $name_type,
        '%path' => $path,
      )), 'error', TRUE);
    }
  }

  $form['ilsauthen_enable_logging'] = array(
    '#type' => 'checkbox',
    '#title' => t('Additional logging'),
    '#description' => t("Adds more detailed logging of ILS authentication's account management activities.  Disabled (default) logs erros only."),
    '#default_value' => variable_get('ilsauthen_enable_logging', 0),
    '#weight' => -20,
  );

  $enable_logging = variable_get('ilsauthen_enable_logging', 0);
  if ($enable_logging) {
    $form['logging_warning'] = array(
      '#prefix' => '<div class="messages warning">',
      '#markup' => t('Warning: UIDs for ILS Authentication accounts are being logged. Turn off <em>Enable external account logging</em>, above, unless you are testing a driver.'),
      '#suffix' => '</div>',
      '#weight' => -20,
    );
  }

  $form['ilsauthen_show_no_mail_warning'] = array(
    '#type' => 'checkbox',
    '#title' => t("Warn user if they haven't set an email address"),
    '#description' => t("Drupal requires users to register an email addresse in their profile. If a driver is unable to get the user's email address from the external authentication source, enabling this option will display a warning until the user adds their email address to their profile."),
    '#default_value' => variable_get('ilsauthen_show_no_mail_warning', 1),
    '#weight' => -20,
  );

  $form['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default roles for external accounts'),
    '#weight' => -15,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $roles = user_roles(TRUE);
  // Remove 'authenticated user' (rid 2), which is always the second element in
  // the roles array. We are only left with roles created by the site
  // administrator. We do this since Drupal will always add a new user to the
  // authenticated user role.
  unset($roles[2]);
  $form['roles']['ilsauthen_default_roles'] = array(
    '#title' => t('Default roles'),
    '#type' => 'checkboxes',
    '#options' => $roles,
    '#default_value' => variable_get('ilsauthen_default_roles', array()),
    '#description' => t('The roles that accounts created using external authentication are placed in. Does not apply to local Drupal accounts.'),
  );

  $drivers = ilsauthen_get_drivers();
  $form['ilsauthen_driver'] = array(
    '#type' => 'select',
    '#title' => t('Driver'),
    '#default_value' => variable_get('ilsauthen_driver', 'sample'),
    '#options' => $drivers,
    '#description' => t('Driver-specific settings will appear below after you have saved this form. Settings for drivers that are not selected are retained.'),
    '#weight' => -12,
  );

  return system_settings_form($form);
}

/**
 * Get list of all drivers.
 */
function ilsauthen_get_drivers() {
  $drivers = array();
  $module_dir = drupal_get_path('module', 'ilsauthen');
  // Ignore *.class.inc.
  $file_options = array('nomask' => '/(\.\.?|CVS|\.class\.inc)$/');
  $driver_files = file_scan_directory($module_dir . '/drivers', '/.*\.inc$/', $file_options);

  foreach ($driver_files as $driver) {
    $drivers[$driver->name] = $driver->name;
  }
  return $drivers;
}
