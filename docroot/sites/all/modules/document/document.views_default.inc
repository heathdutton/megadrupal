<?php

/**
 * Implementation of hook_views_default_views().
 */
function document_views_default_views() {
  /*
   * View 'Last_Uploaded_Documents'
   */
  $view = new view();
  $view->name = 'Last_Uploaded_Documents';
  $view->description = 'Last Uploaded list of Documents';
  $view->tag = 'Document';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Last Uploaded Documents', 'default');
  $handler->override_option('fields', array(
      'title' => array(
          'id' => 'title',
          'table' => 'node',
          'field' => 'title')));
  $handler->override_option('sorts', array(
      'created' => array(
          'order' => 'DESC',
          'granularity' => 'second',
          'id' => 'created',
          'table' => 'node',
          'field' => 'created',
          'relationship' => 'none')));
  $handler->override_option('filters', array(
      'type' => array(
          'operator' => 'in',
          'value' => array(
              'document' => 'document'),
          'group' => '0',
          'exposed' => FALSE,
          'expose' => array(
              'operator' => FALSE,
              'label' => ''),
          'id' => 'type',
          'table' => 'node',
          'field' => 'type',
          'relationship' => 'none'),
      'status' => array(
          'operator' => '=',
          'value' => '1',
          'group' => '0',
          'exposed' => FALSE,
          'expose' => array(
              'operator' => FALSE,
              'label' => ''),
          'id' => 'status',
          'table' => 'node',
          'field' => 'status',
          'relationship' => 'none')));
  $handler->override_option('access', array('type' => 'none'));
  $handler->override_option('cache', array('type' => 'none'));
  $handler->override_option('title', 'Last Uploaded Documents');
  $handler->override_option('row_plugin', 'node');
  $handler->override_option('row_options', array(
      'relationship' => 'none',
      'build_mode' => 'teaser',
      'links' => 1,
      'comments' => 0));
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->override_option('fields', array(
      'title' => array(
          'label' => '',
          'alter' => array(
              'alter_text' => 0,
              'text' => '',
              'make_link' => 0,
              'path' => '',
              'link_class' => '',
              'alt' => '',
              'prefix' => '',
              'suffix' => '',
              'help' => '',
              'trim' => 0,
              'max_length' => '',
              'word_boundary' => 1,
              'ellipsis' => 1,
              'strip_tags' => 0,
              'html' => 0),
          'link_to_node' => 1,
          'exclude' => 0,
          'id' => 'title',
          'table' => 'node',
          'field' => 'title',
          'override' => array(
              'button' => 'Use default'),
          'relationship' => 'none')));
  $handler->override_option('style_plugin', 'list');
  $handler->override_option('style_options', array(
      'grouping' => '',
      'type' => 'ul'));
  $handler->override_option('row_plugin', 'fields');
  $handler->override_option('row_options', array(
      'inline' => array(),
      'separator' => ''));
  $handler->override_option('block_description', '');
  $handler->override_option('block_caching', -1);
  $views[$view->name] = $view;

  return $views;
}

