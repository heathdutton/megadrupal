<?php
/**
 * @file
 * ad.features.inc
 */

/**
 * Implements hook_views_api().
 */
function ad_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_eck_bundle_info().
 */
function ad_eck_bundle_info() {
  $items = array(
    'tracked_event_click' => array(
      'machine_name' => 'tracked_event_click',
      'entity_type' => 'tracked_event',
      'name' => 'click',
      'label' => 'click',
    ),
    'tracked_event_impression' => array(
      'machine_name' => 'tracked_event_impression',
      'entity_type' => 'tracked_event',
      'name' => 'impression',
      'label' => 'impression',
    ),
  );
  return $items;
}

/**
 * Implements hook_eck_entity_type_info().
 */
function ad_eck_entity_type_info() {
  $items = array(
    'tracked_event' => array(
      'name' => 'tracked_event',
      'label' => 'Tracked Event',
      'properties' => array(
        'uid' => array(
          'label' => 'Author',
          'type' => 'integer',
          'behavior' => 'author',
        ),
        'created' => array(
          'label' => 'Created',
          'type' => 'integer',
          'behavior' => 'created',
        ),
        'ad' => array(
          'label' => 'ad',
          'type' => 'integer',
          'behavior' => '',
        ),
        'ip_address' => array(
          'label' => 'IP address',
          'type' => 'text',
          'behavior' => '',
        ),
        'user_agent' => array(
          'label' => 'User Agent',
          'type' => 'text',
          'behavior' => '',
        ),
        'url' => array(
          'label' => 'URL',
          'type' => 'text',
          'behavior' => '',
        ),
        'page_title' => array(
          'label' => 'Page Title',
          'type' => 'text',
          'behavior' => '',
        ),
        'referrer' => array(
          'label' => 'Referrer',
          'type' => 'text',
          'behavior' => '',
        ),
        'page_unique_id' => array(
          'label' => 'Page Unique ID',
          'type' => 'text',
          'behavior' => '',
        ),
        'session' => array(
          'label' => 'Session',
          'type' => 'text',
          'behavior' => '',
        ),
        'unique_id' => array(
          'label' => 'Unique ID',
          'type' => 'text',
          'behavior' => '',
        ),
        'parent_unique_id' => array(
          'label' => 'Parent Unique ID',
          'type' => 'text',
          'behavior' => '',
        ),
      ),
    ),
  );
  return $items;
}
