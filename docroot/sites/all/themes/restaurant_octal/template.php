<?php
/**
 * @file
 * Theme functions
 */

require_once dirname(__FILE__) . '/includes/structure.inc';
require_once dirname(__FILE__) . '/includes/comment.inc';
require_once dirname(__FILE__) . '/includes/form.inc';
require_once dirname(__FILE__) . '/includes/menu.inc';
require_once dirname(__FILE__) . '/includes/node.inc';
require_once dirname(__FILE__) . '/includes/panel.inc';
require_once dirname(__FILE__) . '/includes/user.inc';
require_once dirname(__FILE__) . '/includes/view.inc';

/**
 * Implements hook_css_alter().
 */
function restaurant_octal_css_alter(&$css) {
  $octal_path = drupal_get_path('theme', 'octal');

  // Octal includes compiled stylesheets for demo purposes.
  // We remove these from our subtheme since they are already included 
  // in compass_radix.
  unset($css[$octal_path . '/assets/stylesheets/octal-style.css']);
}

/**
 * Implements template_preprocess_page().
 */
function restaurant_octal_preprocess_page(&$variables) {
  // Add copyright to theme.
  if ($copyright = theme_get_setting('copyright')) {
    $variables['copyright'] = check_markup($copyright['value'], $copyright['format']);
  }
}

/**
 * Implements template_preprocess_maintenance_page().
 */
function restaurant_octal_preprocess_maintenance_page(&$variables) {
  global $install_state;

  if ($install_state) {
    // Find the number of tasks to run.
    $tasks = install_tasks_to_display($install_state);
    $total = sizeof($tasks);
    
    // Find the position of the active task.
    $keys = array_keys($tasks);
    $active_task = $install_state['active_task'];
    $current = array_search($active_task, $keys) + 1;
    
    // Show steps.
    $variables['steps'] = t('Step @current of @total', array(
      '@current' => $current,
      '@total' => $total,
    ));

    $variables['title'] = $tasks[$active_task];
  }

  $profile = isset($_GET['profile']) ? $_GET['profile'] : '';
  if ($profile) {
    $path = drupal_get_path('profile', $profile);
    $info_file = $path . '/' . $profile . '.info';
    $info = drupal_parse_info_file($info_file);

    $variables['site_name'] = $info['name'];
    $version['version'] = $info['version'];

    // Use copyright from distro info file.
    if (isset($info['copyright'])) {
      $variables['copyright'] = $info['copyright'];
    } else {
      $variables['copyright'] = st('@name @version', array(
        '@name' => $info['name'],
        '@version' => $info['version'],
      ));
    }

    // Quick fix to add the required radix-progress.js.
    // We assume that Radix is at profiles/*/themes/radix.
    // TODO: handle this better.
    drupal_add_js($path . '/themes/radix/assets/javascripts/radix-progress.js');
  }
}
