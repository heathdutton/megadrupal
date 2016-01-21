<?php
/**
 * @file
 */

/**
 * Implements hook_page_alter().
 */
function lessons_bookmarks_page_alter(&$page) {
  if (_lessons_bookmarks_page_has_lessons_nodes($page)) {
    if (_lessons_bookmarks_flags_in_links($page)) {
      // Move flags out of links, to the top of lesson nodes, as siblings with fields.
      $flags = array();
      foreach ($page['content']['system_main']['nodes'] as $nid => $node) {
        if (is_numeric($nid)) { // Exclude '#sorted' element in this array.
          $flags[$nid] = $node['links']['flag'];
        }
      }
      foreach ($flags as $nid => $flag) {
        $page['content']['system_main']['nodes'][$nid]['flag'] = $flag;
        $page['content']['system_main']['nodes'][$nid]['flag']['#weight'] = -1;
        unset($page['content']['system_main']['nodes'][$nid]['links']['flag']);
      }
    }
  }
}

/**
 * Helper function. Determine if $page array has lessons nodes.
 * 
 * @param $page
 *  Array.
 *
 * @return
 *  Boolean.
 */
function _lessons_bookmarks_page_has_lessons_nodes($page) {
  $has_lessons = FALSE;
  // Check to see if $page has nodes.
  if (!empty($page['content']['system_main']['nodes'])) {
    // Check for lesson nodes.
    foreach ($page['content']['system_main']['nodes'] as $nid => $node) {
      if ($node['#bundle'] == 'lesson') {
        $has_lessons = TRUE;
        break;
      }
    }
  }

  return $has_lessons;
}

/**
 * Helper function. Determine if flags are under links in the page array (that's where
 * they show up by default). If they are there, we want to move them. If they're not there,
 * presumably they were already moved by another module (e.g. lessons_recommmendations,
 * lessons_bookmarks, or lessons_completed) or by someone who knows what they're doing.
 * 
 * @param $page
 *  Array.
 *
 * @return
 *  Boolean.
 */
function _lessons_bookmarks_flags_in_links($page) {
  $has_flags_in_links = FALSE;
  foreach ($page['content']['system_main']['nodes'] as $nid => $node) {
    if (isset($node['links']['flag'])) {
      $has_flags_in_links = TRUE;
      break;
    }
  }
  return $has_flags_in_links;
}
