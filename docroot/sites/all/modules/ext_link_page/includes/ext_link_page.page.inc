<?php

/**
 * @file
 * The main logic for the External Link Page of the ext_link_page
 * module
 */

/**
 * Link Page
 *
 * Handles page for external linking
 *
 * @return
 *   page output
 */
function _ext_link_page_build_page() {
  $options = ext_link_page_get_options();
  drupal_set_title($options['ext_link_page_page_title']);

  // Sanitize URL from query. Can't use check_plain() at this point.
  $url = drupal_strip_dangerous_protocols(strip_tags($_GET['url']));
  if (valid_url($url, TRUE)) {
    if (empty($options['ext_link_page_direct_disable'])) {
      // Get page delay value
      $delay = (int) $options['ext_link_page_direct_delay'];
      if (!$delay) {
        // If the delay is 0, then redirect immediately.
        drupal_goto($url);
      }
  
      // Make redirect and add to header
      $redirect = array(
        '#tag' => 'meta',
        '#attributes' => array(
          'http-equiv' => 'refresh',
          'content' => $delay . '; url=' . $url,
        )
      );
      drupal_add_html_head($redirect, 'ext_link_page_redirect');
    }

    // Prepare token values
    $site_name = '<span class="ext_link_page-site_name">' . check_plain(variable_get('site_name', 'site_name')) . '</span>';
    $attributes = array('class' => 'ext_link_page-link', 'ext_link_page_allow' => TRUE);
    $link = l($url, $url, $attributes);

    // Build the page message
    $page_message = $options['ext_link_page_page_message'];

    // Replace values in message
    $page_message = str_replace('[link]', $link, $page_message);
    $page_message = str_replace('[url]', check_plain($url), $page_message);
    $page_message = str_replace('[delay]', $delay, $page_message);
    $page_message = str_replace('[site_name]', $site_name, $page_message);

    // Call theme function
    return theme('ext_link_page', array('message' => filter_xss_admin($page_message)));
  }

  return t('URL not valid.');
}
