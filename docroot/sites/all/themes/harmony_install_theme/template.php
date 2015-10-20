<?php

/**
 * @file
 * template.php
 */

/* Force remove Bootstrap CDN assets. */

/**
 * Implements hook_css_alter().
 */
function harmony_install_theme_css_alter(&$css) {
  if (!empty($css)) {
    // Remove Bootstrap supplied by the base theme.
    foreach ($css as $path => $file) {
      if (strpos($path, '//netdna.bootstrapcdn.com') === 0) {
        unset($css[$path]);
      }
    }
  }
}

/**
 * Implements hook_js_alter().
 */
function harmony_install_theme_js_alter(&$javascript) {
  if (!empty($javascript)) {
    // Remove Bootstrap supplied by the base theme.
    foreach ($javascript as $path => $file) {
      if (strpos($path, '//netdna.bootstrapcdn.com') === 0) {
        unset($javascript[$path]);
      }
    }

    // Remove and add in a later version of jQuery.
    $javascript['misc/jquery.js']['version'] = '1.11.1';
    $javascript['misc/jquery.js']['data'] = 'profiles/harmony/themes/harmony_install_theme/assets/js/jquery-1.11.1.min.js';
    $javascript['profiles/harmony/themes/harmony_install_theme/assets/js/jquery-1.11.1.min.js'] = $javascript['misc/jquery.js'];
    unset($javascript['misc/jquery.js']);
  }
}

/**
 * Returns HTML for the status report.
 * Doesn't use theme_table :C
 */
function harmony_install_theme_status_report($variables) {
  $requirements = $variables['requirements'];
  $severities = array(
    REQUIREMENT_INFO => array(
      'title' => t('Info'),
      'class' => 'info',
    ),
    REQUIREMENT_OK => array(
      'title' => t('OK'),
      'class' => 'success',
    ),
    REQUIREMENT_WARNING => array(
      'title' => t('Warning'),
      'class' => 'warning',
    ),
    REQUIREMENT_ERROR => array(
      'title' => t('Error'),
      'class' => 'danger',
    ),
  );
  $output = '<table class="system-status-report table">';

  foreach ($requirements as $requirement) {
    if (empty($requirement['#type'])) {
      $severity = $severities[isset($requirement['severity']) ? (int) $requirement['severity'] : REQUIREMENT_OK];

      // Output table row(s).
      if (!empty($requirement['description'])) {
        $output .= '<tr class="' . $severity['class'] . ' merge-down"><td class="status-title"><strong>' . $requirement['title'] . '</strong></td><td class="status-value">' . $requirement['value'] . '</td></tr>';
        $output .= '<tr class="' . $severity['class'] . ' merge-up"><td colspan="3" class="status-description">' . $requirement['description'] . '</td></tr>';
      }
      else {
        $output .= '<tr class="' . $severity['class'] . '"><td class="status-title"><strong>' . $requirement['title'] . '</strong></td><td class="status-value">' . $requirement['value'] . '</td></tr>';
      }
    }
  }

  $output .= '</table>';
  return $output;
}
