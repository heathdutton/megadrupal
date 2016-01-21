<?php

/**
 * @file
 * Provides a tab on the user profile that will simply direct to the home page.
 * This could of course actually point to a view of the user's own content.
 * Also, just as an example, we jump to the #content part of the page.
 */

$page = new stdClass;
$page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
$page->api_version = 1;
$page->name = 'home_tab';
$page->task = 'page';
$page->admin_title = 'Home page tab';
$page->admin_description = 'Provides a tab on the user profile that will simply direct to the home page. This could of course actually point to a view of the user\'s own content. Also, just as an example, we jump to the #content part of the page.';
$page->path = 'user/%user/nodes';
$page->access = array();
$page->menu = array(
  'type' => 'tab',
  'title' => 'Nodes',
  'name' => 'navigation',
  'weight' => '0',
  'parent' => array(
    'type' => 'none',
    'title' => '',
    'name' => 'navigation',
    'weight' => '0',
  ),
);
$page->arguments = array(
  'user' => array(
    'id' => 1,
    'identifier' => 'User: ID',
    'name' => 'uid',
    'settings' => array(),
  ),
);
$page->conf = array();
$page->default_handlers = array();
$handler = new stdClass;
$handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
$handler->api_version = 1;
$handler->name = 'page_home_tab_redirect_context';
$handler->task = 'page';
$handler->subtask = 'home_tab';
$handler->handler = 'redirect_context';
$handler->weight = 0;
$handler->conf = array(
  'title' => 'Redirect',
  'contexts' => array(),
  'relationships' => array(),
  'path' => '<front>',
  'query' => '',
  'fragment' => '#content',
);
$page->default_handlers[$handler->name] = $handler;
