<?php

/**
 * Commerce booking event selection handler.
 */
class EntityReference_SelectionHandler_CommerceBookingEvent extends EntityReference_SelectionHandler_Generic {

  /**
   * The name of the commerce_booking_event field.
   */
  protected $event_field_name = 'commerce_booking_event';

  /**
   * The path to the referencing entity.
   *
   * An array of keys to track through an entity metadata wrapper.
   *
   * @var array
   */
  protected $referencing_entity_path = array('commerce_booking_event');

  /**
   * The event metadata wrapper.
   *
   * @var EntityDrupalWrapper
   */
  protected $event_wrapper;

  /**
   * Overrides EntityReference_SelectionHandler_Generic::getInstance().
   */
  public static function getInstance($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    $source_entity_type = $instance['entity_type'];

    // Check if the entity type does exist and has a base table.
    $entity_info = entity_get_info($source_entity_type);
    if (empty($entity_info['base table'])) {
      return EntityReference_SelectionHandler_Broken::getInstance($field, $instance);
    }

    return new EntityReference_SelectionHandler_CommerceBookingEvent($field, $instance, $entity_type, $entity);
  }

  /**
   * Overrides EntityReference_SelectionHandler_Generic::__construct().
   *
   * Allow handler settings to override the defaults.
   */
  protected function __construct($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    parent::__construct($field, $instance, $entity_type, $entity);

    // Pull the event field name if it's set in the handler settings.
    if (isset($field['settings']['handler_settings']['event_field_name'])) {
      $this->event_field_name = $field['settings']['handler_settings']['event_field_name'];
    }
    if (isset($instance['settings']['handler_settings']['event_field_name'])) {
      $this->event_field_name = $instance['settings']['handler_settings']['event_field_name'];
    }

    // Pull the referencing entity path if it's set.
    if (isset($field['settings']['handler_settings']['referencing_entity'])) {
      $this->referencing_entity_path = $field['settings']['handler_settings']['referencing_entity'];
    }
    if (isset($instance['settings']['handler_settings']['referencing_entity'])) {
      $this->referencing_entity_path = $instance['settings']['handler_settings']['referencing_entity'];
    }
  }

  /**
   * Get hold of the event entity metadata wrapper.
   *
   * We track over
   * EntityReference_SelectionHandler_CommerceBookingEvent::$referencing_entity_path
   * to find the event entity.
   *
   * @return EntityDrupalWrapper
   *   The entity metadata wrapper object we are working with or FALSE if it
   *   doesn't exist.
   */
  protected function getEventWrapper() {
    if (!isset($this->event_wrapper)) {
      $this->event_wrapper = FALSE;

      // If we have an entity, try and track to the event.
      if ($this->entity) {
        // Get the wrapper so we can track through.
        $this->event_wrapper = entity_metadata_wrapper($this->entity_type, $this->entity);

        // Loop over the paths to get our final entity.
        foreach ($this->referencing_entity_path as $bit) {
          // If this doesn't exist, return NULL.
          if (!isset($this->event_wrapper->{$bit})) {
            $this->event_wrapper = FALSE;
            break;
          }

          // Get the next step of the wrapper.
          $this->event_wrapper = $this->event_wrapper->{$bit};
        }
      }
    }

    return $this->event_wrapper;
  }

  /**
   * Overrides EntityReference_SelectionHandler_Generic_commerce_booking_team::buildEntityFieldQuery().
   *
   * Add a filter based off the event.
   */
  protected function buildEntityFieldQuery($match = NULL, $match_operator = 'CONTAINS') {
    $query = parent::buildEntityFieldQuery($match, $match_operator);

    // Attempt to get hold of the entity which has the event reference on.
    if ($event = $this->getEventWrapper()) {
      $query->addMetaData('commerce_booking_event', $event);
      $query->fieldCondition($this->event_field_name, 'entity_type', $event->type());
      $query->fieldCondition($this->event_field_name, 'entity_id', $event->getIdentifier());
    }
    // Otherwise we are in a default mode which is meaningless...
    else {
      $query->entityCondition('entity_id', 0);
    }

    return $query;
  }
}
