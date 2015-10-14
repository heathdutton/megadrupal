<?php

/**
 * Implements hook_views_decorators().
 */
function views_fieldrewrite_views_decorators() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'views_fieldrewrite') .'/handlers',
    ),
    'handlers' => array(
      'views_fieldrewrite_handler_field' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}
