<?php

/**
 * Process variables for page.tpl.php
 *
 * Perform final addition of variables before passing them into the template.
 * To customize these variables, simply set them in an earlier step.
 *
 * @see twbs_preprocess_page()
 * @see page.tpl.php
 */
function twbs_process_page(&$variables) {
  if (!isset($variables['breadcrumb'])) {
    // Build the breadcrumb last, so as to increase the chance of being able to
    // re-use the cache of an already rendered menu containing the active link
    // for the current page.
    // @see menu_tree_page_data()
    $variables['breadcrumb'] = theme('breadcrumb', array('breadcrumb' => drupal_get_breadcrumb()));
  }
  if (!isset($variables['title'])) {
    $variables['title'] = drupal_get_title();
  }

  // Generate messages last in order to capture as many as possible for the
  // current page.
  if (!isset($variables['messages'])) {
    $variables['messages'] = $variables['show_messages'] ? theme('status_messages') : '';
  }
}
