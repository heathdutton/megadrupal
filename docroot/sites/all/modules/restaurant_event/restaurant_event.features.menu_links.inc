<?php
/**
 * @file
 * restaurant_event.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function restaurant_event_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu_events:events
  $menu_links['main-menu_events:events'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'events',
    'router_path' => 'events',
    'link_title' => 'Events',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'main-menu_events:events',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => 10,
    'customized' => 1,
  );
  // Exported menu link: main-menu_past-events:events/past
  $menu_links['main-menu_past-events:events/past'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'events/past',
    'router_path' => 'events/past',
    'link_title' => 'Past Events',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'main-menu_past-events:events/past',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'parent_identifier' => 'main-menu_events:events',
  );
  // Exported menu link: main-menu_upcoming-events:events
  $menu_links['main-menu_upcoming-events:events'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'events',
    'router_path' => 'events',
    'link_title' => 'Upcoming Events',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'main-menu_upcoming-events:events',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'main-menu_events:events',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Events');
  t('Past Events');
  t('Upcoming Events');


  return $menu_links;
}
