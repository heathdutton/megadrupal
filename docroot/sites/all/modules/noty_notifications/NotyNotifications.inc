<?php

/**
 * Class defines an notifications object
 */
class NotyNotifications {
  // User ID.
  public $uid = NULL;

  /**
   * constructor
   * @param $uid
   */
  public function __construct() {
    global $user;

    $this->uid = $user->uid;
    $this->getPendingNotifications();
  }

  /**
   * Get pending notifications for $uid from database.
   */
  private function getPendingNotifications() {
    $query  = db_select('noty_notifications', 'n');
    $query->fields('n');
    $query->condition('n.uid', $this->uid);
    $query->condition('n.status', 0);
    $query->orderBy('n.created', 'DESC');
    $notifications_data = $query->execute()->fetchAll();

    foreach ($notifications_data as &$notification_data) {
      $notification_data->variables = unserialize($notification_data->variables);
      $notification = new NotyNotification($notification_data);

      NotyStorage::addNotification($notification);
    }
  }

  /**
   * Count pending notifications for $uid.
   *
   * @param type $uid
   * @return type
   */
  public function countNotifications($uid = NULL) {
    if (isset($this) && is_object($this)) $uid = $this->uid;

    $query = db_select('noty_notifications', 'n');
    $query->addExpression('COUNT(n.nid)', 'count');
    $query->condition('n.uid', $uid);
    $query->condition('n.status', 0);

    return $query->execute()->fetchField();
  }

  /**
   * Return array of notifications prepared for noty.
   *
   * @return array
   *   Array of notifications prepared for noty.
   */
  public function getNotificationsJS() {
    $notifications_js = array();

    foreach (NotyStorage::getNotifications() as $notification) {
      $notification->prepare();
      $notifications_js[] = $notification->options;
    }

    return $notifications_js;
  }


  /**
   * Add new notification on the fly.
   *
   * @param type $noty_message
   * @param type $variables
   * @param type $noty_type
   */
  public static function addNotificationToDatabase($uid, $noty_message, $variables = array(), $noty_type = NOTY_NOTIFICATIONS_TYPE_INFORMATION, $removable = 1) {
    $notification = new NotyNotification;
    $notification->uid = $uid;
    $notification->noty_type = $noty_type;
    $notification->noty_message = $noty_message;
    $notification->variables = $variables;
    $notification->removable = $removable;
    $notification->save();

    return NotyStorage::addNotification($notification);
  }

  /**
   * Add new notification on the fly.
   *
   * @param type $noty_message
   * @param type $variables
   * @param type $noty_type
   */
  public static function addNotification($noty_message, $variables = array(), $noty_type = NOTY_NOTIFICATIONS_TYPE_INFORMATION) {
    global $user;

    $notification = new NotyNotification;
    $notification->noty_type = $noty_type;
    $notification->noty_message = $noty_message;
    $notification->variables = $variables;

    return NotyStorage::addNotification($notification);
  }
}

