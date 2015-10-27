<?php

/**
 * @file
 * This example of a page redirect redirects when viewing a Story node to the
 * node author's page at user/#, with the node ID in the query string. This gets
 * picked up by our hook_user, which sets the title of the page with the node
 * title.
 *
 * NOTE: The content type 'story' must exist.
 *
 * @see page_manager_redirect_examples_user()
 */

$handler = new stdClass;
$handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
$handler->api_version = 1;
$handler->name = 'node_view_story_author_redirect';
$handler->task = 'node_view';
$handler->subtask = '';
$handler->handler = 'redirect_context';
$handler->weight = 0;
$handler->conf = array(
  'title' => 'Story authors',
  'contexts' => array(),
  'relationships' => array(
    0 => array(
      'context' => 'argument_nid_1',
      'name' => 'user_from_node',
      'id' => 1,
      'identifier' => 'Node author',
      'keyword' => 'author',
    ),
  ),
  'access' => array(
    'plugins' => array(
      0 => array(
        'name' => 'node_type',
        'settings' => array(
          'type' => array(
            'story' => 'story',
          ),
        ),
        'context' => 'argument_nid_1',
        'not' => FALSE,
      ),
    ),
    'logic' => 'and',
  ),
  'path' => 'user/ %author:uid',
  'query' => 'nid= %node:nid',
  'fragment' => '',
);
