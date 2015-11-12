<?php

/**
 * Implements hook_views_data().
 */
function maestro_initiator_views_data() {
  $data = array();
  _maestro_initiator_maestro_initiator_approval_views_data($data);
  return $data;
}

/**
 * Views data definition for table maestro_initiator_approval
 */
function _maestro_initiator_maestro_initiator_approval_views_data(&$data) {
  $data['maestro_initiator_approval']['table']['group']  = t('Workflow history');
  $data['maestro_initiator_approval']['uniqueid'] = array(
    'title' => t('Unique Id'),
    'help' => t('Unique Id bein tracked.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'Unique Id',
      'numeric' => TRUE,
    ),
  );
  
  $data['maestro_initiator_approval']['process_key'] = array(
    'title' => t('Process Key'),
    'help' => t('Process key used for filtering'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
      'name field' => 'Process Key',
      'numeric' => FALSE,
    ),
  );
  
  $data['maestro_initiator_approval']['uid'] = array(
    'title' => t('User Who Approved'),
    'help' => t('User Who Approved'),
    'field' => array(
      'handler' => 'views_handler_field_user',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'User UID who approved',
      'numeric' => TRUE,
    ),
    'relationship' => array(
      'title' => t('Approved By'),
      'help' => t('User who approved the task'),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'field' => 'uid',
      'label' => t('Approved By'),
    ),
  );
  $data['maestro_initiator_approval']['status'] = array(
    'title' => t('Approved Or Rejected'),
    'help' => t('Approved Or Rejected'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['maestro_initiator_approval']['adate'] = array(
    'title' => t('Date Of Approval'),
    'help' => t('Date Of Approval'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
}
