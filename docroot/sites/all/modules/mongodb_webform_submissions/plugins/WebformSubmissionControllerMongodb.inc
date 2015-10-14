<?php

/**
 * @file
 * Defines the WebformSubmissionControllerMongodb class.
 */

class WebformSubmissionControllerMongodb implements WebformSubmissionController {

  public $controller_type = 'mongodb';

  public function getSubmissions($filters = array(), $header = NULL, $pager_from = NULL, $pager_count = 0) {
    $submissions = array();

    if (!is_array($filters)) {
      $filters = array('nid' => (int) $filters);
    }
    elseif (isset($filters['nid'])) {
      $filters['nid'] = (int) $filters['nid'];
    }

    if (isset($filters['sid'])) {
      $filters['sid'] = (int) $filters['sid'];
    }

    if (isset($filters['uid'])) {
      $filters['uid'] = (int) $filters['uid'];
    }

    if (isset($filters['uuid'])) {
      $filters['_id'] = $filters['uuid'];
      unset($filters['uuid']);
    }

    if (isset($filters['uid']) && $filters['uid'] === 0) {
      if (!empty($_SESSION['webform_submission'])) {
        $filters['sid'] = array('$in' => array_keys($_SESSION['webform_submission']));
      }
      else {
        $filters['sid'] = 0;
      }
    }

    $result = mongodb_collection('webform_submissions')->find($filters);

    if (is_array($header)) {
      $sort_fields = array();
      foreach($header as $field) {
        if (!empty($field['sort'])) {
          $sort_fields[$field['field']] = ($field['sort'] == 'asc' ? 1 : -1);
        }
      }
      $result->sort($sort_fields);
    }
    else {
      $result->sort(array('sid' => 1));
    }

    if ($pager_from) {
      $result->skip($pager_from);
    }

    if ($pager_count) {
      $result->limit($pager_count);
    }

    foreach ($result as $mongodb_id => $row) {
      $submission = new stdClass();
      $submission->_id = $mongodb_id;
      $submission->sid = $row['sid'];
      $submission->nid = $row['nid'];
      $submission->submitted = $row['submitted'];
      $submission->remote_addr = $row['remote_addr'];
      $submission->uid = $row['uid'];
      $submission->name = $row['name'];
      $submission->is_draft = $row['is_draft'];
      $submission->data = $row['data'];
      $submissions[$row['sid']] = $submission;
    }

    // If there are no submissions being retrieved, return an empty array.
    if (empty($submissions)) {
      return $submissions;
    }

    foreach (module_implements('webform_submission_load') as $module) {
      $function = $module . '_webform_submission_load';
      $function($submissions);
    }

    return $submissions;
  }

  public function getSubmissionCount($nid, $uid = NULL, $submission_interval = -1, $reset = FALSE) {
    static $counts;

    if (!isset($counts[$nid][$uid]) || $reset) {
      $search = array();
      $search['nid'] = (int) $nid;
      $search['is_draft'] = 0;
      if (!empty($uid)) {
        $search['uid'] = (int) $uid;
      }
      else {
        $submissions = isset($_SESSION['webform_submission']) ? $_SESSION['webform_submission'] : NULL;
        if ($submissions) {
          $search['sid'] = array('$in' => $submissions);
        }
        elseif ($uid === 0) {
          // Intentionally never match anything if the anonymous user has no
          // submissions.
          $counts[$nid][$uid] = 0;
          return $counts[$nid][$uid];
        }
      }
      if ($submission_interval != -1) {
        $search['submitted'] = array('$gt' => (REQUEST_TIME - $submission_interval));
      }

      $counts[$nid][$uid] = mongodb_collection('webform_submissions')->find($search)->count();
    }
    return $counts[$nid][$uid];
  }

  public function insertSubmission($node, $submission) {
    return $this->updateSubmission($node, $submission);
  }

