<?php

/**
 * @file
 * Provide views data and handlers for ec_address module.
 */

/**
 * Implements hook_views_data().
 */
function ec_address_views_data() {
  $data = array();

  $data['ec_address']['table']['group'] = t('Address');

  $data['ec_address']['table']['base'] = array(
    'field' => 'aid',
    'title' => t('Address'),
    'help' => t('Addresses provided by users on your site.'),
  );

  $data['ec_address']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
      'type' => 'LEFT', // Not all users will have an address
    ),
  );

  $data['ec_address']['uid'] = array(
    'title' => t('User'),
    'help' => t('User of which this address belongs to.'),
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
    ),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'label' => t('User'),
    ),
  );
  $data['ec_address']['firstname'] = array(
    'title' => t('First name'),
    'help' => t('The first name of the customer'),
    'field' => array(
      'field' => 'firstname',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),    
  );
  $data['ec_address']['lastname'] = array(
    'title' => t('Last name'),
    'help' => t('The last name of the customer'),
    'field' => array(
      'field' => 'lastname',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),    
  );
  $data['ec_address']['name'] = array(
    'real field' => 'firstname',
    'title' => t('Name'),
    'help' => t('Full name of the customer'),
    'field' => array(
      'handler' => 'ec_store_views_handler_field_name',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_string',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),    
  );
  $data['ec_address']['street1'] = array(
    'title' => t('Street'),
    'help' => t('The street address of the customer'),
    'field' => array(
      'field' => 'street1',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),    
  );
  $data['ec_address']['street2'] = array(
    'title' => t('Street Additional'),
    'help' => t('The additional street address of the customer'),
    'field' => array(
      'field' => 'street2',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),    
  );
  $data['ec_address']['city'] = array(
    'title' => t('City'),
    'help' => t('The city of the customer'),
    'field' => array(
      'field' => 'city',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),    
  );
  $data['ec_address']['state'] = array(
    'title' => t('State'),
    'help' => t('The state of the customer'),
    'field' => array(
      'field' => 'state',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),    
  );
  $data['ec_address']['zip'] = array(
    'title' => t('Zipcode'),
    'help' => t('The Zipcode of the customer'),
    'field' => array(
      'field' => 'zip',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),    
  );
  $data['ec_address']['country'] = array(
    'title' => t('Country'),
    'help' => t('The country of the customer'),
    'field' => array(
      'field' => 'country',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),    
  );
  $data['ec_address']['edit_address'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit an address'),
      'handler' => 'ec_address_handler_field_link_edit',
    ),
  );
  $data['ec_address']['delete_address'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete an address'),
      'handler' => 'ec_address_handler_field_link_delete',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function ec_address_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'ec_address') . '/views',
    ),
    'handlers' => array(
      'ec_address_handler_field_link_edit' => array(
        'parent' => 'views_handler_field',
      ),
      'ec_address_handler_field_link_delete' => array(
        'parent' => 'ec_address_handler_field_link_edit',
      ),
    ),
  );
}
