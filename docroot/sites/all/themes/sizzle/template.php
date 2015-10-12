<?php
/**
 * @file
 * Theme functions
 */

require_once dirname(__FILE__) . '/includes/structure.inc';
require_once dirname(__FILE__) . '/includes/form.inc';
require_once dirname(__FILE__) . '/includes/menu.inc';
require_once dirname(__FILE__) . '/includes/comment.inc';
require_once dirname(__FILE__) . '/includes/node.inc';

/**
 * Implements hook_css_alter().
 */
function sizzle_css_alter(&$css) {
  $radix_path = drupal_get_path('theme', 'radix');

  // Radix now includes compiled stylesheets for demo purposes.
  // We remove these from our subtheme since they are already included 
  // in compass_radix.
  unset($css[$radix_path . '/assets/stylesheets/radix-style.css']);
  unset($css[$radix_path . '/assets/stylesheets/radix-print.css']);
}

/**
 * Implements template_preprocess_page().
 */
function sizzle_preprocess_page(&$variables) {
  // Add copyright to theme.
  if ($copyright = theme_get_setting('copyright')) {
    $variables['copyright'] = check_markup($copyright['value'], $copyright['format']);
  }
}

/**
 * Implements template_preprocess_maintenance_page().
 */
function sizzle_preprocess_maintenance_page(&$variables) {
  global $install_state;

  if ($install_state) {
    $variables['copyright'] = st('Drupal is a !trademark of !buytaert', array(
      '!trademark' => l(st('registered trademark'), 'http://drupal.org/trademark'),
      '!buytaert' => l(st('Dries Buytaert'), 'http://buytaert.net'),
    ));

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

    // Add some icons.
    $search = array('"done">', '"active">', '<li>');
    $replace = array('"done"><i class="icon-ok"></i>','"active"><i class="icon-check-empty"></i>', '<li><i class="icon-check-empty"></i>');
    $variables['sidebar_first'] = str_replace($search, $replace, $variables['sidebar_first']);
  }
}
