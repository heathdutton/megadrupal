<?php

/**
 * @file
 * Contains the AchieverAchievement abstract class.
 */

/**
 * Class AchieverAchievement
 *
 * The base abstract class at the root of all Achievements. Handles the base
 * logic of checking and unlocking and achievement, and declares the
 * Achievement object interface.
 */
abstract class AchieverAchievement implements ObserverObserverInterface {

  /**
   * {@inheritdoc}
   */
  final public function onNotify(ObserverEventInterface $event) {
    if ($event->getType() != ACHIEVER_OBSERVER_ACTIVITY) {
      return;
    }

    /* @var $activity AchieverActivity */
    $activity = $event->getData();

    if (!$activity instanceof AchieverActivity) {
      return;
    }

    if (!$this->supportsActivity($activity)) {
      return;
    }

    if ($this->isUnlocked($activity->getAccount())) {
      return;
    }

    if ($this->isAchieved($activity)) {
      $this->unlock($activity->getAccount());
    }
  }

  /**
   * Unlocks the achievement.
   *
   * @param object $account
   *  The user account object for which the achievement will be unlocked.
   */
  final private function unlock($account) {
    $fields = array(
      'uid' => $account->uid,
      'achievement' => $this->getKey(),
      'date' => time(),
    );

    $merge = db_merge('achiever_achievements');

    $merge
      ->fields($fields)
      ->key(array('uid' => $fields['uid'], 'achievement' => $fields['achievement']))
      ->execute()
    ;

    // Clear in-memory achievement cache.
    drupal_static_reset(ACHIEVER_CACHE_ACHIEVEMENTS);

    // Notify the user of the achievement unlock.
    achiever_achievement_notify($account, $this);
  }

  /**
   * Checks whether this achievement has been unlocked by the given user.
   *
   * @param null|object $account
   *  The user account object. If null the current logged in user is used.
   * @return bool
   *  TRUE the achievement has been unlocked, FALSE otherwise.
   */
  final static function isUnlocked($account = null) {
    return array_key_exists(self::getKey(), achiever_get_user_achievements($account));
  }

  /**
   * Gets the achievement key.
   *
   * @return string
   *  The achievement key.
   */
  final static function getKey() {
    return get_called_class();
  }

  /**
   * Checks whether this achievement is a root achievement.
   *
   * Non-root achievements are usually used by other achievements as a
   * sub-achievements.
   *
   * @return bool
   *  TRUE this achievement is a root achievement, FALSE otherwise.
   */
  public function isRootAchievement() {
    return TRUE;
  }

  /**
   * Gets the translated achievement title.
   *
   * @return string
   */
  abstract public function getTitle();

  /**
   * Gets the translated achievement description.
   *
   * @return string
   */
  abstract public function getDescription();

  /**
   * Checks whether this achievement supports the given activity.
   *
   * @param AchieverActivity $activity
   *  The activity to be checked.
   * @return bool
   *  TRUE if it supports, FALSE otherwise.
   */
  abstract public function supportsActivity(AchieverActivity $activity);

  /**
   * Checks whether the achievement is achieved.
   *
   * This method is used internally by achiever to unlock the achievement
   * for the given account.
   *
   * @param AchieverActivity $activity
   *  The activity to be checked.
   * @return bool
   *  If TRUE the achievement will be activity user.
   */
  abstract protected function isAchieved(AchieverActivity $activity);

}
