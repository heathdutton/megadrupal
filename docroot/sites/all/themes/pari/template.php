<?php
/**
* Breadcrumb Navigation
*/
function pari_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];

  if (!empty($breadcrumb)) {
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $heading = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    // Uncomment to add current page to breadcrumb
	$breadcrumb[] = drupal_get_title();
    return '<div class="breadcrumb">' . $heading . implode(' / ', $breadcrumb) . '</div>';
  }
}

/**
 * Duplicate of theme_menu_local_tasks() but adds clearfix to tabs.
 */
function pari_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="tabs primary clearfix">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="tabs secondary clearfix">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }
  return $output;
}

/**
 * Override or insert variables into the node template.
 */
function pari_preprocess_node(&$variables) {
  $node = $variables['node'];
  if ($variables['view_mode'] == 'full' && node_is_page($variables['node'])) {
    $variables['classes_array'][] = 'node-full';
  }
  $variables['date'] = t('!datetime', array('!datetime' =>  date('j M Y', $variables['created'])));
}

/**
 * Preprocess variables for region.tpl.php
 *
 * @param $variables
 * An array of variables to pass to the theme template.
 * @param $hook
 * The name of the template being rendered ("region" in this case.)
 */
function pari_preprocess_region(&$variables, $hook) {
  // Use a bare template for the content region.
  if ($variables['region'] == 'content') {
    $variables['theme_hook_suggestions'][] = 'region__bare';
  }
}

/**
 * Override or insert variables into the block templates.
 *
 * @param $variables
 * An array of variables to pass to the theme template.
 * @param $hook
 * The name of the template being rendered ("block" in this case.)
 */
function pari_preprocess_block(&$variables, $hook) {
  // Use a bare template for the page's main content.
  if ($variables['block_html_id'] == 'block-system-main') {
    $variables['theme_hook_suggestions'][] = 'block__bare';
  }
}

/**
 * Override or insert variables into the block templates.
 *
 * @param $variables
 * An array of variables to pass to the theme template.
 * @param $hook
 * The name of the template being rendered ("block" in this case.)
 */
function pari_process_block(&$variables, $hook) {
  // Drupal 7 should use a $title variable instead of $block->subject.
  $variables['title'] = $variables['block']->subject;
}
/**
 * Tell Drupal to look for a custom page template for each node type. 
 */
function pari_preprocess_page(&$vars, $hook) {
    if (isset($vars['node'])) {
        $suggest = "page__node__{$vars['node']->type}";
        $vars['theme_hook_suggestions'][] = $suggest;
    }
}