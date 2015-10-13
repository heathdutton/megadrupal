<?php

/**
 * @file
 * Definition of WoW\character\CharacterController.
 */

/**
 * Controller class for characters.
 *
 * This extends the EntityAPIController class, adding required special handling
 * for character objects.
 */
class WoWCharacterController extends WoWRemoteEntityController {

  /**
   * @param WoWCharacter $character
   * @param WoWHttpResult $result
   */
  protected function processResult($character, $result) {
    $character->realm = wow_realm_to_slug($result->getArray('realm'));
    $character->lastModified = $result->getArray('lastModified') / 1000;
  }
}
