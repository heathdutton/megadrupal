<?php

/**
 * Processes variables for book-export-html.tpl.php.
 *
 * @param $variables
 *   An associative array containing the following keys:
 *   - title
 *   - contents
 *   - depth
 *
 * @see book-export-html.tpl.php
 */
function twbs_preprocess_book_export_html(&$variables) {
  global $base_url, $language;

  $variables['title'] = check_plain($variables['title']);
  $variables['base_url'] = $base_url;
  $variables['language'] = $language;
  $variables['language_rtl'] = ($language->direction == LANGUAGE_RTL);
  $variables['head'] = drupal_get_html_head();
  $variables['dir'] = $language->direction ? 'rtl' : 'ltr';
}
