<?php
/**
 * @file
 * The PaasCache Implementation
 */

class PaasCache implements DrupalCacheInterface {
  protected $has_profiler_cache;
  protected $cache;
  protected $cache_objects;

  public function __construct($bin) {
    $this->cache = $this->getCacheObject($bin); 
  }

  public function getCacheObject($bin) {
    if (!isset($this->cache_objects[$bin])) {
      $class = variable_get('cache_class_' . $bin);
      if (!isset($class)) {
        $class = variable_get('cache_class_before_paas', 'DrupalDatabaseCache');
      }
      $this->cache_objects[$bin] = new $class($bin);
    }
    return $this->cache_objects[$bin];
  }

  public function hasProfilerCache() {
    if (!isset($this->has_profiler_cache)) {
      $this->has_profiler_cache = function_exists('paas_metric_profiler_cache');
    }
    return $this->has_profiler_cache;
  }

  public function paas($string) {
    if (!$this->hasProfilerCache()) return;
    paas_metric_profiler_cache($string);
  }

  public function get($cid) {
    $cids = array($cid);
    $cache = $this->getMultiple($cids);
    if (empty($cache) || !isset($cache[$cid])) {
      return FALSE;
    }
    return $cache[$cid];
  }

  public function isEmpty() {
    return $this->cache->isEmpty();
  }

  public function getMultiple(&$cids) {
    $output = $this->cache->getMultiple($cids);
    
    if (!empty($output)) {
      $this->paas('hit');
    }
    else {
      $this->paas('miss');
    }

    return $output;
  }

  public function clear($cid = NULL, $wildcard = FALSE) {
    $this->cache->clear($cid, $wildcard);    
    $this->paas('clear');
  } 


  public function set($cid, $data, $expire = CACHE_PERMANENT) {
    $this->cache->set($cid, $data, $expire);
    $this->paas('set');
  }
}
