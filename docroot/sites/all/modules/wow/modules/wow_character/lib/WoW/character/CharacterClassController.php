<?php

/**
 * @file
 * Definition of WoW\character\CharacterClassController.
 */

/**
 * Controller class for character classes.
 *
 * This extends the DataResourcesController class, adding required special
 * handling for character class objects.
 */
class WoWCharacterClassController extends WoWDataResourcesController {

  public function create(array $values = array()) {
    $entity = parent::create($values);
    $entity->wow_character_class= array(
      $values['language'] => array(0 => array('name' => $values['name']))
    );
    return $entity;
  }

  public function remotePath() {
    return 'data/character/classes';
  }

}
