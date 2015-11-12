<?php
/**
 * @file
 * Stores code for converting records to parties and vice versa.
 */

interface PartyServiceFormatterInterface {

  /**
   * Convert a party to a record based on a certain format.
   *
   * @param Party $party
   *   The party object to convert.
   *
   * @return stdClass
   *   An object ready to be serialized.
   */
  public function convertPartyToRecord($party);

  /**
   * Convert a record to a party based on a certain format.
   *
   * @param stdClass $record
   *   The record to convert into values on a given party.
   * @param Party $party
   *   The party to put these values.
   */
  public function convertRecordToParty($record, $party);

}

class PartyServiceFormatterBase implements PartyServiceFormatterInterface {

  /**
   * Format Information.
   * @var array
   */
  protected $format;

  public static function createFromEntity($entity) {
    return new PartyServiceFormatterBase($entity->settings['format']);
  }

  public function __construct($format) {
    $this->format = $format;
  }

  /**
   * {@inheritdoc}
   */
  public function convertPartyToRecord($party) {
    $party_wrapper = entity_metadata_wrapper('party', $party);

    $record = new stdClass;
    foreach ($this->format as $key => $property) {
      $record->$key = $this->retrievePropertyValue($party_wrapper, $property);
    }
    return $record;
  }

  /**
   * Retrieve a Property value.
   *
   * @param EntityDrupalWrapper $party_wrapper
   *   The wrapped party.
   * @param string $property
   *   A property identifier in the form data_set_name|field_name:sub_value
   */
  protected function retrievePropertyValue($party_wrapper, $property) {
    $data_set = FALSE;
    $property_bits = explode('|', $property);
    if (count($property_bits) > 1) {
      $data_set = $property_bits[0];
      $property = $property_bits[1];
    }

    $property_bits = explode(':', $property);
    $property = array_shift($property_bits);
    $value = array_shift($property_bits);

    if (!$data_set) {
      if ($value) {
        return $party_wrapper->$property->$value->value();
      }
      else {
        return $party_wrapper->$property->value();
      }
    }
    else {
      $data_set_entity = $party_wrapper->value()
                           ->getDataSetController($data_set)
                           ->getEntity(0);
      if (!$data_set_entity) {
        return NULL;
      }

      $data_set_type = $party_wrapper->value()
                         ->getDataSetController($data_set)
                         ->getDataInfo('entity type');
      $wrapper = entity_metadata_wrapper($data_set_type, $data_set_entity);
      if ($value) {
        return $wrapper->$property->$value->value();
      }
      else {
        return $wrapper->$property->value();
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function convertRecordToParty($record, $party) {
    $party_wrapper = entity_metadata_wrapper('party', $party);

    foreach ($record as $key => $value) {
      if (empty($this->format[$key])) {
        continue;
      }

      $property = $this->format[$key];
      $this->setPropertyValue($party_wrapper, $property, $value);
    }
  }

  /**
   * Set a property value.
   *
   * @param EntityDrupalWrapper $party_wrapper
   *   The wrapped party.
   * @param string $property
   *   A property identifier in the form data_set_name|field_name:sub_value
   * @param mixed $var
   *   The value to set.
   */
  public function setPropertyValue($party_wrapper, $property, $var) {
    $data_set = FALSE;
    $property_bits = explode('|', $property);
    if (count($property_bits) > 1) {
      $data_set = $property_bits[0];
      $property = $property_bits[1];
    }

    $property_bits = explode(':', $property);
    $property = array_shift($property_bits);
    $value = array_shift($property_bits);

    if (!$data_set) {
      if ($value) {
        $party_wrapper->$property->$value = $var;
      }
      else {
        // Build in a special case for UUID.
        if ($property == 'uuid') {
          $party = $party_wrapper->value();
          // Don't allow changing of a UUID.
          if (empty($party->uuid)) {
            $party->uuid = $var;
          }
        }
        else {
          $party_wrapper->$property = $var;
        }
      }
    }
    else {
      $data_set_controller = $party_wrapper->value()->getDataSetController($data_set);
      $entity = $data_set_controller->getEntity(0, TRUE);
      $wrapper = entity_metadata_wrapper($data_set_controller->getDataInfo('entity type'), $entity);
      if ($value) {
        $wrapper->$property->$value = $var;
      }
      else {
        $wrapper->$property = $var;
      }
    }
  }

}
