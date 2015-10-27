<?php

/**
 * @file
 * Views integration for regcode module.
 */

/**
 * Implements hook_views_handlers().
 */
function regcode_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'regcode') . '/includes',
    ),
    'handlers' => array(
      'views_handler_field_term_regcode_tid' => array(
        'parent' => 'views_handler_field_prerender_list',
      ),
    ),
  );
}


/**
 * Implements hook_views_data().
 */
function regcode_views_data() {
  $data = array();

  $data['regcode']['table']['group'] = t('Regcode');

  $data['regcode']['table']['base'] = array(
    'field' => 'rid',
    'title' => t('Registration code'),
    'help' => t('Registration codes created by the regcode module.'),
    'weight' => -10,
  );

  $data['regcode']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  $data['regcode']['rid'] = array(
    'title' => t('ID'),
    'help' => t('The unique index for the regcode'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['regcode']['uid'] = array(
    'title' => t('User'),
    'help' => t('The last user to use this registration code.'),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('Last user'),
    ),
  );

  $data['regcode']['code'] = array(
    'title' => t('Code'),
    'help' => t('The registration code.'),
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
  );

  $data['regcode']['created'] = array(
    'title' => t('Created'),
    'help' => t('Date the registration code was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['regcode']['lastused'] = array(
    'title' => t('Last used'),
    'help' => t('Date the registration code was last used.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['regcode']['begins'] = array(
    'title' => t('Begins'),
    'help' => t('Date the registration code becomes available for use.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['regcode']['expires'] = array(
    'title' => t('Expires'),
    'help' => t('Date the registration code expires.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['regcode']['is_active'] = array(
    'title' => t('Active'),
    'help' => t('Whether the registration code is active.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Published'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['regcode']['maxuses'] = array(
    'title' => t('Maximum uses'),
    'help' => t('The maximum times this code can be used.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['regcode']['uses'] = array(
    'title' => t('Uses'),
    'help' => t('The number of times this code has been used.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Map regcodes to taxonomy terms.
  $data['regcode_term']['table']['group'] = t('Taxonomy');

  $data['regcode_term']['table']['join']['regcode'] = array(
    'left_field' => 'rid',
    'field' => 'rid',
  );

  $data['regcode_term']['tid'] = array(
    'title' => t('Term ID'),
    'help' => t('The taxonomy term ID'),
    'field' => array(
      'title' => t('All terms'),
      'help' => t('Display all taxonomy terms associated with a node from specified vocabularies.'),
      'handler' => 'views_handler_field_term_regcode_tid',
      'skip base' => 'taxonomy_term_data',
    ),
  );

  $data['regcode_term']['rid'] = array(
    'title' => t('Regcode'),
    'help' => t('The regcode ID.'),
    'relationship' => array(
      'base' => 'regcode',
      'field' => 'rid',
      'handler' => 'views_handler_relationship',
      'label' => t('Regcode ID'),
    ),
  );

  // Let taxonomy know about regcode.
  $data['taxonomy_term_data']['table']['join']['regcode'] = array(
    'left_table' => 'regcode_term',
    'left_field' => 'tid',
    'field' => 'tid',
  );

  return $data;
}


/**
 * Implements hook_views_default_views().
 */
function regcode_views_default_views_alter(&$views) {
  if (isset($views['regcode']) && module_exists('views_bulk_operations')) {
    $view = $views['regcode'];
    $handler = $view->display['default'];

    $newfields = array();

    /* Field: Bulk operations: Regcode */
    $newfields['views_bulk_operations']['id'] = 'views_bulk_operations';
    $newfields['views_bulk_operations']['table'] = 'regcode';
    $newfields['views_bulk_operations']['field'] = 'views_bulk_operations';
    $newfields['views_bulk_operations']['label'] = '';
    $newfields['views_bulk_operations']['element_label_colon'] = FALSE;
    $newfields['views_bulk_operations']['vbo_settings']['display_type'] = '0';
    $newfields['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
    $newfields['views_bulk_operations']['vbo_settings']['force_single'] = 0;
    $newfields['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
    $newfields['views_bulk_operations']['vbo_operations'] = array(
      'action::regcode_activate_action' => array(
        'selected' => 1,
        'postpone_processing' => 0,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
      ),
      'action::regcode_deactivate_action' => array(
        'selected' => 1,
        'postpone_processing' => 0,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
      ),
      'action::regcode_delete_action' => array(
        'selected' => 1,
        'postpone_processing' => 0,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
      ),
      'action::regcode_mailer_mail_action' => array(
        'selected' => 1,
        'postpone_processing' => 0,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
      ),
      'action::regcode_tag_action' => array(
        'selected' => 1,
        'postpone_processing' => 0,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
      ),
    );

    $handler->display_options['fields'] = $newfields + $handler->display_options['fields'];
  }
}
