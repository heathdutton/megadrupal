<?php

$view = new view;
$view->name = 'vma_false1and';
$view->description = '';
$view->tag = '';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to false to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('access', array(
  'type' => 'many',
  'access_plugins' => array(
    'false' => 'false',
  ),
  'blacklist' => 'AND',
));
$handler->override_option('cache', array(
  'type' => 'none',
));
$handler->override_option('empty', 'You got through to the view alright, but there are no results.');
$handler->override_option('empty_format', '1');
$handler->override_option('row_plugin', 'node');
$handler->override_option('row_options', array(
  'relationship' => 'none',
  'build_mode' => 'teaser',
  'links' => 1,
  'comments' => 0,
));
$handler = $view->new_display('page', 'Page', 'page');
$handler->override_option('path', 'test/views_access_many/false1and');
$handler->override_option('menu', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  'name' => 'navigation',
));
$handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  'name' => 'navigation',
));