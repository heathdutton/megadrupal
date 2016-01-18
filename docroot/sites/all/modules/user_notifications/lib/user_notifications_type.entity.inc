<?php

/**
 * The class used for model type entities
 */
class UserNotificationsType extends Entity {

  public $name;
  public $label;
  public $locked;

  public function __construct($values = array()) {
    parent::__construct($values, 'user_notifications_type');
  }

  /**
   * Save notification type.
   *
   * Create a recipient uid field for each notification type.
   *
   * @see entity_save()
   */
  public function save() {
    if (parent::save()) {
      user_notifications_configure_notification_type($this->name);
    }
  }

}
