<?php

/**
 * Theme function for the local tasks.
 *
 * @todo
 *   This is a little hacky. We also would like to have the list nested with
 *   secondary local tasks.
 */
function theme_ns_core_local_tasks() {
  // Remove all existing class attributes.
  $primary_local_tasks = preg_replace('/( class="[a-z]*")/', ' class="leaf"', menu_primary_local_tasks());
  // Add a 'leaf' class attribute.
  $primary_local_tasks = preg_replace('/<li/', '<li class="leaf"', $primary_local_tasks);
  return '<ul class="menu">' . $primary_local_tasks . '</ul>';
}

/**
 * Theme function for the page title. This theme function is used by
 * the CTools page content type called 'page_title', defined by ns_core.module.
 */
function theme_ns_core_page_title($variables) {
  return '<h1>' . $variables['title'] . '</h1>';
}

/**
 * Theme function for the custom date.
 */
function theme_ns_core_node_date_custom($variables) {
  $content = '<span class="post-submitted">';
  if ($variables['conf']['label'] == 'inline') {
    $content .=  check_plain($variables['title']);
  }
  $content .= $variables['date'] . '</span>';
  return $content;
}

/**
 * Theme function for the custom date.
 */
function theme_ns_core_today_date(&$variables) {
  $content = '<span>';
  if ($variables['conf']['label'] == 'inline') {
    $content .=  check_plain($variables['title']);
  }
  $content .= $variables['date'] . '</span>';
  return $content;

}
