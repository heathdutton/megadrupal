<?php

/**
 * @file
 * Theme.
 */

/**
 * Render taxonomy autolink term link.
 * @param $variables
 * @return string
 */
function theme_taxonomy_autolink_link($variables) {
  $tid = $variables['tid'];
  $text = $variables['text'];
  // Return a friendly url path if available.
  $url = drupal_lookup_path('alias', 'taxonomy/term/' . $tid, $lang_code = LANGUAGE_NONE);
  if (empty($url)) {
    $url = 'taxonomy/term/' . $tid;
  }
  return l(check_plain($text), $url, array(
    'html' => TRUE,
    'attributes' => array(
      'class' => array('term-link'),
      'title' => $text,
    ),
  ));
}
