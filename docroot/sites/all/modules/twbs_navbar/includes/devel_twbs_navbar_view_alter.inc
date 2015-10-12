<?php

/**
 * Implements hook_twbs_navbar_view_alter().
 */
function devel_twbs_navbar_view_alter(&$build) {
  $links = array();

  $tree = menu_build_tree('devel');
  $items = menu_tree_output($tree);
  foreach ($items as $item) {
    if (isset($item['#title']) && isset($item['#href'])) {
      $links[] = array(
        'title' => $item['#title'],
        'href' => $item['#href'],
      );
    }
  }

  if ($links) {
    $build['navbar_nav']['devel'][] = array(
      '#theme' => 'link',
      '#text' => t('Devel'),
      '#path' => '#',
      '#options' => array(
        'html' => TRUE,
        'attributes' => array(
          'data-toggle' => 'dropdown',
          'class' => array(
            'dropdown-toggle',
            'twbs-navbar-icon',
            'twbs-navbar-icon-devel',
          ),
        ),
      ),
    );
    $build['navbar_nav']['devel'][] = array(
      '#theme' => 'links',
      '#links' => $links,
      '#attributes' => array(
        'class' => 'dropdown-menu',
      ),
    );
  }
}
