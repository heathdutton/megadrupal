<?php

/**
 * @file
 * This is a very simple custom page redirect. Visitors to the new custom page,
 * /pmr_my_page, are immediately redirected to their own user page.
 */

$page = new stdClass;
$page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
$page->api_version = 1;
$page->name = 'own_page_redirect';
$page->task = 'page';
$page->admin_title = 'Own page redirect';
$page->admin_description = 'This is a very simple custom page redirect. Visitors to pmr_my_page are immediately redirected to their own user page.';
$page->path = 'pmr_my_page';
$page->access = array();
$page->menu = array();
$page->arguments = array();
$page->conf = array();
$page->default_handlers = array();
$handler = new stdClass;
$handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
$handler->api_version = 1;
$handler->name = 'page_own_page_redirect_redirect_context';
$handler->task = 'page';
$handler->subtask = 'own_page_redirect';
$handler->handler = 'redirect_context';
$handler->weight = 0;
$handler->conf = array(
  'title' => 'My page redirect',
  'contexts' => array(
    0 => array(
      'name' => 'user',
      'id' => 1,
      'identifier' => 'User',
      'keyword' => 'user',
      'context_settings' => array(
        'type' => 'current',
        'user' => '',
        'uid' => '',
      ),
    ),
  ),
  'relationships' => array(),
  'path' => 'user/%user:uid',
  'query' => 'redirected=true',
  'fragment' => '',
);
$page->default_handlers[$handler->name] = $handler;
