<?php

namespace Drupal\campaignion_action;

/**
 * Every ActionType (Petition) has to implement this interface.
 */
interface TypeInterface {
  /**
   * Return a wizard object for this ActionType.
   *
   * @param object $node The node to edit. Create a new one if NULL.
   *
   * @return \Drupal\oowizard\Wizard
   *  The wizard responsible for changing/adding actions of this type.
   */
  public function wizard($node = NULL);
  /**
   * Check whether or not this action-type is a donation.
   *
   * @return bool
   *   TRUE if this action type should be considered a donation else FALSE.
   */
  public function isDonation();
}
