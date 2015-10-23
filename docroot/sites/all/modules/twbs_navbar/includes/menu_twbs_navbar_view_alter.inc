<?php

/**
 * Implements hook_twbs_navbar_view_alter().
 */
function menu_twbs_navbar_view_alter(&$build) {
  $links = array();

  $menu = db_select('menu_links', 'ml')
    ->fields('ml')
    ->condition('ml.menu_name', 'management')
    ->condition('ml.module', 'system')
    ->condition('ml.link_path', 'admin')
    ->execute()->fetchAssoc();
  if ($menu) {
    $tree = menu_build_tree('management', array(
      'expanded' => array($menu['mlid']),
      'min_depth' => $menu['depth'] + 1,
      'max_depth' => $menu['depth'] + 1,
    ));
    $items = menu_tree_output($tree);
    foreach ($items as $item) {
      if (isset($item['#title']) && isset($item['#href'])) {
        $links[] = array(
          'title' => $item['#title'],
          'href' => $item['#href'],
        );
      }
    }
  }

  if ($links) {
    $build['navbar_nav']['menu'][] = array(
      '#theme' => 'link',
      '#text' => t('Menu'),
      '#path' => '#',
      '#options' => array(
        'html' => TRUE,
        'attributes' => array(
          'data-toggle' => 'dropdown',
          'class' => array(
            'dropdown-toggle',
            'twbs-navbar-icon',
            'twbs-navbar-icon-menu',
          ),
        ),
      ),
    );
    $build['navbar_nav']['menu'][] = array(
      '#theme' => 'links',
      '#links' => $links,
      '#attributes' => array(
        'class' => 'dropdown-menu',
      ),
    );
  }
}
