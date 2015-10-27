<?php

/**
 * The abstract class for owner types
 */
class TemplateAPIRoleOwnerPlugin extends TemplateApiOwnerType {

  /**
   * Permits access to an action and item
   *
   * @abstract
   *
   * @param $op
   * @param Template $template
   *
   * @return Bool
   *
   */
  public function access($op, Template $template) {
    if (isset($template->owner)) {
      global $user;
      if (!array_key_exists($template->owner, $user->roles)) {
        return FALSE;
      }
    }
  }

  /**
   * Get the values that can be an owner
   *
   * @return array
   */
  public function values() {
    return user_roles(TRUE);
  }

  public function label(Template $template) {
    if (!empty($template->owner) && $role = user_role_load($template->owner)) {
      return $role->name;
    }
    return NULL;
  }
}
