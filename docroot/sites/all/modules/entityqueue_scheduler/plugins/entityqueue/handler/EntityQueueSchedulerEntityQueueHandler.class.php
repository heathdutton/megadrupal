<?php
/**
 * @file
 * Contains Entityqueue handler for scheduled queues.
 */

/**
 * A scheduled queue implementation.
 */
class EntityQueueSchedulerEntityQueueHandler extends EntityQueueHandlerBase {

  /**
   * Overrides EntityQueueHandlerBase::settingsForm().
   */
  public function settingsForm() {
    return array();
  }

  /**
   * Overrides EntityQueueHandlerBase::subqueueForm().
   */
  public function subqueueForm(EntitySubqueue $subqueue, &$form_state) {
    $timezone = date_default_timezone_object();
    if (isset($subqueue->scheduler_date)) {
      $date_object = new DateObject($subqueue->scheduler_date, 'UTC');
      $date_object->setTimezone($timezone);
      $default_date = date_format_date($date_object, 'custom', 'Y-m-d H:i');
    }
    else {
      $now = date_now($timezone);
      // Add an hour.
      $now->add(new DateInterval('PT1H'));
      $default_date = date_format_date($now, 'custom', 'Y-m-d H:i');
    }
    $form = array();
    if (!$this->isLiveQueue($subqueue)) {
      $form['date'] = array(
        '#title' => t('Publish Date'),
        '#type' => 'date_select',
        '#default_value' => $default_date,
        '#date_format' => 'm d, Y H:iA',
        '#date_label_position' => 'above',
        '#date_increment' => 15,
        '#date_year_range' => '-0:+3',
        '#required' => TRUE,
        '#weight' => -50,
        '#disabled' => isset($subqueue->subqueue_id),
        '#description' => t('This is the date when this subqueue should be published. Once you set the date for the subqueue it cannot be changed. You will need to create a new subqueue for a new date.'),
      );
      $form['#validate'][] = 'entityqueue_scheduler_subqueue_form_validate';
    }
    return $form;
  }

  /**
   * Overrides EntityQueueHandlerBase::getSubqueueLabel().
   */
  public function getSubqueueLabel(EntitySubqueue $subqueue) {
    if (!empty($subqueue->scheduler_date)) {
      // Use site timezone for label.
      $timezone = date_default_timezone_object(FALSE);
      $date_object = new DateObject($subqueue->scheduler_date, 'UTC');
      $date_object->setTimezone($timezone);
      $label = t('@queue: @date queue', array(
        '@queue' => $this->queue->label,
        '@date' => date_format_date($date_object, 'short'),
      ));
    }
    else {
      $label = t('@queue: Live queue', array(
        '@queue' => $this->queue->label,
      ));
    }
    return $label;
  }

  /**
   * Overrides EntityQueueHandlerBase::canDeleteSubqueue().
   */
  public function canDeleteSubqueue(EntitySubqueue $subqueue) {
    if ($this->isLiveQueue($subqueue)) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Overrides EntityQueueHandlerBase::loadFromCode().
   */
  public function loadFromCode() {
    $this->ensureSubqueue();
  }

  /**
   * Overrides EntityQueueHandlerBase::insert().
   */
  public function insert() {
    $this->ensureSubqueue();
  }

  /**
   * Determines if a subqueue is the live subqueue.
   */
  public function isLiveQueue(EntitySubqueue $subqueue) {
    return $subqueue->name == $this->queue->name . '__live';
  }

  /**
   * Makes sure that every queue has a subqueue.
   */
  protected function ensureSubqueue() {
    global $user;
    static $queues = array();

    if (!isset($queues[$this->queue->name])) {
      $queues[$this->queue->name] = TRUE;

      $transaction = db_transaction();
      $query = new EntityFieldQuery();
      $query
        ->entityCondition('entity_type', 'entityqueue_subqueue')
        ->entityCondition('bundle', $this->queue->name);
      $result = $query->execute();

      // If we don't have a subqueue already, create the live one.
      if (empty($result['entityqueue_subqueue'])) {
        $subqueue = entityqueue_subqueue_create();
        $subqueue->queue = $this->queue->name;
        $subqueue->name = $this->queue->name . '__live';
        $subqueue->label = $this->getSubqueueLabel($subqueue);
        $subqueue->module = 'entityqueue_scheduler';
        $subqueue->uid = $user->uid;

        entity_get_controller('entityqueue_subqueue')->save($subqueue, $transaction);
      }
    }
  }
}
