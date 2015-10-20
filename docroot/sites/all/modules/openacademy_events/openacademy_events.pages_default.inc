<?php
/**
 * @file
 * openacademy_events.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function openacademy_events_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'openacademy_events';
  $page->task = 'page';
  $page->admin_title = 'Events';
  $page->admin_description = '';
  $page->path = 'events';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'access content',
        ),
        'context' => 'logged-in-user',
        'not' => FALSE,
      ),
    ),
    'logic' => 'and',
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'normal',
    'title' => 'Events',
    'name' => 'main-menu',
    'weight' => '1',
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
  $handler->name = 'page_openacademy_events_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'openacademy_events';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Events',
    'no_blocks' => 1,
    'pipeline' => 'ipe',
    'css_id' => 'events-panel',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'burr';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'left' => NULL,
      'right' => NULL,
      'sidebar' => NULL,
      'contentmain' => NULL,
    ),
    'sidebar' => array(
      'style' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Events';
  $display->uuid = '6c1fd895-db8f-ab04-5122-884929db2785';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-2e6e5c76-2056-6934-258f-4d5c52f6d20d';
    $pane->panel = 'contentmain';
    $pane->type = 'views_panes';
    $pane->subtype = 'openacademy_events-all_events';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'items_per_page' => '7',
      'view_mode' => 'teaser',
      'widget_title' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '2e6e5c76-2056-6934-258f-4d5c52f6d20d';
    $display->content['new-2e6e5c76-2056-6934-258f-4d5c52f6d20d'] = $pane;
    $display->panels['contentmain'][0] = 'new-2e6e5c76-2056-6934-258f-4d5c52f6d20d';
    $pane = new stdClass();
    $pane->pid = 'new-3cb4990f-f4db-31a4-ddfe-fd4c6e3a24db';
    $pane->panel = 'sidebar';
    $pane->type = 'views_panes';
    $pane->subtype = 'openacademy_events-monthly_mini_calendar';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '3cb4990f-f4db-31a4-ddfe-fd4c6e3a24db';
    $display->content['new-3cb4990f-f4db-31a4-ddfe-fd4c6e3a24db'] = $pane;
    $display->panels['sidebar'][0] = 'new-3cb4990f-f4db-31a4-ddfe-fd4c6e3a24db';
    $pane = new stdClass();
    $pane->pid = 'new-fdb92a98-3208-3994-c959-2791615a0c51';
    $pane->panel = 'sidebar';
    $pane->type = 'menu_tree';
    $pane->subtype = 'main-menu';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'menu_name' => 'main-menu',
      'parent_mlid' => 'main-menu:592',
      'title_link' => 0,
      'admin_title' => 'Event Types',
      'level' => '2',
      'follow' => 0,
      'depth' => 0,
      'expanded' => 0,
      'sort' => 0,
      'override_title' => 1,
      'override_title_text' => 'Event Types',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
      'style' => 'basic',
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'fdb92a98-3208-3994-c959-2791615a0c51';
    $display->content['new-fdb92a98-3208-3994-c959-2791615a0c51'] = $pane;
    $display->panels['sidebar'][1] = 'new-fdb92a98-3208-3994-c959-2791615a0c51';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['openacademy_events'] = $page;

  return $pages;

}
