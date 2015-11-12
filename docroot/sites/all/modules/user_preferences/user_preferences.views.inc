<?php

/**
 * @file
 * Provide additional Views fields user preferences.
 */

/**
 * Implements hook_views_data().
 */
function user_preferences_views_data() {
  $data = array();
  $definitions = user_preferences_definitions();

  if ($definitions) {
    // If we've setup any data then add in the table definition.
    $data['user_preferences'] = array(
      'table' => array(
        'group' => t('User preferences'),
        'base' => array(
          'field' => 'up_id',
          'title' => t('User preferences'),
          'help' => t('Use this to filter users by their defined preferences.'),
          'weight' => 0,
        ),
        'join' => array(
          'users' => array(
            'left_field' => 'uid',
            'field' => 'uid',
          ),
        ),
      ),
      'name' => array(
        'title' => t('Machine name of the preference'),
        'help' => t('The machine name of the preference for a user.'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_string',
        ),
      ),
      'uid' => array(
        'title' => t('ID of the user which this preference belongs to.'),
        'relationship' => array(
          'base' => 'users',
          'base field' => 'uid',
          'handler' => 'views_handler_relationship',
          'title' => t('User preferences'),
          'help' => t('Access user preference data.'),
        ),
      ),
      //'value' => array(),
      //'module' => array(),
    );

    // Loop through and add in any definitions which have a views handler
    // defined.
    foreach ($definitions as $name => $definition) {
      if (!empty($definition['views_handlers'])) {
        foreach ($definition['views_handlers'] as $handler_name => $handler) {
          $data['user_preferences'][$handler_name] = $handler;
        }
      }
    }
  }

  return $data;
}
