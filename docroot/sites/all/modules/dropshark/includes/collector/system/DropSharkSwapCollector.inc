<?php

/**
 * Class DropSharkSwapCollector.
 */
class DropSharkSwapCollector extends DropSharkMemoryCollector {

  protected $events = array('system');

  /**
   * {@inheritdoc}
   */
  public function collect($event, $data = array()) {

    if (!$this->respond($event)) {
      return DropSharkCollector::RESPONSE_NONE;
    }

    $data = array('type' => 'swap');
    $data['server'] = $this->getServer();

    if (!$this->hasLinfo()) {
      $data['code'] = 'linfo_library_not_available';
      dropshark_queue_data($data);
      return DropSharkCollector::RESPONSE_COMPLETE;
    }

    $memory = $this->getData();

    if (empty($memory['swapInfo'][0])) {
      $data['code'] = 'unable_to_determine_swap';
      dropshark_queue_data($data);
      return DropSharkCollector::RESPONSE_COMPLETE;
    }

    $swap = $memory['swapInfo'][0];

    $data['code'] = 0;
    $data['used'] = $swap['used'];
    $data['size'] = $swap['size'];
    $data['used_percent'] = $swap['size'] ? $swap['used'] / $swap['size'] : NULL;

    dropshark_queue_data($data);
    return DropSharkCollector::RESPONSE_COMPLETE;
  }

}
