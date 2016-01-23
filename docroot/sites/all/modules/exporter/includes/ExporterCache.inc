<?php
/**
 * Author: Vojtěch Kusý <vojtech.kusy@gmail.com> 
 * Date: 18.7.13
 * Time: 17:35
 */

class ExporterCache {

  protected $exporter;
  protected $table;

  static function flushAll() {
    cache_clear_all('*', 'cache_exporter', TRUE);
  }

  function __construct(Exporter $exporter) {
    $this->table = 'cache_exporter';
    $this->exporter = $exporter;
  }

  function set($id, $data) {
    $expire = CACHE_PERMANENT;
    cache_set($this->getKey($id), $data, $this->table, $expire);
  }

  function get($id) {
    $cache = cache_get($this->getKey($id), $this->table);
    return $cache ? $cache->data : NULL;
  }

  function invalidate($id) {
    cache_clear_all($this->getKey($id), $this->table);
  }

  function flush() {
    cache_clear_all($this->exporter->id . ':', $this->table, TRUE);
  }

  protected function getKey($id) {
    return $this->exporter->id . ':' . $id;
  }

}
