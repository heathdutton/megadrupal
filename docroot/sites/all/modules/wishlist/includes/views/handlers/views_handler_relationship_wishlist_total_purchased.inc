<?php

/**
 * @file
 * Views' relationship handler.
 */
class views_handler_relationship_wishlist_total_purchased extends views_handler_relationship {

  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    $this->ensure_my_table();

    $def = $this->definition;
    $def['table'] = 'wishlist_total_purchased';

    // If required relationship, do an inner join
    $def['type'] = empty($this->options['required']) ? 'LEFT' : 'INNER';

    $def['left_table'] = $this->table_alias;
    $def['left_field'] = 'nid'; // the field in the table 'wishlist'
    $def['field'] = 'wishlist_total_purch_nid'; // the field in the joined table (the table we are constructing)
    // Join wishlist_purchased, take the sum of all the purchases for one item.
    $query = db_select('wishlist', 'wl');
    $query->addJoin($def['type'], 'wishlist_purchased', 'wp', 'wp.wishlist_purch_nid = wl.nid');
    $query->addExpression('wl.nid', 'wishlist_total_purch_nid');
    $query->addExpression('sum(wishlist_purch_quantity)', 'sum_wishlist_purch_quantity');
    $query->addExpression('wl.item_quantity_requested - sum(case when wishlist_purch_quantity>0 then wishlist_purch_quantity else 0 end)', 'items_left_to_purchase');
    $query->groupBy('wl.nid');
    $def['table formula'] = $query;

    $join = new views_join();
    $join->definition = $def;
    $join->construct();
    $join->adjusted = TRUE;

    // use a short alias for this:
    $alias = $def['table'] . '_' . $this->table; // def['table'] is defined in this function, this->table is 'wishlist'

    $this->alias = $this->query->add_relationship($alias, $join, $def['table'], $this->relationship);
  }
}
