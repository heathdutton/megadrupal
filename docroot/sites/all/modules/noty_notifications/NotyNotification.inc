<?php

/**
 * Class defines an notification object
 */
class NotyNotification {
  public $nid = NULL;
  public $uid = NULL;
  public $noty_type = NOTY_NOTIFICATIONS_TYPE_INFORMATION;
  public $noty_message = '';
  public $variables = array();
  public $entity_type = '';
  public $entity_id = 0;
  public $status = 0;
  public $removable = 1;
  public $created = 0;

  // settings for noty
  public $options = array();

  // is notification from databasae?
  public $in_database = 0;

  /**
   * constructor
   * @param $notification_data
   *   Object with notification data from database.
   */
  function __construct(stdClass $notification_data = NULL) {
    if (!is_null($notification_data)) {
      foreach ($notification_data as $name => $value) {
        $this->{$name} = $value;
      }
    }
    else {
      $this->created = time();
    }
  }

  /**
   * Save or update current notification in database.
   */
  public function save() {
    try {
      if (is_null($this->nid)) {
        $query = db_insert('noty_notifications')
          ->fields(array(
              'uid' => $this->uid,
              'noty_type' => $this->noty_type,
              'noty_message' => $this->noty_message,
              'variables' => serialize($this->variables),
              'entity_type' => $this->entity_type,
              'entity_id' => $this->entity_id,
              'status' => $this->status,
              'removable' => $this->removable,
              'created' => $this->created,
            ));
        $nid = $query->execute();
        $this->nid = $nid;
      } else {
        $num_updated = db_update('noty_notifications')
          ->fields(array(
              'uid' => $this->uid,
              'noty_type' => $this->noty_type,
              'noty_message' => $this->noty_message,
              'variables' => serialize($this->variables),
              'entity_type' => $this->entity_type,
              'entity_id' => $this->entity_id,
              'status' => $this->status,
              'removable' => $this->removable,
              'created' => $this->created,
            ))
          ->condition('nid', $this->nid)
          ->execute();
      }
      $this->in_database = 1;
    } catch (Exception $e) {
      drupal_set_message($e->getMessage(), 'warning');
      watchdog('noty_notifications', 'Error: notification can not stored or updated in database.', array(), WATCHDOG_ERROR);
    }
  }

  /**
   * Save current notification to database.
   */
  public function delete($nid = NULL) {
    if (isset($this) && is_object($this) && is_null($this->nid) && is_null($nid)) return 0;
    if (isset($this) && is_object($this) && !is_null($this->nid)) $nid = $this->nid;

    try {
      $num_deleted = db_delete('noty_notifications')
        ->condition('nid', $nid)
        ->execute();

      if (isset($this) && is_object($this)) {
        $this->in_database = 0;
        $this->nid = NULL;
      }

      return $num_deleted;
    } catch (Exception $e) {
      drupal_set_message($e->getMessage(), 'warning');
      watchdog('noty_notifications', 'Error: notification can not delete from database.', array(), WATCHDOG_ERROR);

      return 0;
    }
  }

  /**
   * Prepare notification array for noty JS.
   */
  public function prepare() {
    $this->options['nid'] = $this->nid;
    $this->options['layout'] = NOTY_NOTIFICATIONS_LAYOUT_DEFAULT;
    $this->options['type'] = $this->noty_type;
    $this->options['text'] = $this->noty_message;
  }
}
