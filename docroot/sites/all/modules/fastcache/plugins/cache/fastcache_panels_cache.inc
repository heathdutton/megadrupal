<?php

/**
 * @file
 * Provides views spicific caching for panels
 */

// Plugin definition
$plugin = array(
  'title' => t('Fastcache'),
  'description' => t('Panels cache plugin that supports fastcache.'),
  'cache get' => 'fastcache_panels_get_cache',
  'cache set' => 'fastcache_panels_set_cache',
  'cache clear' => 'fastcache_panels_clear_cache',
);

/**
 * Retrieves cached content if available. If not, returns false and thus initiates
 * content cache generation and save via fastcache_panels_set_cache().
 */
function fastcache_panels_get_cache($conf, $display, $args, $contexts, $pane) {
  // We call it first to ensure context is prepared for Ajax pager for anonymous users.
  $cid = _fastcache_panels_cache_get_cid($conf, $display, $args, $contexts, $pane);

  // Pane cache.
  if ($pane) {
    list($view_name, $view_display) = _fastcache_panels_get_view_info($pane);
  }
  // Display cache.
  // @todo: this can be too restrictive, should it be moved to a hook?
  elseif (isset($display->cache_key)) {
    $key = _fastcache_views_panels_cache_normalize_display_cache_key($display->cache_key);
    list($view_name, $view_display) = explode(':', $key, 2);
  }

  if (!$cid) {
    return FALSE;
  }

  if ($cid) {
    $cache = cache_get($cid, 'cache_fastcache');
    if (!$cache) {
      return FALSE;
    }
    // always autoexpire after 30 mins
    if ((time() - $cache->created) > 1800) {
      return FALSE;
    }

    // Allow other module react/alter cache before it is returned.
    foreach (module_implements('fastcache_get') as $module) {
      $function = $module . '_fastcache_get';
      $function($view_name, $view_display, $cache);
    }

    return $cache->data['output'];
  }

  return FALSE;
}

/**
 * Set cached content.
 */
function fastcache_panels_set_cache($conf, $content, $display, $args, $contexts, $pane) {
  $cid = _fastcache_panels_cache_get_cid($conf, $display, $args, $contexts, $pane);

  if ($pane) {
    $view_name = _fastcache_panels_get_view_info($pane);
    list($view_name, $view_display) = $view_name;
  }
  // @todo: technically, this is not a real view that has name and display.
  elseif (isset($display->cache_key)) {
    $key = _fastcache_views_panels_cache_normalize_display_cache_key($display->cache_key);
    list($view_name, $view_display) = explode(':', $key, 2);
  }

  // Do not cache for anonymous users - Varnish or other cache engine
  // will handle that. The main reason for that is possible flagging
  // which may not work if anonymous version of the page was cached.
  // @todo: this may be too restrictive, should it be moved to a hook?
  if (!$cid) {
    return;
  }

  // We save both view output and its result.
  $view_info = drupal_static('fastcache_active_view_info', array('result' => array(), 'cached_output' => ''));
  $meta = fastcache_get_cached_views($view_name . ':' . $view_display);
  if (!empty($meta['flag']) && !empty($view_info['cached_output'])) {
    // Clone output to not operate on real object since it will be shown later.
    $cached_content = clone $content;
    if (isset($cached_content->content) && is_object($cached_content->content)) {
       $cached_content->content = clone $content->content;
       $cached_content->content->content = $view_info['cached_output'];
    }
  }
  $cache = array('output' => isset($cached_content) ? $cached_content : $content, 'result' => $view_info['result']);

  // Allow other module react/alter cache before it is saved.
  foreach (module_implements('fastcache_set') as $module) {
    $function = $module . '_fastcache_set';
    $function($view_name, $view_display, $cache);
  }

  // Consider cache lifetime.
  cache_set($cid, $cache, 'cache_fastcache', fastcache_get_view_expire($view_name, $view_display));
  drupal_static_reset('fastcache_last_result');
}

/**
 * @todo: implement or refuse.
 */
function fastcache_panels_clear_cache($display) {
}

/**
 * Figure out an id for our cache based upon input and settings.
 */
function _fastcache_panels_cache_get_cid($conf, $display, $args, $contexts, $pane) {
  global $language;
  $cache_context = &drupal_static('fastcache_context', 0);
  $cid_base = $cid_context = $cid_args = '';
  $pane_cache = FALSE;

  if ($pane) {
    $view_name = _fastcache_panels_get_view_info($pane);
    list($view_name, $view_display) = $view_name;
    $cid_base = $view_name . ':' . $view_display;
    $pane_cache = TRUE;
  }
  // If not a pane, cache is set for the whole display.
  elseif (isset($display->cache_key)) {
    $cid_base = _fastcache_views_panels_cache_normalize_display_cache_key($display->cache_key);
  }

  $view = fastcache_get_cached_views($cid_base);

  // Check if we should use certain context as cache ID component.
  if (!empty($view['context_type'])) {
    if ($view['context_type'] == 'user') {
      foreach ($contexts as $context) {
        // @todo: that kind of comparison is probably not ideal.
        if ($context->type == array('entity:user', 'entity', 'user')) {
          if (is_object($context->data) && isset($context->data->uid)) {
            $cid_context = ':user/' . $context->data->uid;
            $cache_context = $context->data->uid;
            break;
          }
        }
      }
    }
    elseif ($view['context_type'] == 'path') {
      $cid_context = ':' . md5($_SERVER['REQUEST_URI']);
    }
  }

  // Consider language.
  $cid_base .=  ':' . $language->language;

  // Anonymous co);ntent may differ from authenticated.
  if (user_is_anonymous()) {
    $cid_base .= ':anon';
  }

  // Add arguments present when calling the view, but remove unwanted stuff from
  // request since it may generate false uniqueness.
  // Arguments here are pager, sorting, filtering
  $request = $_REQUEST;
  foreach (array('pager_element', 'ajax_html_ids', 'ajax_page_state', 'view_path', 'view_dom_id') as $key) {
    if (isset($request[$key])) {
      unset($request[$key]);
    }
  }
  $cid_args = ':' . md5(serialize($request));

  return $cid_base . $cid_context . $cid_args;
}

/**
 * Get view details out of pane configuration.
 */
function _fastcache_panels_get_view_info($pane) {
  $view_info = array();
  if ($pane->type == 'views_panes' && !empty($pane->subtype)) {
    $view_info = explode('-', $pane->subtype);
  }
  elseif ($pane->type == 'views') {
    $view_name = $pane->subtype;
    $view_display = $pane->configuration['display'];
    $view_info = array($view_name, $view_display);
  }
  else {
    $view_info = array($pane->type, $pane->subtype);
  }
  return $view_info;
}

/**
 * Normalize the key so that it didn't include argumentsthat may be
 * appended to it by ctools.
 */
function _fastcache_views_panels_cache_normalize_display_cache_key($key) {
  $parts = explode(':', $key);
  return $parts[0] . ':' . $parts[1] . ':' . $parts[3];
}
