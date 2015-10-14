<?php
/**
 * @file
 * Breadcrumb theme overrides.
 */

/**
 * Implements hook_preprocess_breadcrumb().
 */
function bassline_preprocess_breadcrumb(&$variables) {
  // Add the current page title to the breadcrumb.
  $breadcrumb = &$variables['breadcrumb'];
  $breadcrumb_title = theme_get_setting('bassline_breadcrumb_title');
  if ($breadcrumb_title && !empty($breadcrumb)) {
    $item = menu_get_item();
    $breadcrumb[] = array(
      // If we are on a non-default tab, use the tab's title.
      'data' => !empty($item['tab_parent']) ? check_plain($item['title']) : drupal_get_title(),
      'class' => array('active'),
    );
  }
}

/**
 * Overrides theme_breadcrumb().
 *
 * Print breadcrumbs as an ordered list.
 */
function bassline_breadcrumb($variables) {
  $output = '';
  $breadcrumb = $variables['breadcrumb'];

  // Determine if we are to display the breadcrumb.
  $bassline_breadcrumb = theme_get_setting('bassline_breadcrumb');
  if ($bassline_breadcrumb == 1 && !empty($breadcrumb)) {
    $output = theme('item_list', array(
      'attributes' => array(
        'class' => array('breadcrumb'),
      ),
    'items' => $breadcrumb,
    'type' => 'ol',
    ));
  }
  return $output;
}
