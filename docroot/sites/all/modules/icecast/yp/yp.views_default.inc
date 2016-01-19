<?php
/**
 * @file
 * Provide default views for yp.module.
 */

/**
 * Implementation of hook_views_default_views().
 */
function yp_views_default_views() {
  $views = array();

$view = new view;
$view->name = 'ypdir';
$view->description = '';
$view->tag = '';
$view->view_php = '';
$view->base_table = 'yp_stream';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', array(
  'server_name' => array(
    'label' => 'Name',
    'exclude' => 0,
    'id' => 'server_name',
    'table' => 'yp_stream',
    'field' => 'server_name',
    'relationship' => 'none',
  ),
  'genre' => array(
    'label' => 'Genre',
    'exclude' => 0,
    'id' => 'genre',
    'table' => 'yp_stream',
    'field' => 'genre',
    'relationship' => 'none',
  ),
  'description' => array(
    'label' => 'Description',
    'exclude' => 0,
    'id' => 'description',
    'table' => 'yp_stream',
    'field' => 'description',
    'relationship' => 'none',
  ),
  'server_type' => array(
    'label' => 'Type',
    'exclude' => 0,
    'id' => 'server_type',
    'table' => 'yp_stream',
    'field' => 'server_type',
    'relationship' => 'none',
  ),
  'bitrate' => array(
    'id' => 'bitrate',
    'table' => 'yp_stream',
    'field' => 'bitrate',
  ),
  'current_song' => array(
    'label' => 'Current track',
    'exclude' => 0,
    'id' => 'current_song',
    'table' => 'yp_stream',
    'field' => 'current_song',
    'relationship' => 'none',
  ),
  'listeners' => array(
    'id' => 'listeners',
    'table' => 'yp_stream',
    'field' => 'listeners',
  ),
  'listen_url' => array(
    'label' => 'Listen URL',
    'exclude' => 0,
    'id' => 'listen_url',
    'table' => 'yp_stream',
    'field' => 'listen_url',
    'relationship' => 'none',
  ),
));
$handler->override_option('access', array(
  'type' => 'perm',
  'role' => array(),
  'perm' => 'access content',
));
$handler->override_option('title', 'Stream directory');
$handler->override_option('use_ajax', TRUE);
$handler->override_option('items_per_page', 0);
$handler->override_option('style_plugin', 'table');
$handler->override_option('style_options', array(
  'grouping' => '',
  'override' => 1,
  'sticky' => 1,
  'order' => 'desc',
  'columns' => array(
    'server_name' => 'server_name',
    'genre' => 'genre',
    'description' => 'description',
    'server_type' => 'server_type',
    'bitrate' => 'bitrate',
    'current_song' => 'current_song',
    'listeners' => 'listeners',
    'listen_url' => 'listeners',
  ),
  'info' => array(
    'server_name' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'genre' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'description' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'server_type' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'bitrate' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'current_song' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'listeners' => array(
      'sortable' => 1,
      'separator' => '&nbsp;',
    ),
    'listen_url' => array(
      'sortable' => 1,
      'separator' => '',
    ),
  ),
  'default' => '-1',
));
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->override_option('path', 'yp/dir');
$handler->override_option('menu', array(
  'type' => 'default tab',
  'title' => 'Stream directory',
  'weight' => '0',
));
$handler->override_option('tab_options', array(
  'type' => 'normal',
  'title' => 'Stream directory',
  'weight' => '0',
));
$handler = $view->new_display('block', 'Block', 'block_1');
$handler->override_option('title', 'Live radio directory');
$handler->override_option('block_description', 'Icecast YP directory');

  $views[$view->name] = $view;
  return $views;
}
