<?php

/**
 * Load needed libraries for the slideshow.
 *
 * @param  array $config [description]
 */
function go_views_pre_render__slideshow($view_name, $display_id, $config = array()) {
  // Supuport lazy config
  if (is_callable($config)) {
    $config = $config($view_name, $display_id);
  }

  if (!is_array($config)) {
    return;
  }

  if (empty($config['#attached']['js']['jquery.cycle'])) {
    $config['#attached']['js']['jquery.cycle'] = 'sites/all/libraries/jquery.cycle/jquery.cycle.all.min.js';
  }

  if (empty($config['#attached']['js']['go.slideshow'])) {
    $config['#attached']['js']['go.slideshow'] = drupal_get_path('module', 'go') . '/misc/js/go.slideshow.js';
  }

  $config['#attached']['js'][] = array(
    'type' => 'setting',
    'data' => array('go_slideshow' => array('views' => array($view_name => array($display_id => $config)))),
  );

  drupal_process_attached($config);
}
