<?php

/**
 * @file
 * Views integration for ec_store module.
 */

/**
 * Implements hook_views_data().
 */
function ec_store_views_data() {
  $data = array();

  if ($fields = ec_store_get_all_fields()) {
    foreach ($fields as $name => $field) {
      $table = 'ec_transaction_fields_' . $name;
      $data[$table]['table']['group'] = t('Transaction');
      $data[$table]['table']['join'] = array(
        'ec_transaction' => array(
          'table' => 'ec_transaction_fields',
          'left_field' => 'txnid',
          'field' => 'txnid',
          'type' => 'LEFT',
          'extra' => array(
            array(
              'table' => $table,
              'field' => 'name',
              'operator' => '=',
              'value' => $name,
            ),
          ),
        ),
        'ec_receipt' => array(
          'table' => 'ec_transaction_fields',
          'left_field' => 'erid',
          'field' => 'erid',
          'type' => 'LEFT',
          'extra' => array(
            array(
              'table' => $table,
              'field' => 'name',
              'operator' => '=',
              'value' => $name,
            ),
          ),
        ),
        'ec_receipt_allocation' => array(
          'table' => 'ec_transaction_fields',
          'left_field' => 'etid',
          'field' => 'txnid',
          'type' => 'LEFT',
          'extra' => array(
            array(
              'table' => $table,
              'field' => 'name',
              'operator' => '=',
              'value' => $name,
            ),
          ),
        ),    
      );
      $data[$table][$name] = array(
        'title' => $field->field_title,
        'help' => $field->field_description,
        'field' => array(
          'field' => 'value',
          'handler' => 'views_handler_field',
        ),
        'filter' => array(
          'field' => 'value',
          'handler' => 'views_handler_filter_string'
        ),
        'sort' => array(
          'field' => 'value',
          'handler' => 'views_handler_sort',
        ),
        'argument' => array(
          'field' => 'value',
          'handler' => 'views_handler_argument_string',
        )
      );
    }
  }

  $data['ec_transaction']['table']['group'] = t('Transaction');
  $data['ec_transaction']['table']['base'] = array(
    'field' => 'txnid',
    'title' => t('Transaction'),
    'help' => t('Transactions for sales that have been made.'),
  );
  
  $data['ec_transaction']['table']['join'] = array(
    'ec_receipt_allocation' => array(
      'left_field' => 'etid',
      'field' => 'txnid',
      'type' => 'LEFT',
    ),
  );
  
  
  $data['ec_transaction']['txnid'] = array(
    'title' => t('Transaction id'),
    'help' => t('A unique identifier of a transaction that has been made.'),
    'field' => array(
      'field' => 'txnid',
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
  $data['ec_transaction']['type'] = array(
    'title' => t('Transaction type'),
    'help' => t('Provides the type of transaction which usually indicates how the transation was created'),
    'field' => array(
      'handler' => 'ec_store_views_handler_field_txn_type',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'ec_store_views_handler_filter_txn_type',
    ),
  );
  $data['ec_transaction']['mail'] = array(
    'title' => t('Mail address'),
    'help' => t('Provides the mail address which was supplied from the customer at the time of purchase.'),
    'field' => array(
      'field' => 'mail',
      'click sortable' => TRUE,
    ),
  );
  $data['ec_transaction']['allocation'] = array(
    'title' => t('Allocation Status'),
    'help' => t('Provides the status for how much money has been allocated to this transaction.'),
    'field' => array(
      'handler' => 'ec_store_views_handler_field_allocation',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'ec_store_views_handler_filter_allocation',
    ),
  );
  $data['ec_transaction']['shippable'] = array(
    'title' => t('Shippable'),
    'help' => t('Indicates that the transaction is shippable and requires manual intervention to deliver the product to the customer.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
  );
  $data['ec_transaction']['currency'] = array(
    'title' => t('Currency'),
    'help' => t('Provides of the transaction.'),
    'field' => array(
      'field' => 'currency',
      'click sortable' => TRUE,
    ),
  );
  $data['ec_transaction']['gross'] = array(
    'title' => t('Gross'),
    'help' => t('Provides the total gross amount of the transaction.'),
    'field' => array(
      'handler' => 'ec_common_views_handler_field_format_currency',
      'click sortable' => TRUE,
    ),
  );
  $data['ec_transaction']['allocated'] = array(
    'title' => t('Balance'),
    'help' => t('Provides the current amount allocated to the transaction'),
    'field' => array(
      'handler' => 'ec_common_views_handler_field_format_currency',
      'click sortable' => TRUE,
    ),
  );
  $data['ec_transaction']['balance'] = array(
    'title' => t('Balance'),
    'help' => t('Provides the current balance of the transaction'),
    'field' => array(
      'handler' => 'ec_common_views_handler_field_format_currency',
      'click sortable' => TRUE,
    ),
  );
  $data['ec_transaction']['created'] = array(
    'title' => t('Created'),
    'help' => t('Provides the date and time the transaction was created.'),
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
  $data['ec_transaction']['changed'] = array(
    'title' => t('Changed'),
    'help' => t('Provides the date and time the transaction was last changed'),
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
  $data['ec_transaction']['duedate'] = array(
    'title' => t('Due Date'),
    'help' => t('Provides the date and time the transaction is due'),
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
  $data['ec_transaction']['operations'] = array(
    'title' => t('Operations'),
    'help' => t('Provides links for doing other functions'),
    'field' => array(
      'handler' => 'ec_store_views_handler_field_operations',
      'real field' => 'txnid',
    ),
  );
  $data['ec_transaction']['views_bulk_operations'] = array(
    'title' => t('Bulk operations'),
    'help' => t('Provide a checkbox to select the row for bulk operations.'),
    'real field' => 'txnid',
    'field' => array(
      'handler' => 'views_bulk_operations_handler_field_operations',
      'click sortable' => FALSE,
    ),
  );

  $data['ec_transaction_address']['table']['group'] = t('Transaction address');
  $data['ec_transaction_address']['table']['join'] = array(
    'ec_transaction' => array(
      'left_field' => 'txnid',
      'field' => 'txnid',
      'type' => 'LEFT',
    ),
    'ec_receipt' => array(
      'left_field' => 'erid',
      'field' => 'erid',
      'type' => 'LEFT',
    ),
    'ec_receipt_allocation' => array(
      'left_field' => 'etid',
      'field' => 'txnid',
      'type' => 'LEFT',
    ),    
  );
  $data['ec_transaction_address']['type'] = array(
    'title' => t('Type'),
    'help' => t('Select different types of address added to a transaction.'),
    'field' => array(
      'field' => 'type',
    ),
    'filter' => array(
      'handler' => 'ec_store_views_handler_filter_address_type',
    ),
  );
  $data['ec_transaction_address']['firstname'] = array(
    'title' => t('First name'),
    'help' => t('The first name of the customer'),
    'field' => array(
      'field' => 'firstname',
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),    
  );
  $data['ec_transaction_address']['lastname'] = array(
    'title' => t('Last name'),
    'help' => t('The last name of the customer'),
    'field' => array(
      'field' => 'lastname',
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),    
  );
  $data['ec_transaction_address']['fullname'] = array(
    'real field' => 'firstname',
    'title' => t('Name'),
    'help' => t('Full name of the customer'),
    'field' => array(
      'handler' => 'ec_store_views_handler_field_name',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),    
  );
  $data['ec_transaction_address']['formatted_address'] = array(
    'title' => t('Address'),
    'help' => t('A complete fully formatted address'),
    'field' => array(
      'handler' => 'ec_store_views_handler_field_formatted_address',
      'real field' => 'street1',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),    
  );
  $data['ec_transaction_address']['street1'] = array(
    'title' => t('Street'),
    'help' => t('The street address of the customer'),
    'field' => array(
      'field' => 'street1',
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),    
  );
  $data['ec_transaction_address']['street2'] = array(
    'title' => t('Street Additional'),
    'help' => t('The additional street address of the customer'),
    'field' => array(
      'field' => 'street2',
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),    
  );
  $data['ec_transaction_address']['city'] = array(
    'title' => t('City'),
    'help' => t('The city of the customer'),
    'field' => array(
      'field' => 'city',
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),    
  );
  $data['ec_transaction_address']['state'] = array(
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
  $data['ec_transaction_address']['zip'] = array(
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
  $data['ec_transaction_address']['country'] = array(
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

  $data['ec_transaction_product']['table']['group'] = t('Transaction product');
  $data['ec_transaction_product']['table']['join'] = array(
    'ec_transaction' => array(
      'left_field' => 'txnid',
      'field' => 'txnid',
      'type' => 'LEFT',
    ),
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
      'type' => 'LEFT',
    ),
  );
  $data['ec_transaction_product']['txnid'] = array(
    'title' => t('Transaction'),
    'help' => t('Provides transactions that contain the product.'),
    'relationship' => array(
      'base' => 'ec_transaction',
      'field' => 'txnid',
      'label' => t('Transaction'),
    ),
  );
  $data['ec_transaction_product']['nid'] = array(
    'title' => t('Node'),
    'help' => t('Provides the node id of the product.'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'label' => t('Node'),
    ),
  );
  $data['ec_transaction_product']['title'] = array(
    'title' => t('Title'),
    'help' => t('Provides the title of the product that is displayed on the invoice'),
    'field' => array(
      'field' => 'title',
    ),
  );
  $data['ec_transaction_product']['price'] = array(
    'title' => t('Price'),
    'help' => t('Provides the final price of the product for this transaction'),
    'field' => array(
      'handler' => 'ec_store_views_handler_field_ec_transaction_product_price',
    ),
  );
  $data['ec_transaction_product']['qty'] = array(
    'title' => t('Quantity'),
    'help' => t('provides items purchased'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
  );
  $data['ec_transaction_product']['shippable'] = array(
    'title' => t('Shippable'),
    'help' => t('Indicates that the transaction is shippable and requires manual intervention to deliver the product to the customer.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
  );

  $data['ec_workflow_statuses']['table']['group'] = t('Transaction workflow');
  $data['ec_workflow_statuses']['table']['join'] = array(
    'ec_transaction' => array(
      'left_field' => 'workflow',
      'field' => 'workflow',
      'type' => 'LEFT',
    ),
    'ec_donate' => array(
      'left_field' => 'workflow',
      'field' => 'workflow',
      'type' => 'LEFT',
    ),
  );
  $data['ec_workflow_statuses']['workflow'] = array(
    'title' => t('Workflow status'),
    'help' => t('provide the workflow the id within the system'),
    'field' => array(
      'field' => 'workflow',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'ec_store_views_handler_filter_workflow',
    ),
    'sort' => array(
      'handler' => 'ec_store_views_handler_sort_workflow',
    ),
  );
  $data['ec_workflow_statuses']['description'] = array(
    'title' => t('Description'),
    'help' => t('provide the description for the workflow status'),
    'field' => array(
      'field' => 'description',
      'handler' => 'ec_store_views_handler_field_workflow',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'ec_store_views_handler_sort_workflow',
    ),
  );

  $data['ec_receipt_allocation']['table']['join'] = array(
    'ec_transaction' => array(
      'handler' => 'ec_receipt_views_join_cast',
      'left_field' => 'txnid',
      'field' => 'etid',
      'cast' => array('type' => 'int'),
      'type' => 'LEFT',
      'extra' => array(
        array(
          'field' => 'type',
          'value' => 'transaction',
        ),
      ),
    ),
    'ec_donate' => array(
      'handler' => 'ec_receipt_views_join_cast',
      'left_field' => 'txnid',
      'field' => 'etid',
      'cast' => array('type' => 'int'),
      'type' => 'LEFT',
      'extra' => array(
        array(
          'field' => 'type',
          'value' => 'transaction',
        ),
      ),
    ),
  );

  if (module_exists('search')) {
    $data['search_index']['table']['join'] = array(
      'ec_transaction' => array(
        'left_field' => 'txnid',
        'field' => 'sid',
      ),
    );

    $data['search_total']['table']['join'] = array(
      'ec_transaction' => array(
        'left_table' => 'search_index',
        'left_field' => 'word',
        'field' => 'word',
      ),
    );

    $data['search_dataset']['table']['join'] = array(
      'ec_transaction' => array(
        'left_table' => 'search_index',
        'left_field' => 'sid',
        'field' => 'sid',
        'extra' => 'search_index.type = search_dataset.type',
        'type' => 'INNER',
      ),
    );
  }

  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function ec_store_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'ec_store') . '/views',
    ),
    'handlers' => array(
      'ec_store_views_handler_field_allocation' => array(
        'parent' => 'views_handler_field',
      ),
      'ec_store_views_handler_filter_allocation' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'ec_store_views_handler_field_txn_type' => array(
        'parent' => 'views_handler_field',
      ),
      'ec_store_views_handler_filter_txn_type' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'ec_store_views_handler_field_workflow' => array(
        'parent' => 'views_handler_field',
      ),
      'ec_store_views_handler_filter_workflow' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'ec_store_views_handler_field_operations' => array(
        'parent' => 'views_handler_field',
      ),
      'ec_store_views_handler_field_ec_transaction_product_price' => array(
        'parent' => 'views_handler_field',
      ),
      'ec_store_views_handler_field_name' => array(
        'parent' => 'views_handler_field',
      ),
      'ec_store_views_handler_field_formatted_address' => array(
        'parent' => 'views_handler_field',
      ),
      'ec_store_views_handler_filter_address_type' => array(
        'parent' => 'views_handler_filter_many_to_one',
      ),
      'ec_store_views_handler_sort_workflow' => array(
        'parent' => 'views_handler_sort',
      ),
    ),
  );
}
