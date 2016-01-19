<?php
/**
 * @file
 * fastcache_example.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function fastcache_example_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view_panel_context';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Article',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(
      0 => array(
        'identifier' => 'Taxonomy term from Node (on Node: Tags [field_tags])',
        'keyword' => 'taxonomy_term',
        'name' => 'entity_from_field:field_tags-node-taxonomy_term',
        'delta' => '0',
        'context' => 'argument_entity_id:node_1',
        'id' => 1,
      ),
    ),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'node_type',
          'settings' => array(
            'type' => array(
              'article' => 'article',
            ),
          ),
          'context' => 'argument_entity_id:node_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'twocol';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'left' => NULL,
      'right' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '5ad3a90c-c465-4f25-bb40-d70e0fe23747';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-2d8fc497-c871-4826-8999-f3199a1102ac';
    $pane->panel = 'left';
    $pane->type = 'node_content';
    $pane->subtype = 'node_content';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'links' => 1,
      'no_extras' => 0,
      'override_title' => 0,
      'override_title_text' => '',
      'identifier' => '',
      'link' => 1,
      'leave_node_title' => 0,
      'build_mode' => 'full',
      'context' => 'argument_entity_id:node_1',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '2d8fc497-c871-4826-8999-f3199a1102ac';
    $display->content['new-2d8fc497-c871-4826-8999-f3199a1102ac'] = $pane;
    $display->panels['left'][0] = 'new-2d8fc497-c871-4826-8999-f3199a1102ac';
    $pane = new stdClass();
    $pane->pid = 'new-92c5f5b4-c751-4565-809f-a1c35a96f53c';
    $pane->panel = 'left';
    $pane->type = 'views';
    $pane->subtype = 'bookmarked_by';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 1,
      'nodes_per_page' => '0',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'default',
      'context' => array(
        0 => 'argument_entity_id:node_1.nid',
      ),
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array(
      'method' => 'fastcache_panels_cache',
      'settings' => array(),
    );
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '92c5f5b4-c751-4565-809f-a1c35a96f53c';
    $display->content['new-92c5f5b4-c751-4565-809f-a1c35a96f53c'] = $pane;
    $display->panels['left'][1] = 'new-92c5f5b4-c751-4565-809f-a1c35a96f53c';
    $pane = new stdClass();
    $pane->pid = 'new-bba18152-f322-4f1d-b16f-cc2aa353e6b4';
    $pane->panel = 'right';
    $pane->type = 'views';
    $pane->subtype = 'related_articles';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 1,
      'nodes_per_page' => '5',
      'pager_id' => '5',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'default',
      'context' => array(
        0 => 'relationship_entity_from_field:field_tags-node-taxonomy_term_1.tid',
        1 => 'argument_entity_id:node_1.nid',
      ),
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array(
      'method' => 'fastcache_panels_cache',
      'settings' => array(),
    );
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'bba18152-f322-4f1d-b16f-cc2aa353e6b4';
    $display->content['new-bba18152-f322-4f1d-b16f-cc2aa353e6b4'] = $pane;
    $display->panels['right'][0] = 'new-bba18152-f322-4f1d-b16f-cc2aa353e6b4';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-2d8fc497-c871-4826-8999-f3199a1102ac';
  $handler->conf['display'] = $display;
  $export['node_view_panel_context'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'search_node_panel_context';
  $handler->task = 'search';
  $handler->subtask = 'node';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Search',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'onecol';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'middle' => NULL,
    ),
  );
  $display->cache = array(
    'method' => 'fastcache_panels_cache',
    'settings' => array(),
  );
  $display->title = '';
  $display->uuid = '1e09779f-3b8c-4ad1-8bed-f3bd44ae85c4';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-7c84eeab-4639-4e2e-8fb7-3c1bb647e9cb';
    $pane->panel = 'middle';
    $pane->type = 'search_result';
    $pane->subtype = 'search_result';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'type' => 'node',
      'log' => 1,
      'override_empty' => 0,
      'empty_title' => '',
      'empty' => '',
      'empty_format' => 'plain_text',
      'override_no_key' => 0,
      'no_key_title' => '',
      'no_key' => '',
      'no_key_format' => 'plain_text',
      'context' => 'argument_string_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '7c84eeab-4639-4e2e-8fb7-3c1bb647e9cb';
    $display->content['new-7c84eeab-4639-4e2e-8fb7-3c1bb647e9cb'] = $pane;
    $display->panels['middle'][0] = 'new-7c84eeab-4639-4e2e-8fb7-3c1bb647e9cb';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-7c84eeab-4639-4e2e-8fb7-3c1bb647e9cb';
  $handler->conf['display'] = $display;
  $export['search_node_panel_context'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'user_view_panel_context';
  $handler->task = 'user_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'User view',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'twocol_stacked';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'top' => NULL,
      'left' => NULL,
      'right' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '96d441ea-68fd-47c5-8fe0-68f95aaba16f';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-60905117-e599-4c82-8489-0650db6742d6';
    $pane->panel = 'left';
    $pane->type = 'views';
    $pane->subtype = 'content_by_author';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 1,
      'nodes_per_page' => '0',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'default',
      'context' => array(
        0 => 'argument_entity_id:user_1.uid',
      ),
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array(
      'method' => 'fastcache_panels_cache',
      'settings' => array(),
    );
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '60905117-e599-4c82-8489-0650db6742d6';
    $display->content['new-60905117-e599-4c82-8489-0650db6742d6'] = $pane;
    $display->panels['left'][0] = 'new-60905117-e599-4c82-8489-0650db6742d6';
    $pane = new stdClass();
    $pane->pid = 'new-ff27ecad-0498-437e-9c17-b005e30b64d9';
    $pane->panel = 'top';
    $pane->type = 'entity_view';
    $pane->subtype = 'user';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_mode' => 'full',
      'context' => 'argument_entity_id:user_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'ff27ecad-0498-437e-9c17-b005e30b64d9';
    $display->content['new-ff27ecad-0498-437e-9c17-b005e30b64d9'] = $pane;
    $display->panels['top'][0] = 'new-ff27ecad-0498-437e-9c17-b005e30b64d9';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['user_view_panel_context'] = $handler;

  return $export;
}

/**
 * Implements hook_default_page_manager_pages().
 */
