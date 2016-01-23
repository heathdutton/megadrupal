<?php

/**
 * Implements twbs_preprocess_HOOK() for overlay.tpl.php
 *
 * If the current page request is inside the overlay, add appropriate classes
 * to the <body> element, and simplify the page title.
 *
 * @see twbs_process_overlay()
 * @see overlay.tpl.php
 */
function twbs_preprocess_overlay(&$variables) {
  $variables['tabs'] = menu_primary_local_tasks();
  $variables['title'] = drupal_get_title();
  $variables['disable_overlay'] = overlay_disable_message();
  $variables['content_attributes_array']['class'][] = 'clearfix';
}
