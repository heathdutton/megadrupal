<?php

/**
 * @file
 * Contains funtions to interact with character module hooks.
 */

/**
 * Implements hook_wow_guild_member_joined().
 *
 * @param WoWGuild $guild
 * @param WoWCharacter $character
 */
function wow_test_guild_wow_guild_member_joined($guild, $character) {
  $members = variable_get('hook_wow_guild_members_joined', array());
  $members[$character->cid] = $character;
  variable_set('hook_wow_guild_members_joined', $members);
}

/**
 * Implements hook_wow_guild_rank_update().
 *
 * @param WoWGuild $guild
 * @param WoWCharacter $character
 */
function wow_test_guild_wow_guild_rank_update($guild, $character) {
  $members = variable_get('hook_wow_guild_rank_update', array());
  $members[$character->cid] = $character;
  variable_set('hook_wow_guild_rank_update', $members);
}

/**
 * Implements hook_wow_guild_member_left().
 *
 * @param WoWGuild $guild
 * @param WoWCharacter $character
 */
function wow_test_guild_wow_guild_member_left($guild, $character) {
  $members = variable_get('hook_wow_guild_members_left', array());
  $members[$character->cid] = $character;
  variable_set('hook_wow_guild_members_left', $members);
}

/**
 * Implements hook_wow_guild_delete().
 *
 * @param WoWGuild $guild
 */
function wow_test_guild_wow_guild_delete($guild) {
  variable_set('hook_wow_guild_delete', TRUE);
}

