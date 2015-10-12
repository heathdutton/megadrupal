<?php

/**
 * @file
 * Contains functions that repopulate our cached data.
 */

/**
 * Helper for timetable_get_timetable_set().
 *
 * @return
 *  The full array of timetable set data, keyed by timetable bundle name. Each
 *  value is in the same format as the return value of
 *  timetable_get_timetable_set().
 */
function _timetable_get_all_timetable_sets() {
  $timetable_set_data = array();

  // Get the field map and filter it to just entityreference fields.
  $field_map = field_info_field_map();
  $entityreference_field_map = array_filter($field_map, function($item) {
    return ($item['type'] == 'entityreference');
  });

  // Keep track of fields we've seen to join up room fields to timeslots.
  $session_timeslot_field_instance_data = array();
  $session_room_fields = array();

  // Find the fields that interest us: the have our behaviours on them.
  foreach ($entityreference_field_map as $field_name => $field_data) {
    $field = field_info_field($field_name);

    $target_bundles = $field['settings']['handler_settings']['target_bundles'];

    switch (TRUE) {
      case !empty($field['settings']['handler_settings']['behaviors']['schedule_timeslot']['status']):
        // Sanity checks.
        if ($field['settings']['target_type'] != 'timeslot') {
          throw new Exception(t("Field @field-name uses schedule timeslot behaviour but does not target timeslot entities.", array(
            '@field-name' => $field_name,
          )));
        }
        if (count($target_bundles) > 1) {
          throw new Exception(t("Field @field-name uses schedule timeslot behaviour but targets more than one timeslot bundle.", array(
            '@field-name' => $field_name,
          )));
        }

        $target_timeslot_bundle = array_pop($target_bundles);

        // We require there to be only one type and bundle of schedule in a
        // single timetable braid.
        $keys = array_keys($entityreference_field_map[$field_name]['bundles']);
        $entity_type = array_pop($keys);
        $bundle_name = array_pop($entityreference_field_map[$field_name]['bundles'][$entity_type]);

        $timetable_set_data[$target_timeslot_bundle]['schedule'] = array(
          'field_name' => $field_name,
          'entity_type' => $entity_type,
          'bundle_name' => $bundle_name,
        );
        break;

      case !empty($field['settings']['handler_settings']['behaviors']['session_timeslot']['status']):
        // Sanity checks.
        if ($field['settings']['target_type'] != 'timeslot') {
          throw new Exception(t("Field @field-name uses schedule timeslot behaviour but does not target timeslot entities.", array(
            '@field-name' => $field_name,
          )));
        }
        if (count($target_bundles) > 1) {
          throw new Exception(t("Field @field-name uses schedule timeslot behaviour but targets more than one timeslot bundle.", array(
            '@field-name' => $field_name,
          )));
        }

        $target_timeslot_bundle = array_pop($target_bundles);

        $timetable_set_data[$target_timeslot_bundle]['session'] = array(
          'field_name' => $field_name,
          'bundles' => $entityreference_field_map[$field_name]['bundles'],
        );

        // Store a map for room fields to work with. This lists all the
        // instances of session timeslot fields, grouped by entity type and
        // then bundle name. The final value is an array of the timeslot
        // bundles, which should only contain one value.
        foreach ($field_data['bundles'] as $entity_type => $bundle_names) {
          foreach ($bundle_names as $bundle_name) {
            $session_timeslot_field_instance_data[$entity_type][$bundle_name][] = $target_timeslot_bundle;
          }
        }

        break;

      // Room reference on session entities.
      case !empty($field['settings']['handler_settings']['behaviors']['session_room']['status']):
        // We don't have any information about the timeslot here. So we have
        // to figure out where this field's instance are, and then match up
        // with field instances of session timeslot fields that exist on the
        // same bundle. From those, we can get our timeslot type, and then
        // place this session room field into the timetable set.
        // Unfortunately, we can't do this just yet, so instead store the
        // data we'll need later.
        $session_room_fields[$field_name]['entity_type'] = $field['settings']['target_type'];
        $session_room_fields[$field_name]['bundles'] = $field['settings']['handler_settings']['target_bundles'];
        $session_room_fields[$field_name]['instance bundles'] = $field_data['bundles'];

        // Allow for the special handling of entityreference settings where
        // an empty bundle array means all bundles.
        if (empty($session_room_fields[$field_name]['bundles'])) {
          $session_room_fields[$field_name]['bundles'] = array_keys(field_info_bundles($field['settings']['target_type']));
        }

        break;
    }
  } // foreach $entityreference_field_map

  // Match up session room fields to the right timeslot bundle. We have to do
  // this after the loop because we have no control over the order of fields
  // in the loop on the field map, so we might see the room field before the
  // timeslot field.
  foreach ($session_room_fields as $session_room_field_name => $field_data) {
    foreach (array_keys($field_data['instance bundles']) as $entity_type) {
      foreach ($field_data['instance bundles'][$entity_type] as $bundle_name) {
        // Find out whether there is also a session timeslot field on this
        // entity type and bundle. If so, that tells us that the session room
        // field in question is part of the same timetable set, and we can now
        // add its data.
        // First, a sanity check.
        if (count($session_timeslot_field_instance_data[$entity_type][$bundle_name]) > 1) {
          throw new Exception(t("The entity type !type bundle !bundle has more than one timeslot field on it.", array(
            '!type' => $entity_type,
            '!bundle' => $bundle_name,
          )));
        }

        // Get the timeslot bundle that the session timeslot field instance
        // points to.
        $timeslot_bundle = array_pop($session_timeslot_field_instance_data[$entity_type][$bundle_name]);

        // We've found a timeslot bundle. Now set the data about the room
        // entity type, bundles, and field into the timeslot set array.
        $timetable_set_data[$timeslot_bundle]['room'] = array(
          'field_name' => $session_room_field_name,
          // Whereas for the other pieces of the timeslot set, we store the
          // locations of field instances (because that tells us what bundles
          // are schedules and sessions), here we store the target(s) of the
          // field, for the same reason.
          'bundles' => array(
            $field_data['entity_type'] => $field_data['bundles'],
          ),
        );

      }
    }
  }

  return $timetable_set_data;
}
