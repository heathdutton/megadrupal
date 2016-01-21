<?php

/**
 * @file
 * Avatar editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring instead to use
 * views. That offers more flexibility to change a UI that will, more often than not,
 * be end-user facing.
 */



// Ensure that entity module loads.  Needed if installing with dependancies and uninstalls.
//drupal_load('module', 'entity');




/**
 * Fetch a wowguild object.
 *
 * @param $gid
 *   Integer specifying the guild id.
 * @param $reset
 *   A boolean indicating that the internal cache should be reset.
 * @return
 *   A fully-loaded $guild object or FALSE if it cannot be loaded.
 *
 * @see wowguild_load_multiple()
 */
function wowguild_load($gid, $reset = FALSE) {
  $guilds = wowguild_load_multiple(array($gid), array(), $reset);
  return reset($guilds);
}


/**
 * Load multiple guilds based on certain conditions.
 *
 * @param $gids
 *   An array of guild IDs.
 * @param $conditions
 *   An array of conditions to match against the {wowguild_entity} table. [DEPERCIATED]
 * @param $reset
 *   A boolean indicating that the internal cache should be reset.
 * @return
 *   An array of avatar objects, indexed by gid.
 *
 * @see entity_load()
 */
function wowguild_load_multiple($gids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('wowguild', $gids, $conditions, $reset);
}


/**
 * Deletes a guild.
 */
function wowguild_delete(WoWGuild $guild) {
  $guild->delete();
}


/**
 * Delete multiple guilds.
 *
 * @param $gids
 *   An array of guild IDs.
 */
function wowguild_delete_multiple(array $gids) {
  entity_get_controller('wowguild')->delete($gids);
}


/**
 * Create a wowguild object.
 */
function wowguild_create($values = array()) {
  return entity_get_controller('wowguild')->create($values);
}


/**
 * Saves a guild to the database.
 *
 * @param $guild
 *   The guild object.
 */
function wowguild_save(WoWGuild $guild) {
  return $guild->save();
}


/**
 * URI callback for guilds
 */
function wowguild_uri(WoWGuild $guild) {
  return array(
    'path' => 'guild/' . $guild->gid,
  );
}


/**
 * Menu title callback for showing individual entities
 */
function wowguild_page_title(WoWGuild $guild) {
  return $guild->name;
}

/**
 * Sets up content to show an individual guild
 */
function wowguild_view($guild, $view_mode = 'full') {
  $controller = entity_get_controller('wowguild');
  $content = $controller->view(array($guild->gid => $guild), $view_mode);
  return $content;
}

/**
 * Sets up content to show an individual guild
 */
function wowguild_view_page($guild, $view_mode = 'full') {
  $content = wowguild_view($guild, $view_mode);
  return $content;
}


/**
 * Load guild by name and realm.
 */

function wowguild_load_by_realm_name($fullrealm, $name) {
  $guild_key = wowguild_getkey($fullrealm, $name);
  $query = db_select('wowguild_cached_data', 'c');
  $query->fields('c', array('guild_key'));
  $query->leftJoin('wowguild_entity', 'g', 'g.guild_key = c.guild_key');
  $query->fields('g', array('gid'));
  $query->condition('c.guild_key', $guild_key);
  $result = $query->execute()->fetch();
  if (!empty($result->gid)) {
    return wowguild_load($result->gid);
  }
  else {
    return FALSE;
  }
}
