<?php

/**
 * @file
 * ProfileAction for group assignment
 */

class ProfileFlagActionGroup extends ProfileFlagAction {
  public function actionNamespace() {
    return 'group';
  }

  function apply($uid = 0) {
    // Don't waste time if empty array.
    if (is_array($uid) && empty($uid)) return;

    $actionable = $this->actionable();

    // If $uid not set, do it to all users
    if ((empty($uid) && !is_array($uid)) || (!is_numeric($uid) && !is_array($uid))) {
      $uid = array();
      $result = db_query("SELECT uid FROM {users} WHERE 1 ORDER BY uid");
      foreach ($result as $obj) {
        $uid[] = $obj->uid;
      }
    }

    // If we are given an array of uids, break them down and do each individually.
    if (is_array($uid)) {
      foreach ($uid as $individual_uid) {
        $this->apply($individual_uid);
      }
    }
    elseif (is_numeric($uid)) {
      switch ($actionable) {
        case 'addto':
          foreach ($this->options as $gid) {
            // Check to see if the user already belongs to that group
            $num_rows = db_select('og_membership', 'om')
              ->condition('om.etid', $uid)
              ->condition('om.entity_type', 'user')
              ->condition('om.gid', $gid) // coming from $nid of group node
              ->countQuery()
              ->execute()
              ->fetchField();

            if (empty($num_rows)) {
              $id = db_insert('og_membership')
                ->fields(array(
                  'etid' => $uid,
                  'entity_type' => 'user',
                  'name' => 'og_membership_type_default',
                  'state' => 1,
                  'created' => REQUEST_TIME,
                  'gid' => $gid, // coming from $nid of group node
                ))
                ->execute();
            }
          }
          break;
        case 'removefrom':
          foreach ($this->options as $gid) {
            $num_deleted = db_delete('og_membership')
              ->condition('etid', $uid)
              ->condition('entity_type', 'user')
              ->condition('gid', $gid) // coming from $nid of group node
              ->execute();
          }
          break;
      }
    }
  }
}