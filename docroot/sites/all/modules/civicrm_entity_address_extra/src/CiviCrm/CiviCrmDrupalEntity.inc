<?php

class CiviCrmDrupalEntity {

  protected $entityType;

  function __construct($entity_type) {
    $this->entityType = $entity_type;
  }


  /**
   * A generic entity fetching tool that wraps the EntityFieldQuery.
   *
   * @param array $property_conditions A array keyed on property name (db column) and the search value.
   * @param array $property_conditions_operator_overrides Override the default "=" operator for a column by providing an
   * array keyed on the property name and the value being the operator to use in the EntityFieldQuery.
   * @return array|null Returns null or an array of entities keyed on the entity ID.
   */
  function genericEntityFetch($property_conditions, $property_conditions_operator_overrides = array()) {
    $query = new \EntityFieldQuery();
    $query->entityCondition('entity_type', $this->entityType);
    foreach($property_conditions as $column => $value) {
      $query->propertyCondition(
        $column,
        $value,
        isset($property_conditions_operator_overrides[$column])
          ? $property_conditions_operator_overrides[$column]
          : NULL);
    }
    $result = $query->execute();
    $ids = array_keys($result[$this->entityType]);
    if(!empty($ids)) {
      $entities = entity_load($this->entityType, $ids);
      return $entities;
    }
    return null;
  }

} 
