<?php
namespace Drupal\state_flow_sps\Test;

use \Drupal\state_flow_sps\StateFlowOverride;

class StateFlowTestOverride extends StateFlowOverride {
  /**
   * Override the getOverrides function to do nothing but
   * call processOverrides so we can test it.
   */
  public function getOverrides() {
    return $this->processOverrides();
  }

  /**
   * Provide an easy way to set data for processOverrides to deal with
   */
  public function setResults($data) {
    $this->results = $data;
  }
}