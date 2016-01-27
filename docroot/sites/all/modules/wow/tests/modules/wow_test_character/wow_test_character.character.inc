<?php

/**
 * @file
 * Contains funtions to interact with character module hooks.
 */

/**
 * Implements hook_wow_character_delete().
 *
 * @param WoWCharacter $character
 */
function wow_test_character_wow_character_delete($character) {
  variable_set('hook_wow_character_delete', TRUE);
}
