<?php
/**
 * @file
 * Contains default view(s) of the wishlist module
 * Author: cvdenzen (Carl van Denzen)
 * Date  : march 2010
 */
function wishlist_views_default_views() {
  $view = new view;
  $view->name = 'Verlanglijst_1';
  $view->description = 'Wishlist for a user';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'wishlist';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
    'nid_total_purchased' => array(
      'label' => 'Wishlist_total_purchased',
      'required' => 0,
      'id' => 'nid_total_purchased',
      'table' => 'wishlist',
      'field' => 'nid_total_purchased',
      'relationship' => 'none',
    ),
    'nid' => array(
      'label' => 'Wishlist node',
      'required' => 0,
      'id' => 'nid',
      'table' => 'wishlist',
      'field' => 'nid',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('fields', array(
    'item_priority' => array(
      'id' => 'item_priority',
      'table' => 'wishlist',
      'field' => 'item_priority',
    ),
    'title' => array(
      'label' => 'Titel',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'nid',
    ),
    'item_currency' => array(
      'id' => 'item_currency',
      'table' => 'wishlist',
      'field' => 'item_currency',
    ),
    'item_est_cost' => array(
      'id' => 'item_est_cost',
      'table' => 'wishlist',
      'field' => 'item_est_cost',
    ),
    'item_is_public' => array(
      'label' => 'Item is public',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => ',',
      'prefix' => '',
      'suffix' => '',
      'exclude' => 1,
      'id' => 'item_is_public',
      'table' => 'wishlist',
      'field' => 'item_is_public',
      'relationship' => 'none',
    ),
    'item_quantity_requested' => array(
      'id' => 'item_quantity_requested',
      'table' => 'wishlist',
      'field' => 'item_quantity_requested',
    ),
    'sum_wishlist_purch_quantity' => array(
      'id' => 'sum_wishlist_purch_quantity',
      'table' => 'wishlist_total_purchased',
      'field' => 'sum_wishlist_purch_quantity',
      'relationship' => 'nid_total_purchased',
    ),
    'items_left_to_purchase' => array(
      'label' => 'Left to purchase.',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '<None>',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => ',',
      'prefix' => '',
      'suffix' => '',
      'exclude' => 0,
      'id' => 'items_left_to_purchase',
      'table' => 'wishlist_total_purchased',
      'field' => 'items_left_to_purchase',
      'relationship' => 'nid_total_purchased',
    ),
    'item_url1' => array(
      'id' => 'item_url1',
      'table' => 'wishlist',
      'field' => 'item_url1',
    ),
    'item_url2' => array(
      'id' => 'item_url2',
      'table' => 'wishlist',
      'field' => 'item_url2',
    ),
  ));
  $handler->override_option('sorts', array(
    'item_priority' => array(
      'order' => 'ASC',
      'id' => 'item_priority',
      'table' => 'wishlist',
      'field' => 'item_priority',
      'relationship' => 'none',
    ),
    'title' => array(
      'order' => 'ASC',
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'nid',
    ),
  ));
  $handler->override_option('filters', array(
    'uid' => array(
      'operator' => 'in',
      'value' => array(
        '0' => '9',
        '1' => '1',
        '2' => '4',
      ),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'uid_op',
        'identifier' => 'uid',
        'label' => 'User: Name',
        'optional' => 0,
        'remember' => 0,
        'reduce' => 1,
      ),
      'id' => 'uid',
      'table' => 'users',
      'field' => 'uid',
      'relationship' => 'nid',
    ),
    'items_left_to_purchase' => array(
      'operator' => '>=',
      'value' => array(
        'value' => '1',
        'min' => '',
        'max' => '',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'items_left_to_purchase',
      'table' => 'wishlist_total_purchased',
      'field' => 'items_left_to_purchase',
      'relationship' => 'nid_total_purchased',
    ),
    'item_is_public' => array(
      'operator' => '>=',
      'value' => array(
        'value' => '1',
        'min' => '',
        'max' => '',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'item_is_public',
      'table' => 'wishlist',
      'field' => 'item_is_public',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Wishlist');
  $handler->override_option('items_per_page', 30);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'title' => 'title',
      'item_url2' => 'item_url2',
      'item_currency' => 'item_currency',
      'item_est_cost' => 'item_est_cost',
      'item_is_public' => 'item_is_public',
      'items_left_to_purchase' => 'items_left_to_purchase',
      'item_priority' => 'item_priority',
      'item_quantity_requested' => 'item_quantity_requested',
      'sum_wishlist_purch_quantity' => 'sum_wishlist_purch_quantity',
      'item_url1' => 'item_url1',
    ),
    'info' => array(
      'title' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'item_url2' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'item_currency' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'item_est_cost' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'item_is_public' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'items_left_to_purchase' => array(
        'separator' => '',
      ),
      'item_priority' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'item_quantity_requested' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'sum_wishlist_purch_quantity' => array(
        'separator' => '',
      ),
      'item_url1' => array(
        'sortable' => 0,
        'separator' => '',
      ),
    ),
    'default' => '-1',
  ));
  $handler = $view->new_display('page', 'Pagina', 'page_1');
  $handler->override_option('items_per_page', 3000);
  $handler->override_option('path', 'view_verlanglijst_afdrukken_1');
  $handler->override_option('menu', array(
    'type' => 'normal',
    'title' => 'Verlanglijst afdrukken (experimenteel)',
    'description' => '',
    'weight' => '-14',
    'name' => 'menu-verlanglijsten-view',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
  ));
  $views[$view->name] = $view;

  return $views;
} // end of function wishlist_views_default_views()
