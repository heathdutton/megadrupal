<?php 
/**
 * @file
 * Pages include file.
 */

/**
 * Menu callback for pinned_site.js; 
 * generates javascript which contains statements for creating dynamic Jump list.
 */
function pinned_site_generate_js() {
  if (ob_get_level()) {
    ob_end_clean();
  }
  
  drupal_add_http_header('Content-Type', 'text/javascript; charset=utf-8');
  
  $file = './' . drupal_get_path('module', 'pinned_site') . "/pinned_site.js.inc";
  
  if (is_file($file)) {
    
    $icon = check_url(variable_get('pinned_site_recent_posts_icon', ''));
    $category_name = _pinned_site_prepare_dynamic_jump_list_string(variable_get('pinned_site_recent_posts_jump_list_category_name', ''));
    $recent_posts_count = variable_get('pinned_site_recent_posts_count', 5);
    $recent_posts = _pinned_site_get_recent_posts($recent_posts_count);
    
    foreach ($recent_posts as $key => $recent_post) {
      $recent_posts[$key]['title'] = _pinned_site_prepare_dynamic_jump_list_string($recent_post['title']);
    }
    
    include($file);
  }
  
  module_invoke_all('exit');
  
  exit();
}

/**
 * Generates the browserconfig.xml.
 */
function pinned_site_generate_browserconfig() {
  $output = 'test';
  
  $browserconfig = new SimpleXMLElement('<browserconfig/>');
  $msapplication = $browserconfig->addChild('msapplication');
  
  $tile = $msapplication->addChild('tile');
  
  $square70x70logo_data = variable_get('pinned_site_msapplication_tile_square70x70logo', '');
  if (!empty($square70x70logo_data)) {
    $square70x70logo = $tile->addChild('square70x70logo');
    $square70x70logo->addAttribute('src', $square70x70logo_data);
  }
  
  $square150x150logo_data = variable_get('pinned_site_msapplication_tile_square150x150logo', '');
  if (!empty($square150x150logo_data)) {
    $square150x150logo = $tile->addChild('square150x150logo');
    $square150x150logo->addAttribute('src', $square150x150logo_data);
  }
  
  $wide310x150logo_data = variable_get('pinned_site_msapplication_tile_wide310x150logo', '');
  if (!empty($wide310x150logo_data)) {
    $wide310x150logo = $tile->addChild('wide310x150logo');
    $wide310x150logo->addAttribute('src', $wide310x150logo_data);
  }
  
  $square310x310logo_data = variable_get('pinned_site_msapplication_tile_square310x310logo', '');
  if (!empty($square310x310logo_data)) {
    $square310x310logo = $tile->addChild('square310x310logo');
    $square310x310logo->addAttribute('src', $square310x310logo_data);
  }
  
  $TileImage_data = variable_get('pinned_site_msapplication_tileimage', '');
  if (!empty($TileImage_data)) {
    $TileImage = $tile->addChild('TileImage');
    $TileImage->addAttribute('src', $TileImage_data);
  }
  
  $TileColor_data = variable_get('pinned_site_msapplication_tilecolor', '');
  if (!empty($TileColor_data)) {
    $tile->addChild('TileColor', $TileColor_data);
  }
  
  $polling_uri_1_data = variable_get('pinned_site_notification_polling_uri1', '');
  $polling_uri_2_data = variable_get('pinned_site_notification_polling_uri2', '');
  $polling_uri_3_data = variable_get('pinned_site_notification_polling_uri3', '');
  $polling_uri_4_data = variable_get('pinned_site_notification_polling_uri4', '');
  $polling_uri_5_data = variable_get('pinned_site_notification_polling_uri5', '');
  
  if (!empty($polling_uri_1_data) || !empty($polling_uri_2_data) || !empty($polling_uri_3_data) || !empty($polling_uri_4_data) || !empty($polling_uri_5_data)) {
    $notification = $msapplication->addChild('notification');
    
    if (!empty($polling_uri_1_data)) {
      $polling_uri_1 = $notification->addChild('polling-uri');
      $polling_uri_1->addAttribute('src', $polling_uri_1_data);
    }
    
    if (!empty($polling_uri_2_data)) {
      $polling_uri_2 = $notification->addChild('polling-uri2');
      $polling_uri_2->addAttribute('src', $polling_uri_2_data);
    }
    
    if (!empty($polling_uri_3_data)) {
      $polling_uri_3 = $notification->addChild('polling-uri3');
      $polling_uri_3->addAttribute('src', $polling_uri_3_data);
    }
    
    if (!empty($polling_uri_4_data)) {
      $polling_uri_4 = $notification->addChild('polling-uri4');
      $polling_uri_4->addAttribute('src', $polling_uri_4_data);
    }
    
    if (!empty($polling_uri_5_data)) {
      $polling_uri_5 = $notification->addChild('polling-uri5');
      $polling_uri_5->addAttribute('src', $polling_uri_5_data);
    }
    
    $notification->addChild('frequency', variable_get('pinned_site_notification_freqency', 30));
    $notification->addChild('cycle', variable_get('pinned_site_notification_cycle', 1));
  }
  
  $badge_polling_uri_data = variable_get('pinned_site_badge_polling_uri', '');
  
  if (!empty($badge_polling_uri_data)) {
    $badge = $msapplication->addChild('badge');
    $badge_polling_uri = $badge->addChild('polling-uri');
    $badge_polling_uri->addAttribute('src', $badge_polling_uri_data);
    $badge->addChild('frequency', variable_get('pinned_site_badge_freqency', 30));
  }

  // Allow for altering of the browserconfig.
  drupal_alter('pinned_site_browserconfig', $browserconfig);
  
  drupal_add_http_header('Content-Type', 'application/xml; charset=utf-8');
  print $browserconfig->asXML();
}

/**
 * Gets recent nodes according to the settings of allowed content types 
 * and sorts them by descending of creation date. 
 * Then sort nodes ascending and returns $limit of them. 
 * The ascending sorting is necessary because taskbar jump list works like stack:
 * the last added is the first in list.
 * 
 * @param int $limit
 */
function _pinned_site_get_recent_posts($limit) {
  
  // Get allowed content types.
  $node_types = variable_get('pinned_site_recent_posts_node_types', array());
  
  $selected_types = array();
  foreach ($node_types as $type => $value) {
    if (!empty($value)) {
      $selected_types[] = $type;
    }
  }
  
  // If no one content type is allowed then returns empty array.
  if (empty($selected_types)) {
    return array();
  }
  
  $result = db_select('node', 'n')
    ->fields('n', array('nid', 'title', 'created'))
    ->condition('type', $selected_types)
    ->condition('status', 1)
    ->orderBy('created', 'DESC')
    ->range(0, $limit)
    ->addTag('node_access')
    ->execute();
    
  $recent_posts = array();
  while ($post = $result->fetchAssoc()) {
    $recent_posts[$post['created']] = $post;
    $recent_posts[$post['created']]['url'] = url('node/' . $post['nid']);
  }
  
  // Sort ascending by keys of array (creation dates).
  ksort($recent_posts);
  
  return $recent_posts;
}

/**
 * Prepares string to output it as a parameter of javascript function.
 * It is unacceptable to use check_plain() function instead this 
 * because it converts quotes to their HTML equivalents,
 * and html entities are not rendered in the jump list.
 * The function just escapes double quote and removes any HTML tags from it.
 * @param string $content - value for preparing
 */
function _pinned_site_prepare_dynamic_jump_list_string($content) {
  $content = str_replace("\"", "\\\"", $content);
  return strip_tags($content);
}
