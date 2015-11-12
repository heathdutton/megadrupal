<?php // $Id$

/*
* Initialize theme settings
*/
/*
if (is_null(theme_get_setting('darkblue_fancydates'))) {
  global $theme_key;

  $defaults = array(
    'darkblue_fancydates' => 1,
  );

  // Get default theme settings.
  $settings = theme_get_settings($theme_key);
  // Don't save the toggle_node_info_ variables.
  if (module_exists('node')) {
    foreach (node_get_types() as $type => $name) {
      unset($settings['toggle_node_info_' . $type]);
    }
  }
  // Save default theme settings.
  variable_set(
    str_replace('/', '_', 'theme_'. $theme_key .'_settings'),
    array_merge($defaults, $settings)
  );
  // Force refresh of Drupal internals.
  theme_get_setting('', TRUE);
}
*/

/**
 * Override or insert variables into the page template.
 */
function darkblue_preprocess_page(&$vars) {
  $vars['primary_local_tasks'] = $vars['tabs'];
  unset($vars['primary_local_tasks']['#secondary']);
  $vars['secondary_local_tasks'] = array(
    '#theme' => 'menu_local_tasks',
    '#secondary' => $vars['tabs']['#secondary'],
  );
}

/**
 * Override or insert variables into the node templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
function darkblue_preprocess_node(&$vars, $hook) {
  $node = $vars['node'];
   switch ($node->type) {
    case 'blog':
      $fancydates = theme_get_setting('darkblue_fancydates');
      $vars['fancydates'] = $fancydates; // we pass wether or not we have the setting on, so that we can style appropiately
      if ($fancydates){
        $vars['blog_date'] = darkblue_blog_date($node);
      }
      else {
        $vars['blog_date'] = format_date($node->created);
        }
      break;
  }
}


/**
 * Formats calendar style dates for blog posts.
 *
 * @param $node
 *   The node object from which to extract submitted date information.
 * @return themed date.
 */
function darkblue_blog_date($node) {
  $day = format_date($node->created, 'custom', "j");
  $month = format_date($node->created, 'custom', "M");
  $year = format_date($node->created, 'custom', "Y");
  $output = '<span class="day">'. $day .'</span>';
  $output .= '<span class="month">'. $month .'</span>';
  $output .= '<span class="year">'. $year .'</span>';
  return $output;
}
