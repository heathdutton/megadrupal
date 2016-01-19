<?php

/**
 * @file
 * Contains entity_api class files.
 */

/**
 * The class used for studyroom_reservation entities
 */
class StudyroomReservation extends Entity {
  /**
   * Default constructor.
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'studyroom_reservation');
  }

  /**
   * Default defaultLabel.
   */
  protected function defaultLabel() {
    return 'Reservation ' . $this->reservation_id;
  }

  /**
   * Default defaultUri.
   */
  protected function defaultUri() {
    return array('path' => STUDYROOM_RESERVATION_PATH . $this->reservation_id);
  }

  /**
   * Function to determine if reservation is locked.
   */
  public function isLocked() {
    return isset($this->locked) ? $this->locked : 0;
  }

  /**
   * Determine if a conflict exists.
   */
  public function checkConflict() {
    $conflict = FALSE;
    $reservations = studyroom_reservation_per_space_per_day($this->space_id, $this->start_time->format('Y-m-d'), $this);
    $this->start_time_object = $this->start_time;
    $this->start_time = $this->start_time->format('U');
    $this->end_time = $this->end_time->format('U');

    foreach ($reservations as $current) {
      if ($current->end_time <= $this->start_time) {
        // This start time is after this reservation end time, so OK.
        continue;
      }
      if ($this->end_time > $current->start_time) {
        // End time is past start time of this reservation, so no good.
        $conflict = TRUE;
        $this->conflict_resv_ids[] = $current->reservation_id;
      }
    }

    return $conflict;
  }

  /**
   * Determine if a conflict exists.
   */
  public function checkRuleMaxResvPer($user_name = NULL) {
    $type = studyroom_reservation_type_load($this->type);
    if (!$type->public || !$type->ui) {
      return 0;
    }

    // Load user info.
    $account = user_load_by_name($user_name);
    $uid = $account->uid;

    $space = studyroom_space_load($this->space_id);
    $spaces = studyroom_space_get_spaces_by_type($space->type);
    $space_ids = array_keys($spaces);

    // Get midnight of first day of week.
    $weekday = $this->start_time_object->format('w');
    $week_start = $this->start_time_object->sub(new DateInterval('P' . $weekday . 'D'));
    $week_start->setTime(0, 0, 0);
    $rule_week_start = $week_start->format('Y-m-d H:i:s');

    // Get weeks end.
    $week_end = $week_start->add(new DateInterval('P7D'));
    $rule_week_end = $week_end->format('Y-m-d H:i:s');

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'studyroom_reservation')
      ->propertyCondition('type', $this->type)
      ->propertyCondition('space_id', $space_ids, 'IN')
      ->propertyCondition('uid', $uid)
      ->fieldCondition('field_reservation_datetime', 'value', array($rule_week_start, $rule_week_end), 'BETWEEN')
      ->fieldOrderBy('field_reservation_datetime', 'value', 'ASC');
    $results = $query->execute();

    $resvs = array();
    if (isset($results['studyroom_reservation'])) {
      $resvs = array_keys($results['studyroom_reservation']);
    }

    return count($resvs);
  }
}

/**
 * The class used for studyroom_reservation type entities
 */
class StudyroomReservationType extends Entity {
  public $type;
  public $label;
  public $locked;

  /**
   * Default constructor.
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'studyroom_reservation_type');
  }

  /**
   * Override save function to add default values.
   */
  public function save() {
    if (!isset($this->min_duration)) {
      $this->min_duration = $this->defaultMinDuration();
    }
    if (!isset($this->max_duration)) {
      $this->max_duration = $this->defaultMaxDuration();
    }
    parent::save();
  }

  /**
   * Function to determine if reservation type is locked.
   */
  public function isLocked() {
    return isset($this->locked) ? $this->locked : 0;
  }

  /**
   * Return default value.
   */
  public function defaultMinDuration() {
    return '900';
  }

  /**
   * Return default value.
   */
  public function defaultMaxDuration() {
    return '14400';
  }

  /**
   * Build array of options for min_duration.
   *
   * @return array
   *   array of options formated for use in select form element
   */
  public function optionsMinDuration() {
    return drupal_map_assoc(array(0, 900, 1800, 3600, 7200, 10800, 14400), 'format_interval');
  }

