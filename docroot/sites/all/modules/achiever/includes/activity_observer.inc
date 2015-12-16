<?php

/**
 * @file
 * Contains the AchieverActivityObserver class.
 */

/**
 * Class AchieverActivityObserver
 *
 * Responsible for registering all events that occur on the Observer event
 * system as user activities. This class acts as the the back-bone of the
 * Achievements system by recording data needed by it.
 */
class AchieverActivityObserver implements ObserverObserverInterface {

  /**
   * {@inheritdoc}
   */
  public function onNotify(ObserverEventInterface $event) {

    // Bailout on achiever activity events.
    if ($event->getType() == ACHIEVER_OBSERVER_ACTIVITY) {
      return;
    }

    $account = null;

    // When a user registers, he's anonymous. Force the activity as to be
    // originated from the newly created user.
    if ($event->getType() == ObserverEventTypes::USER_INSERTED) {
      $account = $event->getData();
    }

    achiever_register_activity(array(), $event->getType(), null, null, $event->getData(), $account);
  }

}
