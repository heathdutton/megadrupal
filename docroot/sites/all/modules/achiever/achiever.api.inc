<?php

/**
 * @file
 * File containing achiever hooks and alter functions.
 */

/**
 * Alters an activity before it is stored in the database.
 *
 * Developers can use this to modify an activity value.
 *
 * @param $keys
 *  The keys that will be used to build the activity namespace.
 * @param $type
 *  The activity type.
 * @param $variant
 *  The activity variant.
 * @param $variant
 *  The activity value.
 * @param $data
 *  The activity data.
 * @param $account
 *  The account the activity belongs to.
 */
function hook_achiever_activity_alter(&$keys, &$type, &$variant, &$value, $data, $account) {
  if (strpos($type, 'node:') === 0) {
    $node = $data;
    $variant = $node->type;
    $value = 1.5;
  }

  if (strpos($type, 'flag:') === 0) {
    $node = node_load($data['content_id']);
    $variant = $data['flag']->name;
    $value = 0.5;
  }

  if (!empty($node)) {
    $keys['node'] = $node->nid;
  }
}

/**
 * Registers Achievements.
 *
 * @return AchieverAchievement[]
 *  An array of object instances that extend the AchieverAchievement class.
 */
function hook_achiever_achievements_info() {
  return array(
    new UserOfTheYearAchievement(),
    new TopBloggerAchievement()
  );
}
