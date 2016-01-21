<?php

/**
 * @file
 * Implements all functions and classes for the Avatar entity.
 *
 */


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
function avatar_access($op, $avatar = NULL, $account = NULL) {
  global $user;
  if (user_access('administer avatars', $account)) {
    return TRUE;
  }
  if (isset($avatar) && $type_name = $avatar->type) {
    switch ($op) {
      case 'view':
        if (user_access("$op any $type_name avatar", $account)) {
          return TRUE;
        }
        break;
      case 'edit':
        
        if (!isset($account)) {
          $loggedin_account = $user;
        } else {
          $loggedin_account = $account;
        }
        
        if (!empty($loggedin_account->uid) && $loggedin_account->uid == $avatar->uid && user_access("$op own $type_name avatar", $loggedin_account)) {
          return TRUE;
        }
        elseif (user_access("$op any $type_name avatar", $account)) {
          return TRUE;
        }
        break;
      case 'create':
        if (user_access("create $type_name avatar", $account)) {
          return TRUE;
        }
    }
  }
  return FALSE;
}


/**
* Fetch a avatar object. Make sure that the wildcard you choose
* in the avatar entity definition fits the function name here.
*
* @param $tid
*   Integer specifying the avatar id.
* @param $reset
*   A boolean indicating that the internal cache should be reset.
* @return
*   A fully-loaded $avatar object or FALSE if it cannot be loaded.
*
* @see mmoguild_load_multiple()
*/
function avatar_load($aid, $reset = FALSE) {
  $avatars = avatar_load_multiple(array($aid), array(), $reset);
  return reset($avatars);
}


/**
 * Load multiple avatars based on certain conditions.
 *
 * @param $tids
 *   An array of avatar IDs.
 * @param $conditions
 *   An array of conditions to match against the {avatar} table. [DEPERCIATED]
 * @param $reset
 *   A boolean indicating that the internal cache should be reset.
 * @return
 *   An array of avatar objects, indexed by tid.
 *
 * @see entity_load()
 * @see mmoguild_load()
 */
function avatar_load_multiple($aids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('avatar', $aids, $conditions, $reset);
}


/**
 * Deletes a avatar.
 */
function avatar_delete(Avatar $avatar) {
  $avatar->delete();
}


/**
 * Delete multiple avatars.
 *
 * @param $tids
 *   An array of avatar IDs.
 */
function avatar_delete_multiple(array $aids) {
  entity_get_controller('avatar')->delete($aids);
}


/**
 * Create a avatar object.
 */
function avatar_create($values = array()) {
  return entity_get_controller('avatar')->create($values);
}


/**
 * Saves a avatar to the database.
 *
 * @param $avatar
 *   The avatar object.
 *
 * @see avatar_page_view()
 */
function avatar_save(Avatar $avatar) {
  // Clear popup cache.
  //cache_clear_all('alter_ego:avatar_popup_text:' . $avatar->aid);
  return $avatar->save();
}


/**
 * URI callback for avatars
 */
function avatar_uri(Avatar $avatar) {
  return array(
    'path' => 'avatar/' . $avatar->aid,
  );
}


/**
 * Menu title callback for showing individual entities
 */
function avatar_page_title(Avatar $avatar) {
  return $avatar->name;
}

/**
 * Sets up content to show an individual avatar
 */
function avatar_view($avatar, $view_mode = 'full', $langcode = NULL) {
  if (!isset($langcode)) {
    $langcode = $GLOBALS['language_content']->language;
  }
  $controller = entity_get_controller('avatar');
  $content = $controller->view(array($avatar->aid => $avatar), $view_mode, $langcode);
  return $content;
}

/**
 * Sets up content to show an individual avatar.
 *
 */
function avatar_page_view($avatar, $view_mode = 'full', $langcode = NULL) {
  $content = avatar_view($avatar, $view_mode, $langcode);
  if ($view_mode == 'popup') {
    // If we are rendering a popup, just send the content, not the whole page.
    print(render($content));
  }
  else {
    return $content;
  }
  
}


/**
 *
 * AvatarType Functions / Classes
 *
 */

/**
* Access callback for the entity API.
*/
function avatar_type_access($op, $type = NULL, $account = NULL) {
  return user_access('administer avatar types', $account);
}

/**
 * Gets an array of all avatar types, keyed by the type name.
 *
 * @param $type_name
 *   If set, the type with the given name is returned.
 * @return AvatarType[]
 *   Depending whether $type isset, an array of avatar types or a single one.
 */
function avatar_get_types($type_name = NULL) {
  // entity_load will get the Entity controller for our avatar entity and call the load
  // function of that object - we are loading entities by name here.
  $types = entity_load_multiple_by_name('avatar_type', isset($type_name) ? array($type_name) : FALSE);
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
function avatar_type_load($type) {
  return avatar_get_types($type);
}





/**
 * Saves a avatar type to the db.
 */
function avatar_type_save(AvatarType $type) {
  $type->save();
}


/**
 * Deletes a avatar type from the db.
 */
function avatar_type_delete(AvatarType $type) {
  $type->delete();
}

