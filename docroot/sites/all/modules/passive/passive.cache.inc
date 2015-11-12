<?php

define('PASSIVE_REFRESH_SIGNATURE_HEADER', 'X-Passive-Refresh');
define('PASSIVE_REFRESH_SIGNATURE_HEADER_CGI', 'HTTP_X_PASSIVE_REFRESH');
define('PASSIVE_SIGNATURE_SALT_UPDATE_DELAY', 20);
define('PASSIVE_SIGNATURE_SALT_UPDATE_INTERVAL', 86400);
define('PASSIVE_REQUEST_QUICK_TIMEOUT', 0.5);
define('PASSIVE_REQUEST_TRY_LIMIT', 5);
define('PASSIVE_REQUEST_CAPACITY_LOCK_TIMEOUT', 30);
define('PASSIVE_REQUEST_CONCURRENCY_LIMIT', 20);
define('PASSIVE_HUMAN_USER_AGENT_PATTERN', '`Mozilla|Opera`');

// Trigger a session check.
passive_check_session();

// Trigger a backend refresh check.
passive_check_backend_refresh();

/**
 * Checks session ID.
 */
function passive_check_session() {
  if (!empty($_COOKIE[session_name()])) {
    global $user;

    // Preempt page cache bypass and reset session early.
    $phase = drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION, FALSE);
    if ($phase < DRUPAL_BOOTSTRAP_SESSION) {
      // Abort check if cannot boot to session, e.g. when called in update.php.
      return;
    }
    if (empty($user->uid) && empty($_SESSION)) {
      unset($_COOKIE[session_name()]);
    }
  }
}

/**
 * Checks whether this is a backend request and sets cache hints.
 *
 * If in a backend request, attempt to prevent page caches from loading cache.
 */
function passive_check_backend_refresh() {
  $is_backend = &drupal_static(__FUNCTION__);
  $is_prevented = &drupal_static(__FUNCTION__ . ':prevented');

  if (!isset($is_backend)) {
    $is_backend = FALSE;

    /** @var callable $check */
    $check = variable_get('passive_check_backend_refresh', 'passive_default_check_backend_refresh');
    if (!$is_prevented && $check()) {
      // Lock request.
      list(, $signature) = passive_current_request();
      if (!passive_refresh_is_locked($signature) && passive_refresh_lock($signature)) {
        $is_backend = TRUE;

        // Backend refreshes should NEVER trigger cache except for storing. This
        // flag is reset in hook_boot().

        // Bypass default Drupal page cache.
        drupal_page_is_cacheable(FALSE);

        // Add vary header to bypass agents like Authcache if instructed.
        if (variable_get('passive_page_cache_refresh_bypass')) {
          drupal_add_http_header('Vary', 'X-Passive-Refresh', TRUE);
        }

        // Bypass other external proxy cache.
        $GLOBALS['conf']['page_cache_maximum_age'] = 0;
      }
    }
  }

  return $is_backend;
}

/**
 * Prevents the current request from being verified as a backend refresh.
 */
function passive_prevent_backend_refresh() {
  $is_prevented = &drupal_static('passive_check_backend_refresh:prevented');
  $is_prevented = TRUE;
}

/**
 * Checks a request against its signature.
 */
