<?php
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file
 * Views integration.
 */

/**
 * Implementation of hook_default_view_views().
 */
function openx_manager_views_default_views() {
  $views = array();
  _openx_manager_default_views_banner($views);
  _openx_manager_default_views_campaign($views);
  _openx_manager_default_views_zone($views);
  return $views;
}

/**
 * Create the banner list.
 */
function _openx_manager_default_views_banner(&$views) {
  $view = new view;
  $view->name = 'openx_manager_banners';
  $view->description = 'OpenX Manager - Banner';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => 'Title',
      'link_to_node' => TRUE,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'destination_url' => array(
      'label' => 'Destination URL',
      'exclude' => 0,
      'id' => 'destination_url',
      'table' => 'openx_manager_banner',
      'field' => 'destination_url',
      'relationship' => 'none',
    ),
    'btype' => array(
      'label' => 'Type',
      'exclude' => 0,
      'id' => 'btype',
      'table' => 'openx_manager_banner',
      'field' => 'btype',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'ocid' => array(
      'default_action' => 'empty',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'ocid',
      'table' => 'openx_manager_banner',
      'field' => 'ocid',
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'openx_advertiser' => 0,
        'openx_banner' => 0,
        'openx_campaign' => 0,
        'openx_manager' => 0,
        'openx_publisher' => 0,
        'openx_user' => 0,
        'openx_zone' => 0,
        'page' => 0,
        'story' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '1' => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'openx_banner' => 'openx_banner',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'view openx banners',
  ));
  $handler->override_option('empty', 'None created.');
  $handler->override_option('empty_format', '1');
  $handler->override_option('use_ajax', TRUE);
  $handler->override_option('use_pager', 'mini');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'title' => 'title',
      'contract_expiration' => 'contract_expiration',
      'inventory_clicks' => 'inventory_clicks',
      'inventory_conversions' => 'inventory_conversions',
      'inventory_impressions' => 'inventory_impressions',
    ),
    'info' => array(
      'title' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'contract_expiration' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'inventory_clicks' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'inventory_conversions' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'inventory_impressions' => array(
        'sortable' => 1,
        'separator' => '',
      ),
    ),
    'default' => '-1',
  ));
  $views[$view->name] = $view;
}

/**
 * Create the campaign list.
 */
function _openx_manager_default_views_campaign(&$views) {
  $view = new view;
  $view->name = 'openx_manager_campaigns';
  $view->description = 'OpenX Manager - Campaign';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => 'Title',
      'link_to_node' => TRUE,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'contract_expiration' => array(
      'label' => 'Expiration',
      'date_format' => 'small',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'contract_expiration',
      'table' => 'openx_manager_campaign',
      'field' => 'contract_expiration',
      'relationship' => 'none',
    ),
    'inventory_clicks' => array(
      'label' => 'Clicks',
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => ',',
      'prefix' => '',
      'suffix' => '',
      'exclude' => 0,
      'id' => 'inventory_clicks',
      'table' => 'openx_manager_campaign',
      'field' => 'inventory_clicks',
      'relationship' => 'none',
    ),
    'inventory_conversions' => array(
      'label' => 'Conversions',
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => ',',
      'prefix' => '',
      'suffix' => '',
      'exclude' => 0,
      'id' => 'inventory_conversions',
      'table' => 'openx_manager_campaign',
      'field' => 'inventory_conversions',
      'relationship' => 'none',
    ),
    'inventory_impressions' => array(
      'label' => 'Impressions',
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => ',',
      'prefix' => '',
      'suffix' => '',
      'exclude' => 0,
      'id' => 'inventory_impressions',
      'table' => 'openx_manager_campaign',
      'field' => 'inventory_impressions',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'oaid' => array(
      'default_action' => 'empty',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'oaid',
      'table' => 'openx_manager_campaign',
      'field' => 'oaid',
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'openx_advertiser' => 0,
        'openx_banner' => 0,
        'openx_campaign' => 0,
        'openx_manager' => 0,
        'openx_publisher' => 0,
        'openx_user' => 0,
        'openx_zone' => 0,
        'page' => 0,
        'story' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '1' => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'openx_campaign' => 'openx_campaign',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'view openx campaigns',
  ));
  $handler->override_option('empty', 'None created.');
  $handler->override_option('empty_format', '1');
  $handler->override_option('use_ajax', TRUE);
  $handler->override_option('use_pager', 'mini');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'title' => 'title',
      'contract_expiration' => 'contract_expiration',
      'inventory_clicks' => 'inventory_clicks',
      'inventory_conversions' => 'inventory_conversions',
      'inventory_impressions' => 'inventory_impressions',
    ),
    'info' => array(
      'title' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'contract_expiration' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'inventory_clicks' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'inventory_conversions' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'inventory_impressions' => array(
        'sortable' => 1,
        'separator' => '',
      ),
    ),
    'default' => '-1',
  ));
  $views[$view->name] = $view;
}

/**
 * Create the campaign list.
 */
function _openx_manager_default_views_zone(&$views) {
  $view = new view;
  $view->name = 'openx_manager_zones';
  $view->description = 'OpenX Manager - Zone';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => 'Title',
      'link_to_node' => TRUE,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'zone_type' => array(
      'label' => 'Type',
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => ',',
      'prefix' => '',
      'suffix' => '',
      'exclude' => 0,
      'id' => 'zone_type',
      'table' => 'openx_manager_zone',
      'field' => 'zone_type',
      'relationship' => 'none',
    ),
    'size_height' => array(
      'label' => 'Height',
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => ',',
      'prefix' => '',
      'suffix' => '',
      'exclude' => 0,
      'id' => 'size_height',
      'table' => 'openx_manager_zone',
      'field' => 'size_height',
      'relationship' => 'none',
    ),
    'size_width' => array(
      'label' => 'Width',
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => ',',
      'prefix' => '',
      'suffix' => '',
      'exclude' => 0,
      'id' => 'size_width',
      'table' => 'openx_manager_zone',
      'field' => 'size_width',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'opid' => array(
      'default_action' => 'empty',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'opid',
      'table' => 'openx_manager_zone',
      'field' => 'opid',
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'openx_advertiser' => 0,
        'openx_banner' => 0,
        'openx_campaign' => 0,
        'openx_manager' => 0,
        'openx_publisher' => 0,
        'openx_user' => 0,
        'openx_zone' => 0,
        'page' => 0,
        'story' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '1' => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'openx_zone' => 'openx_zone',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'view openx zones',
  ));
  $handler->override_option('empty', 'None created');
  $handler->override_option('empty_format', '1');
  $handler->override_option('use_ajax', TRUE);
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'title' => 'title',
      'zone_type' => 'zone_type',
      'size_height' => 'size_height',
      'size_width' => 'size_width',
    ),
    'info' => array(
      'title' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'zone_type' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'size_height' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'size_width' => array(
        'sortable' => 1,
        'separator' => '',
      ),
    ),
    'default' => 'title',
  ));
  $views[$view->name] = $view;
}