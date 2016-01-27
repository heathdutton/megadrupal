<?php
namespace Drupal\state_flow_sps;

use \Drupal\sps\Plugins\Override\NodeDateOverride;

class StateFlowOverride extends NodeDateOverride {
  protected $results = array();
  /**
   * Returns a list of vid's to override the default vids to load.
   *
   * @return
   *  An array of override vids.
   */
  public function getOverrides() {
    $select = db_select('state_flow_schedule', 'sfs')
      ->fields('sfs', array('sid', 'nid', 'vid'))
      ->condition('date', $this->timestamp, '<=')
      ->orderBy('date')
      ->orderBy('vid');

    $this->results = $select->execute()->fetchAllAssoc('sid', \PDO::FETCH_ASSOC);

    return $this->processOverrides();
  }

  protected function processOverrides() {
    $list = array();
    foreach($this->results as $key => $result) {
      if (isset($result['nid'])) {
        $result = array($result);
      }

      foreach($result as $sub => $row) {
        $transform = array();
        $transform['id'] = $row['nid'];
        $transform['type'] = 'node';
        $transform['revision_id'] = $row['vid'] == 0 ? NULL : $row['vid'];
        $transform['status'] = $row['vid'] > 0 ? 1 : 0;
        $list['node-' . $row['nid']] = $transform;
      }
    }

    return $list;
  }
}