function fastcache_example_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'frontpage';
  $page->task = 'page';
  $page->admin_title = 'Frontpage';
  $page->admin_description = '';
  $page->path = '<front>';
  $page->access = array();
  $page->menu = array();
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_frontpage_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'frontpage';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Frontpage',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(
      0 => array(
        'identifier' => 'User',
        'keyword' => 'user',
        'name' => 'user',
        'type' => 'current',
        'uid' => '',
        'id' => 1,
      ),
    ),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'twocol_stacked';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'left' => NULL,
      'right' => NULL,
      'top' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'aeb699d3-0f1e-482f-a22e-d7e62049c0c2';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-1884f4db-f63d-4f98-8718-42e937c64b36';
    $pane->panel = 'left';
    $pane->type = 'views';
    $pane->subtype = 'latest_contents';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 1,
      'nodes_per_page' => '10',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'page',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array(
      'method' => 'fastcache_panels_cache',
      'settings' => array(),
    );
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '1884f4db-f63d-4f98-8718-42e937c64b36';
    $display->content['new-1884f4db-f63d-4f98-8718-42e937c64b36'] = $pane;
    $display->panels['left'][0] = 'new-1884f4db-f63d-4f98-8718-42e937c64b36';
    $pane = new stdClass();
    $pane->pid = 'new-deb2e79e-34f6-4795-9d0a-882436e9f8e5';
    $pane->panel = 'right';
    $pane->type = 'views';
    $pane->subtype = 'latest_users';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 1,
      'nodes_per_page' => '10',
      'pager_id' => '1',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'page',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array(
      'method' => 'fastcache_panels_cache',
      'settings' => array(),
    );
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'deb2e79e-34f6-4795-9d0a-882436e9f8e5';
    $display->content['new-deb2e79e-34f6-4795-9d0a-882436e9f8e5'] = $pane;
    $display->panels['right'][0] = 'new-deb2e79e-34f6-4795-9d0a-882436e9f8e5';
    $pane = new stdClass();
    $pane->pid = 'new-68fae88f-859f-4fe3-92eb-389d6fb4f2c4';
    $pane->panel = 'right';
    $pane->type = 'block';
    $pane->subtype = 'user-new';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array(
      'method' => 'fastcache_panels_cache',
      'settings' => array(),
    );
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '68fae88f-859f-4fe3-92eb-389d6fb4f2c4';
    $display->content['new-68fae88f-859f-4fe3-92eb-389d6fb4f2c4'] = $pane;
    $display->panels['right'][1] = 'new-68fae88f-859f-4fe3-92eb-389d6fb4f2c4';
    $pane = new stdClass();
    $pane->pid = 'new-85246e44-eb5e-4c8f-9830-a2be256ac1f1';
    $pane->panel = 'top';
    $pane->type = 'views';
    $pane->subtype = 'favorite_articles';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 1,
      'nodes_per_page' => '10',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'page',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array(
      'method' => 'fastcache_panels_cache',
      'settings' => array(),
    );
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '85246e44-eb5e-4c8f-9830-a2be256ac1f1';
    $display->content['new-85246e44-eb5e-4c8f-9830-a2be256ac1f1'] = $pane;
    $display->panels['top'][0] = 'new-85246e44-eb5e-4c8f-9830-a2be256ac1f1';
    $pane = new stdClass();
    $pane->pid = 'new-8af9a8c3-374f-40cf-a964-b332ab7b8bb7';
    $pane->panel = 'top';
    $pane->type = 'views';
    $pane->subtype = 'followed_users';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 1,
      'nodes_per_page' => '10',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'page',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array(
      'method' => 'fastcache_panels_cache',
      'settings' => array(),
    );
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '8af9a8c3-374f-40cf-a964-b332ab7b8bb7';
    $display->content['new-8af9a8c3-374f-40cf-a964-b332ab7b8bb7'] = $pane;
    $display->panels['top'][1] = 'new-8af9a8c3-374f-40cf-a964-b332ab7b8bb7';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['frontpage'] = $page;

  return $pages;

}
