<?php

/**
 * @file
 * File containing observer hooks and alter functions.
 */

/**
 * Registers observers to the global observable object.
 *
 * @return ObserverObserverInterface[]
 *  An array of object instances implement the ObserverObserverInterface.
 */
function hook_observer_info() {
  return array(
    new NsaObserver()
  );
}

/**
 * Allows removal, addition or verification of observers registered to
 * the global observable object.
 *
 * @param ObserverObserverInterface[] $observers
 */
function hook_observer_info_alter(&$observers) {

  // Remove unwanted observer.
  foreach ($observers as $index => $observer) {
    if (is_a($observer, 'NsaObserver')) {
      unset($observers[$index]);
    }
  }
}
