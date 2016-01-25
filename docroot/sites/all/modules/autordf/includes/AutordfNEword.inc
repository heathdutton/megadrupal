<?php

/**
 * This class used to store the Computed Named entity for the word
 */
class AutordfNEword {
  
  public $entity;
  
  public $entityType;
  
  /**
   * Entity type tag found in labelled data
   * Whether is a Organization or a Location
   */
  public $neType;
  
  /**
   * Entity type tag predicted from user annotation, retrieved from SQL
   */
  public $neTypeUser;
  
  function __construct($word, $type) {
    $this->entity = trim($word);
    $this->entityType = $type;  
  }
  
  function __toString() {
    return $this->entityType;
  }
}