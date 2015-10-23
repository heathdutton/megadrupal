<?php

/**
 * Behaviour handler for use in referencing a room from a session.
 *
 * The sole purpose of this is to mark the field on sessions that is used for
 * rooms. In future it may hold additionl settings.
 */
class SessionRoomBehaviorHandler extends EntityReference_BehaviorHandler_Abstract {

  /**
   * Implements EntityReferenceHandler::settingsForm().
   */
  public function settingsForm($field, $instance) {
    $form = array();

    return $form;
  }

}
