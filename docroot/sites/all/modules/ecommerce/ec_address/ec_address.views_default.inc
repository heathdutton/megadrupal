<?php

/**
 * @file
 * Default views for ec_address module.
 */

/**
 * Implements hook_views_default_views().
 */
function ec_address_views_default_views() {
  /*
   * View 'ec_address_list'
   */
  $view = new view;
  $view->name = 'ec_address_list';
  $view->description = 'List of addresses for a customer';
  $view->tag = 'ec_address';
  $view->view_php = '';
  $view->base_table = 'ec_address';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'name' => array(
      'label' => 'Customer',
      'include_multiline_address' => 1,
      'exclude' => 0,
      'id' => 'name',
      'table' => 'ec_address',
      'field' => 'name',
      'relationship' => 'none',
    ),
    'edit_address' => array(
      'label' => 'Operations',
      'text' => '',
      'exclude' => 0,
      'id' => 'edit_address',
      'table' => 'ec_address',
      'field' => 'edit_address',
      'relationship' => 'none',
    ),
    'delete_address' => array(
      'label' => '',
      'text' => '',
      'exclude' => 0,
      'id' => 'delete_address',
      'table' => 'ec_address',
      'field' => 'delete_address',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'uid' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '%1',
      'default_argument_type' => 'user',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'empty',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'uid',
      'table' => 'ec_address',
      'field' => 'uid',
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_user' => 1,
      'default_argument_fixed' => '1',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'deadwood_category' => 0,
        'deadwood_item' => 0,
        'amazon_book' => 0,
        'file' => 0,
        'page' => 0,
        'product' => 0,
        'product_some' => 0,
        'story' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(),
      'validate_argument_type' => 'tid',
      'validate_argument_php' => '',
      'override' => array(
        'button' => 'Override',
      ),
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'ec_customer',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('empty', 'No addresses have been entered.');
  $handler->override_option('empty_format', '1');
  $handler->override_option('use_ajax', TRUE);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'name' => 'name',
      'edit_address' => 'edit_address',
      'delete_address' => 'edit_address',
    ),
    'info' => array(
      'name' => array(
        'separator' => '',
      ),
      'edit_address' => array(
        'separator' => ' ',
      ),
      'delete_address' => array(
        'separator' => '',
      ),
    ),
    'default' => '-1',
  ));
  $views[$view->name] = $view;

  return $views;
}


