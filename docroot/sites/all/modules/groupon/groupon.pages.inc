<?php

/**
 * @file
 * Important administration functions for the groupon module.
 */


/**
 * Callback function for Groupon says page.
 *
 * @return string
 *   string of error page.
 */
function groupon_says_callback() {
  $says = groupon_get_sayings();
  if (array_key_exists('error', $says)) {
    $output = theme('groupon_error', $says);
  }
  else {
    $output = theme('groupon_sayings', $says);
    $groupon_logo_path = drupal_get_path('module', 'groupon') . '/images/powered_by_groupon.png';
    $output .= l("<span class='groupon-logo'><img src='$groupon_logo_path' /></span>", 'http://www.groupon.com', array('attributes' => array('target' => '_blank'), 'html' => TRUE));
  }
  return $output;
}

/**
 * Callback function for Groupon deals page.
 *
 * @return string
 *   string of deals page.
 */
function groupon_deals_callback() {
  $deals = groupon_get_deals();
  if (array_key_exists('error', $deals)) {
    $output = theme('groupon_error', $deals);
  }
  else {
    $page = isset($_GET['page']) ? check_plain($_GET['page']) : 0;
    $total = count($deals['deals']);
    pager_default_initialize($total, 6, $element = 0);
    $chunks = array_chunk($deals['deals'], 6);
    $deals['deals'] = $chunks[$page];
    $output = theme('groupon_deals', $deals);
    $groupon_logo_path = drupal_get_path('module', 'groupon') . '/images/powered_by_groupon.png';
    $output .= l("<span class='groupon-logo'><img src='$groupon_logo_path' /></span>", 'http://www.groupon.com', array('attributes' => array('target' => '_blank'), 'html' => TRUE));
    $output .= theme('pager', array('quantity' => $total));
  }
  return $output;
}

/**
 * Callback function for Groupon channels page.
 *
 * @return string
 *   string of channel deals.
 */
function groupon_channels_callback() {
  $channels = groupon_get_channels();
  if (array_key_exists('error', $channels)) {
    $output = theme('groupon_error', $channels);
  }
  else {
    $page = isset($_GET['page']) ? check_plain($_GET['page']) : 0;
    $total = count($channels['channels']);
    pager_default_initialize($total, 6, $element = 0);
    $chunks = array_chunk($channels['channels'], 6);
    $channels['channels'] = $chunks[$page];
    $output = theme('groupon_channels', $channels);
    $groupon_logo_path = drupal_get_path('module', 'groupon') . '/images/powered_by_groupon.png';
    $output .= l("<span class='groupon-logo'><img src='$groupon_logo_path' /></span>", 'http://www.groupon.com', array('attributes' => array('target' => '_blank'), 'html' => TRUE));
    $output .= theme('pager', array('quantity' => $total));
  }
  return $output;
}
