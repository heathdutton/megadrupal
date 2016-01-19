<?php

/**
 * Export User addressbook to Views.
 */

/**
 * Implements hook_views_data()
 */
function user_addressbook_views_data() {
  $data = array();

  $data['user_address']['table']['group']  = t('User address');

  $data['user_address']['table']['base'] = array(
    'field' => 'address_id',
    'title' => t('User address'),
    'help' => t('User address containing user defined addresses.'),
    'defaults' => array(
      'field' => 'name',
    ),
   // 'access query tag' => 'user_addressbook_access',
  );
  $data['user_address']['table']['entity type'] = 'user_address';

  // Expose the address ID.
  $data['user_address']['address_id'] = array(
    'title' => t('User address ID'),
    'help' => t('The unique internal identifier of the user address.'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the user address name.
  $data['user_address']['name'] = array(
    'title' => t('User address name'),
    'help' => t('The name identifier of the user address.'),
    'field' => array(
      'handler' => 'user_addressbook_handler_field_user_address',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'help' => t('User address name.'),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'help' => t('User address name.'),
      'many to one' => TRUE,
      'empty field name' => t('Uncategorized'),
    ),
  );

  // Expose the owner uid.
  $data['user_address']['uid'] = array(
    'title' => t('Owner uid'),
    'help' => t('The user who owns the address. If you need more fields than the uid add the content: owner relationship'),
    'relationship' => array(
      'title' => t('Address owner'),
      'help' => t('Relate address to the user who created it.'),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('owner'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_name',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'field' => array(
      'handler' => 'views_handler_field_user',
    ),
  );

  // Expose the created and changed timestamps.
  $data['user_address']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the user address was created.'),
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

  $data['user_address']['created_fulldate'] = array(
    'title' => t('Created date'),
    'help' => t('In the form of CCYYMMDD.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_fulldate',
    ),
  );

  $data['user_address']['created_year_month'] = array(
    'title' => t('Created year + month'),
    'help' => t('In the form of YYYYMM.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_year_month',
    ),
  );

  $data['user_address']['created_timestamp_year'] = array(
    'title' => t('Created year'),
    'help' => t('In the form of YYYY.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_year',
    ),
  );

  $data['user_address']['created_month'] = array(
    'title' => t('Created month'),
    'help' => t('In the form of MM (01 - 12).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_month',
    ),
  );

  $data['user_address']['created_day'] = array(
    'title' => t('Created day'),
    'help' => t('In the form of DD (01 - 31).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_day',
    ),
  );

  $data['user_address']['created_week'] = array(
    'title' => t('Created week'),
    'help' => t('In the form of WW (01 - 53).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_week',
    ),
  );

  $data['user_address']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the profile was last updated.'),
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

  $data['user_address']['changed_fulldate'] = array(
    'title' => t('Updated date'),
    'help' => t('In the form of CCYYMMDD.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_fulldate',
    ),
  );

  $data['user_address']['changed_year_month'] = array(
    'title' => t('Updated year + month'),
    'help' => t('In the form of YYYYMM.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_year_month',
    ),
  );

  $data['user_address']['changed_timestamp_year'] = array(
    'title' => t('Updated year'),
    'help' => t('In the form of YYYY.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_year',
    ),
  );

  $data['user_address']['changed_month'] = array(
    'title' => t('Updated month'),
    'help' => t('In the form of MM (01 - 12).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_month',
    ),
  );

  $data['user_address']['changed_day'] = array(
    'title' => t('Updated day'),
    'help' => t('In the form of DD (01 - 31).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_day',
    ),
  );

  $data['user_address']['changed_week'] = array(
    'title' => t('Updated week'),
    'help' => t('In the form of WW (01 - 53).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_week',
    ),
  );

  // Expose links to operate on the profile.
  $data['user_address']['view_user_address'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the administrator view of the profile.'),
      'handler' => 'user_addressbook_handler_field_user_address_link',
    ),
  );
  $data['user_address']['edit_user_address'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the address.'),
      'handler' => 'user_addressbook_handler_field_user_address_link_edit',
    ),
  );
  $data['user_address']['delete_user_address'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the profile.'),
      'handler' => 'user_addressbook_handler_field_user_address_link_delete',
    ),
  );

  $data['user_address']['empty_text'] = array(
    'title' => t('Empty text'),
    'help' => t('Displays an appropriate empty text message for customer profile lists.'),
    'area' => array(
      'handler' => 'user_addressbook_handler_area_empty_text',
    ),
  );

  $data['user_address']['user_address_set_default'] = array(
    'title' => t('Set default address'),
    'help' => t('Set the current address as default.'),
    'field' => array(
      'handler' => 'user_addressbook_field_handler_user_address_set_default',
    ),
  );

  $data['user_address_defaults']['table']['group'] = t('User Addressbook Defaults');
  $data['user_address_defaults']['table']['join'] = array(
    'user_address' => array(
      'left_field' => 'address_id',
      'field' => 'address_id',
    ),
  );

  // Expose the profile type.
  $data['user_address_defaults']['type'] = array(
    'title' => t('User address default type'),
    'help' => t('The human-readable name of the type of the customer profile.'),
    'field' => array(
      'handler' => 'user_addressbook_handler_field_user_address_default_type',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'user_addressbook_handler_filter_user_address_default_type',
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

/**
 * Implements hook_views_data_alter()
 */
function user_addressbook_views_data_alter(&$data) {
  // Reverse relationship from User.
  $data['users']['user_address'] = array(
    'relationship' => array(
      'title' => t('User address owned'),
      'help' => t('Relate user address to the user who owns it. This relationship will create one record for each user address item owned by the user.'),
      'handler' => 'views_handler_relationship',
      'base' => 'user_address',
      'base field' => 'uid',
      'field' => 'uid',
      'label' => t('User addresses'),
    ),
  );
}
