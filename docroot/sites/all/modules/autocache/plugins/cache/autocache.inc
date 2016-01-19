<?php

/**
 * @file
 * Provides a simple time-based caching option for panel panes.
 */

// Plugin definition.
$plugin = array(
  'title' => t('Autocache'),
  'description' => t('Use automatic caching for this panel/pane.'),
  'cache get' => 'autocache_cache_get_cache',
  'cache set' => 'autocache_cache_set_cache',
  'cache clear' => 'autocache_cache_clear_cache',
  'cache clear pane' => 'autocache_cache_clear_pane_cache',
/* We dont have any settings .. 
   'settings form' => 'autocache_cache_settings_form',
   'settings form validate' => 'autocache_cache_settings_form_validate',
   'settings form submit' => 'autocache_cache_settings_form_submit',
*/
  'defaults' => array(
    'granularity' => 'none',
    'language' => 1,
    'language_content' => 1,
  ),
);

/**
 * Get cached content.
 */
function autocache_cache_get_cache($conf, $display, $args, $contexts, $pane = NULL) {

  $cid = autocache_cache_get_id($conf, $display, $args, $contexts, $pane);

  $cache = cache_get($cid, 'cache');
  
  if (!$cache) {
    return FALSE;
  }

  return $cache->data;
}

/**
 * Set cached content.
 */
function autocache_cache_set_cache($conf, $content, $display, $args, $contexts, $pane = NULL) {

  cache_set(
    autocache_cache_get_id(
      $conf,
      $display,
      $args,
      $contexts,
      $pane
    ),
    $content
  );

}

/**
 * Clear cached content.
 *
 * @param stdClass $display
 *   The display object. If this object has a property named clear_pane, then
 *   that pane will be cleared.
 */
function autocache_cache_clear_cache($display) {
  
  cache_clear_all(
    autocache_cache_get_id_for_display($display),
    'cache', 
    TRUE
  );

}

/**
 * Generate an ID for display and pane
 */
function autocache_cache_get_id_for_display(&$display, $pane = NULL) {
  $cid = array();
  $cid[] = 'ac';

  foreach ($display->context as $key => &$context) {
    $entity_type = is_array($context->type) ? (isset($context->type[2]) ? $context->type[2] : AUTOCACHE_NOBUNDLE) : $context->type; 
    $info = entity_get_info($entity_type);
    $cid[] = $info['base table'];
    $cid[] = isset($context->restrictions['type']) ? $context->restrictions['type'][0] : AUTOCACHE_NOBUNDLE;
    $cid[] = $context->argument;
  }

  $cid[] = 'display-' . $display->did;

  if ($pane) {
    $cid[] = 'pane-' . $pane->pid;    
  }

  // Add get into the mix (for possible views params)
  if (count($_GET) > 0) {
    $cid[] = md5(serialize($_GET));
  }

  $cid = implode('-', $cid);
  $cid = autocache_register_cache_key($cid);
  return $cid;
}

/**
 * Figure out an id for our cache based upon input and settings.
 */
function autocache_cache_get_id($conf, $display, $args, $contexts, $pane) {

  return autocache_cache_get_id_for_display($display, $pane);

}

