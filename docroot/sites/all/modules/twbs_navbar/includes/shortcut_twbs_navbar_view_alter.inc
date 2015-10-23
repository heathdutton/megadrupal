<?php

/**
 * Implements hook_twbs_navbar_view_alter().
 */
function shortcut_twbs_navbar_view_alter(&$build) {
  $links = array();

  $items = shortcut_renderable_links();
  foreach ($items as $item) {
    if (isset($item['#title']) && isset($item['#href'])) {
      $links[] = array(
        'title' => $item['#title'],
        'href' => $item['#href'],
      );
    }
  }

  if ($links) {
    $build['navbar_nav']['shortcut'][] = array(
      '#theme' => 'link',
      '#text' => t('Shortcut'),
      '#path' => '#',
      '#options' => array(
        'html' => TRUE,
        'attributes' => array(
          'data-toggle' => 'dropdown',
          'class' => array(
            'dropdown-toggle',
            'twbs-navbar-icon',
            'twbs-navbar-icon-shortcut',
          ),
        ),
      ),
    );
    $build['navbar_nav']['shortcut'][] = array(
      '#theme' => 'links',
      '#links' => $links,
      '#attributes' => array(
        'class' => 'dropdown-menu',
      ),
    );
  }
}
