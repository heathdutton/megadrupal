<?php

/**
 * @file
 * Default template.php for Sport theme.
 */

/**
 * Custom search form.
 */
function sports_zone_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'search_block_form') {
    $form['search_block_form']['#attributes']['class'] = array('search-input');
    $form['actions']['submit']['#type'] = 'image_button';
    $form['actions']['submit']['#src'] = drupal_get_path('theme', 'sports_zone') . '/images/search.png';
    $form['actions']['submit']['#attributes']['class'] = array('search-button');
  }
}
