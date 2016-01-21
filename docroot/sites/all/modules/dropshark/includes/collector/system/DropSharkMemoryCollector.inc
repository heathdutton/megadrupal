<?php

class DropSharkMemoryCollector extends DropSharkLinfoCollector {

  protected $events = array('system');

  /**
   * {@inheritdoc}
   */
  public function collect($event, $data = array()) {

    if (!$this->respond($event)) {
      return DropSharkCollector::RESPONSE_NONE;
    }

    $data = array('type' => 'memory');
    $data['server'] = $this->getServer();

    if (!$this->hasLinfo()) {
      $data['code'] = 'linfo_library_not_available';
      dropshark_queue_data($data);
      return DropSharkCollector::RESPONSE_COMPLETE;
    }

    $memory = $this->getData();

    if (empty($memory)) {
      $data['code'] = 'unable_to_determine_memory';
      dropshark_queue_data($data);
      return DropSharkCollector::RESPONSE_COMPLETE;
    }

    $data['code'] = 0;
    $data['free'] = $memory['free'];
    $data['total'] = $memory['total'];
    $data['used_percent'] = 1 - $memory['free'] / $memory['total'];

    dropshark_queue_data($data);
    return $data;
  }

  /**
   * Get memory usage information from the OS.
   *
   * @return array
   *  Memory usage information, keyed by type ("Mem:", "Swap:").
   */
  protected function getData() {
    static $data = NULL;

    if ($data === NULL) {
      $data = $this->parser->getRam();
    }

    return $data;
  }

}
