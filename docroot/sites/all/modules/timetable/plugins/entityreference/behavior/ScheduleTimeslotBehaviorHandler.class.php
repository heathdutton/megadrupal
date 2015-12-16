<?php

/**
 * Timeslot behaviour handler for use in referencing from a session.
 *
 * This should be used for the entityreference field on schedule entities which
 * point to multiple timeslots that they comprise.
 *
 * The sole purpose of this is to hold the settings that form the combination of
 * schedule-timeslot-session entity types.
 *
 * It is recommended to use the Inline Entity Form widget on schedule timeslot
 * reference fields, which allows them to be quickly created.
 */
class ScheduleTimeslotBehaviorHandler extends EntityReference_BehaviorHandler_Abstract {

  /**
   * Implements EntityReferenceHandler::settingsForm().
   */
  public function settingsForm($field, $instance) {
    $target = $field['settings']['target_type'];
    if ($target != 'timeslot') {
      $form['ti-on-timeslot'] = array(
        '#markup' => t('This behavior can only be set when the target type is timeslot, but the target of this field is %target.', array(
          '%target' => $target,
        )),
      );
    }

    $form = array();

    return $form;
  }

}
