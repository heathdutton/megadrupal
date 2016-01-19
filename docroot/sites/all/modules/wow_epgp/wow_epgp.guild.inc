<?php

/**
 * @file
 * Contains funtions to interact with wow_guild hooks.
 */

/**
 * Implements hook_guild_member_left().
 *
 * @ingroup guild
 */
function epgp_guild_member_left($guild, $character) {
  if (wow_guild_default('gid') == $guild->gid) {
    db_delete('wow_epgp')
      ->condition('cid', $character->cid)
      ->execute();
  }
}
