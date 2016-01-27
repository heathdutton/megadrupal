<?php

/**
 * @file
 * Contains rules integration for the breadcrumb2 module needed during evaluation.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Condition: Breadcrumb object already exist.
 */
function breadcrumb2_rules_breadcrumb_exist($path) {
  $breadcrumb = breadcrumb2_load_by_path($path);
  if (!empty($breadcrumb)) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Action: Append breadcrumb trail to breadcrumb object.
 */
function breadcrumb2_rules_append_breadcrumb_trail(EntityDrupalWrapper $breadcrumb, $title, $url) {
  if (empty($title) || empty($url)) {
    return;
  }
  $link = array(
    'title' => $title,
    'url' => $url,
  );
  $links = $breadcrumb->link->value();
  $links[] = $link;
  $breadcrumb->link->set($links);
  //drupal_set_message('title:'.$title.'url:'.$url);  
}

/**
 * Action: Delete breadcrumb by path.
 */
function breadcrumb2_rules_delete_breadcrumb_by_path($path) {
  $breadcrumb = breadcrumb2_load_by_path($path);
  if (!empty($breadcrumb)) {
    breadcrumb2_delete($breadcrumb);
  }
}

/**
 * Action: Disable breadcrumb.
 */
function breadcrumb2_rules_disable_breadcrumb() {
  drupal_set_breadcrumb(array());
}
