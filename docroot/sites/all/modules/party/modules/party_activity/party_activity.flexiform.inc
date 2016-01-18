<?php

/**
 * @file
 */

/**
 * Implements hook_flexiform_element_info().
 */
function party_activity_flexiform_element_info() {
  $elements = array();

  $types = db_select('party_activity_type', 'at')
    ->fields('at', array('type', 'label'))
    ->execute()
    ->fetchAllKeyed();

  foreach ($types as $type => $label) {
      $elements['party_activity'][$type]['party_activity:title'] = array(
        'label' => 'Title',
        'class' => 'PartyActivityElementTitle',
        'type' => 'title',
        'group' => 'Activity',
      );
  }

  return $elements;
}
