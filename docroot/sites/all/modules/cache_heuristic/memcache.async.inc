<?php
/**
 * @file
 * Implementation of cache.inc with memcache logic included.
 */

require_once __DIR__ . '/memcache.deferred.inc';

/**
 * Returns an Memcache object based on the bin requested. Note that there is
 * nothing preventing developers from calling this function directly to get the
 * Memcache object. Do this if you need functionality not provided by this API
 * or if you need to use legacy code. Otherwise, use the dmemcache (get, set,
 * delete, flush) API functions provided here.
 *
 * @param $bin The bin which is to be used.
 *
 * @param $flush Rebuild the bin/server/cache mapping.
 *
 * @return an Memcache object or FALSE.
 */
function dmemcache_object_new($bin = NULL, $flush = FALSE) {
  static $extension, $memcache_servers, $memcache_bins, $memcache_persistent, $failed_connection_cache;

  if (!isset($extension)) {
    // If an extension is specified in settings.php, use that when available.
    $preferred = variable_get('memcache_extension', NULL);
    if (isset($preferred) && class_exists($preferred)) {
      $extension = $preferred;
    }
    // If no extension is set, default to Memcache.
    // The Memcached extension has some features that the older extension lacks
    // but also an unfixed bug that affects cache clears.
    // @see http://pecl.php.net/bugs/bug.php?id=16829
    elseif (class_exists('Memcache')) {
      $extension = 'Memcache';
    }
    elseif (class_exists('Memcached')) {
      $extension = 'Memcached';
    }

    // Indicate whether to connect to memcache using a persistent connection.
    // Note: this only affects the Memcache PECL extension, and does not
    // affect the Memcached PECL extension.  For a detailed explanation see:
    //  http://drupal.org/node/822316#comment-4427676
    if (!isset($memcache_persistent)) {
      $memcache_persistent = variable_get('memcache_persistent', FALSE);
    }
  }

  if ($flush) {
    foreach ($memcacheCache as $cluster) {
      memcache_close($cluster);
    }
    $memcacheCache = array();
  }

  // $memcache_servers and $memcache_bins originate from settings.php.
  // $memcache_servers_custom and $memcache_bins_custom get set by
  // memcache.module. They are then merged into $memcache_servers and
  // $memcache_bins, which are statically cached for performance.
  if (empty($memcache_servers)) {
    // Values from settings.php
    $memcache_servers = variable_get('memcache_servers', array('127.0.0.1:11211' => 'default'));
    $memcache_bins    = variable_get('memcache_bins', array('cache' => 'default'));
  }

  // If there is no cluster for this bin in $memcache_bins, cluster is 'default'.
  $cluster = empty($memcache_bins[$bin]) ? 'default' : $memcache_bins[$bin];

  // If this bin isn't in our $memcache_bins configuration array, and the
  // 'default' cluster is already initialized, map the bin to 'cache' because
  // we always map the 'cache' bin to the 'default' cluster.
  if (empty($memcache_bins[$bin])) {
    $memcache_bins[$bin] = $memcache_bins['cache'];
  }

  // Create a new Memcache object. Each cluster gets its own Memcache object.
  if ($extension == 'Memcached') {
    $memcache = new Memcached();
    $default_opts = array(
      Memcached::OPT_COMPRESSION => FALSE,
      Memcached::OPT_DISTRIBUTION => Memcached::DISTRIBUTION_CONSISTENT,
    );
    foreach ($default_opts as $key => $value) {
      $memcache->setOption($key, $value);
    }
    // See README.txt for setting custom Memcache options when using the
    // memcached PECL extension.
    $memconf = variable_get('memcache_options', array());
    foreach ($memconf as $key => $value) {
      $memcache->setOption($key, $value);
    }
  }
  elseif ($extension == 'Memcache') {
    $memcache = new Memcache();
  }
  else {
    drupal_set_message(t('You must enable the PECL memcached or memcache extension to use memcache.inc.'), 'error');
    return;
  }
  // A variable to track whether we've connected to the first server.
  $init = FALSE;

  // Link all the servers to this cluster.
  foreach ($memcache_servers as $s => $c) {
    if ($c == $cluster && !isset($failed_connection_cache[$s])) {
      list($host, $port) = explode(':', $s);

      // Using the Memcache PECL extension.
      if ($memcache instanceof Memcache) {
        // Support unix sockets in the format 'unix:///path/to/socket'.
        if ($host == 'unix') {
          // When using unix sockets with Memcache use the full path for $host.
          $host = $s;
          // Port is always 0 for unix sockets.
          $port = 0;
        }
        // When using the PECL memcache extension, we must use ->(p)connect
        // for the first connection.
        if (!$init) {
          $track_errors = ini_set('track_errors', '1');
          $php_errormsg = '';

          if ($memcache_persistent && @$memcache->pconnect($host, $port)) {
            $init = TRUE;
          }
          elseif (!$memcache_persistent && @$memcache->connect($host, $port)) {
            $init = TRUE;
          }

          if (!empty($php_errormsg)) {
            register_shutdown_function('watchdog', 'memcache', 'Exception caught in dmemcache_object: !msg', array('!msg' => $php_errormsg), WATCHDOG_WARNING);
            $php_errormsg = '';
          }
          ini_set('track_errors', $track_errors);
        }
        else {
          $memcache->addServer($host, $port, $memcache_persistent);
        }
      }
      else {
        // Support unix sockets in the format 'unix:///path/to/socket'.
        if ($host == 'unix') {
          // Memcached expects just the path to the socket without the protocol
          $host = substr($s, 7);
          // Port is always 0 for unix sockets.
          $port = 0;
        }
        if ($memcache->addServer($host, $port) && !$init) {
          $init = TRUE;
        }
      }

      if (!$init) {
        // We can't use watchdog because this happens in a bootstrap phase
        // where watchdog is non-functional. Register a shutdown handler
        // instead so it gets recorded at the end of page load.
        register_shutdown_function('watchdog', 'memcache', 'Failed to connect to memcache server: !server', array('!server' => $s), WATCHDOG_ERROR);
        $failed_connection_cache[$s] = FALSE;
      }
    }
  }

  return empty($memcache) ? FALSE : $memcache;
}

