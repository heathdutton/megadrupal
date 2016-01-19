<?php

/**
 * Implements hook_process_page().
 */
function cloudy_process_page(&$variables) {
  // Remove the page title from the front page.
  if (drupal_is_front_page()) {
    $variables['title'] = '';
  }
  // Move 'Add a block here' from context_ui to the end of the region.
  if (!empty($variables['page'])) {
    foreach (element_children($variables['page']) as $region_key) {
      $region = &$variables['page'][$region_key];
      if (is_array($region) && !empty($region) && isset($region['context']['#weight'])) {
        $region['context']['#weight'] = 100;
        $region['#sorted'] = FALSE;
      }
    }
  }
}