function passive_default_check_backend_refresh() {
  // Verify backend request signature.
  if (!empty($_SERVER[PASSIVE_REFRESH_SIGNATURE_HEADER_CGI])) {
    list(, $verify_signature) = passive_current_request();
    if ($verify_signature == $_SERVER[PASSIVE_REFRESH_SIGNATURE_HEADER_CGI]) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * Obtains the current request.
 */
function passive_current_request() {
  $request = &drupal_static('passive_current_request:request');
  $signature = &drupal_static('passive_current_request:signature');
  if (!isset($request) || !isset($signature)) {
    $request_build = passive_request_build();
    if (isset($request_build)) {
      $signature_build = passive_request_signature($request_build);
      if (isset($signature_build)) {
        $request = $request_build;
        $signature = $signature_build;
      }
    }
  }
  return array($request, $signature);
}

/**
 * Builds the backend request.
 *
 * If either the current request or the object is invalid, NULL is returned.
 *
 * @return array
 *   Request array for dispatch.
 */
function passive_request_build() {
  /** @var callable $build */
  $build = variable_get('passive_request_build', 'passive_request_default_build');
  return $build();
}

/**
 * Builds the default request array for the current request.
 *
 * This default implementation of 'passive_request_build' assembles the
 * complete URL and HTTP headers of a current GET request. Other HTTP methods
 * are treated as invalid.
 *
 * @return array
 *   Request array, consisting of:
 *   - url: Full URL to be used with drupal_http_request().
 *   - All other attributes to be used with $options in drupal_http_request().
 */
function passive_request_default_build() {
  if ($_SERVER['REQUEST_METHOD'] != 'GET') {
    return NULL;
  }

  // Extract encoded CGI headers.
  $headers = array();
  foreach ($_SERVER as $key => $value) {
    if (is_string($key) && substr($key, 0, 5) == 'HTTP_') {
      $header_name = substr($key, 5);
      $header_name = strtolower(str_replace('_', '-', $header_name));
      $headers[$header_name] = $value;
    }
  }

  // Build full request URL.
  global $base_root;
  $url = $base_root . request_uri();

  $request = array(
    'url' => $url,
    'headers' => $headers,
  );
  return $request;
}

/**
 * Computes the signature for a request.
 *
 * The signature is verified in a backend refresh to mitigate cache bypasses.
 *
 * @param array $request
 *   Request object containing:
 *   - url: The full URL of the request.
 *   - headers: An array of HTTP request headers. Per standard, all header names
 *     are case-insensitive and lowercase.
 *
 * @return mixed
 *   String signature, or NULL if the request could not be signed, e.g. the salt
 *   is not available.
 */
function passive_request_signature($request) {
  $hash_salt = passive_signature_salt();
  if (isset($hash_salt)) {
    $hash_index = passive_request_index($request);
    // Compute fast MD5 signature.
    return md5("$hash_index;$hash_salt");
  }
  else {
    return NULL;
  }
}

/**
 * Computes the unique index for a request.
 */
function passive_request_index($request) {
  /** @var callable $generate */
  $generate = variable_get('passive_request_index', 'passive_request_default_index');
  return $generate($request);
}

/**
 * Computes the default request index.
 */
function passive_request_default_index($request) {
  $url = $request['url'];
  $session_id = '';
  if (isset($request['headers']['cookie'])) {
    $session_id = passive_extract_session_id($request['headers']['cookie']);
  }
  $hash_index = "url:$url;session_id:$session_id";
  return $hash_index;
}

/**
 * Computes the anonymous request index.
 */
function passive_request_anon_index($request) {
  $url = $request['url'];
  $hash_index = "url:$url";
  return $hash_index;
}

/**
 * Compiles the index of a request as its URL and key.
 */
function passive_request_authcache_index($request) {
  global $base_root;

  $url = $request['url'];
  $authcache_key = '';
  if (isset($request['headers']['cookie'])) {
    $session_id = passive_extract_session_id($request['headers']['cookie']);
    if ($cache = cache_get($base_root . ':' . $session_id, 'cache_authcache_key')) {
      $authcache_key = $cache->data;
    }
  }
  $hash_index = "url:$url;authcache_key:$authcache_key";
  return $hash_index;
}

/**
 * Extracts the session ID for a request cookie.
 */
function passive_extract_session_id($cookie) {
  $session_name = session_name();
  if (preg_match('/(?:^|;)\s*' . preg_quote($session_name, '/') . '=([^;]+)(?:$|;)/', $cookie, $match)) {
    return $match[1];
  }
  return FALSE;
}

/**
 * Generates the request signature hash salt.
 *
 * @return mixed
 *   String hash salt, or NULL if the salt is not available, e.g. being updated.
 */
function passive_signature_salt() {
  $phase = drupal_bootstrap(DRUPAL_BOOTSTRAP_VARIABLES, FALSE);
  if ($phase < DRUPAL_BOOTSTRAP_VARIABLES) {
    return NULL;
  }
  $lock_timeout = variable_get('passive_signature_salt_update_delay', PASSIVE_SIGNATURE_SALT_UPDATE_DELAY);
  $hash_salt = variable_get('passive_signature_salt', '');
  $hash_updated = variable_get('passive_signature_salt_updated');
  $update_interval = variable_get('passive_signature_salt_update_interval', PASSIVE_SIGNATURE_SALT_UPDATE_INTERVAL);

  // Refresh hash salt during idle.
  $update_blocked = FALSE;
  if ($hash_updated + $update_interval <= REQUEST_TIME && lock_may_be_available('passive_signature_salt')) {
    // Pessimistically acquire update lock since variables may have already been
    // loaded and thus cannot be reloaded.
    if (lock_acquire('passive_signature_salt_update', 0.05)
      && lock_acquire('passive_signature_salt_update', 0.1)
      && lock_acquire('passive_signature_salt_update', 5)) {
      $hash_salt = md5(mt_rand());
      variable_set('passive_signature_salt', $hash_salt);
      variable_set('passive_signature_salt_updated', REQUEST_TIME);
      lock_release('passive_signature_salt_update');
    }
    // Mark salt update as blocked.
    else {
      $update_blocked = TRUE;
    }
  }

  // Abort if another request is updating the salt.
  if ($update_blocked || !lock_may_be_available('passive_signature_salt_update')) {
    return NULL;
  }
  // Check and acquire/extend an idle lock to prevent salt update. Note that the
  // specified timeout is not to last the entire backend request, but to last
  // until backend request verification across all concurrent requests have all
  // reasonably completed.
  elseif (lock_may_be_available('passive_signature_salt')) {
    lock_acquire('passive_signature_salt', $lock_timeout);
  }

  return $hash_salt;
}

/**
 * Dispatches the backend request.
 *
 * The default implementation to dispatch within lock can be overridden.
 *
 * WARNING: the page request may time out before this function returns.
 */
function passive_request_dispatch($request, $timeout = NULL) {
  $phase = drupal_bootstrap(DRUPAL_BOOTSTRAP_VARIABLES, FALSE);
  if ($phase < DRUPAL_BOOTSTRAP_VARIABLES) {
    return;
  }

  $signature = passive_request_signature($request);

  // Acquire lock for cache lifetime once per signature.
  if (!passive_refresh_is_locked($signature)) {
    passive_shutdown_trigger_refresh(TRUE, $signature, $request);

    // Perform passive request.
    /** @var callable $dispatch */
    $dispatch = variable_get('passive_request_dispatch', 'passive_request_default_dispatch');
    $dispatch($request, $signature, $timeout);

    passive_shutdown_trigger_refresh(FALSE, $signature);
  }
}

/**
 * Performs a cURL request to the backend.
 */
function passive_request_default_dispatch($request, $signature, $timeout) {
  // Normalize User-Agent header so drupal_http_request() does not add another.
  if (isset($request['headers']['user-agent'])) {
    $request['headers']['User-Agent'] = $request['headers']['user-agent'];
    unset($request['headers']['user-agent']);
  }

  // Prepare request.
  $url = $request['url'];
  // The case of the signature header name is directly normalized in CGI.
  $request['headers'][PASSIVE_REFRESH_SIGNATURE_HEADER] = $signature;
  $signature_header_lower = strtolower(PASSIVE_REFRESH_SIGNATURE_HEADER);
  if (isset($request['headers'][$signature_header_lower])) {
    unset($request['headers'][$signature_header_lower]);
  }

  // Remove extra Host header for drupal_http_request().
  if (isset($request['headers']['host'])) {
    unset($request['headers']['host']);
  }

  // Set additional options.
  if (!isset($request['timeout']) && !empty($timeout)) {
    $request['timeout'] = $timeout;
  }
  $request['max_redirects'] = variable_get('passive_request_max_redirect', 1);

  require_once DRUPAL_ROOT . '/includes/common.inc';
  drupal_http_request($url, $request);
}

/**
 * Locks a refresh request.
 */
function passive_refresh_lock($signature) {
  // Acquire lock for cache lifetime once per signature.
  // Lock will not survive page finish.
  $lock_timeout = max(1, (int) variable_get('cache_lifetime'));
  return lock_acquire('passive_backend_refresh:' . $signature, $lock_timeout);
}

/**
 * Checks whether a refresh request is locked.
 */
function passive_refresh_is_locked($signature) {
  return !lock_may_be_available('passive_backend_refresh:' . $signature);
}

/**
 * Checks whether the request is made from a passive proxy.
 */
function passive_is_passive_proxy() {
  if (isset($_SERVER['HTTP_X_PASSIVE_PROXY']) && strtolower($_SERVER['HTTP_X_PASSIVE_PROXY']) == 'true') {
    return TRUE;
  }

  return FALSE;
}

/**
 * Verifies the current request as a passive request using the passive key.
 *
 * @return mixed
 *   TRUE if valid passive request, FALSE if invalid, and NULL if no passive
 *   request detected.
 */
function passive_check_passive_request() {
  list(, $verify_signature) = passive_current_request();
  if (isset($verify_signature) && isset($_SERVER['HTTP_X_PASSIVE_REQUEST'])) {
    $actual_signature = $_SERVER['HTTP_X_PASSIVE_REQUEST'];
    if ($verify_signature == $actual_signature) {
      return TRUE;
    }
    return FALSE;
  }
}

/**
 * Checks whether the request is made from a passive proxy.
 *
 * @param string $passive_key
 *   A uniqueness key for identifying variants of a cacheable page.
 * @return bool
 *   Whether the passive prompt was successfully served.
 */
function passive_serve_passive_prompt($passive_key) {
  $code = variable_get('passive_prompt_status_code', 363);

  list(, $signature) = passive_current_request();
  if (isset($signature)) {
    drupal_add_http_header('Status', "$code Passive Prompt");
    drupal_add_http_header('X-Passive-Request', $signature);
    drupal_add_http_header('X-Passive-Key', $passive_key);
    return TRUE;
  }

  return FALSE;
}

/**
 * Passive cache backend implementation.
 */
class PassiveCache implements DrupalCacheInterface {
  /**
   * @var string
   */
  protected $bin;

  /**
   * @var DrupalCacheInterface
   */
  protected $cache;

  /**
   * @var array
   */
  protected $capacityLocks;

  /**
   * Constructs a passive cache object.
   *
   * A passive cache backend is instantiated to handle actual operations.
   *
   * @param string $bin
   *   The cache bin for which the object is created.
   */
  public function __construct($bin) {
    $this->bin = $bin;

    $class = variable_get('passive_cache_class_' . $bin);
    if (!isset($class)) {
      $class = variable_get('passive_cache_default_class');
      if (!isset($class)) {
        $class = variable_get('cache_default_class', 'DrupalDatabaseCache');
      }
    }

    $this->cache = new $class($bin);
    $this->capacityLocks = array();
  }

  /**
   * Implements DrupalCacheInterface::get().
   */
  function get($cid) {
    $result = $this->cache->get($cid);
    $results = $this->prepareMultiple(array($result));
    return reset($results);
  }

  /**
   * Implements DrupalCacheInterface::getMultiple().
   */
  function getMultiple(&$cids) {
    $results = $this->cache->getMultiple($cids);
    return $this->prepareMultiple($results);
  }

  /**
   * Prepares cache results for passive refresh.
   */
  protected function prepareMultiple($entries) {
    $results = array();
    foreach ($entries as $i => $cache) {
      // Pass through unrecognized objects.
      if ($cache === FALSE || !is_object($cache->data) || !isset($cache->data->passive_expire)) {
        $results[$i] = $cache;
      }
      else {
        $cache_lifetime = variable_get('cache_lifetime', 0);

        $cache->expire = $cache->data->passive_expire;
        $cache->data = $cache->data->data;
        $cache->is_passive = TRUE;

        // Only handle expired temporary passive cache.
        // In other words, a repeated cache refresh short circuits here. If
        // rebuilding this cache would rebuild the cache of contained elements,
        // a renewed timestamp is assumed to imply that the timestamps of the
        // caches of contained elements have also been renewed.
        if ($cache->expire != CACHE_PERMANENT && $cache_lifetime && REQUEST_TIME > ($cache->created + $cache_lifetime)) {
          // Ignore cache in validated passive request.
          if (passive_check_backend_refresh()) {
            // Acquire capacity lock for the entire request resetting expired cache.
            $capacity_lock_name = $this->bin . ':' . $cache->cid;
            if ($capacity_lock = passive_backend_acquire_capacity_lock($capacity_lock_name)) {
              $cache = FALSE;

              // Stash lock to release locks when done.
              $this->capacityLocks[$capacity_lock_name] = $capacity_lock;
            }
          }
          // Trigger passive refresh for frontend request.
          else {
            passive_request_trigger_refresh();
          }
        }

        $results[$i] = $cache;
      }
    }

    return $results;
  }

  /**
   * Implements DrupalCacheInterface::set().
   */
  function set($cid, $data, $expire = CACHE_PERMANENT) {
    // Always set as permanent.
    $data = (object) array(
      'passive_expire' => $expire,
      'data' => $data,
    );

    // Mark as permanent for passive refresh only on a nonzero minimum lifetime.
    // This triggers the encapsulated secondary cache to expire a result so as
    // to avoid triggering a passive request on every page load.
    if (variable_get('cache_lifetime', 0)) {
      $expire = CACHE_PERMANENT;
    }

    $this->cache->set($cid, $data, $expire);

    // Release capacity lock.
    $capacity_lock_name = $this->bin . ':' . $cid;
    if (isset($this->capacityLocks[$capacity_lock_name])) {
      foreach ($this->capacityLocks[$capacity_lock_name] as $lock) {
        lock_release($lock);
      }
      unset($this->capacityLocks[$capacity_lock_name]);
    }
  }

  /**
   * Implements DrupalCacheInterface::clear().
   */
  function clear($cid = NULL, $wildcard = FALSE) {
    // Intercept temporary flush and do nothing.
    if (empty($cid)) {
      return;
    }

    $this->cache->clear($cid, $wildcard);
  }

  /**
   * Implements DrupalCacheInterface::isEmpty().
   */
  function isEmpty() {
    return $this->cache->isEmpty();
  }
}

/**
 * Triggers a backend request for the current request.
 */
function passive_request_trigger_refresh() {
  $triggered = &drupal_static(__FUNCTION__, FALSE);

  if (!$triggered) {
    $triggered = TRUE;
    list($request,) = passive_current_request();
    if (isset($request)) {
      // Check whether to try request now.
      if (passive_request_check_try_criteria($request)) {
        passive_request_try_refresh($request);
      }
      else {
        passive_request_queue_refresh($request);
      }
    }
  }
}

/**
 * Checks whether to dispatch the request.
 */
function passive_request_check_try_criteria($request) {
  /** @var callable $check */
  $check = variable_get('passive_request_check_try_criteria', 'passive_request_default_check_try_criteria');
  return $check($request);
}

/**
 * Checks default dispatch criteria.
 */
function passive_request_default_check_try_criteria($request) {
  // Check maximum try limit.
  $try_limit = variable_get('passive_request_try_limit', PASSIVE_REQUEST_TRY_LIMIT);
  $try = 0;
  if (isset($request['headers']['x-passive-try']) && is_numeric($request['headers']['x-passive-try'])) {
    // Cap at non-negative count.
    $try = max((int) $request['headers']['x-passive-try'], 0);
  }

  // Block exceeded tries.
  if ($try_limit >= 0 && $try >= $try_limit) {
    return FALSE;
  }

  // Allow verified backend refresh.
  if (passive_check_backend_refresh()) {
    return TRUE;
  }

  // Check if the request seems human.
  $human_ua_pattern = variable_get('passive_human_user_agent_pattern', PASSIVE_HUMAN_USER_AGENT_PATTERN);
  if (isset($request['headers']['user-agent']) && preg_match($human_ua_pattern, $request['headers']['user-agent'])) {
    // Try not to queue human requests.
    return TRUE;
  }

  // Queue by default.
  return FALSE;
}

/**
 * Re-dispatches the current request.
 */
function passive_request_try_refresh($request) {
  /** @var callable $try */
  $try = variable_get('passive_request_try_refresh', 'passive_request_default_try_refresh');
  return $try($request);
}

/**
 * Dispatches the current request while incrementing count.
 */
function passive_request_default_try_refresh($request) {
  // Dispatch request with quick timeout.
  $passive_timeout = variable_get('passive_timeout', PASSIVE_REQUEST_QUICK_TIMEOUT);

  $try = 0;
  if (isset($request['headers']['x-passive-try']) && is_numeric($request['headers']['x-passive-try'])) {
    // Cap at non-negative count.
    $try = max((int) $request['headers']['x-passive-try'], 0);
  }

  $request['headers']['x-passive-try'] = $try + 1;
  passive_request_dispatch($request, $passive_timeout);
}

/**
 * Queues a backend request.
 */
function passive_request_queue_refresh($request) {
  /** @var callable $queue */
  $queue = variable_get('passive_request_queue_refresh', 'passive_request_default_queue_refresh');
  return $queue($request);
}

/**
 * Adds backend request to Drupal cron queue.
 */
function passive_request_default_queue_refresh($request) {
  // Remove retry header.
  if (isset($request['headers']['x-passive-try'])) {
    unset($request['headers']['x-passive-try']);
  }

  // Add to passive queue.
  $phase = drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL, FALSE);
  if ($phase < DRUPAL_BOOTSTRAP_FULL) {
    return;
  }
  if (module_exists('passive')) {
    passive_queue_add(passive_request_index($request), $request);
  }
}

/**
 * Acquires a capacity lock.
 *
 * A capacity lock imposes concurrency limits and stampede protection for a
 * backend request.
 *
 * The concurrency limit forces the page to wait for the next available slot
 * until request timeout, in which case the request is queued for next refresh
 * without timeout (i.e. cron).
 *
 * Stampede protection is similar to the Memcache implementation in that the
 * lock fails for more than one request attempting to refresh the same cache.
 *
 * Attempting to acquire a capacity lock results in two possible outcomes:
 *
 * - The attempt fails if another backend request is refreshing the same cache.
 *   In this case, any existing stale cache should NOT expire.
 * - The attempt times out if no available slot can be found in the concurrency
 *   pool before timing out. The implementation must (re-)queue the request so
 *   it can be run again.
 *
 * This function delegates to an overridable implementation.
 *
 * NOTE: The $name argument is the name of the stampede protection lock.
 * Concurrency handling is internal to the implementation.
 *
 * WARNING: the page request may time out before this function returns.
 *
 * @return mixed
 *   Either an array of names of locks acquired for this capacity lock, or FALSE
 *   if the lock could not be acquired.
 */
function passive_backend_acquire_capacity_lock($name) {
  // Trigger shutdown refresh for current request.
  list($request, $signature) = passive_current_request();
  passive_shutdown_trigger_refresh(TRUE, $signature, $request);

  /** @var callable $acquire_lock */
  $acquire_lock = variable_get('passive_backend_acquire_capacity_lock', 'passive_backend_default_acquire_capacity_lock');
  $acquired = $acquire_lock($name);

  passive_shutdown_trigger_refresh(FALSE, $signature);
  return $acquired;
}

/**
 * Acquires a capacity lock.
 *
 * This default implementation behaves as specified in the entry function.
 *
 * The capacity lock, which is a combined stampede and concurrency lock, is
 * acquired using the following method:
 *
 * - For the duration of acquiring the lock, a stampede lock given by $name is
 *   maintained so that multiple requests do not compete for multiple
 *   concurrency locks. This should dramatically reduce stampede conflicts.
 * - Once the stampede lock is acquired, a concurrency lock is attempted. In
 *   this implementation, concurrency locks are a list of locks numbering to
 *   the limit.
 * - Each concurrency lock in the list is attempted in a round-robin fashion
 *   until a lock is acquired.
 * - Each attempt to acquire a concurrency lock renews the stampede lock.
 *
 * Both acquired locks are returned. Otherwise, FALSE is returned.
 *
 * @see passive_backend_acquire_capacity_lock()
 */
function passive_backend_default_acquire_capacity_lock($name) {
  $phase = drupal_bootstrap(DRUPAL_BOOTSTRAP_VARIABLES, FALSE);
  if ($phase < DRUPAL_BOOTSTRAP_VARIABLES) {
    return FALSE;
  }

  $lock_timeout = variable_get('passive_request_capacity_lock_timeout', PASSIVE_REQUEST_CAPACITY_LOCK_TIMEOUT);
  $concurrency_limit = variable_get('passive_request_concurrency_limit', PASSIVE_REQUEST_CONCURRENCY_LIMIT);
  $concurrency_index = 0;

  $concurrency_lock = &drupal_static('passive_request_concurrency_lock');

  $acquired = FALSE;
  while (!$acquired) {
    // Acquire (and extend) stampede lock.
    $stampede_lock_name = $name;
    if (!lock_acquire($stampede_lock_name, $lock_timeout)) {
      // Abort. Note that this can only occur in the first iteration, i.e.
      // another request has acquired the lock.
      break;
    }

    // Reuse existing concurrency lock. The first concurrency lock acquired in
    // this request is assumed to be the topmost lock, i.e. applicable to the
    // rest of this request.
    if ($concurrency_lock) {
      $acquired = array($stampede_lock_name);
    }
    // Attempt to acquire concurrency lock.
    else {
      $concurrency_lock_name = "passive_request_concurrency:$concurrency_index";
      if (lock_acquire($concurrency_lock_name, $lock_timeout)) {
        // Return acquired locks.
        $concurrency_lock = $concurrency_lock_name;
        $acquired = array($stampede_lock_name, $concurrency_lock_name);
      }
      else {
        usleep(100000);
        $concurrency_index = ($concurrency_index + 1) % $concurrency_limit;
      }
    }
  }

  return $acquired;
}

/**
 * Sets a request to trigger on shutdown.
 *
 * This is a fail-safe handler for an action that may time out the page.
 */
function passive_shutdown_trigger_refresh($trigger, $signature, $request = NULL) {
  // Abort during shutdown.
  if (drupal_static('_passive_shutdown_trigger_refresh')) {
    return;
  }

  // Track active requests to retry.
  $locks = &drupal_static(__FUNCTION__);

  if (!isset($locks)) {
    $locks = array();
    drupal_register_shutdown_function('_passive_shutdown_trigger_refresh');
  }

  if ($trigger) {
    $locks[$signature] = $request;
  }
  else {
    $locks[$signature] = FALSE;
  }
}

/**
 * Shutdown callback to trigger refresh on failure.
 */
function _passive_shutdown_trigger_refresh() {
  $shutting_down = &drupal_static(__FUNCTION__);
  $shutting_down = TRUE;

  // Retry marked requests.
  $locks = &drupal_static('passive_shutdown_trigger_refresh', array());

  foreach ($locks as $request) {
    if ($request !== FALSE) {
      passive_request_try_refresh($request);
    }
  }
}
