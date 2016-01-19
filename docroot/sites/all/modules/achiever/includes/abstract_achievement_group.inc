<?php

/**
 * @file
 * Contains the AchieverAchievementGroup class.
 */

/**
 * Class AchieverAchievementGroup
 *
 * This is the base class for Achievements which are composed by a collection
 * of Sub-Achievements. It contains the logic for unlocking the achievement,
 * and also declares the interface for Achievement Groups.
 */
abstract class AchieverAchievementGroup extends AchieverAchievement {

  /**
   * {@inheritdoc}
   */
  public function isAchieved(AchieverActivity $activity) {
    $achievements = $this->getAchievements();
    $achieved = 0;

    foreach ($achievements as $achievement) {
      if ($achievement->isUnlocked($activity->getAccount())) {
        $achieved++;
      }
    }

    return $achievements > 0 ? ($achieved == count($achievements)) : FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function supportsActivity(AchieverActivity $activity) {

    // An achievement group only runs when it's sub-achievements are completed.
    return $this->isAchieved($activity);
  }

  /**
   * Get the list of sub-achievements.
   *
   * @return AchieverAchievement[]
   *  A list of sub-achievements.
   */
  abstract public function getAchievements();

}
