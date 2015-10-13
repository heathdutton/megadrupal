<?php

/**
 * @file
 * Provides support for the Views module.
 */

/*
 * Implements hook_views_data_alter().
 */
function commerce_gc_views_data_alter(&$data) {
  if (isset($data['commerce_coupon'])) {
    // Giftcard balance summary field
    $data['commerce_coupon']['balance'] = array(
      'field' => array(
        'title' => t('Balance summary'),
        'help' => t('Dispalys a transaction balance summary with a link to a list of the giftcard\'s transactions.'),
        'handler' => 'commerce_gc_handler_field_balance_summary',
      ),
    );
    
    // Giftcard balance summary field
    $data['commerce_coupon']['balance'] = array(
      'field' => array(
        'title' => t('Balance summary'),
        'help' => t('Dispalys a transaction balance summary with a link to a list of the giftcard\'s transactions.'),
        'handler' => 'commerce_gc_handler_field_balance_summary',
      ),
    );    
    
    // Giftcard balance area
    $data['commerce_coupon']['balance_area'] = array(
      'title' => t('Transaction balance'),
      'help' => t('Displays the total balance for a giftcard'),
      'area' => array(
        'handler' => 'commerce_gc_handler_area_balance_summary'
      )
    );
    
    // Add-transaction form area.
    $data['commerce_coupon']['add_transaction_form'] = array(
      'title' => t('Add transaction form'),
      'help' => t('Simple add transaction form.'),
      'area' => array(
        'handler' => 'commerce_gc_handler_area_add_transaction_form'
      )
    );      
  }
}


/**
 * Commerce discount entity Views definitions.
 */
class CommerceGCTransactionViewsController extends EntityDefaultViewsController {

  /**
   * Override EntityDefaultViewsController::views_data().
   */
  public function views_data() {
    $data = parent::views_data();
    $data['commerce_gc_transaction']['table']['base']['access query tag'] = 'commerce_gc_transaction_access';
    // Make amount a currency field
    $data['commerce_gc_transaction']['amount']['field'] = array(
      'handler' => 'commerce_gc_handler_field_amount'
    );
    $data['commerce_gc_transaction']['date'] = array(
      'title' => t('Date'),
      'help' => t('The date the transaction occurred.'),
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
    
    // Coupon relationship
    $data['commerce_gc_transaction']['coupon_id']['relationship'] = array(
      'handler' => 'views_handler_relationship',
      'base' => 'commerce_coupon',
      'field' => 'coupon_id',
      'label' => t('Coupon'),        
    );    
    
    // Giftcard balance summary field
    $data['commerce_gc_transaction']['delete'] = array(
      'field' => array(
        'title' => t('Delete transaction link'),
        'help' => t('Provides a link to delete a transaction.'),
        'handler' => 'commerce_gc_handler_field_transaction_delete',
      ),
    );      
    return $data;
  }
}

/*
 * Implements hook_views_plugins().
 */
function commerce_gc_views_plugins() {
  return array(
    'argument validator' => array(
      'user_giftcards' => array(
        'title' => t('Access user giftcards list'),
        'handler' => 'commerce_gc_views_plugin_argument_validate_user_giftcards',
        'path' => drupal_get_path('module', 'commerce_gc') . '/includes/views/plugins'
      ),
    ),
  );
}
