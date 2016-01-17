<?php
/**
 * @file
 * Administrative page callbacks for the Mobile Detect Import module.
 */

/**
 * Implements hook_form().
 */
function mobile_detect_import_settings_form($form, &$form_state) {
  // Check the local Mobile Detect class version number
  $version = variable_get('mobile_detect_import_version', 0);

  // Current local Mobile Detect class version.
  $library['version'] = FALSE;
  $library_path = libraries_get_path(MOBILE_SWITCH_LIBRARY_NAME, $base_path = FALSE);
  $library_uri = $library_path . '/' . MOBILE_SWITCH_LIBRARY_FILE_NAME;

  if ($library_path) {
    $library['version'] = mobile_switch_mobile_detect_get_version($library_uri);
  }

  $readme_link = 'README.txt';
  if (module_exists('help')) {
    $readme_link = l('README.txt', 'admin/help/mobile_detect_import');
  }

  $note_style = '';
  $install_note = '';
  if ($version > $library['version']) {
    $note_style = 'color: maroon';
    $install_note = '<div class="description">' . t('To install the new version see !readme, section HOW WORKS, for installation instructions.', array('!readme' => $readme_link)) . '</div>';
  }

  $form['data'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mobile Detect settings'),
  );
  $form['data']['mobile_detect_current_version'] = array(
    '#markup' => '<p>' . t('Currently used Mobile Detect version: %fileversion', array('%fileversion' => $library['version'])) . '</p>',
  );

  if ($version > $library['version']) {
    $form['data']['mobile_detect_import_version'] = array(
      '#markup' => '<p><div style="' . $note_style . '">' . t('Available is the new Mobile Detect version: %fileversion', array('%fileversion' => $version)) . '</div>' . $install_note . '</p>',
    );
  }
  // If the version number is 0 then Mobile Detect class ha never been fetched.
  if ($version == 0) {
    $form['data']['mobile_detect_import_version'] = array(
      '#markup' => '<p><div style="' . $note_style . '">' . t('New Mobile Detect version: %fileversion', array('%fileversion' => t('Never fetched'))) . '</div>' . $install_note . '</p>',
    );
  }
  $form['data']['mobile_detect_import_enable_automatic_updates'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable automatic version check'),
    '#default_value' => variable_get('mobile_detect_import_enable_automatic_updates', FALSE),
    '#description' => t('Automatically checks new versions of the Mobile Detect class.'),
  );
  $form['data']['mobile_detect_import_automatic_updates_timer'] = array(
    '#type' => 'select',
    '#title' => t('Check for new Mobile Detect version every'),
    '#default_value' => variable_get('mobile_detect_import_automatic_updates_timer', 604800),
    '#options' => drupal_map_assoc(array(86400, 172800, 259200, 604800, 1209600, 2419200, 4838400, 9676800), 'format_interval'),
    '#description' => t('Newer Mobile Detect class will be automatically downloaded and stored in the files directory. (Requires a correctly configured <a href="@cron">cron maintenance task</a>.)', array('@cron' => url('admin/reports/status'))),
    '#states' => array(
      'visible' => array(
        ':input[name="mobile_detect_import_enable_automatic_updates"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['actions']['mobile_detect_import_refresh'] = array(
    '#type' => 'submit',
    '#value' => t('Refresh Mobile Detect'),
    '#submit' => array('mobile_detect_import_refresh_submit'),
    '#weight' => 10,
  );

  return system_settings_form($form);
}

/**
 * Submit handler for the refresh Mobile Detect button.
 */
function mobile_detect_import_refresh_submit($form, &$form_state) {
  // Update the Mobile Detect class file.
  _mobile_detect_import_import(FALSE);

  // Record when the Mobile Detect class file was updated.
  variable_set('mobile_detect_import_imported', REQUEST_TIME);
}
