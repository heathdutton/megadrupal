<?php

/**
 * @file
 * AJAX pane cache plugin.
 */

// Plugin definition.
$plugin = array(
  'title' => t('AJAX pane'),
  'description' => t('AJAX pane. Panes are rendered as wrappers which wrappers will be replaced by javascript.'),
  'cache get' => 'ajax_pane_cache_get_cache',
  'cache set' => 'ajax_pane_cache_set_cache',
  'cache clear' => 'ajax_pane_cache_clear_cache',
  'settings form' => 'ajax_pane_cache_settings_form',
  'settings form submit' => 'ajax_pane_cache_settings_form_submit',
  'defaults' => array(
    'use_ajax_pane' => FALSE,
  ),
);

/**
 * Get cached content.
 */
function ajax_pane_cache_get_cache($conf, $display, $args, $contexts, $pane = NULL) {
  // We do not need cache because a lot of custom panes add javascript in render
  // function and this cause unexpected behavior.

//  if (!empty($pane) && empty($display->content[$pane->pid]->ajax_render)) {
//    $cid = ajax_pane_get_id($conf, $display, $args, $contexts, $pane);
//    $cache_table = variable_get('ajax_pane_cache_table', 'cache');
//    $cache = cache_get($cid, $cache_table);
//    if (!$cache) {
//      return FALSE;
//    }
//
//    if ((time() - $cache->created) > $conf['lifetime']) {
//      return FALSE;
//    }
//
//    // Add the javascript to the page you render.
//    drupal_add_js(drupal_get_path('module', 'ajax_pane') . '/js/ajax_pane.js');
//
//    return $cache->data;
//  }
}

/**
 * Sets cached content.
 */
function ajax_pane_cache_set_cache($conf, $content, $display, $args, $contexts, $pane = NULL) {
//  if (!empty($pane) && !empty($content->content->use_ajax_render)) {
//    $cid = ajax_pane_get_id($conf, $display, $args, $contexts, $pane);
//    $cache_table = variable_get('ajax_pane_cache_table', 'cache');
//    cache_set($cid, $content, $cache_table);
//  }
}

/**
 * Clears cached content.
 *
 * Cache clears are always for an entire display, regardless of arguments.
 */
function ajax_pane_cache_clear_cache($display) {
//  $cid = 'panels_ajax_cache';
//  $cache_table = variable_get('ajax_pane_cache_table', 'cache');
//  cache_clear_all($cid, $cache_table, TRUE);
}

/**
 * Figures out an id for our cache based upon input and settings.
 */
function ajax_pane_get_id($conf, $display, $args, $contexts, $pane) {
  $id = 'panels_ajax_cache';

  if ($pane) {
    $id .= ':' . $pane->pid;
  }

  return $id;
}

/**
 * Settings form.
 */
function ajax_pane_cache_settings_form($conf, $display, $pid) {
  $intervals = array(
    15, 30, 60, 120, 180, 240, 300, 600, 900, 1200, 1800, 3600, 7200, 14400,
    28800, 43200, 86400, 172800, 259200, 345600, 604800,
  );
  $options = drupal_map_assoc($intervals, 'format_interval');

  $form['lifetime'] = array(
    '#title' => t('Lifetime'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $conf['lifetime'],
  );

  return $form;
}

/**
 * Settings form submit callback.
 *
 * @todo Should this do something?
 */
function ajax_pane_cache_settings_form_submit($form, &$form_state) {

}
