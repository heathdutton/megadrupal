<?php

/**
 * @file
 * Export CRM Core Profile to views.
 */

/**
 * Implements hook_views_data().
 */
function crm_core_profile_views_data() {

  $title = t('CRM Core Profile');
  $data = array();

  $data['crm_core_profile']['table']['group'] = $title;
  $data['crm_core_profile']['table']['base'] = array(
    'field' => 'name',
    'title' => $title,
    'help' => t('CRM Core Profile that generates forms to collect contact information.'),
  );

  $data['crm_core_profile']['table']['join']['node'] = array(
    'left_table' => 'crm_core_profile_nodes',
    'left_field' => 'profile_name',
    'field' => 'name',
  );

  $data['crm_core_profile']['name'] = array(
    'title' => t('Machine name'),
    'help' => t('Machine readable name. The unique internal identifier for a profile form.'),
    'field' => array(
      'handler' => 'views_handler_field_machine_name',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['crm_core_profile']['label'] = array(
    'title' => t('Label'),
    'help' => t('The human-readable name of a profile form.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['crm_core_profile']['block'] = array(
    'title' => t('Block'),
    'help' => t('Indicate if a block should be generated from this profile form.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['crm_core_profile']['block_title'] = array(
    'title' => t('Block title'),
    'help' => t('Title that would be used for this profile form when shown in block.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['crm_core_profile']['page'] = array(
    'title' => t('Page'),
    'help' => t('Indicate if a page should be generated from this profile form.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['crm_core_profile']['page_title'] = array(
    'title' => t('Page title'),
    'help' => t('Title that would be used for this profile form when shown in page.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['crm_core_profile']['page_path'] = array(
    'title' => t('Page path'),
    'help' => t('The drupal path that should be created for this form.'),
    'field' => array(
      'handler' => 'views_handler_field_crm_core_profile_page_path',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['crm_core_profile']['page_redirect'] = array(
    'title' => t('Page redirect'),
    'help' => t('The redirect path when the form is submitted.'),
    'field' => array(
      'handler' => 'views_handler_field_crm_core_profile_page_redirect',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['crm_core_profile']['message'] = array(
    'title' => t('Message'),
    'help' => t('The message displayed when the form is submitted.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['crm_core_profile']['status'] = array(
    'title' => t('Profile status'),
    'help' => t('The status of the profile form.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['crm_core_profile']['uid'] = array(
    'title' => t('User ID'),
    'help' => t('The profile form creator uid.'),
    'field' => array(
      'handler' => 'views_handler_field_user',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_current',
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
    ),
  );

  $data['crm_core_profile']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the profile form was created.'),
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

  $data['crm_core_profile']['contact_match'] = array(
    'title' => t('Match type'),
    'help' => t('Type of contact matching for this profile form.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['crm_core_profile_nodes']['table']['group'] = $title;

  $data['crm_core_profile_nodes']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );

  $data['crm_core_profile_nodes']['profile_name'] = array(
    'title' => t('CRM Core Profile name'),
    'help' => t('CRM Core Profile machine readable name attached to node.'),
    'field' => array(
      'handler' => 'views_handler_field_machine_name',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  return $data;
}
