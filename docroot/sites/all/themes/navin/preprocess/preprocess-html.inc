<?php

/**
 * @file
 * This file contains Omega html preprocessing for the Navin theme.
 */

function navin_alpha_preprocess_html(&$variables) {
  drupal_add_js(array(
    'navin' => array(
      'search_text' => 'Search ' . variable_get('site_name', 'this site') . '...',
    )
  ), 'setting');
}
