<?php

/**
 * @file
 * Views integration hooks.
 */

/**
 * Implements hook_views_post_render().
 */
function wp_blog_views_post_render(&$view, &$output, &$cache) {
  // Add an appropriate menu-trail for the wp_blog view.
  if ($view->name != 'wp_blog') {
    return;
  }

  // Default trail is: Home >> Blog
  $trail = array(
    array(
      'title' => t('Home'),
      'href' => '<front>',
      'localized_options' => array(),
      'type' => MENU_NORMAL_ITEM,
    ),
    array(
      'title' => t('Blog'),
      'href' => 'blog',
      'localized_options' => array(),
      'type' => MENU_NORMAL_ITEM,
    ),
  );

  // Add breadcrumb entries for the arguments.
  if (count($view->args)) {
    // Build a timestamp for the provided arguments.
    $date = (object) array(
      'year'  => isset($view->args[0]) ? $view->args[0] : date('Y'),
      'month' => isset($view->args[1]) ? $view->args[1] : date('n'),
      'day'   => isset($view->args[2]) ? $view->args[2] : date('j'),
    );
    $date = mktime(12, 0, 0, $date->month, $date->day, $date->year);

    // If there's only a YEAR argument, add a sacrificial breadcrumb (to be
    // removed in menu_get_active_breadcrumb()).
    if (count($view->args) == 1) {
      $trail[] = array(
        'title' => '-',
        'href' => "blog",
        'localized_options' => array(),
        'type' => MENU_NORMAL_ITEM,
      );
    }

    // If there's a MONTH argument, add the year to the breadcrumb.
    if (isset($view->args[1])) {
      $trail[] = array(
        'title' => $view->args[0],
        'href' => "blog/{$view->args[0]}",
        'localized_options' => array(),
        'type' => MENU_NORMAL_ITEM,
      );
    }
    // If there's a DAY argument, add the month to the breadcrumb.
    if (isset($view->args[2])) {
      // Translate the numeric month argument to a month name.
      $month = date('F', $date);
      $trail[] = array(
        'title' => $month,
        'href' => "blog/{$view->args[0]}/{$view->args[1]}",
        'localized_options' => array(),
        'type' => MENU_NORMAL_ITEM,
      );
    }
  }

  // Views tries to set the breadcrumb. Reset it so it's controlled by the
  // active menu trail instead.
  drupal_static_reset('drupal_set_breadcrumb');
  menu_set_active_trail($trail);
}
