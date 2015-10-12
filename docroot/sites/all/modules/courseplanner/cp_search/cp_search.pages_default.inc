<?php
/**
 * @file
 * cp_search.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function cp_search_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cp_browse';
  $page->task = 'page';
  $page->admin_title = 'Browse Course planner items';
  $page->admin_description = 'A page for browsing/searching Course planner items.';
  $page->path = 'browse/all';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'tab',
    'title' => 'All items',
    'name' => 'main-menu',
    'weight' => '10',
    'parent' => array(
      'type' => 'none',
      'title' => '',
      'name' => 'navigation',
      'weight' => '0',
    ),
  );
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_cp_browse_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cp_browse';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
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
  $display->cache = array();
  $display->title = 'Browse Course planner items';
  $display->uuid = '948c4809-cb58-4dc8-a274-6cfeae08f0bc';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-cbd7b816-7a72-4712-8380-3ff8aa302cd4';
    $pane->panel = 'middle';
    $pane->type = 'views_panes';
    $pane->subtype = 'cp_browse-browse_pane';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
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
    $pane->uuid = 'cbd7b816-7a72-4712-8380-3ff8aa302cd4';
    $display->content['new-cbd7b816-7a72-4712-8380-3ff8aa302cd4'] = $pane;
    $display->panels['middle'][0] = 'new-cbd7b816-7a72-4712-8380-3ff8aa302cd4';
  $display->hide_title = PANELS_TITLE_PANE;
  $display->title_pane = 'new-cbd7b816-7a72-4712-8380-3ff8aa302cd4';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cp_browse'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cp_browse_offerings';
  $page->task = 'page';
  $page->admin_title = 'Browse course offerings';
  $page->admin_description = 'A search/browse page for course offerings.';
  $page->path = 'browse/offerings';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'tab',
    'title' => 'Course offerings',
    'name' => 'navigation',
    'weight' => '6',
    'parent' => array(
      'type' => 'none',
      'title' => '',
      'name' => 'navigation',
      'weight' => '0',
    ),
  );
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_cp_browse_offerings_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cp_browse_offerings';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
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
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'e834e1cf-6a3c-4190-a52e-c4be0bbb1bbf';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-ca8bef2c-7a7b-4f03-a97b-49b4ecca6772';
    $pane->panel = 'middle';
    $pane->type = 'views_panes';
    $pane->subtype = 'cp_item_lists-cp_course_offerings';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
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
    $pane->uuid = 'ca8bef2c-7a7b-4f03-a97b-49b4ecca6772';
    $display->content['new-ca8bef2c-7a7b-4f03-a97b-49b4ecca6772'] = $pane;
    $display->panels['middle'][0] = 'new-ca8bef2c-7a7b-4f03-a97b-49b4ecca6772';
  $display->hide_title = PANELS_TITLE_PANE;
  $display->title_pane = 'new-ca8bef2c-7a7b-4f03-a97b-49b4ecca6772';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cp_browse_offerings'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cp_browse_outlines';
  $page->task = 'page';
  $page->admin_title = 'Browse course outlines';
  $page->admin_description = 'A search/browse page for course offerings.';
  $page->path = 'browse/outlines';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'tab',
    'title' => 'Course outlines',
    'name' => 'navigation',
    'weight' => '7',
    'parent' => array(
      'type' => 'none',
      'title' => '',
      'name' => 'navigation',
      'weight' => '0',
    ),
  );
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_cp_browse_outlines_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cp_browse_outlines';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
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
  $display->cache = array();
  $display->title = '';
  $display->uuid = '25df944a-e0b8-4683-81e5-047e3e36cc83';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-33c8bed8-f26f-4e47-b4ce-496295f57bfe';
    $pane->panel = 'middle';
    $pane->type = 'views_panes';
    $pane->subtype = 'cp_item_lists-cp_course_outlines';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
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
    $pane->uuid = '33c8bed8-f26f-4e47-b4ce-496295f57bfe';
    $display->content['new-33c8bed8-f26f-4e47-b4ce-496295f57bfe'] = $pane;
    $display->panels['middle'][0] = 'new-33c8bed8-f26f-4e47-b4ce-496295f57bfe';
  $display->hide_title = PANELS_TITLE_PANE;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cp_browse_outlines'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cp_browse_resources';
  $page->task = 'page';
  $page->admin_title = 'Browse Course planner resources';
  $page->admin_description = 'A page for searching/browsing Course planner resources';
  $page->path = 'browse/resources';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'default tab',
    'title' => 'Resources',
    'name' => 'main-menu',
    'weight' => '3',
    'parent' => array(
      'type' => 'normal',
      'title' => 'Browse',
      'name' => 'main-menu',
      'weight' => '0',
    ),
  );
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_cp_browse_resources_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cp_browse_resources';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
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
  $display->cache = array();
  $display->title = 'Browse resources';
  $display->uuid = '7740fb7f-15c2-41db-9f18-520b2391259d';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-b8bc9fd7-7612-4751-bc81-d9067dfcbd7d';
    $pane->panel = 'middle';
    $pane->type = 'views_panes';
    $pane->subtype = 'cp_browse-panel_pane_1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
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
    $pane->uuid = 'b8bc9fd7-7612-4751-bc81-d9067dfcbd7d';
    $display->content['new-b8bc9fd7-7612-4751-bc81-d9067dfcbd7d'] = $pane;
    $display->panels['middle'][0] = 'new-b8bc9fd7-7612-4751-bc81-d9067dfcbd7d';
  $display->hide_title = PANELS_TITLE_PANE;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cp_browse_resources'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cp_browse_resources_all';
  $page->task = 'page';
  $page->admin_title = 'Browse all Course planner resources';
  $page->admin_description = 'A page for searching/browsing Course planner resources';
  $page->path = 'browse/resources-all';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'tab',
    'title' => 'All resources',
    'name' => 'main-menu',
    'weight' => '5',
    'parent' => array(
      'type' => 'normal',
      'title' => 'Browse',
      'name' => 'main-menu',
      'weight' => '0',
    ),
  );
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_cp_browse_resources_all_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cp_browse_resources_all';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
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
  $display->cache = array();
  $display->title = 'Browse resources';
  $display->uuid = 'c5901332-aabe-48c6-9b68-e326951850ea';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-26ffe525-64d7-4998-90e7-cfc216a59e1f';
    $pane->panel = 'middle';
    $pane->type = 'views_panes';
    $pane->subtype = 'cp_browse-cp_browse_resources';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
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
    $pane->uuid = '26ffe525-64d7-4998-90e7-cfc216a59e1f';
    $display->content['new-26ffe525-64d7-4998-90e7-cfc216a59e1f'] = $pane;
    $display->panels['middle'][0] = 'new-26ffe525-64d7-4998-90e7-cfc216a59e1f';
  $display->hide_title = PANELS_TITLE_PANE;
  $display->title_pane = 'new-26ffe525-64d7-4998-90e7-cfc216a59e1f';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cp_browse_resources_all'] = $page;

  return $pages;

}
