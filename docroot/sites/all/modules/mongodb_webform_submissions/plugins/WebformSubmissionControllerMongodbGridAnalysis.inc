<?php

/**
 * @file
 * Defines the WebformSubmissionControllerMongodbGridAnalysis class.
 */

class WebformSubmissionControllerMongodbGridAnalysis extends WebformSubmissionControllerMongodb {

  public function getComponentSubmissionData($component, $sids = array(), $fields = array('no', 'data')) {
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
    if (in_array('no', $fields)) {
      $get_row_number = TRUE;
    }
    else {
      $get_row_number = FALSE;
    }
    $submission = mongodb_collection('webform_submissions')->findOne($search, array('data.' . $component['cid'] => 1));
    $return = array();
    foreach ($submission['data'][$component['cid']] as $field => $data) {
      $distinct_cursor = mongodb_collection('webform_submissions')->distinct('data.' . $component['cid'] . '.' . $field, $search);
      foreach ($distinct_cursor as $distinct_value) {
        $value_search = $search;
        unset($value_search['data.' . $component['cid']]);
        $value_search['data.' . $component['cid'] . '.' . $field] = $distinct_value;
        $value_count_cursor = mongodb_collection('webform_submissions')->find($value_search);
        $result = new stdClass();
        $result->data = $distinct_value;
        $result->no = $field;
        $result->datacount = $value_count_cursor->count();
        $return[] = $result;
      }
    }
    return $return;
  }

}
