<?php

class MongoEntityDefaultUIController extends EntityDefaultUIController {

  public function hook_menu() {
    $items = parent::hook_menu();
    // Add a tab to manage embedded entities
    // @todo Don't display this if there are no embedded entity types for this entity type
    $items[$this->path . '/embedded'] = array(
      'title' => 'Manage embedded entity types',
      'page callback' => 'mongo_entity_embedded_entity_types',
      'page arguments' => array($this->entityType),
      'access callback' => 'user_access',
      'access arguments' => array('administer site configuration'),
      'type' => MENU_LOCAL_TASK,
      'weight' => 100,
    );
    return $items;
  }

}