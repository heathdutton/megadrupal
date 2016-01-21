<?php

/**
 * @file
 * Definition of WoW\guild\GuildController.
 */

/**
 * Controller class for guilds.
 *
 * This extends the EntityAPIController class, adding required special handling
 * for guild objects.
 */
class WoWGuildController extends WoWRemoteEntityController {

  /**
   * Implements EntityAPIControllerInterface.
   *
   * @param WoWGuild $guild
   *   The wow_guild entity to save.
   * @param DatabaseTransaction $transaction
   *   Optionally a DatabaseTransaction object to use. Allows overrides to pass
   *   in their transaction object.
   */
  public function save($guild, DatabaseTransaction $transaction = NULL) {
    $transaction = isset($transaction) ? $transaction : db_transaction();

    // Saves the guild members.
    if (isset($guild->members)) {
      $this->saveMembers($guild, $transaction);
    }

    return parent::save($guild, $transaction);
  }

  /**
   * Permanently save a guild roster.
   *
   * This function triggers three different hooks:
   *   - 'rank_update' when a rank has changed since last update.
   *   - 'member_joined' when a new member joined the guild.
   *   - 'member_left' when a member left the guild.
   *
   * @param WoWGuild $guild
   */
  protected function saveMembers(WoWGuild $guild, DatabaseTransaction $transaction = NULL) {

    $main_characters = array();
    // The only point where the wow_guild_rank module can know if a rank changed
    // is in the guild entity save function itself. Check that the guild is the
    // site's default and the use_guild_ranks flag is set to TRUE.
    $process_ranks = wow_character_use_guild_ranks($guild);

    // Load every known members of the guild from the local database.
    $db_cids = db_select('wow_characters', 'c')
      ->fields('c', array('name', 'cid'))
      ->condition('c.gid', $guild->gid)
      ->execute()
      ->fetchAllKeyed();

    $db_characters = wow_character_load_multiple(array_values($db_cids));

    // Load every not yet known members of the guild. This build the list of
    // new characters who joined the guild.
    $new_cids = db_select('wow_characters', 'c')
      ->fields('c', array('name', 'cid'))
      ->condition('c.region', $guild->region)
      ->condition('c.realm', $guild->realm)
      ->condition('c.name', array_keys($guild->members), 'IN')
      ->condition('c.gid', $guild->gid, '<>')
      ->execute()
      ->fetchAllKeyed();

    $new_characters = wow_character_load_multiple(array_values($new_cids));

    // Process the roster from the service.
    foreach ($guild->members as $name => $values) {
      $cid = array_key_exists($name, $db_cids) ? $db_cids[$name] : FALSE;

      if ($cid) {
        $character = $db_characters[$cid];
        // The member already exists in the local database. For efficiency
        // manually save the original character before applying any changes.
        $character->original = clone $character;
        $character->merge($values);

        if ($character->original->rank != $character->rank) {
          // The member rank changed since last update.
          module_invoke_all($this->entityType . '_rank_update', $guild, $character);
        }

        // Remove character from the array if they are found. Remaining array
        // contains the list of players who are not in the guild anymore.
        unset($db_characters[$cid]);
      }
      else {
        // The member is not in the list of character having this guild as gid.
        // Try to lookup it anyway by name.
        $cid = array_key_exists($name, $new_cids) ? $new_cids[$name] : FALSE;

        if ($cid) {
          $character = $new_characters[$cid];
          // This character already exists in the db, but yet not linked with
          // the guild. Merge the gid and rank so we know this is an update.
          $character->original = clone $character;
          $character->merge($values);
        }
        else {
          // This character entity does not yet exists at all in the db. Creates
          // a new wow_character entity with the provided values.
          $character = entity_create('wow_character', $values);
        }

        module_invoke_all($this->entityType . '_member_joined', $guild, $character);
      }

      // Only process ranks for character which are associated with a user and
      // activated.
      if ($process_ranks && !empty($character->uid) && !empty($character->status)) {
        // Characters are sorted by ranks: the first time a user character is
        // encountered, he is the main. The following characters for the same
        // user will have a rank lower or equal to the first one; they are
        // alternatives characters.
        $character->isMain = empty($main_characters[$character->uid]);
        $main_characters[$character->uid] = TRUE;
      }

      // Permanently save new or existing characters into database.
      $character->save();
    }

    // Now process the remaining array of characters which is composed of
    // characters present in the database but not in the response from the
    // service. This mean they are not in the guild anymore.
    foreach ($db_characters as $character) {
      $character->original = clone $character;
      $character->isMain = 0;
      $character->gid = 0;
      $character->rank = 0;

      module_invoke_all($this->entityType . '_member_left', $guild, $character);
      $character->save();
    }
  }

  protected function formatMembers(WoWGuild $guild) {
    // Sort members by ranks.
    usort($guild->members, '_wow_character_rank_cmp');

    $members = array();
    $guild_value = array('gid' => $guild->gid, 'region' => $guild->region, 'realm' => $guild->realm);

    // Walk through every guild members to flattern the array.
    foreach ($guild->members as $member) {
      $name = $member['character']['name'];
      $members[$name] = array('rank' => $member['rank']) + $guild_value + $member['character'];
    }

    $guild->members = $members;
  }

  protected function saveAchievements(array $achievements) {

  }

  protected function saveNews(array $news) {

  }

  /**
   * @param WoWGuild $guild
   * @param WoWHttpResult $result
   */
  protected function processResult($guild, $result) {
    $guild->realm = wow_realm_to_slug($result->getArray('realm'));
    $guild->lastModified = $result->getArray('lastModified') / 1000;

    // Process the guild members.
    if (isset($guild->members)) {
      $this->formatMembers($guild);
    }
  }
}

/**
 * Callback used to sort an array of characters by rank.
 *
 * The lowest the highest.
 */
function _wow_character_rank_cmp($a, $b) {
  if ($a['rank'] == $b['rank']) {
    return 0;
  }
  return ($a['rank'] < $b['rank']) ? -1 : 1;
}
