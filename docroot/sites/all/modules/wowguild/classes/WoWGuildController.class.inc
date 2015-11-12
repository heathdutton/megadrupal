<?php



/**
 * Controller class for wowguilds.
 *
 * This extends the DrupalDefaultEntityController class, adding required
 * special handling for user objects.
 */

class WoWGuildController extends EntityAPIController {
  function attachLoad(&$guilds, $revision_id = FALSE) {

    parent::attachLoad($guilds, $revision_id);

    $guild_key_lookup = array();
    foreach ($guilds as $key => $record) {
      $guild_key_lookup[$record->guild_key] = $key;
    }
    if (!empty($guild_key_lookup)) {
      $query = db_select('wowguild_cached_data', 'g');
      $query->fields('g');
      $query->condition('guild_key', array_keys($guild_key_lookup), 'IN');
      $results = $query->execute()->fetchAllAssoc('guild_key', PDO::FETCH_ASSOC);
    }
    
    foreach ($guilds as $index => $record) {
      if (array_key_exists($record->guild_key, $results)) {
        foreach ($results[$record->guild_key] as $key => $value) {
          $guilds[$index]->$key = $value;
        }
      }
      else {
        // No cached data in the database.  Load from armory.
        $guilds[$index]->loadFromArmory();
        $guilds[$index]->reloadMembers();
      }
      if (!is_array($guilds[$index]->tabard)) {
        $guilds[$index]->tabard = unserialize($guilds[$index]->tabard);
      }
      if (!is_array($guilds[$index]->members)) {
        $guilds[$index]->members = unserialize($guilds[$index]->members);
      }
      if (!is_array($guilds[$index]->raid_progression)) {
        $guilds[$index]->raid_progression = unserialize($guilds[$index]->raid_progression);
      }

      $guilds[$index]->fullrealm = $guilds[$index]->field_wowtoon_realm[LANGUAGE_NONE][0]['value'];
      $guilds[$index]->zone = strtolower(substr($guilds[$index]->fullrealm, strlen($guilds[$index]->fullrealm) - 2));
      $guilds[$index]->realm = substr($guilds[$index]->fullrealm, 0, strlen($guilds[$index]->fullrealm) - 3);

    }

  }


  /**
   * Create a avatar - we first set up the values that are specific
   * to our avatar schema but then also go through the EntityAPIController
   * function.
   *
   * @param $type
   *   The machine-readable type of the avatar.
   *
   * @return
   *   A avatar object with all default fields initialized.
   */
  public function create(array $values = array()) {
    // Add values that are specific to our Avatar
    $values += array(
        'gid' => '',
        'is_new' => TRUE,
        'name' => '',
    );

    //$avatar = parent::create($values);
    // Add is_new property if it is not set.
    $values += array('is_new' => TRUE);

    return new WoWGuild($values);
  }
}
