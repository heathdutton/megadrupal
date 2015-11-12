<?php

/**
 * @file
 * Handles achiever theme related logic.
 */

/**
 * Implements template_preprocess_THEME() for achiever_achievements theme.
 */
function template_preprocess_achiever_achievements(&$vars) {
  global $user;

  $account = $vars['account'];

  /** @var AchieverAchievement[] $achievements */
  $achievements = $vars['achievements'];

  // Sort achievements alphabetically.
  $achievement_sorter = function ($a, $b) {
    return strcmp($a->getTitle(), $b->getTitle());
  };

  uasort($achievements, $achievement_sorter);

  // Render the achievements.
  $rendered_achievements = array();

  foreach ($achievements as $achievement) {

    // Non-root achievements are not rendered at this level.
    if (!$achievement->isRootAchievement()) continue;

    $theme_options = array(
      'achievement' => $achievement,
      'account' => $account
    );

    if ($achievement instanceof AchieverAchievementGroup) {
      $rendered_achievements[] = theme('achiever_achievement_group', $theme_options);
    } else {
      $rendered_achievements[] = theme('achiever_achievement', $theme_options);
    }
  }

  $vars['rendered_achievements'] = $rendered_achievements;
  $vars['is_current_user'] = ($account->uid === $user->uid);

  // Prepare the attributes.
  $vars['attributes'] = array(
    'class' => array('achiever-achievements')
  );
}

/**
 * Implements template_preprocess_THEME() for achiever_achievement theme.
 */
function template_preprocess_achiever_achievement(&$vars) {
  $account = $vars['account'];

  /** @var AchieverAchievement $achievement */
  $achievement = $vars['achievement'];

  $vars['title'] = $achievement->getTitle();
  $vars['description'] = $achievement->getDescription();
  $vars['classes_array'][] = $achievement->isUnlocked($account) ? 'achiever-unlocked' : 'achiever-locked';
  $vars['classes_array'][] = achiever_css_identifier($achievement->getKey());
}

/**
 * Implements template_preprocess_THEME() for achiever_achievement_group theme.
 */
function template_preprocess_achiever_achievement_group(&$vars) {
  $account = $vars['account'];

  /** @var AchieverAchievementGroup $achievement */
  $achievement = $vars['achievement'];
  $achievements = $achievement->getAchievements();

  $vars['title'] = $achievement->getTitle();
  $vars['description'] = $achievement->getDescription();
  $vars['classes_array'][] = $achievement->isUnlocked($account) ? 'achiever-unlocked' : 'achiever-locked';
  $vars['classes_array'][] = achiever_css_identifier($achievement->getKey());
  $vars['achievements'] = $achievements;
  $vars['unlocked'] = 0;
  $vars['total'] = 0;

  foreach ($achievements as $sub_achievement) {
    if ($sub_achievement->isUnlocked($account)) {
      $vars['unlocked']++;
    }

    $vars['total']++;
  }

  if ($vars['unlocked'] > 0) {
    $vars['classes_array'][] = 'achiever-in-progress';
  }
}
