<?php

/**
 * @file
 * Theme functions for multipage jumplist.
 */

/**
 * Renders the entire jumplist block content.
 */
function theme_multipage_jumplist($variables) {
  $item_list_options = array(
    'attributes' => array('class' => 'multipage-jumplist'),
    'type' => $variables['jumplist']['list_type'],
  );
  $counter = 1;
  foreach ($variables['jumplist']['pages'] as $page_name => $jump) {
    $jump['counter'] = $counter;
    $item_list_options['items'][] = theme('multipage_jumplist_item', $jump);
    $counter++;
  }
  return theme('item_list', $item_list_options);
}

/**
 * Render a single jumplist item.
 */
function theme_multipage_jumplist_item($variables) {
  $path = current_path();
  $prefix = $variables['counter_type'] == 'rendered' ? $variables['counter'] . '.&nbsp;' : '';
  $options = array(
    'attributes' => array('id' => $variables['jump_id']),
    'html' => TRUE,
  );
  return l($prefix . $variables['label'], $path, $options);
}
