<?php

/**
 * @file
 * Views handler override for field that exposes role user count.
 */


/**
 * @inheritdoc
 */
class architecture_role_users_views_handler extends views_handler_field {
  function render($values) {
    $value = $this->get_value($values);
    $uids = array();
    // Getting users by role is a bit complicated.
    $query = 'SELECT DISTINCT(ur.uid)
      FROM {users_roles} AS ur
      WHERE ur.rid IN (:rids)';
    $result = db_query($query, array(':rids' => array($value)));

    foreach ($result as $row) {
      $uids[] = $row->uid;
    }

    // Return the count of users with this role.
    return count($uids);
    }
}