<?php

/**
 * @file
 * A class used for hierarchies.
 */

class Hierarchy extends Entity {

  /**
   * Entity ID.
   *
   * @var integer
   */
  public $hid = NULL;

  /**
   * The hierarchy type.
   *
   * @var string
   */
  public $type = '';

  /**
   * The hierarchy name.
   *
   * @var string
   */
  public $name = '';

  /**
   * The identifier of the user that created the hierarchy.
   *
   * @var int
   */
  public $uid = 0;

  /**
   * The Unix timestamp when the hierarchy was created.
   *
   * @var int
   */
  public $timestamp = 0;

  /**
   * Serialized array with misc options.
   *
   * @var array
   */
  public $data = array();

  public function __construct($values = array()) {
    if (!isset($values['uid']) && isset($values['user'])) {
      $values['uid'] = $values['user']->uid;
      unset($values['user']);
    }

    if (isset($values['type']) && is_object($values['type'])) {
      $values['type'] = $values['type']->type;
    }

    parent::__construct($values, 'hierarchy');

    if (!isset($this->uid)) {
      $this->uid = $GLOBALS['user']->uid;
    }

    if (empty($this->timestamp)) {
      $this->timestamp = time();
    }
  }

  protected function defaultUri() {
    return array('path' => 'admin/structure/hierarchy/' . $this->hid);
  }

  /**
   * Gets the hierarchy type entity.
   *
   * @return HierarchyType
   *   The hierarchy type.
   */
  public function getType() {
    return hierarchy_type_load($this->type);
  }

  /**
   * Returns the user that created the hierarchy.
   */
  public function user() {
    return user_load($this->uid);
  }

  /**
   * Sets a new user that created the hierarchy.
   *
   * @param object|int $account
   *   The user account object or the user account id (uid).
   */
  public function setUser($account) {
    $this->uid = is_object($account) ? $account->uid : $account;
  }

  public function buildContent($view_mode = 'full', $langcode = NULL) {
    $content = parent::buildContent($view_mode, $langcode);

    if (user_access('reorder hierarchies') && ($this->getType()->is_ordered)) {
      $content['hierarchy_items'] = drupal_get_form('hierarchy_reorder_items_form', $this, $langcode);
    }
    elseif (user_access('view hierarchies')) {
      $content['hierarchy_items'] = hierarchy_item_tree_content($this, $langcode);
    }

    return $content;
  }

  public function save() {
    if (empty($this->uuid)) {
      $this->uuid = hierarchy_generate_uuid();
    }

    // @todo Check correctness (uniqueness).
    parent::save();
  }

}
