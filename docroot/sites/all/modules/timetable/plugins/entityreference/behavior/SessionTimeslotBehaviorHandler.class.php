<?php

/**
 * Timeslot behaviour handler for use in referencing from a session.
 *
 * The sole purpose of this is to hold the settings that form the combination of
 * schedule-timeslot-session entity types.
 */
class SessionTimeslotBehaviorHandler extends EntityReference_BehaviorHandler_Abstract {

  /**
   * Implements EntityReferenceHandler::settingsForm().
   */
  public function settingsForm($field, $instance) {
    $form = array();

    $target = $field['settings']['target_type'];
    if ($target != 'timeslot') {
      $form['ti-on-timeslot'] = array(
        '#markup' => t('This behavior can only be set when the target type is timeslot, but the target of this field is %target.', array(
          '%target' => $target,
        )),
      );
    }

    // TODO: add this when https://drupal.org/node/2221945 is fixed and released
    // otherwise it causes a crash!
    //$form['#element_validate'][] = 'timetable_entityreference_field_settings_form_element_validate';

    return $form;
  }

}