  /**
   * Build array of options for max_duration.
   *
   * @return array
   *   array of options formated for use in select form element
   */
  public function optionsMaxDuration() {
    $values = array(
      0, 900, 1800, 3600, 7200, 10800, 14400, 21600, 32400, 43200, 64800, 86400);
    return drupal_map_assoc($values, 'format_interval');
  }
}


/**
 * The Controller for StudyroomReservation entities
 */
class StudyroomReservationController extends EntityAPIController {
  /**
   * Default constructor.
   */
  public function __construct($entity_type) {
    parent::__construct($entity_type);
  }

  /**
   * Create a studyroom_reservation.
   *
   * We first set up the values that are specific to our studyroom_reservation
   * schema but then also go through the EntityAPIController function.
   */
  public function create(array $values = array()) {
    // Add values that are specific to our StudyroomReservation.
    $values += array(
      'reservation_id' => '',
      'is_new' => TRUE,
      'title' => '',
      'created' => '',
      'changed' => '',
    );

    $entity = parent::create($values);
    return $entity;
  }

  /**
   * Delete override.
   *
   * Override delete to prevent deletion of locked types.
   */
  public function delete($ids) {
    $entities = $ids ? $this->load($ids) : FALSE;
    if (!$entities) {
      // Do nothing, in case invalid or no ids have been passed.
      return;
    }

    foreach ($entities as $id => $entity) {
      if ($entity->locked) {
        drupal_set_message(t('Locked reservation cannot be deleted.'));
      }
      else {
        parent::delete(array($id));

        // If not admin type, notify user of delete.
        if ($entity->type != 'admin') {
          /*
          $entity->uid
          notify_user_hook here? OR use entity_delete in studyroom_user module?
          */
          if (user_access('fake')) {
            drupal_set_message(t('@todo Notify user about delete!'), 'status', FALSE);
          }
        }
      }
    }
  }

  /**
   * Delete locked reservartions.
   */
  public function deleteLocked($ids) {
    parent::delete($ids);
  }

  /**
   * Load override.
   *
   * Allow theming of reservation label for availabilty module.
   */
  public function load($ids = array(), $conditions = array()) {
    $reservations = parent::load($ids, $conditions);
    foreach ($reservations as &$reservation) {
      $function = 'theme_studyroom_reservation_' . $reservation->type . '_label';
      if (function_exists($function)) {
        $reservation->label = $function();
      }
      else {
        $reservation->label = t('RESERVED');
      }
    }

    return $reservations;
  }
}

/**
 * The Controller for StudyroomReservation entities.
 */
class StudyroomReservationTypeController extends EntityAPIControllerExportable {
  /**
   * Default constructor.
   */
  public function __construct($entity_type) {
    parent::__construct($entity_type);
  }

  /**
   * Create a studyroom_reservation type.
   *
   * We first set up the values that are specific to our reservation type
   * schema but then also go through the EntityAPIController function.
   */
  public function create(array $values = array()) {
    // Add values that are specific to our StudyroomReservation.
    $values += array(
      'is_new' => TRUE,
      'locked' => 0,
      'public' => 1,
      'module' => 'studyroom_reservation',
    );
    $entity = parent::create($values);
    return $entity;
  }


  /**
   * Save override.
   */
  public function save($entity) {
    parent::save($entity);

    // Create new instance of field.
    studyroom_reservation_create_field_instances($entity->type);

    // Clear types cache and force menu rebuild.
    studyroom_reservation_types_reset();
    variable_set('menu_rebuild_needed', TRUE);
  }

  /**
   * Delete override.
   *
   * Override delete to prevent deletion of locked types.
   */
  public function delete($ids) {
    $entities = $ids ? $this->load($ids) : FALSE;
    if (!$entities) {
      // Do nothing, in case invalid or no ids have been passed.
      return;
    }

    foreach ($entities as $entity) {
      if ($entity->locked) {
        drupal_set_message(t('Locked reservation types cannot be deleted.'));
      }
      else {
        parent::delete(array('0' => $entity->type));
      }
    }

    // Clear types cache and force menu rebuild.
    studyroom_reservation_types_reset();
    variable_set('menu_rebuild_needed', TRUE);
  }
}