/**
 * Retrieve multiple values from the cache.
 *
 * @param $keys The keys with which the items were stored.
 * @param $bin The bin in which the item was stored.
 *
 * @return The item which was originally saved or FALSE
 */
function dmemcache_get_delayed($keys, $bin = 'cache', $mc = NULL) {
  $collect_stats = dmemcache_stats_init();
  $multi_stats = array();

  $full_keys = array();

  foreach ($keys as $key => $cid) {
    $full_key = dmemcache_key($cid, $bin);
    $full_keys[$cid] = $full_key;

    if ($collect_stats) {
      $multi_stats[$full_key] = FALSE;
    }
  }

  $results = array();
  if ($mc || ($mc = dmemcache_object($bin))) {
    if ($mc instanceof Memcached) {
      return $mc->getDelayed($full_keys);
    }
    elseif ($mc instanceof Memcache) {
      $track_errors = ini_set('track_errors', '1');
      $php_errormsg = '';

      $results = @$mc->get($full_keys);

      if (!empty($php_errormsg)) {
        register_shutdown_function('watchdog', 'memcache', 'Exception caught in dmemcache_get_multi: !msg', array('!msg' => $php_errormsg), WATCHDOG_WARNING);
        $php_errormsg = '';
      }
      ini_set('track_errors', $track_errors);
    }
  }

  // If $results is FALSE, convert it to an empty array.
  if (!$results) {
    $results = array();
  }

  if ($collect_stats) {
    foreach ($multi_stats as $key => $value) {
      $multi_stats[$key] = isset($results[$key]) ? TRUE : FALSE;
    }
  }

  // Convert the full keys back to the cid.
  $cid_results = array();
  $cid_lookup = array_flip($full_keys);
  foreach ($results as $key => $value) {
    $cid_results[$cid_lookup[$key]] = $value;
  }
  return $cid_results;
}

/**
 * Memcache class extended to support Cache Heuristic.
 */
class MemCacheDrupalAsync extends MemCacheDrupalDeferred {
  static protected $extension;

  /**
   * Constructor.
   */
  public function __construct($bin) {
    parent::__construct($bin);

    if (!isset(self::$extension)) {
      self::$extension = $this->memcache ? get_class($this->memcache) : FALSE;
    }
  }
  /**
   * Implements DrupalCacheDeferredInterface::getMultipleDeferred().
   */
  public function getMultipleDeferred($cids) {
    if (self::$extension == 'Memcached') {
      $memcache = dmemcache_object_new($this->bin);
      $result = dmemcache_get_delayed($cids, $this->bin, $memcache);
      return $result ? $memcache : NULL;
    }
    return dmemcache_get_multi($cids, $this->bin, $this->memcache);
  }

  /**
   * Implements DrupalCacheDeferredInterface::prepareDeferredItem().
   */
  public function prepareDeferredItem($cid, $item) {
    return $this->prepareItem($item);
  }

  /**
   * Implements DrupalCacheDeferredInterface::fetchDeferredItems().
   */
  public function fetchDeferredItems($cids, $result) {
    if ($result instanceof Memcached) {
      $result = $result->fetchAll();
    }
    $return = array();
    $results = $result ? $result : array();
    foreach ($results as $result) {
      $cid = NULL;
      if ($result && !empty($result['value']) && isset($result['value']->cid)) {
        $cid = $result['value']->cid;
      }
      if (isset($cid) && $this->valid($cid, $result['value'])) {
        if (is_object($result['value']) && !empty($result['value']->multi_part_data)) {
          $result['value'] = _dmemcache_get_pieces($result['value']->data, $result['value']->cid, $this->bin, $this->memcache);
        }
        $return[$cid] = $result['value'];
      }
    }
    return $return;
  }

}
