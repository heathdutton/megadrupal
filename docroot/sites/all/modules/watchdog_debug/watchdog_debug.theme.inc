<?php

function theme_watchdog_debug_function($variables) {
  $output = '<span class="function">' . $variables['trace']['function'] . '</span> (';
  if (!empty($variables['trace']['args'])) {
    $output .= ' <span class="args">' . $variables['trace']['args'] . '</span>';
  }
  $output .= ' )';
  return $output;
}

function theme_watchdog_debug_location($variables) {
  $location = array();
  if (!empty($variables['trace']['file'])) {
    $path = str_replace(realpath('.'), '', $variables['trace']['file']);
  }
  if (!empty($path)) {
    $path = explode('/', $path);
    $path[] = '<span class="path-last">' . array_pop($path) . '</span>';
    $location[] = '<span class="path">' . implode('/', $path) . '</span>';
  }
  if (!empty($variables['trace']['line'])) {
    $location[] = '<span class="line">' . $variables['trace']['line'] . '</span>';
  }
  return implode(' : ', $location);
}

/**
 *
 */
function watchdog_debug_watchdog_message($variables) {
  drupal_add_css(drupal_get_path('module', 'watchdog_debug') . '/watchdog_debug.css');
  $output = '';
  $event = $variables['event'];
  // Check for required properties.
  if (isset($event->message) && isset($event->variables)) {
    // Messages without variables or user specified text.
    if ($event->variables === 'N;') {
      $output = $event->message;
    }
    // Message to translate with injected variables.
    else {
      $output = t($event->message, unserialize($event->variables));
    }
    if ($variables['link'] && isset($event->wid)) {
      $output = str_replace(realpath('.'), '', $output);
      // Truncate message to 56 chars.
      $output = truncate_utf8(filter_xss_admin($output, array()), 400, TRUE, TRUE);
      $output = l($output, 'admin/reports/event/' . $event->wid, array('html' => TRUE));
    }
  }
  return $output;
}
