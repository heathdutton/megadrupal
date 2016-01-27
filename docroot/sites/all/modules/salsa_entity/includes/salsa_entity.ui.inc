<?php

/**
 * @file
 * Definition of SalsaEntityUIController.
 */

/*
 * UI Controller class for the Salsa entity.
 */
class SalsaEntityUIController extends EntityDefaultUIController {

  /*
   * Overrides EntityDefaultUIController::__construct().
   */
  public function __construct($entityType, $entity_info) {
    parent::__construct($entityType, $entity_info);
    $this->salsaType = str_replace('salsa_', '', $entityType);
  }

  /*
   * Overrides EntityDefaultUIController::hook_menu().
   */
  public function hook_menu() {
    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%entity_object';

    $items[$this->path . '/' . $wildcard] = array(
      'title callback' => 'entity_label',
      'title arguments' => array($this->entityType, $id_count),
      'page callback' => 'salsa_entity_view',
      'page arguments' => array($id_count),
      'load arguments' => array($this->entityType),
      'access callback' => 'entity_access',
      'access arguments' => array('view', $this->entityType, $id_count),
      'type' => MENU_NORMAL_ITEM,
    );
    return $items;
  }
}
