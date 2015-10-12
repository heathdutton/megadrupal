<?php
/**
 * @file
 * Pages for the Adminer module.
 */

/**
 * Returns Adminer main page.
 */
function adminer_main_page() {
  if (!adminer_check_components()) {
    return '';
  }
  $options = array();
  if (user_access('use adminer without login')) {
    $db = Database::getConnectionInfo();
    $options['query'] = array(
      'username' => '',
      'db' => $db['default']['database'],
    );
  }

  return theme('adminer', array('src' => url('admin/config/development/adminer/callback', $options)));
}

/**
 * Check all required components.
 */
function adminer_check_components() {
  $params = array('!settings' => l(t('settings'), 'admin/config/development/adminer/settings'));

  //Check for adminer-*.php file.
  $php = adminer_get_php();
  if (!file_exists($php)) {
    drupal_set_message(t('Adminer not installed. Please check !settings.', $params), 'error');
    return FALSE;
  }

  // Check for all required plugins.
  $all_required_plugins = TRUE;
  $plugins = adminer_get_plugins();
  foreach (adminer_get_all_plugins() as $name => $plugin) {
    if (isset($plugin['required']) && $plugin['required'] && !isset($plugins[$name])) {
      $all_required_plugins = FALSE;
    }
  }
  if (!$all_required_plugins) {
    drupal_set_message(t('Required plugin(s) not installed. Please check !settings.', $params), 'error');
    return FALSE;
  }

  return TRUE;
}

/**
 * Page callback for the Adminer iframe.
 */
function adminer_get_adminer() {
  include DRUPAL_ROOT . '/' . adminer_get_php();
  drupal_exit();
}
