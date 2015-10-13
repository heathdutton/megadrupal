<?php

namespace Drupal\heatmap_me;

class Heatmap {
  public static function hook_page_alter(&$page) {
    $js = drupal_get_path('module', 'heatmap_me') . '/js/heatmap.js';
    $page['page_bottom']['#attached']['js'][] = $js;
  }
}
