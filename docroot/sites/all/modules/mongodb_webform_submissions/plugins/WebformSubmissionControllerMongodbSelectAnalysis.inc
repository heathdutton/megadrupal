<?php

/**
 * @file
 * Defines the WebformSubmissionControllerMongodbSelectAnalysis class.
 */

class WebformSubmissionControllerMongodbSelectAnalysis extends WebformSubmissionControllerMongodb {

  public function getComponentSubmissionDataExtended($component, $sids = array(), $single = FALSE) {
    $options = array_keys(_webform_select_options($component, TRUE));
    array_walk($options,
      function (&$value) {
        $value = (string) $value;
      }
    );
    $options_operator = $single ? '$nin' : '$in';
    $search = array(
      'nid' => (int) $component['nid'],
      'data.' . $component['cid'] => array(
        '$ne' => '',
        $options_operator => $options,
      )
    );
    if (count($sids)) {
      $search['sid'] = array('$in' => $sids);
    }
    $cursor = mongodb_collection('webform_submissions')->distinct('data.' . $component['cid'], $search);
    $return = array();
    foreach ($cursor as $distinct_value) {
      $result = array();
      $value_search = $search;
      $value_search['data.' . $component['cid']] = $distinct_value;
      $value_count_cursor = mongodb_collection('webform_submissions')->find($value_search);
      $result['data'] = $distinct_value;
      $result['datacount'] = $value_count_cursor->count();
      $return[] = $result;
    }
    return $return;
  }

  public function getComponentSubmissionDataCount($component, $sids = array()) {
    $search = array(
      'nid' => (int) $component['nid'],
      'data.' . $component['cid'] => array(
        '$exists' => 1,
        '$ne' => '',
      )
    );
    if (count($sids)) {
      $search['sid'] = array('$in' => $sids);
    }
    $cursor = mongodb_collection('webform_submissions')->find($search, array('data.' . $component['cid'] => 1));
    $return['datacount'] = $cursor->count();
    return $return;
  }

}
