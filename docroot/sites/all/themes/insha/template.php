<?php
/**
 * @file
 * Include preprocess functions and alter hooks.
 */

/**
 * Override hook_breadrumb(). Print breadcrumbs as a list, with separators.
 */
function insha_breadcrumb($variables) {
  global $base_url;
  $breadcrumb = $variables['breadcrumb'];
  if (!empty($breadcrumb)) {
    $breadcrumb[] = drupal_get_title();
    $breadcrumbs = '<ol class="breadcrumb">';
    foreach ($breadcrumb as $key => $value) {
      if ($key == 0) {
        $breadcrumbs .= '<li><a href="' . $base_url . '"><span class="icon-home"></span></a>' . $value . '</li>';
      }
      else {
        $breadcrumbs .= '<li>' . $value . '</li>';
      }
    }
    $breadcrumbs .= '</ol>';
    return $breadcrumbs;
  }
}

/**
 * Override hook_links__system_secondary_menu(). Adding class in menu links.
 */
function insha_links__system_secondary_menu(&$variables) {
  $output = '';
  $extra_classes = array(
    'user' => 'secondary_nav_account',
    'user/logout' => 'secondary_nav_logout',
  );

  foreach ($variables['links'] as $item => $link) {
    $classes = array($item);
    if (isset($extra_classes[$link['href']])) {
      $classes[] = $extra_classes[$link['href']];
    }
    $output .= sprintf('<li class="%s">%s</li>', implode(' ', $classes), l($link['title'], $link['href']));
  }
  return $output;
}

/**
 * Override hook_status_messages(). Change structure of messages.
 */
function insha_status_messages($variables) {
  $display = $variables['display'];
  $output = '';

  $status_heading = array(
    'status' => t('Status message'),
    'error' => t('Error message'),
    'warning' => t('Warning message'),
  );
  foreach (drupal_get_messages($display) as $type => $messages) {
    $output .= "<div class=\"messages $type\">\n";
    if (!empty($status_heading[$type])) {
      $output .= '<h2 class="element-invisible">' . $status_heading[$type] . "</h2>\n";
    }
    if (count($messages) > 0) {
      $output .= " <ul>\n";
      foreach ($messages as $message) {
        $output .= '  <li>' . $message . "</li>\n";
      }
      $output .= " </ul>\n";
    }
    else {
      $output .= $messages[0];
    }
    $output .= "</div>\n";
  }
  return $output;
}
