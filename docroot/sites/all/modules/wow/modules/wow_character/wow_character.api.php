<?php

/**
 * @file
 * Hooks provided by the WoW Character module.
 */

/**
 * @addtogroup hooks
 * @{
 */
/**
 * A character is about to be main.
 *
 * This hook is primarily intended for modules that want to modify properties in
 * the character entity before saving.
 *
 * @param WoWCharacter $character
 *   The wow_character entity which is becoming main.
 *
 * @see hook_wow_character_insert(), hook_wow_character_update()
 */
function hook_wow_character_set_main($character) {
  // Check the character level.
  if ($character->level < 90) {
    // This character can't be main ! Cancels the operation.
    $character->isMain = FALSE;

    drupal_set_message(t('@name needs to be level 90 before you can claim it to be main!', array('@name' => $character->name)));
  }
}

/**
 * A character is about to be created or updated.
 *
 * This hook is primarily intended for modules that want to modify properties in
 * the character entity before saving.
 *
 * @param WoWCharacter $character
 *   The wow_character entity on which the operation is performed.
 *
 * @see hook_wow_character_insert()
 * @see hook_wow_character_update()
 */
function hook_wow_character_presave($character) {
  // Block the character.
  if (!$character->status) {
    // This character is being blocked and can't be main anymore.
    $character->isMain = FALSE;
  }
}

/**
 * A character was created.
 *
 * The module should save its custom additions to the character object into the
 * database.
 *
 * @param WoWCharacter $character
 *   The wow_character entity on which the operation is performed.
 *
 * @see hook_wow_character_presave()
 * @see hook_wow_character_update()
 */
function hook_wow_character_insert($character) {
  db_insert('mytable')
    ->fields(array(
      'myfield' => $custom,
      'cid' => $character->cid,
    ))
    ->execute();
}

/**
 * A character was updated.
 *
 * Modules may use this hook to update their character data in a custom storage
 * after a character object has been updated.
 *
 * @param WoWCharacter $character
 *   The character object on which the operation is performed.
 *
 * @see hook_wow_character_presave()
 * @see hook_wow_character_insert()
 */
function hook_wow_character_update($character) {
  db_insert('wow_character_changes')
    ->fields(array(
      'cid' => $character->cid,
      'changed' => REQUEST_TIME,
    ))
    ->execute();
}

/**
 * Respond to character deletion.
 *
 * This hook is invoked from wow_character_delete_multiple() before
 * field_attach_delete() is called and before characters are actually removed
 * from the database.
 *
 * @param WoWCharacter $character
 *   The wow_character entity on which the operation is performed.
 *
 * @see wow_character_delete_multiple()
 */
function hook_wow_character_delete($character) {
  db_delete('mytable')
    ->condition('cid', $character->cid)
    ->execute();
}

/**
 * The character's object information is being displayed.
 *
 * The module should format its custom additions for display and add them to
 * the $character->content array.
 *
 * @param WoWCharacter $character
 *   The wow_character entity on which the operation is being performed.
 * @param string $view_mode
 *   View mode, e.g. 'full'.
 * @param string $langcode
 *   The language code used for rendering.
 *
 * @see hook_wow_character_view_alter()
 * @see hook_entity_view()
 */
function hook_wow_character_view($character, $view_mode, $langcode) {

}

/**
 * The character was built; the module may modify the structured content.
 *
 * This hook is called after the content has been assembled in a structured array
 * and may be used for doing processing which requires that the complete character
 * content structure has been build.
 *
 * If the module wishes to act on the rendered HTML of the character rather than the
 * structured content array, it may use this hook to add a #post_render callback.
 *
 * @param array $build
 *   A renderable array representing the character.
 *
 * @see wow_character_view()
 * @see hook_entity_view_alter()
 */
function hook_wow_character_view_alter(&$build) {
  // Check for the existence of a field added by another module.
  if (isset($build['an_additional_field'])) {
    // Change its weight.
    $build['an_additional_field']['#weight'] = -10;
  }

  // Add a #post_render callback to act on the rendered HTML of the character.
  $build['#post_render'][] = 'my_module_wow_item_post_render';
}

/**
 * @} End of "addtogroup hooks".
 */
