<?php

/**
 * @file
 * Definition of WoW\character\CharacterRaceController.
 */

/**
 * Controller class for character races.
 *
 * This extends the DataResourcesController class, adding required special
 * handling for character race objects.
 */
class WoWCharacterRaceController extends WoWDataResourcesController {

  public function create(array $values = array()) {
    $entity = parent::create($values);
    $entity->wow_character_race= array(
      $values['language'] => array(0 => array('name' => $values['name']))
    );
    return $entity;
  }

  public function remotePath() {
    return 'data/character/races';
  }

}
