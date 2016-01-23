<?php

/**
 * Returns HTML for status and/or error messages, grouped by type.
 *
 * An invisible heading identifies the messages for assistive technology.
 * Sighted users see a colored box. See http://www.w3.org/TR/WCAG-TECHS/H69.html
 * for info.
 *
 * @param $variables
 *   An associative array containing:
 *   - display: (optional) Set to 'status' or 'error' to display only messages
 *     of that type.
 */
function twbs_status_messages($variables) {
  $output = '';

  $display = $variables['display'];

  foreach (drupal_get_messages($display) as $type => $messages) {
    switch ($type) {
      case 'error':
        $heading = t('Error message');
        $class = 'danger';
        break;
      case 'warning':
        $heading = t('Warning message');
        $class = 'warning';
        break;
      case 'status':
      default:
        $heading = t('Status message');
        $class = 'success';
    }

    foreach ($messages as $message) {
      $output .= '<div class="alert alert-dismissable alert-' . $class . '">';
      $output .= '<h2 class="sr-only">' . $heading . '</h2>';
      $output .= $message;
      $output .= '<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>';
      $output .= '</div>';
    }
  }

  return $output;
}
