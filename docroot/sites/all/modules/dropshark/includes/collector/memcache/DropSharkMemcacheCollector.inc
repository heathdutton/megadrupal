<?php

class DropSharkMemcacheCollector extends DropSharkCollectorBase {

  protected $events = array('system');

  /**
   * {@inheritdoc}
   */
  public function collect($event, $data = array()) {

    if (!$this->respond($event)) {
      return DropSharkCollector::RESPONSE_NONE;
    }

    $data = array('type' => 'memcache');

    if (!module_exists('memcache')) {
      $data['code'] = 'memcache_module_not_exists';
      dropshark_queue_data($data);
      return DropSharkCollector::RESPONSE_COMPLETE;
    }

    $mc = dmemcache_object('default');
    if ($mc instanceof Memcached) {
      $extension = 'Memcached';
    } elseif ($mc instanceof Memcache) {
      $extension = 'Memcache';
    } else {
      $data['code'] = 'unknown_pecl_extension';
      dropshark_queue_data($data);
      return DropSharkCollector::RESPONSE_COMPLETE;
    }

    $stats = dmemcache_stats('cache', 'default', TRUE);
    $stats = $stats['cache'];

    if (empty($stats)) {
      $data['code'] = 'memcache_module_config';
      dropshark_queue_data($data);
      return DropSharkCollector::RESPONSE_COMPLETE;
    }

    $total = array_pop($stats);
    $data['extension'] = $extension;
    $data['version'] = phpversion($extension);
    $data['code'] = DropSharkCollector::STATUS_SUCCESS;
    $data['total'] = $total;
    $data['servers'] = $stats;

    dropshark_queue_data($data);
    return DropSharkCollector::RESPONSE_COMPLETE;
  }

}
