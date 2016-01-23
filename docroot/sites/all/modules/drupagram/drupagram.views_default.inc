<?php
/**
 * @file
 * Default views.
 */

/**
 * Implements hook_views_default_views().
 */
function drupagram_views_default_views() {
  $view = new view;
  $view->name = 'instagrams';
  $view->description = 'Displays Instagram.com media for users who have associated Instagram accounts.';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'drupagram';
  $view->is_cacheable = '0';
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Instagrams', 'default');
  $handler->override_option('fields', array(
    'images' => array(
      'label' => 'Instagram image',
      'exclude' => 0,
      'id' => 'images',
      'table' => 'drupagram',
      'field' => 'images',
      'relationship' => 'none',
      'link_to_post' => FALSE,
    ),
    'filter' => array(
      'label' => '',
      'exclude' => 0,
      'id' => 'filter',
      'table' => 'drupagram',
      'field' => 'filter',
      'relationship' => 'none',
      'hide_empty' => TRUE,
    ),
    'caption' => array(
      'label' => '',
      'exclude' => 0,
      'id' => 'caption',
      'table' => 'drupagram',
      'field' => 'caption',
      'relationship' => 'none',
      'hide_empty' => TRUE,
    ),
    'link' => array(
      'label' => 'Link to post',
      'exclude' => 0,
      'id' => 'link',
      'table' => 'drupagram',
      'field' => 'link',
      'relationship' => 'none',
      'display_as_link' => TRUE,
    ),
    'created_time' => array(
      'label' => 'Created Time',
      'date_format' => 'time ago',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'created_time',
      'table' => 'drupagram',
      'field' => 'created_time',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'created_time' => array(
      'order' => 'DESC',
      'id' => 'created_time',
      'table' => 'drupagram',
      'field' => 'created_time',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'uid' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All author',
      'title' => '%1\'s instagrams',
      'default_argument_type' => 'user',
      'default_argument' => '',
      'validate_type' => 'php',
      'validate_options' => array(
        'code' => '$uid = arg(1);
                   if (!empty($uid)) {
                     $account = user_load($uid);
                     $drupagram_accounts = drupagram_drupagram_accounts($account);
                     if(count($drupagram_accounts)) {
                       return TRUE;
                     }
                     else {
                       return FALSE;
                     }
                   }',
      ),
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'uid',
      'table' => 'users',
      'field' => 'uid',
      'relationship' => 'none',
      'default_argument_user' => 1,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'poll' => 0,
        'page' => 0,
        'review' => 0,
        'story' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(),
      'validate_argument_type' => 'tid',
      'validate_argument_php' => '',
    ),
  ));

  $handler->override_option('access', array(
    'type' => 'none',
    'role' => array(),
    'perm' => '',
  ));
  $handler->override_option('use_pager', 'mini');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'images' => 'images',
      'filter' => 'images',
      'caption' => 'images',
      'link' => 'link',
      'created_time' => 'created_time',
    ),
    'info' => array(
      'images' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'filter' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'caption' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'link' => array(
        'sortable' => 0,
        'separator' => ' &#8212; ',
      ),
      'created_time' => array(
        'sortable' => 0,
        'separator' => '',
      ),
    ),
    'default' => '-1',
  ));
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->override_option('path', 'user/%/instagram');
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => 'Instagram',
    'weight' => '1',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'weight' => 0,
  ));
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->override_option('fields', array(
    'images' => array(
      'label' => '',
      'exclude' => 0,
      'id' => 'images',
      'table' => 'drupagram',
      'field' => 'images',
      'relationship' => 'none',
      'link_to_post' => FALSE,
    ),
    'created_time' => array(
      'label' => '',
      'date_format' => 'time ago',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'created_time',
      'table' => 'drupagram',
      'field' => 'created_time',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('items_per_page', 5);
  $handler->override_option('use_more', 1);
  $handler->override_option('style_plugin', 'list');
  $handler->override_option('row_options', array(
    'inline' => array(
      'images' => 'images',
      'created_time' => 'created_time',
    ),
    'separator' => ' &#8212; ',
  ));
  $handler->override_option('block_description', 'User Instagrams');

  return array('instagrams' => $view);
}
