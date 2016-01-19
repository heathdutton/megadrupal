<?php

/**
 * @file
 * Contains entity_api class files.
 */

/**
 * The class used for studyroom_space entities.
 */
class StudyroomSpace extends Entity {
  /**
   * Default constructor.
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'studyroom_space');
  }

  /**
   * Override default label to allow adding location.
   */
  protected function defaultLabel() {
    $loc = variable_get('studyroom_space_title_location', 0);
    if ($loc) {
      $location = studyroom_space_location_load($this->type);
      return $location->label() . ' - ' . $this->label;
		}
		else {
      return $this->label;
		}
  }

  /**
   * Default defaultUri.
   */
  protected function defaultUri() {
    return array('path' => STUDYROOM_SPACE_PATH . $this->space_id);
  }

  /**
   * Override default label to allow adding location.
   */
  public function shortLabel() {
    return $this->label;
  }

  /**
   * Return the hours per specific date.
   */
  public function hoursPerDate($date) {
    $modules = module_implements('studyroom_space_hours');
    if (!empty($modules)) {
      if (count($modules) > 1) {
        watchdog(t('studyroom_space'), t('Multiple implementations of hook_studyroom_space_hours were found; only 0 or 1 implementations are supported.'), WATCHDOG_EMERGENCY);
        drupal_exit();
      }
      return module_invoke(array_shift($modules), 'studyroom_space_hours', $this, $date);
    }
	else {
      $week_day = date_day_of_week($date);
      return studyroom_hours_day_hours($this->field_space_hours, $week_day, $date, $this->language);
    }
  }

  /**
   * Build array of options for spaces.
   *
   * @return array
   *   Array of options formated for use in select form element.
   */
  public function optionsSpaces() {
    $ret = array();
    $locations = studyroom_space_locations();
    foreach ($locations as $location) {
      $spaces = $location->getSpaces();
      if (!empty($spaces)) {
        foreach ($spaces as $space) {
          $uri = studyroom_space_uri($space);
          $ret[$space->space_id]['title'] = array(
            'data' => array(
              '#type' => 'link',
              '#title' => $space->label(),
              '#href' => $uri['path'],
            ),
            'class' => 'title',
          );
          $ret[$space->space_id]['location'] = $space->location_label;
        }
      }
    }
    return $ret;
  }
}


/**
 * The class used for studyroom_space type entities.
 */
class StudyroomSpaceLocation extends Entity {
  /**
   * Default constructor.
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'studyroom_space_location');
  }

  /**
   * Return all spaces in system.
   */
  public function getSpaces($status = FALSE) {
    $query = db_select('studyroom_space', 's')
      ->condition('type', $this->type)
      ->fields('s', array('space_id'));

    // Add status condition if needed.
    if ($status) {
      $query->condition('status', '1');
    }

    $spaces = $query->execute()->fetchAllAssoc('space_id');
    if (!empty($spaces)) {
      return studyroom_space_load_multiple(array_keys($spaces));
    }

    // Return false if no spaces found.
    return FALSE;
  }

}

/**
 * The Controller for StudyroomSpace entities.
 */
class StudyroomSpaceController extends EntityAPIController {
  public $year;
  public $month;

  /**
   * Default constructor.
   */
  public function __construct($entity_type) {
    parent::__construct($entity_type);
  }

  /**
   * Create a studyroom_space.
   */
  public function create(array $values = array()) {
    // Add is_new property if it is not set.
    $values += array(
      'is_new' => TRUE,
      'label' => NULL,
    );

    $entity = parent::create($values);
    return $entity;
  }

  /**
   * AttachLoad override to unseralize field.
   */
  public function attachLoad(&$queried_entities, $revision_id = FALSE) {
    parent::attachLoad($queried_entities, $revision_id);

    // We need to go through and unserialize our serialized fields.
    foreach ($queried_entities as $entity) {
      foreach (array('reservation_types') as $key) {
        if (is_string($entity->$key)) {
          $entity->$key = unserialize($entity->$key);
        }
      }
    }
  }

  /**
   * BuildQuery override.
   */
  protected function buildQuery($ids, $conditions = array(), $revision_id = FALSE) {
    $query = parent::buildQuery($ids, $conditions, $revision_id);
    // Add the real name field from the {studyroom_space_location} table.
    $query->innerJoin('studyroom_space_location', 'l', 'base.type = l.type');
    $query->addField('l', 'label', 'location_label');
    $query->addField('l', 'allowed_reservation_types', 'reservation_types');
    return $query;
  }
}

/**
 * The Controller for StudyroomSpace entities.
 */
class StudyroomSpaceLocationController extends EntityAPIControllerExportable {
  /**
   * Default constructor.
   */
  public function __construct($entity_type) {
    parent::__construct($entity_type);
  }

  /**
   * Create override.
   */
  public function create(array $values = array()) {
    // Add values that are specific to our StudyroomSpace.
    $values += array(
      'is_new' => TRUE,
    );
    $entity = parent::create($values);
    return $entity;
  }

  /**
   * Invoke override.
   *
   * If update and type changed, update all spaces to new type.
   */
  public function invoke($hook, $entity) {
    parent::invoke($hook, $entity);
    if ($hook == 'update' && $entity->original->type != $entity->type) {
      db_update('studyroom_space')
        ->fields(array('type' => $entity->type))
        ->condition('type', $entity->original->type)
        ->execute();
    }
  }

  /**
   * Save override.
   *
   * Override to clear cache and rebuild menus.
   */
  public function save($entity) {
    $return = parent::save($entity);

    if ($return == SAVED_NEW) {
      // Configure each location to add nessessary items.
      studyroom_space_configure_location($entity->type);
    }

    // Clear types cache and force menu rebuild.
    if (module_exists('studyroom_reservation')) {
      studyroom_reservation_types_reset();
    }
    variable_set('menu_rebuild_needed', TRUE);
  }

  /**
   * Delete override.
   *
   * Override to clear cache and rebuild menus.
   */
  public function delete($ids) {
    parent::delete($ids);

    // Clear types cache and force menu rebuild.
    if (module_exists('studyroom_reservation')) {
      studyroom_reservation_types_reset();
    }
    variable_set('menu_rebuild_needed', TRUE);
  }
}
