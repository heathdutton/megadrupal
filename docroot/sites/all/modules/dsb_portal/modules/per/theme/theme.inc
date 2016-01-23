<?php

/**
 * @file
 * Theme hooks, helper functions and preprocess functions.
 */

/**
 * Link to PER website.
 *
 * If a URL code is given, will use the specific URL for accessing the page
 * concerning that code directly. Otherwise, will simply point to a generic
 * page.
 *
 * @param string $url_part
 *    (optional) If available, the PER code we should add to the URL.
 *
 * @return string
 *    And HTML-formatted string.
 */
function dsb_portal_per_theme_per_site_link($url_part) {
  $attributes = array(
    'attributes' => array(
      'target' => '_blank',
    ),
  );

  if (isset($url_part)) {
    return l(
      t("More information on PER site"),
      str_replace('[CODE]', $url_part, variable_get('dsb_portal_per_code_url', 'http://www.plandetudes.ch/web/guest/[CODE]')),
      $attributes
    );
  }
  else {
    return l(
      t("More information on PER site"),
      variable_get('dsb_portal_per_site_url', 'http://www.plandetudes.ch'),
      $attributes
    );
  }
}

/**
 * PER curriculum cycle filter link.
 *
 * Prepare a link that can be used to filter search results by a specific
 * PER curriculum cycle. This returns an HTML formatted link.
 *
 * @param string $per_cycle
 *    The curriculum cycle.
 *
 * @return string
 *    A HTML-formatted string.
 */
function dsb_portal_per_theme_per_curriculum_cycle_filter_link($per_cycle) {
  return l($per_cycle, 'dsb-portal/search', array(
    'query' => array(
      'filter' => array(
        'perCycle' => array($per_cycle),
      ),
    ),
  ));
}

/**
 * PER curriculum code filter link.
 *
 * Prepare a link that can be used to filter search results by a specific
 * PER curriculum code. This returns an HTML formatted link.
 *
 * @param string $curriculum_code
 *    The curriculum code.
 *
 * @return string
 *    A HTML-formatted string.
 */
function dsb_portal_per_theme_per_curriculum_code_filter_link($curriculum_code) {
  return l($curriculum_code, 'dsb-portal/search', array(
    'query' => array(
      'filter' => array(
        'perObjectiveCode' => array($curriculum_code),
      ),
    ),
  ));
}