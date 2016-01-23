<?php

/**
 * @file
 * Entity hooks and callbacks for user notifications.
 */

/**
 * Main class for User Notification entities.
 */
class UserNotification extends Entity {

  public
    $user_notifications_id,
    $label,
    $type,
    $author_uid,
    $tag,
    $state,
    $wrapper;

  /**
   * Constructor.
   *
   * @see Entity::__construct()
   */
  public function __construct(array $values = array(), $entityType = NULL) {
    parent::__construct($values, $entityType);
    $this->wrapper = entity_metadata_wrapper($this->entityType, $this);
  }

  /**
   * Save notification.
   *
   * @see entity_save()
   */
  public function save() {
    // Set the product's uid if it's being created at this time.
    if (empty($this->author_uid)) {
      global $user;
      $this->author_uid = $user->uid;
    }

    // Set a default state if not provided.
    $wrapper = entity_metadata_wrapper('user_notifications', $this);
    $state = $wrapper->state->value();
    if (!$state) {
      $default_state = user_notifications_get_default_state();
      if ($default_state) {
        $this->state = $default_state->identifier();
      }
    }

    $this->updated = REQUEST_TIME;

    if (!$this->user_notifications_id && empty($this->created)) {
      $this->created = REQUEST_TIME;
    }
    return parent::save();
  }

  /**
   * Specify URI.
   */
  protected function defaultUri() {
    return array('path' => 'user-notifications/' . $this->internalIdentifier());
  }

  /**
   * Display a user notification.
   *
   * When viewing a notification, if it is still in the default state, then we
   * mark it as seen if the user notification state seen exists.
   *
   * TODO: This should be configurable and we should not assume notices should
   * be marked as seen.
   *
   * @see Entity::view()
   */
  public function view($view_mode = 'full', $langcode = NULL, $page = NULL) {
    if ($this->is_default_state()) {
      $this->update_state('seen');
    }
    return parent::view($view_mode = 'full', $langcode = NULL, $page = NULL);
  }

  /**
   * Updates the state of a user notification after checking to confirm it
   * exists.
   *
   * @return
   *  Boolean TRUE if the update was successful or FALSE if the update was
   *  unsuccessful.
   */
  public function update_state($state = NULL) {
    if ($this->does_state_exist($state)) {
      $this->state = $state;
      $this->save();
      return TRUE;
    } else {
      return FALSE;
    }
  }

  /**
   * Check whether the user notification is in the default state.
   *
   * @return
   *  Boolean TRUE if the User Notification is in the default state and FALSE if
   *  not.
   */
  public function is_default_state() {
    $default_state = user_notifications_get_default_state();
    return ($default_state && $this->state == $default_state->identifier()) ? TRUE : FALSE;
  }

  /**
   * Check whether a specific user notification state exists.
   *
   * @return
   *  Boolean TRUE if the state exists, FALSE if not.
   */
  public function does_state_exist($state) {
    $entity_type = 'user_notifications_state';
    $query = new EntityFieldQuery();
    $query
      ->entityCondition('entity_type', $entity_type)
      ->propertyCondition('name', $state)
      ->propertyCondition('active', 1);
    $result = $query->execute();
    return (isset($result[$entity_type])) ? $result[$entity_type] : FALSE;
  }
}
