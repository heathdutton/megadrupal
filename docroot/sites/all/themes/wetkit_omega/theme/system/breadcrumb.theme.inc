<?php
/**
 * @file
 * breadcrumb.theme.inc
 */


/**
 * Returns HTML for a breadcrumb trail.
 *
 * Implements breadcrumb trails as an ordered list, wrapping each
 * crumb in li elements and the seperators in span elements to make life easier
 * for themers. Additionally .crumb, .crumb-first and .crumb-last classes are
 * printed on the li elements.
 *
 * @param array $variables
 *   An associative array containing:
 *   - breadcrumb: An array containing the breadcrumb links.
 */
function wetkit_omega_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];

  // Custom Logic Based on Theme Selection.
  if (variable_get('wetkit_wetboew_theme', 0) == 1) {
    $theme_prefix = 'gcwu';
  }
  elseif (variable_get('wetkit_wetboew_theme', 0) == 2) {
    $theme_prefix = 'gcwu';
  }
  elseif (variable_get('wetkit_wetboew_theme', 0) == 3) {
    $theme_prefix = 'base';
  }
  elseif (variable_get('wetkit_wetboew_theme', 0) == 4) {
    $theme_prefix = 'ogpl';
  }
  else {
    $theme_prefix = 'wet';
  }

  if (omega_theme_get_setting('wetkit_breadcrumb_display') == 1) {
    if (omega_theme_get_setting('wetkit_breadcrumb_home') == 0) {
      array_shift($breadcrumb);
    }
    // Remove the rather pointless breadcrumbs for reset password and user
    // register pages, they link to the page you are on.
    if (arg(0) === 'user' && (arg(1) === 'password' || arg(1) === 'register')) {
      array_pop($breadcrumb);
    }
    if (!empty($breadcrumb)) {
      $separator = filter_xss_admin(omega_theme_get_setting('wetkit_breadcrumb_separator'));
      // Push the page title onto the end of the breadcrumb array.
      if (omega_theme_get_setting('wetkit_breadcrumb_title') == 1) {
        $breadcrumb[] = drupal_get_title();
      }
      $output = '';
      $output = '<div id="' . $theme_prefix . '-bc">';
      $output .= '<h2>' . t('Breadcrumb trail') . '</h2>';
      $output .= '<div id="' . $theme_prefix . '-bc-in"><ol>';
      foreach ($breadcrumb as $key => $val) {
        if ($key == 0) {
          $output .= '<li>' . $val . '</li>';
        }
        elseif (!empty($separator)) {
          $output .= '<li>' . '<span>' . $separator . '</span>' . $val . '</li>';
        }
        else {
          $output .= '<li>' . $val . '</li>';
        }
      }
      $output .= '</ol></div></div>';

      return $output;
    }
  }
  else {
    return;
  }
}
