<?php

class DropSharkDrupalUpdateStatusCollector extends DropSharkCollectorBase {

  protected $events = array('drupal');

  /**
   * {@inheritdoc}
   */
  public function collect($event, $data = array()) {

    if (!$this->respond($event)) {
      return DropSharkCollector::RESPONSE_NONE;
    }

    $data = array('type' => 'drupal_updates');

    if (!module_exists('update')) {
      $data['code'] = 'update_not_enabled';
      dropshark_queue_data($data);
      return DropSharkCollector::RESPONSE_COMPLETE;
    }

    $data['projects'] = array();
    $data['status'] = array();

    if ($available = update_get_available(TRUE)) {
      module_load_include('inc', 'update', 'update.compare');
      $info = update_calculate_project_data($available);

      foreach ($info as $project) {
        $data['status'][$project['status']][] = $project['name'];
        $data['projects'][$project['name']] = $project['existing_version'];
      }
      $data['code'] = 0;
    }
    else {
      $data['code'] = 'unable_to_get_update_data';
    }

    dropshark_queue_data($data);
    return DropSharkCollector::RESPONSE_COMPLETE;
  }

}
