<?php

/**
 * @file
 * Provides a simple time-based caching option for panel panes.
 */

// Plugin definition
$plugin = array(
  'title' => t("Graceful cache"),
  'description' => t('Graceul caching is a two factor time-based cache (hard expire and soft expire).'),
  'cache get' => 'cache_graceful_panels_cache_get_cache',
  'cache set' => 'cache_graceful_panels_cache_set_cache',
  'cache clear' => 'cache_graceful_panels_cache_clear_cache',
  'settings form' => 'cache_graceful_panels_cache_settings_form',
  'settings form submit' => 'cache_graceful_panels_cache_settings_form_submit',
  'defaults' => array(
    'lifetime' => 60,
    'prefetch' => 30,
    'granularity' => 'none',
  ),
);

/**
 * Get cached content.
 */
function cache_graceful_panels_cache_get_cache($conf, $display, $args, $contexts, $pane = NULL) {
  $cid = cache_graceful_panels_cache_get_id($conf, $display, $args, $contexts, $pane);
  $cache = cache_graceful(
             $cid,
             array('_cache_graceful_panels_recache', $display, $pane), 
             'cache',
             $conf['lifetime'], $conf['prefetch'],
             array('ensure_result' => TRUE)
           );
  return $cache ? $cache->data : FALSE;
}

/**
 * Set cached content.
 */
function cache_graceful_panels_cache_set_cache($conf, $content, $display, $args, $contexts, $pane = NULL) {
}

/**
 * Clear cached content.
 *
 * Cache clears are always for an entire display, regardless of arguments.
 */
function cache_graceful_panels_cache_clear_cache($display) {
  $cid = 'cache_graceful_panels_cache';

  // This is used in case this is an in-code display, which means did will be something like 'new-1'.
  if (isset($display->owner) && isset($display->owner->id)) {
    $cid .= ':' . $display->owner->id;
  }
  $cid .= ':' . $display->did;

  cache_clear_all($cid, 'cache', TRUE);
}

/**
 * Figure out an id for our cache based upon input and settings.
 */
function cache_graceful_panels_cache_get_id($conf, $display, $args, $contexts, $pane) {
  $id = 'cache_graceful_panels_cache';

  // If the panel is stored in the database it'll have a numeric did value.
  if (is_numeric($display->did)) {
    $id .= ':' . $display->did;
  }
  // Exported panels won't have a numeric did but may have a usable cache_key.
  elseif (!empty($display->cache_key)) {
    $id .= ':' . str_replace('panel_context:', '', $display->cache_key);
  }
  // Alternatively use the css_id.
  elseif (!empty($display->css_id)) {
    $id .= ':' . $display->css_id;
  }
  // Failover to just appending the did, which may be the completely unusable
  // string 'new'.
  else {
    $id .= ':' . $display->did;
  }

  if ($pane) {
    $id .= ':' . $pane->pid;
  }

  if (user_access('view pane admin links')) {
    $id .= ':admin';
  }

  switch ($conf['granularity']) {
    case 'args':
      foreach ($args as $arg) {
        $id .= ':' . $arg;
      }
      break;

    case 'context':
      if (!is_array($contexts)) {
        $contexts = array($contexts);
      }
      foreach ($contexts as $context) {
        if (isset($context->argument)) {
          $id .= ':' . $context->argument;
        }
      }
  }
  if (module_exists('locale')) {
    global $language;
    $id .= ':' . $language->language;
  }

  if(!empty($pane->configuration['use_pager']) && !empty($_GET['page'])) {
    $id .= ':p' . check_plain($_GET['page']);
  }

  return $id;
}

function cache_graceful_panels_cache_settings_form($conf, $display, $pid) {
  $options = drupal_map_assoc(array(15, 30, 60, 120, 180, 240, 300, 600, 900, 1200, 1800, 3600, 7200, 14400, 28800, 43200, 86400, 172800, 259200, 345600, 604800), 'format_interval');
  $form['lifetime'] = array(
    '#title' => t('Lifetime'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $conf['lifetime'],
  );
  $form['prefetch'] = array(
    '#title' => t('Prefetch'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $conf['prefetch'],
  );

  $form['granularity'] = array(
    '#title' => t('Granularity'),
    '#type' => 'select',
    '#options' => array(
      'args' => t('Arguments'),
      'context' => t('Context'),
      'none' => t('None'),
    ),
    '#description' => t('If "arguments" are selected, this content will be cached per individual argument to the entire display; if "contexts" are selected, this content will be cached per unique context in the pane or display; if "neither" there will be only one cache for this pane.'),
    '#default_value' => $conf['granularity'],
  );

  return $form;
}

