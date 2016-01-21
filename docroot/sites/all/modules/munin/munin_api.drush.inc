<?php

/**
 * @file
 * Contains the hook definitions necessary to enable drush function munin-run.
 */

/**
 * Implements hook_drush_command().
 */
function munin_api_drush_command() {
  $items = array();

  $items['munin-run'] = array(
    'callback' => 'munin_api_drush_callback',
    'arguments' => array(
      'plugin' => 'Run munin plugin.',
    ),
    'options' => array(
      'get-config' => 'Get munin plugin configuration options.',
    ),
    'description' => 'Run munin plugins.',
    'examples' => array(
      'drush munin-run plugin_name' => 'Run munin plugin.',
    ),
    'drupal dependencies' => array('munin_api'),
  );

  return $items;
}

/**
 *  Callback function that creats the response for munin to read and create the
 * graphs. This function is executed when drush gets munin-run argument (See
 * munin_api_drush_command).
 *
 * @param type $plugin_name
 *   The name of the called plugin. Can contain or not drupal_ prefix (it will
 * be detected and removed).
 */
function munin_api_drush_callback($plugin_name = NULL) {
  $definitions = _munin_plugin_definitions(TRUE);
  $plugins = array();

  if ($plugin_name) {
    // If $plugin_name starts with drupal_ prefix we trim it here. This is the
    // case for calls through drush as I didn't find an option yet to trim it in
    // the bash script.
    $plugin_name = preg_replace("/^(drupal_?)?([a-z0-9_\-])?/i", "$2", $plugin_name);

    if ($plugin_name) {
      $plugins = array($plugin_name);
    }
  }

  if (!count($plugins)) {
    $plugins = array_keys($definitions);
  }

  foreach ($plugins as $plugin_name) {
    if (!$definitions[$plugin_name]) {
      drush_print(dt('Plugin not available or not enabled.'));
    }
    else {
      // We are requested to print cofiguration settings for this plugin.
      if (drush_get_option('get-config', FALSE)) {
        _munin_api_get_config($definitions[$plugin_name], $plugin_name);
      }
      // We are requested to print the value for this plugin.
      else {
        _munin_api_get_values($definitions[$plugin_name], $plugin_name);
      }
    }
  }
}

function _munin_api_get_config($plugin, $plugin_name) {
  $config = $plugin['#config'];

  // It might not be set at all in which case we use the bellow defaults.
  !is_array($config) ? $config = array() : 0;

  // We set here some defaults.
  if (!$config['title']) {
    $config['title'] = $plugin['#title'];
  }
  if (!$config['vlabel']) {
    $config['vlabel'] = $plugin['#vlabel'];
  }
  if (!$config['category']) {
    $config['category'] = 'Drupal';
  }

  drush_print('multigraph ' . drupal_clean_css_identifier($plugin_name));

  // Every config set in #config array will overwrite the defaults.
  foreach ($config as $prop => $value) {
    drush_print('graph_' . $prop . ' ' . $value);
  }

  foreach ($plugin as $name => $props) {
    if (is_array($props)) {
      $config = $props['#config'];
      // We set here some defaults.
      if (!$config['type']) {
        $config['type'] = 'GAUGE';
      }
      if (!$config['label']) {
        $config['label'] = $props['#label'] ? $props['#label'] :
            $props['#title'];
      }

      // Every config set in #config array will overwrite the defaults.
      foreach ($config as $prop => $value) {
        drush_print($name . '.' . $prop . ' ' . $value);
      }
    }
  }

  drush_print("\n");
}

function _munin_api_get_values($definition, $plugin_name) {
  drush_print('multigraph ' . drupal_clean_css_identifier($plugin_name));
  foreach ($definition as $name => $plugin) {
    if (is_array($plugin)) {
      // Statement and sql are the same thing. We left both for backwards
      // compatibility.
      if ($plugin['#statement']) {
        $value = db_query($plugin['#statement'])->fetchField();
      }
      elseif ($plugin['#sql']) {
        $value = db_query($plugin['#sql'])->fetchField();
      }
      elseif ($plugin['#callback']) {
        // User function is called instead of sql command.
        if ($plugin['#file']) {
          // File property is set so we include it.
          $file = DRUPAL_ROOT . '/' . $plugin['#file'];
          if (is_file($plugin['#file'])) {
            require_once $file;
          }
        }

        if (function_exists($plugin['#callback'])) {
          try {
            // Call custom callback.
            $value = call_user_func_array($plugin['#callback'], $plugin['#callback_arguments']);
          } catch (Exception $e) {
            // Let's cach possible errors from callbacks.
            drush_log($e->getMessage(), 'error');
            $value = NULL;
          }
        }
      }

      // And we print everything to console.
      drush_print($name . '.value ' . $value);
    }
  }
  drush_print("\n");
}
