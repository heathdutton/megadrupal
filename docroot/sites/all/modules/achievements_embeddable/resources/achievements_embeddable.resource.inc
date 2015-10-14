<?php

/**
 * @file
 * Callbacks for all achievements_embeddable services.
 */

/**
 * Resource get_user_achievements callback function.
 * Returns a list of all achievements that the specified user account has unlocked.
 *
 * @param integer $uid
 *   The id of the user whose achievements we will retrieve.
 *
 * @return array
 *   An array containing all of the achievements that the specified user
 *   account has unlocked keyed by achievement ID.
 */
function _achievements_embeddable_get_user_achievements($uid) {
  $result = t('No Achievements Found');
  $achievements = achievements_unlocked_already(NULL, $uid);
  if (!empty($achievements)) {
    $result = array();
    foreach ($achievements as $key => $achievement) {
      $result[$key] = achievements_load($key);
    }
  }
  return $result;
}

/**
 * Resource get_all_achievements callback function.
 * Retrieves a list of all achievements available on the site.
 *
 * @return array
 *   An array containing all of the achievements available on the site.
 */
function _achievements_embeddable_get_all_achievements() {
  return achievements_load(NULL, TRUE);
}

/**
 * Resource achievement_trigger callback function.
 * Executes any pre-defined callback function as defined when implementing
 * hook_achievements_embeddable_callbacks() on any module.
 * This module already implements this hook, defining the default trigger
 * events that need to take place. That is:
 *   - Notifies the Achievements module that the specified user has earned
 *     credit towards an achievement,
 *   - Unlocks any milestones (levels) if applicable.
 *
 * @param integer $uid
 *   The id of the user who has earned credit towards an achievement.
 * @param integer $achievement_id
 *   The ID of the achievement to award credit towards.
 * @param array $data
 *   (Optional) Additional data to pass to the achievements module.
 *
 * @return String
 *   Information about the earned credit.
 */
function _achievements_embeddable_achievement_trigger($uid, $achievement_id, $data = NULL) {
  $results = array();

  // Check $uid.
  if (!db_query("SELECT COUNT(*) FROM {users} WHERE uid = :uid", array(':uid' => $uid))->fetchField()) {
    return 'The specified user ' . check_plain($uid) . ' doesn\'t exists';
  }

  // Check $achievement_id.
  if (!_achievements_embeddable_achievement_exist($achievement_id)) {
    return 'The specified achievement ' . check_plain($achievement_id) . ' doesn\'t exist';
  }

  $callbacks = _achievements_embeddable_get_callbacks();
  foreach ($callbacks as $key => $callback) {
    if (function_exists($callback['callback'])) {
      // Call the functions, passing the proper arguments.
      $results[$key] = $callback['callback']($achievement_id, $uid);
    }
  }
  return $results;
}

/**
 * Uses module_invoke_all to invoke hook_achievement_callbacks() in all modules.
 */
function _achievements_embeddable_get_callbacks() {
  $callbacks = &drupal_static(__FUNCTION__);
  if (!isset($callbacks) || empty($callbacks)) {
    // Allow modules to define the callbacks they want to perform.
    $callbacks = module_invoke_all('achievements_embeddable_callbacks');
  }

  // Allow modules to alter the callbacks.
  drupal_alter('achievements_embeddable_callbacks', $callbacks);

  if (empty($callbacks)) {
    return FALSE;
  }

  return $callbacks;
}

/**
 * Helper function. Checks if the given Achievement exists.
 *
 * @param integer $achievement_id
 *   The ID of the achievement check.
 *
 * @return boolean
 *   TRUE if the achievement exits. FALSE if not.
 */
function _achievements_embeddable_achievement_exist($achievement_id) {
  $achievements = achievements_load(NULL, TRUE);
  foreach ($achievements as $key => $achievement) {
    $achievement_key = $key;
    if ($achievement_key == '-none-') {
      reset($achievement['achievements']);
      $achievement_key = key($achievement['achievements']);
    }
    $achievement_keys[] = $achievement_key;
  }
  return in_array($achievement_id, $achievement_keys);
}
