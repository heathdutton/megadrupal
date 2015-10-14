<?php
/**
 * @file
 * SendinBlue Signup entity class file.
 */

/**
 * Wrapper of sendinblue entity.
 */
class SendinBlueSignup extends Entity {
  public $mcsId;
  public $name;
  public $mcLists;
  public $mode;
  public $title;
  public $settings;

  /**
   * Override parent constructor to set the entity type.
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'sendinblue_signup');
  }

  /**
   * Return a label for a signup form.
   */
  public function label() {
    return $this->title;
  }

  /**
   * Overrides Entity\Entity::uri().
   */
  public function uri() {
    return array(
      'path' => 'admin/config/system/sendinblue/signup/manage/' . $this->name,
      'options' => array(
        'entity_type' => $this->entityType,
        'entity' => $this,
      ),
    );
  }
}
