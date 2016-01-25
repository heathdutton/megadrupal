<?php
/**
 * @file
 * Override of theme_preprocess_...
 */

function tb_sirate_preprocess_html(&$vars) {
  $current_skin = theme_get_setting('skin');
  if (isset($_COOKIE['nucleus_skin'])) {
    $current_skin = $_COOKIE['nucleus_skin'];
  }
  $vars['classes_array'][] = !empty($current_skin) ? (" " . $current_skin . "-skin") : "";  
}

/**
 * Override or insert variables into the page template.
 *
 * @param array $vars
 *   An array of variables to pass to the theme template.
 */
function tb_sirate_preprocess_page(&$vars) {
  if (isset($vars['node'])) {
    if ($vars['node']->type != 'page') {
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
 *
 * @param array $vars
 *   An array of variables to pass to the theme template.
 */
function tb_sirate_preprocess_node(&$vars) {
  $vars['page'] = ($vars['type'] == 'page') ? TRUE : FALSE;
}
