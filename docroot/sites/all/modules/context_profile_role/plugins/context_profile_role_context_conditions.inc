<?php
/**
 * @file
 * Context condition plugin to provide a trigger for profile page role.
 */

/**
 * Expose the profile page role as the context condition.
 */
class ContextProfileRoleContextConditionProfileRole extends context_condition {

  /**
   * Condition available values.
   */
  public function condition_values() {
    $roles = user_roles(TRUE);

    $formatted_roles = array();
    foreach ($roles as $role) {
      $formatted_roles[$role] = $role;
    }
    return $formatted_roles;
  }

  /**
   * Condition form.
   */
  public function condition_form($context) {
    $defaults = $this->fetch_from_context($context, 'values');
    return array(
      '#title'         => $this->title,
      '#description'   => t('Select which profile role case should this condition activate.'),
      '#type'          => 'checkboxes',
      '#options'       => $this->condition_values(),
      '#default_value' => isset($defaults) ? $defaults : array(),
    );
  }

  /**
   * Execute.
   *
   * @param object $account
   *   The account being viewed.
   */
  public function execute($account) {
    if ($this->condition_used()) {
      foreach ($this->get_contexts() as $context) {
        // Get the context values.
        $profile_roles = $this->fetch_from_context($context, 'values');
        foreach ($profile_roles as $role) {
          if (in_array($role, $account->roles)) {
            $this->condition_met($context, $role);
          }
        }
      }
    }
  }

}