  public function updateSubmission($node, $submission) {
    if (!empty($submission->sid)) {
      $search = array(
        'sid' => $submission->sid,
      );
    }
    else {
      $insert_sid = mongodb_next_id('webform_submissions');
      $search = array(
        'sid' => $insert_sid,
      );
      $submission->sid = $insert_sid;
    }
    if (!isset($submission->no)) {
      $submission->no = 0;
    }
    if (isset($submission->uuid)) {
      $submission->_id = $submission->uuid;
      unset($submission->uuid);
    }
    if (empty($submission->name)) {
      if (!empty($submission->uid) && $user = user_load($submission->uid)) {
        $submission->name = $user->name;
      }
      else {
        $submission->name = variable_get('anonymous', t('Anonymous'));
      }
    }
    mongodb_collection('webform_submissions')
      ->update($search, $submission, array('upsert' => TRUE));

    return $submission;
  }

  public function deleteSubmissions($node, $submission = FALSE) {
    $search = array(
      'nid' => (int) $node->nid,
    );
    if ($submission != FALSE) {
      $search['sid'] = (int) $submission->sid;
    }
    mongodb_collection('webform_submissions')->remove($search);
  }

  public function getPreviousSubmissionID($node, $submission) {
    $search = array(
      'nid' => (int) $node->nid,
      'sid' => array('$lt' => (int) $submission->sid),
    );
    $field = array('sid' => 1);
    $sort = array('sid' => -1);
    $result = mongodb_collection('webform_submissions')->find($search, $field)->sort($sort)->limit(1);
    foreach($result as $mongodb_id => $row) {
      return $row['sid'];
    }
  }

  public function getNextSubmissionID($node, $submission) {
    $search = array(
        'nid' => (int) $node->nid,
        'sid' => array('$gt' => (int) $submission->sid),
    );
    $field = array('sid' => 1);
    $sort = array('sid' => 1);
    $result = mongodb_collection('webform_submissions')->find($search, $field)->sort($sort)->limit(1);
    foreach($result as $mongodb_id => $row) {
      return $row['sid'];
    }
  }

  public function getComponentSubmissionData($component, $sids = array(), $fields = array('no', 'data')) {
    $search = array(
      'nid' => (int) $component['nid'],
      'data.' . $component['cid'] => array('$exists' => 1),
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
    $cursor = mongodb_collection('webform_submissions')->find($search, array('data.' . $component['cid'] => 1));
    $return = array();
    foreach($cursor as $entry) {
      foreach($entry['data'][$component['cid']]['value'] as $row_number => $data) {
        $result = array();
        $result['data'] = $data;
        if ($get_row_number) {
          $result['no'] = $row_number;
        }
        $return[] = $result;
      }
    }
    return $return;
  }

  public function deleteComponentSubmissionData($component) {
    $search = array(
      'nid' => $component['nid'],
    );
    $remove = array(
      '$unset' => array(
        'data.' . $component['cid'] => 1,
      )
    );
    mongodb_collection('webform_submissions')->update($search, $remove, array('multiple' => TRUE));
  }

  public function validateComponentSubmissionUnique($nid, $cid, $value, $sid = FALSE) {
    $search = array(
      'nid' => (int) $nid,
      'data.' . $cid => $value,
    );
    if ($sid != FALSE) {
      $search['sid'] = array('$ne' => (int) $sid);
    }
    return (mongodb_collection('webform_submissions')->count($search) == 0);
  }

  public function getDraftSubmissionID($nid, $uid) {
    $search = array(
      'nid' => (int) $nid,
      'uid' => (int) $uid,
      'is_draft' => 1,
    );
    $fields = array('sid' => 1);
    $sort_order = array('submitted' => -1);
    $result = mongodb_collection('webform_submissions')->find($search, $fields)->sort($sort_order)->limit(1);
    foreach ($result as $mid => $row) {
      return $row['sid'];
    }
  }
}
