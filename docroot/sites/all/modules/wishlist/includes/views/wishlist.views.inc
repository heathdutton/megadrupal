<?php

/**
 * @file
 * Implementation of hook_views_tables().
 *
 * Initial views support contributed by draco2002 http://drupal.org/user/76079
 * Adapted for views 2 by Carl van Denzen http://drupal.org/user/459446
 */
function wishlist_views_data() {
  $data = array();
  // The group will appear in the UI in the dropdown that allows you
  // to narrow down which fields and filters are available.
  $data['wishlist']['table']['group'] = t('Wishlist');

  // Expose the table wishlist as a base table, with as primary key nid.
  // This table references the {node} table.
  // This creates an 'implicit' relationship to the node table, so that when 'Node'
  // is the base table, the fields are automatically available.
  $data['wishlist']['table']['join'] = array(
    // Index this array by the table name to which this table refers.
    // 'left_field' is the primary key in the referenced table.
    // 'field' is the foreign key in this table (i.e. table wishlist_purchased).
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'wishlist_purchased' => array(
      'left_field' => 'wishlist_purch_nid',
      'field' => 'nid',
    ),
  );

  //
  // Fields of the table wishlist
  //
  $data['wishlist']['nid'] = array(
    'title' => t('Wishlist item description node'),
    'help' => t('The description of the item.'),
    // Because this is a foreign key to the {node} table. This allows us to
    // have, when the view is configured with this relationship, all the fields
    // for the related node available.
    'relationship' => array(
      'base' => 'node',
      'left field' => 'nid',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Wishlist node'),
    ),
  );
  // To calculate the total number of purchased items, we create a special relationship
  // handler.
  $data['wishlist']['nid_total_purchased'] = array(
    'title' => t('How much has been bought of this item'),
    'help' => t('How much has been bought of this item and how much is left to purchase'),
    'relationship' => array(
      'base' => 'wishlist_total_purchased',
      'field' => 'nid_total_purchased',
      'handler' => 'views_handler_relationship_wishlist_total_purchased',
      'label' => t('Wishlist_total_purchased'),
    ),
  );

  $data['wishlist']['item_is_public'] = array(
    'title' => t('Item is public'),
    'help' => t('Item is visible for other people.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['wishlist']['item_est_cost'] = array(
    'title' => t('Est. Cost'),
    'help' => t('Estimated cost for this item.'),
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

  $data['wishlist']['item_url1'] = array(
    'title' => t('URL of website.'),
    'help' => t('Link to website where item is described or can be bought.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['wishlist']['item_url2'] = array(
    'title' => t('Additional URL of website.'),
    'help' => t('Link to website where item is described or can be bought.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['wishlist']['item_quantity_requested'] = array(
    'title' => t('Quantity requested'),
    'help' => t('The requested number of items requested.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['wishlist']['item_currency'] = array(
    'title' => t('Currency'),
    'help' => t('Currency the estimated cost is expressed in.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
  );

  $data['wishlist']['item_priority'] = array(
    'title' => t('Priority'),
    'help' => t('How eager you are to get this item.'),
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

  //
  // The table wishlist_purchased
  //
  $data['wishlist_purchased']['table']['group'] = t('Wishlist purchased');

  // This table references the {node} table.
  // This creates an 'implicit' relationship to the node table, so that when 'Node'
  // is the base table, the fields are automatically available.
  $data['wishlist_purchased']['table']['join'] = array(
    // Index this array by the table name to which this table refers.
    // 'left_field' is the primary key in the referenced table.
    // 'field' is the foreign key in this table (i.e. table wishlist_purchased).
    'node' => array(
      'left_field' => 'nid',
      'field' => 'wishlist_purch_nid',
    ),
    'wishlist' => array(
      'left_field' => 'nid',
      'field' => 'wishlist_purch_nid',
    ),
  );

  //
  // The fields of the table wishlist_purchased
  //
  $data['wishlist_purchased']['wishlist_purch_nid'] = array(
    'title' => t('Wishlist purchased link to node.'),
    'help' => t('A wishlist purchased node identifier.'),
  );
  $data['wishlist_purchased']['wishlist_purch_buyer_uid'] = array(
    'title' => t('Who bought this item.'),
    'help' => t('Who bought this item.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric', // filter uid?
    ),
    // Related to users
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'left field' => 'uid',
      'field' => 'wishlist_purch_buyer_uid',
      'label' => t('buyer'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['wishlist_purchased']['wishlist_purch_quantity'] = array(
    'title' => t('Quantity bought.'),
    'help' => t('The number of items that has been bought by this buyer.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric', // filter uid?
    ),
  );
  //
  // Strange name: purch_date should be wishlist_purch_date TODO
  $data['wishlist_purchased']['purch_date'] = array(
    'title' => t('The date of purchase.'),
    'help' => t('The date when this item has been bought by this buyer.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
  /**
   * The artificial view wishlist_total_purchased
   * This 'table' returns per nid the next items:
   *  - sum_wishlist_purch_quantity   : sum of purchased items
   *  - items_left_to_purchase        : items left to purchase
   *
   * This table is magically constructed in the relationship handler
   */
  $data['wishlist_total_purchased']['table']['group'] = t('Wishlist total purchased');
  //
  // The fields (that will be constructed in the relationship handler)
  //
  $data['wishlist_total_purchased']['sum_wishlist_purch_quantity'] = array(
    'title' => t('Sum of quantity bought.'),
    'help' => t('The total number of items that has been bought.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric', // filter uid?
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['wishlist_total_purchased']['items_left_to_purchase'] = array(
    'title' => t('Left to purchase.'),
    'help' => t('The total number of items that is left to purchase (=requested-bought.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  return $data;
}
// end of function wishlist_views_data()
