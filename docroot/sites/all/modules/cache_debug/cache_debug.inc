<?php

class DrupalDebugCache implements DrupalCacheInterface {

  protected $bin;

  protected $cacheHandler;

  protected $loaded = FALSE;

  protected $config = array(
    'log' => array(
      'get' => FALSE,
      'set' => TRUE,
      'clear' => TRUE,
      'getMultiple' => FALSE,
      'init' => FALSE,
    ),
    'stacktrace' => array(
      'get' => FALSE,
      'set' => FALSE,
      'clear' => FALSE,
      'getMultiple' => FALSE,
      'init' => FALSE,
    ),
  );

  protected $log_filepath;

  /**
   * Implements interface DrupalCacheInterface::__construct().
   */
  public function __construct($bin) {
    $this->bin = $bin;
    $this->loadConfiguration();
  }

  /**
   * Load Configuration.
   *
   * As this cache class may load before the variables system is loaded,
   * the configuration stored in Drupal variables may not be available immediately
   * so instead we'll have to assume defaults until we've bootstrapped past
   * that stage.
   */
  protected function loadConfiguration() {
    if (!$this->loaded && (drupal_bootstrap() > DRUPAL_BOOTSTRAP_VARIABLES)) {
      foreach ($this->config as $type => $actions) {
        foreach ($actions as $action => $setting) {
          $local_key  = implode('_', array('cache_debug', $this->bin, $type, $action));
          $this->config[$type][$action] = variable_get($local_key, NULL);

          if ($this->config[$type][$action] === NULL) {
            $global_key = implode('_', array('cache_debug', $type, $action));
            $this->config[$type][$action] = variable_get($global_key, FALSE);
          }
        }
      }

      $this->config['common'] = variable_get('cache_debug_common_settings', array());
      $this->log_filepath = variable_get('cache_debug_log_filepath', '/tmp/cachedebug.log');
      $this->loaded = TRUE;
    }
    return $this->config;
  }

  /**
   * Get the real cache handler to execute to cache call.
   */
  protected function cacheHandler() {
    if ($this->cacheHandler) {
      return $this->cacheHandler;
    }
    // Cannot use variable_get as this system may have not loaded yet.
    global $conf;
    if (isset($conf['cache_debug_class_' . $this->bin])) {
      $class = $conf['cache_debug_class_' . $this->bin];
    }
    elseif (isset($conf['cache_debug_class'])) {
      $class = $conf['cache_debug_class'];
    }
    else {
      $class = 'DrupalDatabaseCache';
    }
    $this->log('init', $this->bin . ' uses ' . $class);
    $this->cacheHandler = new $class($this->bin); 
    return $this->cacheHandler;
  }

  /**
   * Log the debugging done.
   */
  protected function log($action, $cid, $extra = '', $force = FALSE) {
    if (!$this->loaded) {
      $this->loadConfiguration();
    }
    $force_stacktrace = FALSE;
    $key = implode('->', array($this->bin, $action, $cid));

    if (!empty($this->config['common']['variables']) && $key == 'cache_bootstrap->clear->variables') {
      $force_stacktrace = $force = TRUE;
    }
    elseif (!empty($this->config['common']['theme_registry']) && $key == 'cache->clear->theme_registry') {
      $force_stacktrace = $force = TRUE;
    }

    // Allow certain cache actions to not be logged to reduce verbosity.
    if (!$force && !$this->config['log'][$action]) {
      return FALSE;
    }
    $log = date("[Y-m-d G:i:s]", REQUEST_TIME) . ' ' . $key . ' ' . $extra . PHP_EOL;

    // Allow stack tracing to be switched on and off per action type.
    if ($this->config['stacktrace'][$action] || $force_stacktrace) {
      $backtrace = debug_backtrace();
      array_shift($backtrace);
      $stack = array();
      foreach ($backtrace as $trace) {
        $stack[] =  (isset($trace['class']) ? $trace['class'] . '::' : '' ) . $trace['function'];
      }
      $log .= "\tstack: " . implode("\n\tstack: ", $stack) . PHP_EOL;
    }

    switch (variable_get('cache_debug_logging_destination', 'error_log')) {
      case 'watchdog':
        watchdog('cache_debug', $log, array(), WATCHDOG_DEBUG);
        break;
      case 'error_log':
        error_log($log);
        break;
      case 'file':
      default:
        if (!$this->loaded) {
          error_log($log);
          break;
        }
        file_put_contents($this->log_filepath, $log, FILE_APPEND); 
        break;
    }
  }
  
  /**
   * Implements interface DrupalCacheInterface::get().
   */
  public function get($cid) {
    $this->log(__FUNCTION__, $cid);
    return $this->cacheHandler()->get($cid);
  }  

 /**
   * Implements interface DrupalCacheInterface::getMultiple().
   */
  public function getMultiple(&$cids) {
    $this->log(__FUNCTION__, implode(',', $cids));
    return $this->cacheHandler()->getMultiple($cids);
  }

  /**
   * Implements interface DrupalCacheInterface::set().
   */
  public function set($cid, $data, $expire = CACHE_PERMANENT) {
    if (!function_exists('format_size')) {
      require_once DRUPAL_ROOT . '/includes/common.inc';
    }
    $this->log(__FUNCTION__, $cid, 'size=' . format_size(strlen(serialize($data))) . ' expire=' . $expire);
    return $this->cacheHandler()->set($cid, $data, $expire);
  }

  /**
   * Implements interface DrupalCacheInterface::clear().
   */
  public function clear($cid = NULL, $wildcard = FALSE) {
    $this->log(__FUNCTION__, $cid, 'wildcard=' . $wildcard);
    return $this->cacheHandler()->clear($cid, $wildcard);
  }

  /**
   * Implements interface DrupalCacheInterface::isEmpty().
   */
  public function isEmpty() {
    return $this->cacheHandler()->isEmpty();
  }
}
