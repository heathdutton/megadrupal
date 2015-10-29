<?php

/**
 * @file
 * Views functionality for the Payment API
 */

/**
 * Implementation of hook_views_data().
 * TODO
 *  - Account for payment_type - probably as a selection.
 *  - We're going to want some aggregate views (SUM, division, etc.) before this
 * is very useful.
 */
function pay_views_data() {
  $data = array();

  $data['pay_form']['table']['group']  = t('Payment form');
  $data['pay_activity']['table']['group']  = t('Payment activity');

  $data['pay_form']['table']['base'] = array(
    'field' => 'pfid',
    'title' => t('Payment forms'),
    'help' => t("Forms for collecting payments through the Payment API."),
  );

  $data['pay_form']['form'] = array(
    'real field' => 'pfid',
    'title' => t('Payment form'),
    'help' => t('The contents of a payment form.'),
    'field' => array(
      'handler' => 'pay_handler_field_pay_form',
      'pay_type' => 'pay_form',
      'pay_key' => 'pfid',
    ),
  );

  $data['pay_form']['view_pay_form'] = array(
    'real field' => 'pfid',
    'title' => t('Link'),
    'help' => t('Provide a link to the payment form.'),
    'field' => array(
      'handler' => 'pay_handler_field_pay_form_link',
      'pay_type' => 'pay_form',
      'pay_key' => 'pfid',
    ),
  );

  // TODO implement name argument.
  $data['pay_form']['pfid'] = array(
    'title' => t('Form ID'),
    'help' => t('The unique identifier for a payment form.'),
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
      'numeric' => TRUE,
    ),
  );

  $data['pay_form']['status'] = array(
    'title' => t('Active'),
    'help' => t('Whether or not the payment form is active.'),
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

  $data['pay_form']['handler'] = array(
    'title' => t('Handler'),
    'help' => t('The handler in use.'),
    'field' => array(
      'handler' => 'pay_handler_field_handler',
      'pay_type' => 'pay_form',
      'pay_key' => 'pfid',
    ),
    'filter' => array(
      'handler' => 'pay_handler_filter_handler',
      'pay_type' => 'pay_form',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['pay_form']['uid'] = array(
    'title' => t('Owner'),
    'help' => t('The user who created the payment form.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'base field' => 'uid',
    ),
  );

  $data['pay_form']['title'] = array(
    'title' => t('Form title'),
    'help' => t('The title of the payment form.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['pay_form']['total_goal'] = array(
    'title' => t('Goal amount'),
    'help' => t('The total goal amount for a particular form.'),
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

  $data['pay_form']['count'] = array(
    'title' => t('Count'),
    'help' => t('The number of transactions against this form.'),
    'field' => array(
      'handler' => 'pay_handler_field_count',
      'click sortable' => TRUE,
    ),
  );

  $data['pay_form']['total_paid'] = array(
    'title' => t('Total payments'),
    'help' => t('The total amount paid on a particular form.'),
    'field' => array(
      'handler' => 'pay_handler_field_amount',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'pay_handler_filter_amount',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['pay_form']['total'] = array(
    'title' => t('Total'),
    'help' => t('The total amount committed on a particular form.'),
    'field' => array(
      'handler' => 'pay_handler_field_amount',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'pay_handler_filter_amount',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['pay_transaction']['table']['group']  = t('Payment transaction');
  $data['pay_transaction']['table']['base'] = array(
    'field' => 'pxid',
    'title' => t('Payment transactions'),
    'help' => t("Payment transactions collected throught Payment API."),
  );

  $data['pay_transaction']['pxid'] = array(
    'title' => t('Transaction ID'),
    'help' => t('The unique identifier for a payment transaction.'),
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
      'numeric' => TRUE,
    ),
  );

  $data['pay_transaction']['view_link'] = array(
    'real field' => 'pxid',
    'title' => t('View link'),
    'help' => t('A link to the payment transaction details page.'),
    'field' => array(
      'handler' => 'pay_handler_field_pay_transaction_link',
      'pay_type' => 'pay_transaction',
      'pay_key' => 'pxid',
    ),
  );

  $data['pay_transaction']['pfid'] = array(
    'title' => t('Payment form'),
    'help' => t('The payment form a transaction was applied against.'),
    'relationship' => array(
      'base' => 'pay_form',
      'field' => 'pfid',
      'base field' => 'pfid',
      'label' => t('Payment form'),
      'type' => 'INNER',
    ),
  );

  $data['pay_transaction']['uid'] = array(
    'title' => t('User'),
    'help' => t('The user who created the transaction.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'base field' => 'uid',
      'label' => t('user'),
    ),
  );

  $data['pay_transaction']['created'] = array(
    'title' => t('Transaction date'),
    'help' => t('The time of initial activity on the transaction.'),
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

  $data['pay_transaction']['completed'] = array(
    'title' => t('Completion date'),
    'help' => t('The date that transaction was completed.'),
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

  $data['pay_transaction']['state'] = array(
    'title' => t('State'),
    'help' => t('Current state of the transaction.'),
    'field' => array(
      'handler' => 'pay_handler_field_transaction_state',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_in_operator',
      'options callback' => 'pay_transaction_states',
    ),
    'argument' => array(
      'handler' => 'pay_handler_argument_state',
    ),
  );

  $data['pay_transaction']['total'] = array(
    'title' => t('Total amount'),
    'help' => t('The total amount due. This may be different from the amount paid if the transaction is not completed.'),
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

  $data['pay_transaction']['total_paid'] = array(
    'title' => t('Total paid'),
    'help' => t('The total amount due. This may be different from the total due if the transaction is not completed.'),
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

  $data['pay_transaction']['notes'] = array(
    'title' => t('Transaction notes'),
    'help' => t('Notes or comments for this transaction.'),
    'field' => array(
      'handler' => 'views_handler_field_markup',
      'format' => 'notes_format',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['pay_activity']['table']['join'] = array(
    'pay_transaction' => array(
      'left_field' => 'pxid',
      'field' => 'pxid',
      'type' => 'INNER',
    ),
  );

  $data['pay_activity']['paid'] = array(
    'title' => t('Activity ID'),
    'help' => t('The unique identifier for a payment activity.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['pay_activity']['pxid'] = array(
    'title' => t('Transaction'),
    'help' => t('The transaction associated with a payment activity.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'pay_transaction',
      'field' => 'pxid',
      'base field' => 'pxid',
    ),
  );

  $data['pay_activity']['uid'] = array(
    'title' => t('User'),
    'help' => t('The user who created the activity.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'base field' => 'uid',
    ),
  );

  // TODO cleanup/selectable activity?
  $data['pay_activity']['activity'] = array(
    'title' => t('Activity'),
    'help' => t('The activity performed.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['pay_activity']['result'] = array(
    'title' => t('Activity Succes'),
    'help' => t('Whether or not the activity was successful.'),
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

  $data['pay_activity']['total'] = array(
    'title' => t('Activity amount'),
    'help' => t('The total amount for the activity.'),
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

  $data['pay_activity']['transaction_total'] = array(
    'title' => t('Activity transaction total'),
    'help' => t('The total amount applied to the transaction. This may be different from the total due if the activity is split among transactions.'),
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

  $data['pay_activity']['timestamp'] = array(
    'title' => t('Activity timestamp'),
    'help' => t('The time of a payment activity.'),
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

  $data['pay_activity']['hostname'] = array(
    'title' => t('Activity hostname'),
    'help' => t('Hostname of user that created a payment activity.'),
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

  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
function pay_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'pay') . '/includes/views',
    ),
    'handlers' => array(
      'pay_handler_filter_handler' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'pay_handler_argument_state' => array(
        'parent' => 'views_handler_argument_string',
      ),
      'pay_handler_field_pay_form' => array(
        'parent' => 'views_handler_field',
      ),
      'pay_handler_field_pay_form_link' => array(
        'parent' => 'views_handler_field',
      ),
      'pay_handler_field_handler' => array(
        'parent' => 'views_handler_field',
      ),
      'pay_handler_field_transaction_state' => array(
        'parent' => 'views_handler_field',
      ),
      'pay_handler_field_pay_transaction_link' => array(
        'parent' => 'views_handler_field',
      ),
      'pay_handler_filter_amount' => array(
        'parent' => 'views_handler_filter_numeric',
      ),
      'pay_handler_field_count' => array(
        'parent' => 'views_handler_field_numeric',
      ),
      'pay_handler_field_amount' => array(
        'parent' => 'views_handler_field_numeric',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_plugins().
 */
function pay_views_plugins() {
  return array(
    'argument default' => array(
      'pay_form' => array(
        'title' => t('Payment form ID from URL'),
        'handler' => 'pay_handler_argument_default_form',
        'path' => drupal_get_path('module', 'pay') . '/includes/views',
        'parent' => 'fixed',
      ),
    ),
  );
}
