<?php

/**
 * @file
 * Contains funtions to interact with character module hooks.
 */

/**
 * Implements hook_character_presave().
 *
 * @ingroup character
 */
function wow_character_wow_character_presave($character) {
  // We are about to set main status of a character.
  if ((!empty($character->is_new) && $character->isMain)
      || !empty($character->original) && $character->isMain != $character->original->isMain) {

    // Invokes set_main hook on behalf the wow_character entity. Modules
    // implementers can check the entity and change the isMain property for
    // instance.
    module_invoke_all('wow_character_set_main', $character);

    if ($character->isMain) {
      // Updates other character to ensure main unicity. By convention a user
      // owns one and only one main, we can safely use a db_update instead of
      // completely saving the entity.
      // The set_main hook has been triggered for this character, meaning that
      // others characters of the same user has been set to alternative ones.
      db_update('wow_characters')
        ->fields(array('isMain' => 0))
        ->condition('uid', $character->uid)
        ->condition('cid', $character->cid, '<>')
        ->execute();
    }
  }
}
