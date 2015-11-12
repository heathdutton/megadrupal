<?php
/**
 * @file
 * Restore operation: role class.
 */

/**
 * The role restore operation class.
 */
class RestoreOperationRoles extends RestoreOperation {

  /**
   * Overriden RestoreOperation::item().
   */
  protected function item($key) {
    return $key;
  }

  /**
   * Overriden RestoreOperation::itemLabel().
   */
  protected function itemLabel($key, $item) {
    return $key;
  }

  /**
   * Overriden RestoreOperation::itemStatus().
   */
  protected function itemStatus($key, $item) {
    return user_role_load_by_name($key) ? RestoreScript::STATUS_ACTIVE : RestoreScript::STATUS_MISSING;
  }

  /**
   * Overriden RestoreOperation::itemRestore().
   */
  protected function itemRestore($key, $item) {
    $role = user_role_load_by_name($key);
    if (!$role) {
      $role = (object) array('name' => $key);
      user_role_save($role);
    }
  }

  /**
   * Overriden RestoreOperation::available().
   */
  public function available($filter = FALSE, $show_selected = FALSE) {
    $roles = array();

    foreach (user_roles() as $role) {
      if (
        (!$show_selected && $this->has($role)) ||
        ($filter && stripos($role, $filter) === FALSE)
      ) {
        continue;
      }

      $roles[$role] = $role;
    }

    return $roles;
  }
}
