<?php

/**
 * Implements hook_views_data().
 */
function tribune_local_views_data() {
  return array(
    'tribune_local' => array(
      'table' => array(
        'group' => t('Tribune (local)'),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
          ),
        ),
      ),
      'users' => array(
        'title' => t('Active users'),
        'help' => t('How many users are viewing this tribune.'),
        'sort' => array(
          // It... should work fine as the first number of a serialized
          // array is its size.
          'handler' => 'views_handler_sort',
        ),
      ),
    ),
  );
}
