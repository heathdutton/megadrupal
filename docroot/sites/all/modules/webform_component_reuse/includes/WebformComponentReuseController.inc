<?php

class WebformComponentReuseController extends EntityAPIController {
  public function create(array $values = array()) {
    $values += array( 
      'wcid' => '',
      'is_new' => TRUE,
      'bundle' => 'webform_component_reuse',
      'value' => '',
      'extra' => '',
    );      
    $entity = parent::create($values);
    return $entity;
  }
}
