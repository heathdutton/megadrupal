<?php

/**
 * The class used for user notification state entities
 */
class UserNotificationsState extends Entity {

  public $name, $label, $description, $default_state,
    $active, $show_on_form, $weight;

  public function __construct($values = array()) {
    parent::__construct($values, 'user_notifications_state');
  }
}
