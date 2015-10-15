<?php
/**
 * @file
 * Contains entity_api class files.
 */

/**
 * The class used for studyroom_closed entities.
 */
class StudyroomClosed extends Entity {
  /**
   * Default constructor.
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'studyroom_closed');
  }

  /**
   * Default defaultLabel.
   */
  protected function defaultLabel() {
    return $this->name;
  }
}

/**
 * The Controller for StudyroomClosed entities
 */
class StudyroomClosedController extends EntityAPIController {
  /**
   * Default constructor.
   */
  public function __construct($entity_type) {
    parent::__construct($entity_type);
  }

  /**
   * Create a studyroom_closed.
   *
   * We first set up the values that are specific to our studyroom_closed
   * schema but then also go through the EntityAPIController function.
   */
  public function create(array $values = array()) {
    // Add values that are specific to our StudyroomClosed.
    $values += array(
      'studyroom_closed_id' => '',
      'is_new' => TRUE,
      'name' => '',
      'created' => '',
      'changed' => '',
      'spaces_all' => FALSE,
    );

    $entity = parent::create($values);
    return $entity;
  }

  /**
   * Delete override.
   */
  public function delete($ids) {
    $entities = $ids ? $this->load($ids) : FALSE;
    if ($entities) {
      foreach ($entities as $id => $entity) {
        // Delete all old reservations.
        $this->deleteReservations($entity->studyroom_closed_id);

        // Delete conflicts.
        $this->deleteConflicts($entity->studyroom_closed_id);
      }

      parent::delete($ids);
    }
  }

  /**
   * Save override.
   */
  public function save($entity) {
    parent::save($entity);
    $this->entity = $entity;
    $this->name = $entity->name;

    // Delete all old reservations.
    // @todo Figure out if old = new and do nothing if true.
    $this->deleteReservations($entity->studyroom_closed_id);

    // Delete conflicts.
    $this->deleteConflicts($entity->studyroom_closed_id);

    // Get spaces for this reservation.
    $spaces = ($entity->spaces_all) ? studyroom_space_get_spaces() : studyroom_space_load_multiple($entity->spaces);

    // Get date for this reservation.
    $langcode = field_language('studyroom_closed', $entity, 'field_studyroom_closed_dates');
    $dates = $entity->field_studyroom_closed_dates[$langcode];

    $this->dates = array();
    $this->processDates($dates);

    $this->reservations = array();
    foreach ($this->dates as $date) {
      $this->duration = $date['start']->difference($date['end'], 'seconds');
      if ($this->duration == 0) {
        // One second less than 24 hours.
        $this->duration = '86399';
      }

      foreach ($spaces as $space) {
        $reservations[] = $this->createReservation($space->space_id, $date['start']);
      }
    }

    foreach ($reservations as $reservation) {
      $reservation_ids[] = $reservation->reservation_id;
    }

    // Write reservations id to entity.
    db_update('studyroom_closed')
      ->fields(array('reservations' => serialize($reservation_ids)))
      ->condition('studyroom_closed_id', $entity->studyroom_closed_id)
      ->execute();

    $this->findConflicts($reservations);
  }

  /**
   * Delete all reservations associated with a closed reservation.
   */
  protected function deleteReservations($studyroom_closed_id) {
    $reservations = db_select('studyroom_closed', 'c')
      ->fields('c', array('reservations'))
      ->condition('c.studyroom_closed_id', $studyroom_closed_id)
      ->execute()->fetchField();
    if (!empty($reservations)) {
      $reservations = unserialize($reservations);
      studyroom_reservation_delete_locked_multiple($reservations);
    }
  }

  /**
   * Delete all conflicts associated with a closed reservation.
   */
  protected function deleteConflicts($studyroom_closed_id) {
    db_delete('studyroom_closed_conflict')
      ->condition('closed', $studyroom_closed_id)
      ->execute();
  }

  /**
   * Get array of all dates to process.
   */
  protected function processDates($dates) {
    foreach ($dates as $date) {
      $date['value'] = new DateObject($date['value']);
      $date['value2'] = new DateObject($date['value2']);

      $days_diff = $date['value']->difference($date['value2'], 'days');
      if ($days_diff > 0 && !isset($date['processed'])) {
        $sub_dates = array();

        for ($i = 0; $i <= $days_diff; $i++) {
          $sub_date = array(
            'value' => $date['value']->format('Y-m-d H:i:s'),
          );
          if (isset($date['all_day']) && $date['all_day']) {
            $sub_date['value2'] = $date['value']->format('Y-m-d H:i:s');
            $sub_date['all_day'] = 1;
          }
          else {
            if ($i == 0) {
              $sub_date['value2'] = $date['value']->format('Y-m-d') . ' 00:00:00';
            }
            elseif ($i == $days_diff) {
              $sub_date['value'] = $date['value']->format('Y-m-d') . ' 00:00:00';
              $sub_date['value2'] = $date['value2']->format('Y-m-d H:i:s');
            }
            else {
              $sub_date['value'] = $date['value']->format('Y-m-d') . ' 00:00:00';
              $sub_date['value2'] = $date['value']->format('Y-m-d') . ' 00:00:00';
            }
          }
          $sub_dates[] = $sub_date;
          $date['value']->add(new DateInterval("P1D"));
        }
        $this->processDates($sub_dates);
        continue;
      }
      $this->dates[] = array('start' => $date['value'], 'end' => $date['value2']);
    }
  }

  /**
   * Create a reservation and find conflicts.
   */
  protected function createReservation($space_id, $start) {
    $values = array(
      'type' => 'closed',
      'space_id' => $space_id,
      'uid' => $GLOBALS['user']->uid,
      'duration' => $this->duration,
      'title' => $this->name,
      'created' => time(),
      'changed' => time(),
      'locked' => 1,
    );
    $reservation = entity_create('studyroom_reservation', $values);
    $reservation->field_reservation_datetime[LANGUAGE_NONE][0] = array(
      'value' => $start->format('Y-m-d H:i:s'),
    );
    entity_save('studyroom_reservation', $reservation);

    return $reservation;
  }

  /**
   * Determine if conflict exists.
   */
  protected function findConflicts($reservations) {
    foreach ($reservations as $reservation) {
      $field = $reservation->field_reservation_datetime[LANGUAGE_NONE][0];
      $reservation->start_time = new DateObject($field['value'], $field['timezone']);
      $reservation->end_time = clone $reservation->start_time;
      $reservation->end_time = $reservation->end_time->add(new DateInterval('PT' . $this->duration . 'S'));
      $conflict = $reservation->checkConflict();

      // If conflict, create entry in conflict table.
      if ($conflict) {
        $this->createConflictEntries($reservation->conflict_resv_ids);
      }
    }
  }

  /**
   * Create conflict entries in database.
   */
  protected function createConflictEntries($ids) {
    foreach ($ids as $id) {
      $conflict = array(
        'reservation' => $id,
        'closed' => $this->entity->studyroom_closed_id,
      );
      drupal_write_record('studyroom_closed_conflict', $conflict);
    }
  }
}
