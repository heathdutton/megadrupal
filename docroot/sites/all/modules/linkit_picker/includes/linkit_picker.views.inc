<?php

/**
 * @file
 * Provide views data for linkit_picker.module.
 */


/**
 * Implements hook_views_data().
 */
function linkit_picker_views_data() {

  // Node field
  $data['linkit_picker_node']['table']['group'] = t('Linkit Picker');
  $data['linkit_picker_node']['table']['join'] = array(
    'node' => array(),
  );

  $data['linkit_picker_node']['linkit_picker'] = array(
    'group' => t('Linkit Picker'),
    'field' => array(
      'title' => t('Linkit Picker'),
      'help' => t('Provides info to Linkit Picker (nodes)'),
      'handler' => 'linkit_picker_handler_field_node_info',
    ),
  );

  // User field
  $data['linkit_picker_user']['table']['group'] = t('Linkit');
  $data['linkit_picker_user']['table']['join'] = array(
    'users' => array(),
  );

  $data['linkit_picker_user']['linkit_picker'] = array(
    'group' => t('Linkit Picker'),
    'field' => array(
      'title' => t('Linkit Picker'),
      'help' => t('Provides info to Linkit Picker (users)'),
      'handler' => 'linkit_picker_handler_field_user_info',
      'skip base' => 'term_data',
    ),
  );

  // Taxonomy field
  $data['linkit_picker_taxonomy']['table']['group'] = t('Linkit');
  $data['linkit_picker_taxonomy']['table']['join'] = array(
    'taxonomy_term_data' => array(),
  );

  $data['linkit_picker_taxonomy']['linkit_picker'] = array(
    'group' => t('Linkit Picker'),
    'field' => array(
      'title' => t('Linkit Picker'),
      'help' => t('Provides info to Linkit Picker (terms)'),
      'handler' => 'linkit_picker_handler_field_term_info',
    ),
  );

  return $data;
}