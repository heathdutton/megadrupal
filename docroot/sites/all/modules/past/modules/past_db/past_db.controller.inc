<?php

/**
 * @file
 * Contains the controller classes.
 */

/**
 * Controller class for the past event entity.
 */
class PastEventController extends EntityAPIController {

  /**
   *
   * @var InsertQuery
   */
  protected $insert;

  public function create(array $values = array()) {
    $entity = parent::create($values);

    if (empty($entity->type)) {
      $entity->type = 'past_event';
    }
    if (empty($entity->timestamp)) {
      $entity->timestamp = REQUEST_TIME;
    }
    if (empty($entity->severity)) {
      $entity->severity = PAST_SEVERITY_INFO;
    }
    if (empty($entity->uid) && !empty($GLOBALS['user'])) {
      $entity->uid = $GLOBALS['user']->uid;
    }
    return $entity;
  }

  public function save($entity, DatabaseTransaction $transaction = NULL) {
    parent::save($entity, $transaction);

    // Save the arguments.
    foreach ($entity->getArguments() as $argument) {
      $argument->event_id = $entity->event_id;
      $argument->save();
    }

    // Update child events to use the parent_event_id.
    if ($child_events = $entity->getChildEvents()) {
    db_update('past_event')
      ->fields(array(
        'parent_event_id' => $entity->event_id
      ))
      ->condition('event_id', $child_events)
      ->execute();
    }

    // If running in a WebTestCase environment, propagate the log into simpletest log.
  }

  public function delete($ids, \DatabaseTransaction $transaction = NULL) {
    // Delete the arguments first.
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'past_event_argument');
    $query->propertyCondition('event_id', $ids);
    $result = $query->execute();
    if ($result) {
      entity_delete_multiple('past_event_argument', array_keys($result['past_event_argument']));
    }

    parent::delete($ids, $transaction);
  }

}

/**
 * Controller class for the past event argument entity.
 */
class PastEventArgumentController extends EntityAPIController {

  public function create(array $values = array()) {

    $entity = parent::create($values);

    return $entity;
  }

  public function save($entity, DatabaseTransaction $transaction = NULL) {

    // Make sure the type is set.
    $entity->ensureType();

    parent::save($entity, $transaction);

    // Save data.
    if ($data = $entity->getOriginalData()) {
      $this->insert = db_insert('past_event_data')
        ->fields(array('argument_id', 'parent_data_id', 'type', 'name', 'value', 'serialized'));
      $this->normalizeData($entity->argument_id, $data);
      try {
        $this->insert->execute();
      }
      catch (Exception $e) {
        watchdog_exception('past', $e);
      }
    }
  }

  public function delete($ids, \DatabaseTransaction $transaction = NULL) {
    // Delete the data first.
    db_delete('past_event_data')
      ->condition('argument_id', $ids)
      ->execute();

    parent::delete($ids, $transaction);
  }

  /**
   *
   * @param type $data
   */
  protected function normalizeData($argument_id, $data, $parent_data_id = 0) {
    if (is_array($data) || is_object($data)) {
      foreach ($data as $name => $value) {

        // @todo: Allow to make this configurable. Ignore NULL.
        if ($value === NULL) {
          continue;
        }

        $this->insert->values(array(
          'argument_id' => $argument_id,
          'parent_data_id' => $parent_data_id,
          'type' => is_object($value) ? get_class($value) : gettype($value),
          'name' => $name,
          // @todo: Support recursive inserts.
          'value' => is_scalar($value) ? $value : serialize($value),
          'serialized' => is_scalar($value) ? 0 : 1,
        ));
      }
    }
    else {
      $this->insert->values(array(
        'argument_id' => $argument_id,
        'parent_data_id' => 0,
        'type' => gettype($data),
        'name' => '',
        'value' => $data,
        'serialized' => 0,
      ));
    }
  }

}

/**
 * Controller class for the Entity Metadata.
 */
class PastDBEventMetadataController extends EntityDefaultMetadataController {
  protected static $modules = array();

  /**
   * Overrides EntityDefaultMetadataController::entityPropertyInfo().
   */
  public function entityPropertyInfo() {
    // Loading property information and make em better usable in here.
    $info = parent::entityPropertyInfo();
    $prop = &$info[$this->type]['properties'];

    // The timestamp should be rendered/shown as a date.
    $prop['timestamp']['type'] = 'date';
    $prop['severity']['options list'] = 'past_event_severities';
    $prop['uid'] = array(
      'label' => t('User'),
      'type' => 'user',
      'description' => t('The user who triggered the event.'),
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'uid',
    );

    return $info;
  }
}

/**
 * Implements the default UI Controller.
 */
class PastDBEventUIController extends EntityDefaultUIController {

  /**
   * Overrides EntityDefaultUIController::hook_menu().
   */
  public function hook_menu() {
    $id_count = count(explode('/', $this->path));
    $plural_label = isset($this->entityInfo['plural label']) ? $this->entityInfo['plural label'] : $this->entityInfo['label'] . 's';
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%entity_object';
    $items[$this->path . '/' . $wildcard] = array(
      'title callback' => 'entity_label',
      'title arguments' => array($this->entityType, $id_count),
      'page callback' => 'past_db_event_view',
      'page arguments' => array($id_count),
      'load arguments' => array($this->entityType),
      'access callback' => 'entity_access',
      'access arguments' => array('view', $this->entityType, $id_count),
    );
    return $items;
  }
}

/**
 * UI controller.
 */
class PastEventTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage past event, including fields.';
    return $items;
  }
}

/**
 * Controller class for the Past Argument Metadata.
 */
class PastDBEventArgumentMetadataController extends EntityDefaultMetadataController {

  /**
   * Overrides EntityDefaultMetadataController::entityPropertyInfo().
   */
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();

    $properties = &$info[$this->type]['properties'];

    // Link the event id property to the corresponding event entity.
    $properties['event_id']['type'] = 'past_event';

    return $info;
  }
}

/**
 * Controller class for the Past Data Metadata.
 */
class PastDBEventDataMetadataController extends EntityDefaultMetadataController {

  /**
   * Overrides EntityDefaultMetadataController::entityPropertyInfo().
   */
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();

    $properties = &$info[$this->type]['properties'];

    // Link the argument id property to the corresponding argument entity.
    $properties['argument_id']['type'] = 'past_event_argument';

    return $info;
  }
}

/**
 * Controller for past_event views data.
 */
class PastDBEventViewsController extends EntityDefaultViewsController {

  public function views_data() {
    $data = parent::views_data();
    $data['past_event']['argument_data'] = array(
      'title' => t('Argument data'),
      'help' => t('Display or filter by a specific argument data'),
      'real field' => 'value',
      'field' => array(
        'handler' => 'past_db_handler_field_event_argument_data',
      ),
      'filter' => array(
        'handler' => 'past_db_handler_filter_event_argument_value',
      ),
    );
    return $data;
  }
}
