<?php
/**
 * @file
 * Override of preprocess functions.
 */

/**
 * Override or insert variables into the page template.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 */
function tb_neris_preprocess_page(&$vars) {
  if (isset($vars['node'])) {
    if($vars['node']->type != 'page')
    {
      $result = db_select('node_type', NULL, array('fetch' => PDO::FETCH_ASSOC))
        ->fields('node_type', array('name'))
        ->condition('type', $vars['node']->type)
        ->execute();
      foreach ($result as $item) {
        $vars['title'] = $item['name'];
      }
    }
  }
}

/**
 * Override or insert variables into the node template.
 */
function tb_neris_preprocess_node(&$vars) {
  global $theme, $user;
  $vars['first_image'] = false;
  foreach($vars['content'] as $key => $image) {
    if(isset($image['#field_type']) && isset($image['#weight']) && $image['#field_type'] == 'image' && $image['#weight'] <= 0) {
      $vars['first_image'] = drupal_render($image);
      unset($vars['content'][$key]);
      break;
    }
  }
  $vars['page'] = ($vars['type'] == 'page') ? true : false; 
  $vars['date'] = format_date($vars['created'], 'custom', 'd.m.Y');
}

