<?php

/**
 * @file
 * Hooks provided by the WoW Guild module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * A guild is about to be created or updated.
 *
 * This hook is primarily intended for modules that want to modify properties in
 * the guild entity before saving.
 *
 * @param object $guild
 *   The guild object on which the operation is performed.
 *
 * @see hook_wow_guild_insert()
 * @see hook_wow_guild_update()
 */
function hook_wow_guild_presave($guild) {

}

/**
 * A guild was created.
 *
 * The module should save its custom additions to the guild object into the
 * database.
 *
 * @param object $guild
 *   The guild object on which the operation is performed.
 *
 * @see hook_wow_guild_presave()
 * @see hook_wow_guild_update()
 */
function hook_wow_guild_insert($guild) {
  db_insert('mytable')
    ->fields(array(
      'myfield' => $custom,
      'gid' => $guild->gid,
    ))
    ->execute();
}

/**
 * A guild was updated.
 *
 * Modules may use this hook to update their guild data in a custom storage
 * after a guild object has been updated.
 *
 * @param object $guild
 *   The guild object on which the operation is performed.
 *
 * @see hook_wow_guild_presave()
 * @see hook_wow_guild_insert()
 */
function hook_wow_guild_update($guild) {
  db_insert('wow_guild_changes')
    ->fields(array(
      'gid' => $guild->gid,
      'changed' => REQUEST_TIME,
    ))
    ->execute();
}

/**
 * Respond to guild deletion.
 *
 * This hook is invoked from wow_guild_delete_multiple() before
 * field_attach_delete() is called and before guilds are actually removed
 * from the database.
 *
 * @param object $guild
 *   The guild object on which the operation is performed.
 *
 * @see wow_guild_delete_multiple()
 */
function hook_wow_guild_delete($guild) {
  db_delete('mytable')
    ->condition('gid', $guild->gid)
    ->execute();
}

/**
 * A character joined the guild.
 *
 * The module should save its custom additions to the character object into the
 * database.
 *
 * @param object $guild
 *   The $guild object which the character belongs.
 * @param object $character
 *   The character object on which the operation is performed.
 */
function hook_wow_guild_member_joined($guild, $character) {
  // If the character belongs to the site's default guild, displays a message.
  if (wow_guild_default('gid') == $guild->gid) {
    drupal_set_message(t('A new member has joined the guild ! Welcome to !character.', array('!character' => theme('wow_character_name', array('character' => $character)))));
  }
}

/**
 * Respond to a character whose left the guild.
 *
 * @param object $guild
 *   The $guild object which the character belongs.
 * @param object $character
 *   The character object on which the operation is performed.
 */
function hook_wow_guild_member_left($guild, $character) {
  if (wow_guild_default('gid') == $guild->gid) {
    db_delete('epgp')
      ->condition('cid', $character->cid)
      ->execute();
  }
}

/**
 * Respond to a character rank update.
 *
 * @param object $guild
 *   The $guild object which the character belongs.
 * @param object $character
 *   The character object on which the operation is performed.
 */
function hook_wow_guild_rank_update($guild, $character) {
  if ($character->rank > $character->original->rank) {
    $ranks = wow_guild_ranks();
    $message = '@name has been promoted in @guild to @rank.';
    $variables = array('@name' => $character->name, '@guild' => $guild->name, '@rank' => $ranks[$character->rank]->name);
    watchdog('wow_module', $message, $variables);
  }
}

/**
 * @} End of "addtogroup hooks".
 */
