<?php

/**
 * @file
 * Contains the theme function overrides for icons.
 */

/**
 * Remove hardcoded gif in calendar module.
 */
function circle_calendar_ical_icon($vars) {
  $url = $vars['url'];
  return '<a href="' . check_url($url) . '" class="ical-icon" title="ical"><div>iCal</div></a>';
}

/**
 * Removes the <img> tag in rss feed and puts the icon into a css class.
 */
function circle_feed_icon($variables) {
  $text = t('Subscribe to @feed-title', array('@feed-title' => $variables['title']));
  return '<div class="feed">' . l($text, $variables['url'], array(
    'html' => TRUE,
    'attributes' => array(
      'class' => array('feed-icon'),
      'title' => $text,
    ))) . '</div>';
}

/**
 * Removes the <img> from a tables order.
 * @file theme_tablesort_indicator
 */
function circle_tablesort_indicator($variables) {
  if ($variables['style'] == "asc") {
    return '<div class="table-order-asc"></div>';
  }
  else {
    return '<div class="table-order-desc"></div>';
  }
}
