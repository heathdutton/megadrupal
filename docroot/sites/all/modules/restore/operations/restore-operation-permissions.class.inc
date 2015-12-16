<?php
/**
 * @file
 * Restore operation: permissions class.
 */

/**
 * The permission restore operation class.
 */
class RestoreOperationPermissions extends RestoreOperation {

  /**
   * Overriden RestoreOperation::item().
   */
  protected function item($key) {
    list($role, $permission) = explode(':', $key);

    $granted = FALSE;
    $role_obj = user_role_load_by_name($role);
    if ($role_obj) {
      $role_permissions = user_role_permissions(array($role_obj->rid => $role_obj->rid));
      $granted = $role_permissions[$role_obj->rid][$permission];
    }

    return array(
      'role' => $role,
      'permission' => $permission,
      'granted' => $granted,
    );
  }

  /**
   * Overriden RestoreOperation::itemLabel().
   */
  protected function itemLabel($key, $item) {
    return "{$item['role']} - {$item['permission']} (" . ($item['granted'] ? t('granted') : t('revoked')) . ')';
  }

  /**
   * Overriden RestoreOperation::itemStatus().
   */
  protected function itemStatus($key, $item) {
    $role = user_role_load_by_name($item['role']);
    if ($role) {
      $role_permissions = user_role_permissions(array($role->rid => $role->rid));
      if (isset($role_permissions[$role->rid][$item['permission']]) && $role_permissions[$role->rid][$item['permission']] == $item['granted']) {
        return RestoreScript::STATUS_ACTIVE;
      }
      else {
        return RestoreScript::STATUS_OVERRIDEN;
      }
    }
    else {
      return RestoreScript::STATUS_MISSING;
    }
  }

  /**
   * Overriden RestoreOperation::itemRestore().
   */
  protected function itemRestore($key, $item) {
    $role = user_role_load_by_name($item['role']);
    if ($role) {
      user_role_grant_permissions($role->rid, $item['permission']);
    }
  }

  /**
   * Overriden RestoreOperation::available().
   */
  public function available($filter = FALSE, $show_selected = FALSE) {
    $permissions = array();

    foreach (user_roles() as $rid => $role) {
      $role_permissions = user_role_permissions(array($rid => $rid));
      foreach ($role_permissions[$rid] as $permission => $granted) {
        if (
          (!$show_selected && $this->has("{$role}:{$permission}")) ||
          ($filter && stripos("{$role} - {$permission}", $filter) === FALSE)
        ) {
          continue;
        }

        $permissions["{$role}:{$permission}"] = "{$role} - {$permission} (" . ($granted ? t('granted') : t('revoked')) . ')';
      }
    }

    return $permissions;
  }
}
