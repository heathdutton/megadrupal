<?php

if (passive_authcache_builtin_serve_proxy()) {
  exit;
}

/**
 * Serves Authcache built-in backend as passive proxy.
 */
function passive_authcache_builtin_serve_proxy() {
  if (variable_get('passive_authcache_proxy_bypass_default')) {
    return FALSE;
  }

  if (passive_is_passive_proxy()) {
    // Prevent backend refresh from hitting a passive prompt.
    if (passive_check_backend_refresh()) {
      return FALSE;
    }
    // Serve passive response.
    elseif ($check = passive_check_passive_request()) {
      passive_authcache_builtin_serve_passive_response();
    }
    // Obtain the passive key for the current non-passive request.
    elseif (!isset($check) && $passive_key = passive_authcache_builtin_get_passive_key()) {
      return passive_serve_passive_prompt($passive_key);
    }
  }

  return FALSE;
}

/**
 * Checks backend response and alters Authcache built-in behavior.
 */
function passive_authcache_builtin_serve_passive_response() {
  // Force Authcache built-in backend to use Vary: X-Passive-Key.
  global $conf;
  drupal_add_http_header('Vary', 'X-Passive-Key');
  $conf['authcache_builtin_vary'] = 'X-Passive-Key';
}

/**
 * Obtains the Authcache key if the current request warrants a passive prompt.
 */
function passive_authcache_builtin_get_passive_key() {
  global $base_root;

  if (!function_exists('authcache_backend_init')) {
    return FALSE;
  }

  // Attempt to construct Authcache key.
  $key = FALSE;
  if (isset($_COOKIE[session_name()])) {
    $session_id = $_COOKIE[session_name()];

    // Attempt to bootstrap variables for certain caches.
    $phase = drupal_bootstrap(DRUPAL_BOOTSTRAP_VARIABLES, FALSE);
    if ($phase < DRUPAL_BOOTSTRAP_VARIABLES) {
      return FALSE;
    }

    // Retrieve cached Authcache key.
    // The key should be available upon login. We ignore all other scenarios
    // since the passive proxy is always responsible for passing back the given
    // passive key so even if one hasn't been generated yet the next request
    // will be different. (The risk of a security bypass is dependent on the
    // hash salt update and whether the reverse proxy is configured to defeat
    // any user attempt to smuggle a passive key through the proxy.)
    if ($cache = cache_get($base_root . ':' . $session_id, 'cache_authcache_key')) {
      $key = $cache->data;
    }
  }
  else {
    // Pass non-session requests through to regular page cache.
    return FALSE;
  }

  return $key;
}
