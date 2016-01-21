<?php

/**
 * Class DropSharkSwapActivityCollector.
 */
class DropSharkSwapActivityCollector extends DropSharkCollectorBase {

  protected $events = array('system');
  protected $timespan = 3;

  /**
   * {@inheritdoc}
   */
  public function collect($event, $data = array()) {

    if (!$this->respond($event)) {
      return DropSharkCollector::RESPONSE_NONE;
    }

    $data = array('type' => 'swap_activity');
    $data['server'] = $this->getServer();

    $swap = $this->getData();

    if (!isset($swap['si']) || !isset($swap['so'])) {
      $data['code'] = 'unable_to_process_swap_activity';
      dropshark_queue_data($data);
      return DropSharkCollector::RESPONSE_COMPLETE;
    }

    $data['code'] = 0;
    $data['si'] = $swap['si'];
    $data['so'] = $swap['so'];

    dropshark_queue_data($data);
    return DropSharkCollector::RESPONSE_COMPLETE;
  }

  /**
   * Get swap activity information from the OS.
   *
   * @return array
   *  Swap activity information.
   */
  protected function getData() {
    static $data = NULL;

    if ($data === NULL) {
      $data = array();
      $cmd = array_map('trim', explode("\n", `vmstat {$this->timespan} 2`));
      array_shift($cmd);
      $header = explode(' ', $this->collapseData(array_shift($cmd)));

      if (array_diff(array('si', 'so'), $header)) {
        return $data;
      }
      $start = explode(' ', $this->collapseData(array_shift($cmd)));
      $end = explode(' ', $this->collapseData(array_shift($cmd)));
      if (!$start || !$end) {
        return $data;
      }
      $start = array_combine($header, $start);
      $end = array_combine($header, $end);
      $data['si'] = round(($end['si'] - $start['si']) / $this->timespan);
      $data['so'] = round(($end['so'] - $start['so']) / $this->timespan);
    }

    return $data;
  }

  protected function collapseData($data) {
    while (strpos($data, '  ') !== FALSE) {
      $data = str_replace('  ', ' ', $data);
    }
    return $data;
  }
}
