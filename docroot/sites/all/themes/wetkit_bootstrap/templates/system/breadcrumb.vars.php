<?php
/**
 * @file
 * breadcrumb.vars.php
 */

/**
 * Implements hook_preprocess_breadcrumb().
 */
function wetkit_bootstrap_preprocess_breadcrumb(&$variables) {
  $breadcrumb = &$variables['breadcrumb'];

  if (theme_get_setting('bootstrap_breadcrumb_title') && !empty($breadcrumb)) {
    $item = menu_get_item();

    $page_title = !empty($item['tab_parent']) ? check_plain($item['title']) : drupal_get_title();

    // Handle wetkit_bootstrap not completely overriding Bootstrap itself.
    $last_breadcrumb = end($breadcrumb);
    if ($page_title == $last_breadcrumb['data']) {
      array_pop($breadcrumb);
    }

    if (!empty($page_title)) {
      $breadcrumb[] = array(
        // If we are on a non-default tab, use the tab's title.
        'data' => $page_title,
        'class' => array('active'),
      );
    }
  }
}
