<?php
/**
 * @file
 * Defines the OgFlagUser class.
 */

/**
 * OG Version of the flag_user class.
 */
class OgFlagUser extends flag_user implements OgFlagEntityInterface {
  /**
   * The original flag settings from the flag module.
   *
   * @var array
   */
  protected $defaultValues = array();

  /**
   * Indicates that this instance of the flag is overridden in the current context.
   *
   * @var bool
   */
  public $isOverridden = FALSE;

  /**
   * Indicates that the overrides have been applied to this instance.
   *
   * @var bool
   */
  public $overridesApplied = FALSE;

  /**
   * The OG Context applied to this instance.
   *
   * Is either FALSE for not in an og context or an array with the following structure:
   * array(
   *   'group_type' => The type of group,
   *   'gid' => The ID of the group,
   * )
   *
   * @var bool|array
   */
  public $ogContext = FALSE;

  /**
   * Determines if the user has permission to use the flag in the current context.
   *
   * @param string $action
   *   The flag action, either 'flag' or 'unflag'.
   *
   * @param object|null $account
   *   The user account performing the action.
   *
   * @return bool
   *   TRUE if the user has permission to use the flag.
   *   FALSE if the user does not have permission ot use the flag.
   */
  public function user_access($action = 'flag', $account = NULL) {
    $access = parent::user_access($action, $account);
    $access = og_flag_user_access_check($this, $access, $action, $account);

    return $access;
  }

  /**
   * Updates the flag for the context of the current request.
   */
  public function setContext($entity_id = NULL) {
    if ($this->overridesApplied) {
      $this->ResetOverrides();
    }

    $og_context = og_flag_get_og_context($this, $entity_id);
    if ($og_context) {
      og_flag_apply_overrides($og_context, $this);
    }
  }

  /**
   * Initializes og_flag specific variables.
   */
  public function OgFlagInit() {
    foreach ($this as $k => $v) {
      if ($k != 'defaultValues') {
        $this->defaultValues[$k] = $v;
      }
    }
  }

  /**
   * Overrides the base class implementation in order to insert the OG context.
   *
   * @param object $entity
   *   Entity that the flag applies to.
   *
   * @return int
   *   The ID of the entity.
   */
  public function get_content_id($entity) {
    $this->setContext($entity->uid);
    return parent::get_content_id($entity);
  }

  /**
   * Overrides the base class implementation in order to insert the OG context.
   *
   * @param string $action
   *   The flag operation being performed.
   *
   * @param int $content_id
   *   The ID of the content the flag operation is being performed on.
   *
   * @param object $account
   *   The user account object performing the action.
   *
   * @param bool $skip_permission_check
   *   Flag to bypass the permission checks.
   *
   * @return bool
   *   FALSE if some error occurred (e.g., user has no permission, flag isn't
   *   applicable to the item, etc.), TRUE otherwise.
   */
  public function flag($action, $content_id, $account = NULL, $skip_permission_check = FALSE) {
    $this->setContext($content_id);
    return parent::flag($action, $content_id, $account, $skip_permission_check);
  }

  /**
   * Resets the instance to it's default values.
   */
  protected function ResetOverrides() {
    foreach ($this->defaultValues as $k => $v) {
      if ($k != 'default_values') {
        $this->$k = $v;
      }
    }
  }
}
