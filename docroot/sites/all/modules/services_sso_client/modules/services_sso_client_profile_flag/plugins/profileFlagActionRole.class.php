<?php

/**
 * @file
 * ProfileAction for role assignment
 */

class ProfileFlagActionRole extends ProfileFlagAction {
  public function actionNamespace() {
    return 'role';
  }

  function apply($uid = 0) {
    // Don't waste time if empty array.
    if (is_array($uid) && empty($uid)) return;

    $actionable = $this->actionable();

    // If uid not set, do it to all users
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
          foreach ($this->options as $rid) {
            $count = db_select('users_roles', 'ur')
              ->fields('ur')
              ->condition('uid', $uid)
              ->condition('rid', $rid)
              ->countQuery()
              ->execute()
              ->fetchField();
            if (empty($count)) {
              $num_inserted = db_insert('users_roles')
               ->fields(array(
                'uid' => $uid,
                'rid' => $rid,
              ))
              ->execute();
            }
          }
          break;
        case 'removefrom':
          foreach ($this->options as $rid) {
            $num_deleted = db_delete('users_roles')
              ->condition('uid', $uid)
              ->condition('rid', $rid)
              ->execute();
          }
          break;
      }
    }
  }
}