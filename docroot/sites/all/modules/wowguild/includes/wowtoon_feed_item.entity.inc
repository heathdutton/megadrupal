<?php



/**
 * Determines whether the given user has access to a avatar.
 *
 * @param $op
 *   The operation being performed. One of 'view', 'update', 'create', 'delete'
 *   or just 'edit' (being the same as 'create' or 'update').
 * @param $avatar
 *   Optionally a avatar or a avatar type to check access for. If nothing is
 *   given, access for all avatars is determined.
 * @param $account
 *   The user to check for. Leave it to NULL to check for the global user.
 * @return boolean
 *   Whether access is allowed or not.
 */
function wowtoon_feed_item_access($op, $avatar = NULL, $account = NULL) {
  if (user_access('administer avatars', $account)) {
    return TRUE;
  }
  if (isset($avatar) && $type_name = $avatar->type) {
    switch ($op) {
      case 'view':
        return user_access("access content", $account);
      case 'edit':
      case 'create':
        return FALSE;
        break;
    }
  }
  return FALSE;
}




/**
 * Fetch a avatar object. Make sure that the wildcard you choose
 * in the avatar entity definition fits the function name here.
 *
 * @param $aid
 *   Integer specifying the avatar id.
 * @param $reset
 *   A boolean indicating that the internal cache should be reset.
 * @return
 *   A fully-loaded $avatar object or FALSE if it cannot be loaded.
 *
 * @see alter_ego_load_multiple()
 */
function wowtoon_feed_item_load($achid, $reset = FALSE) {
  $feed_items = wowtoon_feed_item_load_multiple(array($achid), array(), $reset);
  return reset($feed_items);
}


/**
 * Load multiple avatars based on certain conditions.
 *
 * @param $aids
 *   An array of avatar IDs.
 * @param $conditions
 *   An array of conditions to match against the {avatar} table. [DEPERCIATED]
 * @param $reset
 *   A boolean indicating that the internal cache should be reset.
 * @return
 *   An array of avatar objects, indexed by aid.
 *
 * @see entity_load()
 * @see alter_ego_load()
 */
function wowtoon_feed_item_load_multiple($achids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('wowtoon_feed_item', $achids, $conditions, $reset);
}

function wowtoon_feed_item_load_achids_by_keyid($keyid, $type = '') {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'wowtoon_feed_item', '=');
  if (!empty($type)) {
    $query->propertyCondition('type', $type, '=');
  }
  $query->propertyCondition('keyid', $keyid, '=');
  $results = $query->execute();
  if (!empty($results['wowtoon_feed_item']) && is_array($results['wowtoon_feed_item'])) {
    $achids = array();
    foreach ($results['wowtoon_feed_item'] as $ach) {
      $achids[] = $ach->achid;
    }
    return $achids;
  }
  else {
    return array();
  }
  
}
function wowtoon_feed_item_load_by_keyid($keyid, $type = '') {
  $achids = wowtoon_feed_item_load_achids_by_keyid($keyid, $type);
  return wowtoon_feed_item_load_multiple($achids);
}

/**
 * Deletes a avatar.
 */
function wowtoon_feed_item_delete(WoWToonFeedItem $feed_item) {
  $feed_item->delete();
}


/**
 * Delete multiple avatars.
 *
 * @param $aids
 *   An array of avatar IDs.
 */
function wowtoon_feed_item_delete_multiple(array $achids) {
  entity_get_controller('wowtoon_feed_item')->delete($achids);
}


/**
 * Create a avatar object.
 */
function wowtoon_feed_item_create($values = array()) {
  return entity_get_controller('wowtoon_feed_item')->create($values);
}


/**
 * Saves a avatar to the database.
 *
 * @param $avatar
 *   The avatar object.
 */
function wowtoon_feed_item_save(WoWToonFeedItem $feed_item) {
  return $feed_item->save();
}


/**
 * URI callback for avatars
 */
function wowtoon_feed_item_uri(WoWToonFeedItem $feed_item){
  return array(
    'path' => 'achievement/' . $feed_item->achid,
  );
}


/**
 * Menu title callback for showing individual entities
 */
function wowtoon_feed_item_page_title(WoWToonFeedItem $feed_item) {
  return $feed_item->name;
}

/**
 * Sets up content to show an individual avatar
 */
function wowtoon_feed_item_view($feed_item, $view_mode = 'full') {
  $controller = entity_get_controller('wowtoon_feed_item');
  $content = $controller->view(array($feed_item->achid => $feed_item), $view_mode);
  return $content;
}

/**
 * Sets up content to show an individual avatar
 * @todo - get rid of drupal_set_title();
 */
function wowtoon_feed_item_page_view($feed_item, $view_mode = 'full') {
  $content = wowtoon_feed_item_view($feed_item, $view_mode);
  //drupal_set_title($avatar->name);
  return $content;
}




/**
* Access callback for the entity API.
*/
function wowtoon_feed_item_type_access($op, $type = NULL, $account = NULL) {
  return user_access('administer site configuration', $account);
}

/**
 * Gets an array of all avatar types, keyed by the type name.
 *
 * @param $type_name
 *   If set, the type with the given name is returned.
 * @return AvatarType[]
 *   Depending whether $type isset, an array of avatar types or a single one.
 */
function wowtoon_feed_item_get_types($type_name = NULL) {
  // entity_load will get the Entity controller for our avatar entity and call the load
  // function of that object - we are loading entities by name here.
  $types = entity_load_multiple_by_name('wowtoon_feed_item_type', isset($type_name) ? array($type_name) : FALSE);
  return isset($type_name) ? reset($types) : $types;
}


/**
 * Menu argument loader; Load a avatar type by string.
 *
 * @param $type
 *   The machine-readable name of a avatar type to load.
 * @return
 *   A avatar type array or FALSE if $type does not exist.
 */
function wowtoon_feed_item_type_load($type) {
  return wowtoon_feed_item_get_types($type);
}





/**
 * Saves a avatar type to the db.
 */
function wowtoon_feed_item_type_save(WoWToonFeedItemType $type) {
  $type->save();
}


/**
 * Deletes a avatar type from the db.
 */
function wowtoon_feed_item_type_delete(WoWToonFeedItemType $type) {
  $type->delete();
}






