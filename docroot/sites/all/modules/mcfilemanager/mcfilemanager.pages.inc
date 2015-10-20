<?php
// $Id: mcfilemanager.pages.inc, v 0.1, 2009/05/04 20:13:57, skilip Exp $;

/**
 * @file
 *
 * Include file for MCFileManager.
 */

/**
 * Implements hook_form().
 */
function mcfilemanager_settings_form($form, &$form_state) {
  // Check the module requirements
  mcfilemanager_check_status();

  // Get the stored settings
  $config_options = variable_get("mcfilemanager_config_options", mcfilemanager_config_defaults());

  foreach ($config_options as $key => $default_value) {
    $form['options'][str_replace('.', '-', $key)] = array(
      '#type' => (is_bool($default_value) ? 'checkbox' : 'textfield'),
      '#title' => $key,
      '#default_value' => $default_value,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
  );

  return $form;
}

/**
 * Validation handler for mcfilemanager_settings_form
 */
function mcfilemanager_settings_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['filesystem-rootpath'])) {
    file_prepare_directory($form_state['values']['filesystem-rootpath'], FILE_CREATE_DIRECTORY);
  }
}

/**
 * Submit handler for mcfilemanager_settings_form
 */
function mcfilemanager_settings_form_submit($form, &$form_state) {

  // Check whether or not the settings must be reset
  if ($form_state['values']['op'] == t('Reset to defaults')) {
    drupal_goto('admin/config/mcfilemanager/reset');
  }

  // Replace stripes with dots
  foreach (element_children($form['options']) as $key) {
    $config[str_replace('-', '.', $key)] = $form_state['values'][$key];
  }
  variable_set("mcfilemanager_config_options", $config);

  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Reset form
 */
function mcfilemanager_settings_reset_form($form, &$form_state) {

  $form['reset'] = array(
    '#type' => 'value',
    '#value' => TRUE,
  );
  return confirm_form($form, t('Are you sure you want to reset the configuration options to their default values?'), 'admin/config/mcfilemanager', t('This cannot be undone.'), t('Reset'), t('Cancel'));
}

/**
 * Submit handler for the reset form
 */
function mcfilemanager_settings_reset_form_submit($form, &$form_state) {
  if ($form_state['values']['reset']) {
    variable_del('mcfilemanager_config_options');
    drupal_set_message(t('The configuration options have been reset to their default values.'));
    $form_state['redirect'] = 'admin/config/mcfilemanager';
  }
}

/**
 * Theme function for the settings form.
 */
function theme_mcfilemanager_settings_form($variables) {
  $form = $variables['form'];
  foreach (element_children($form['options']) as $child) {
    // Get the form element's title
    $title = $form['options'][$child]['#title'];
    unset($form['options'][$child]['#title']);

    $rows[] = array(
      $title,
      render($form['options'][$child]),
    );
  }

  return theme('table', array('header' => array(t('Option'), t('Value')), 'rows' => $rows)) . drupal_render_children($form);
}

/**
 * Helper function for mcfilemanager_requirements().
 * The script is placed in this include file to keep the .module file small and clean.
 */
function mcfilemanager_runtime_requirements() {
  // Ensure translations don't break at install time
  $t = get_t();
  $requirements = array();

  $library_path = mcfilemanager_library_directory();

  // Check if the file auth_drupal.php resides in Drupal's root directory
  if (!file_exists('auth_drupal.php')) {
    $requirements['mcfilemanager_auth_file'] = array(
      'title' => $t('MCFileManager authentication file'),
      'value' => $t('The required file %file could not be found! Refer to MCFileManager\'s !help section for installation instructions.', array('%file' => 'auth_drupal.php', '!help' => l(t('help'), 'admin/help/mcfilemanager'))),
      'severity' => REQUIREMENT_ERROR,
    );
  }

  // Only proceed if the libraries directory exists.
  if ($library_path) {

    // Iterate over each plugin to see if ity is properly configured.
    foreach (mcfilemanager_moxicode_libraries() as $short_name => $data) {

      variable_set('mcfilemanager_' . $short_name . '_status_ok', FALSE);

      // Define the replacements for the t() function.
      $replacements = array('!name' => $data->human_name);

      // Check if Moxicode's McImagemanager is present in this module's directory
      if (!is_dir("$library_path/$short_name") || !file_exists("$library_path/$short_name/config.php")) {
        $requirements[$short_name][$data->system_name . '_directory'] = array(
          'title' => $t('!name Directory', $replacements),
          'value' => $t('!library is missing', array('!library' => l($t('!name library', $replacements), "http://moxiecode.com/products_$short_name.php", array('attributes' => array('target' => '_blank'))))),
          'description' => $t('Moxiecode\'s !name library is not present in mcfilemanager\'s module directory.', $replacements),
          'severity' => REQUIREMENT_WARNING,
        );
      }
      else { // The library is present
        // Try to open config.php
        if ($file = file_get_contents("$library_path/$short_name/config.php")) {

          // Extract the configuration authenticator
          preg_match("/($data->conf_key\[\'authenticator\'\])([ =]{1,5})(\"|\')(.+?)(\"|\')(;)/i", $file, $matches);

          // Check if the authenticator is set to external
          if ($matches[4] !== 'ExternalAuthenticator') {
            $requirements[$data->system_name . '_config_authenticator'] = array(
              'title' => $t('!name Authenticator', $replacements),
              'value' => $t('config.php is not properly configured!'),
              'description' => $t('The authenticator set in !name\'s config.php should be \'ExternalAuthenticator\'.', $replacements),
              'severity' => REQUIREMENT_ERROR,
            );
          }

          // Store how many directories deep config.php resides, relative from Drupal's root directory
          $deep = substr_count("$library_path/imagemanager/config.php", '/');

          // Extract the configuration authenticator
          preg_match("/($data->conf_key\[\'ExternalAuthenticator.external_auth_url\'\])([ =]{1,5})(\"|\')(.+?)(\"|\')(;)/i", $file, $matches);
          $auth_drupal_path = str_repeat('../', $deep) . '../../auth_drupal.php';

          // Extract the configuration external authenticator is correctly configured
          if ($matches[4] !== $auth_drupal_path) {
            $requirements[$short_name][$data->system_name . '_config_external_auth_url'] = array(
              'title' => $t('!name external authenticator url', $replacements),
              'value' => $t('config.php is not properly configured!'),
              'description' => $t('The external authenticator\'s url set in !name\'s config.php should be \'!auth_path\'.', array('!auth_path' => $auth_drupal_path) + $replacements),
              'severity' => REQUIREMENT_ERROR,
            );
          }
        }
      }

      if (empty($requirements[$short_name])) {
        // Save the status to the DB, so we do not have to check this on each page load
        variable_set('mcfilemanager_' . $short_name . '_status_ok', TRUE);

        // No errors. Notify the user
        $requirements[$short_name][$data->system_name . '_ok'] = array(
          'title' => $data->human_name,
          'value' => $t('!name is properly installed', $replacements),
          'severity' => REQUIREMENT_OK,
        );
      }
      else {
        $requirements[$short_name][array_shift(array_keys($requirements[$short_name]))]['description'] .= ' ' . $t('Refer to !name\'s !help section for installation instructions.', array('!help' => l(t('help'), 'admin/help/mcfilemanager')) + $replacements);
      }
      $requirements += $requirements[$short_name];
      unset($requirements[$short_name]);
    }
  }
  else {
    $requirements['mcfilemanager_libraries'] = array(
      'title' => $t('Javascript libraries directory'),
      'value' => $t('Not present'),
      'severity' => $library_exists ? REQUIREMENT_OK : REQUIREMENT_ERROR,
    );
  }
  return $requirements;
}
