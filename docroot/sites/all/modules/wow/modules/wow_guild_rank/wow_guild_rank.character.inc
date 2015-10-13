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
function wow_guild_rank_wow_character_presave($character) {
  if (empty($character->gid) || !wow_character_use_guild_ranks($character)) {
    // If the character guild is not the default guild or the use of guild rank
    // is not managed by this module, returns directly without going further.
    return;
  }

  if (isset($character->original) && $character->original->status != $character->status) {

    // Check if this is the highest rank in the guild.
    $query = db_select('wow_characters', 'c')
      ->orderBy('rank', 'ASC')
      ->fields('c', array('cid'))
      ->condition('status', 1)
      ->condition('uid', $character->uid)
      ->condition('gid', wow_guild_default('gid'))
      ->condition('cid', $character->cid, '<>');

    // If we are about to unblock this user, when no character have a higher
    // rank than this one, then he can become main.
    // If we are about to block this user, we need to select the next
    // candidate to become main.
    if ($character->status) {
      $query->condition('rank', $character->rank, '<');
    }

    $select = $query->execute();

    // Unblock the user.
    if ($character->status && !$select->rowCount()) {
      // No one is a better candidate than this character.
      $character->isMain = TRUE;
    }
    // Block the user.
    elseif (!$character->status) {
      // This character is being blocked and can't be main anymore.
      $character->isMain = FALSE;

      if ($select->rowCount()) {
        // This candidate is set as main.
        $candidate = wow_character_load($select->fetchColumn());
        $candidate->original = clone $candidate;
        $candidate->isMain = TRUE;
        $candidate->save();
      }
    }
  }
}
