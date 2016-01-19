<?php

/**
 * @file
 * Views integration hooks
 */

/**
 * @todo Please document this function.
 */
function tripletex_views_data() {

  $data = array(
    'tripletex_invoice' => array(
      'table' => array(
        'base' => array(
          'field' => 'id',
          'title' => t('Invoice'),
          'help' => t("Tripletex invoices stored and maintained."),
          'weight' => 0,
        ),
        'group' => 'Tripletex',
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
          ),
        ),
        'join' => array(
          'users' => array(
            'left_field' => 'uid',
            'field' => 'uid',
          ),
        ),
      ),
      'id' => array(
        'title' => t('Invoice Id'),
        'help' => t('The invoice ID of the invoice.'), // The help that appears on the UI,
        // Information for displaying the nid
        'field' => array(
          'handler' => 'views_handler_field_node',
          'click sortable' => TRUE,
          'numeric' => TRUE,
        ),
        // Information for accepting a id as an argument
        'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          'name field' => 'title', // the field to display in the summary.
          'numeric' => TRUE,
          'validate type' => 'id',
        ),
        // Information for accepting a nid as a filter
        'filter' => array(
          'handler' => 'views_handler_filter_numeric',
        ),
        // Information for sorting on a nid.
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'nid' => array(
        'title' => t('Node Id'),
        'help' => t('Node ID to which the invoice is attached.'),
        // Information for displaying the nid
        'field' => array(
          'handler' => 'views_handler_field_node',
          'click sortable' => TRUE,
          'numeric' => TRUE,
        ),
        // Information for accepting a nid as an argument
        'argument' => array(
          'handler' => 'views_handler_argument_node_nid',
          'parent' => 'views_handler_argument_numeric', // make sure parent is included
          'name field' => 'title', // the field to display in the summary.
          'numeric' => TRUE,
          'validate type' => 'nid',
        ),
          // Information for accepting a nid as a filter
        'filter' => array(
          'handler' => 'views_handler_filter_numeric',
        ),
        // Information for sorting on a nid.
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'relationship' => array(
          'base' => 'node',
          'field' => 'nid',
          'handler' => 'views_handler_relationship',
          'label' => t('Invoice node'),
        ),
      ),
      'uid' => array(
        'title' => t('User Id'),
         'help' => t('User ID of the customer.  This will be 0 for anonymous users.'),
         // Information for displaying the uid
         'field' => array(
           'handler' => 'views_handler_field_user',
           'click sortable' => TRUE,
           'numeric' => TRUE,
          ),
        // Information for accepting a nid as an argument
        'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          'name field' => 'name', // display this field in the summary
        ),
        // Information for accepting a nid as a filter
        'filter' => array(
          'title' => t('Name'),
          'handler' => 'views_handler_filter_user_name',
        ),
        // Information for sorting on a nid.
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'relationship' => array(
          'base' => 'users',
          'field' => 'uid',
          'handler' => 'views_handler_relationship',
          'label' => t('Invoice user'),
        ),
      ),
      'mail' => array(
        'title' => t('E-mail'),
        'help' => t('E-mail address of customer.'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
      ),
      'name' => array(
        'title' => t('Name'),
        'help' => t('Name of the customer'),
        'field' => array(
          'handler' => 'views_handler_field',
        'click sortable' => TRUE,
        ),
      ),
      'currency' => array(
        'title' => t('Currency'),
        'help' => t('A three letter abbreviation for the currency ID.'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
      ),
      'total_amount' => array(
        'title' => t('Amount'),
        'help' => t('The total invoice amount.'),
        'field' => array(
          'handler' => 'tripletex_handler_field_amount',
          'click sortable' => TRUE,
        ),
        // Information for accepting a total_amount as a filter
        'filter' => array(
          'handler' => 'views_handler_filter_numeric',
        ),
        // Information for sorting on total_amount.
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'remaining_amount' => array(
        'title' => t('Remaining'),
        'help' => t('The remaining amount to be paid.'),
        'field' => array(
          'handler' => 'tripletex_handler_field_amount',
          'float' => TRUE,
        ),
        // Information for accepting a remaining_amount as a filter
        'filter' => array(
          'handler' => 'views_handler_filter_numeric',
        ),
        // Information for sorting on remaining_amount.
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'invoice_date' => array(
        'title' => t('Date'),
        'help' => t('Timestamp of the invoice.'),
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
      ),
      'due_date' => array(
        'title' => t('Due'),
        'help' => t('Timestamp of the invoices due date.'),
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
      ),
      'settled_date' => array(
        'title' => t('Settled'),
        'help' => t('Timestamp of the invoices settled date.'),
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
      ),
      'payed' => array(
        'title' => t('Payed'),
        'real field' => 'settled_date',
        'help' => t('Typically Yes/No booleand indication of the payed status of the invoice.'),
        'field' => array(
          'handler' => 'tripletex_handler_field_payed',
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_boolean_operator',
        ),
      ),
    ),
  );

  return $data;
}

/**
 * @todo Document this function.
 */
function tripletex_views_data_alter() {

}

/**
 * @todo Document this function.
 */
function tripletex_views_plugins() {
  // This is where we define the plugins

}

/**
 * @todo Document this function.
 */
function tripletex_views_handlers() {
  // This is where we define handlers
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'tripletex') . '/views/handlers',
    ),
    'handlers' => array(
      'tripletex_handler_field_amount' => array(
        'parent' => 'views_handler_field_numeric',
      ),
      'tripletex_handler_field_payed' => array(
        'parent' => 'views_handler_field_boolean',
      ),
      'tripletex_handler_filter_tripletex_invoice_status' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
    ),
  );

}
