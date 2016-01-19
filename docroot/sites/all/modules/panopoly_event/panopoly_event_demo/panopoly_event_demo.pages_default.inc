<?php
/**
 * @file
 * panopoly_event_demo.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function panopoly_event_demo_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'event_homepage_demo';
  $page->task = 'page';
  $page->admin_title = 'Event Homepage Demo';
  $page->admin_description = '';
  $page->path = 'event-demo';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'normal',
    'title' => 'Event Demo',
    'name' => 'main-menu',
    'weight' => '0',
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
  $handler->name = 'page_event_homepage_demo_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'event_homepage_demo';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Event Homepage Demo',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'burr_flipped';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'sidebar' => NULL,
      'contentmain' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Event Homepage Demo';
  $display->uuid = '74f1d6d0-4a97-4f19-84dd-369cba7db99f';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-820a3a68-5ef4-46e8-a854-e2989a374e08';
    $pane->panel = 'contentmain';
    $pane->type = 'views_panes';
    $pane->subtype = 'panopoly_event_demo_views-panel_pane_1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_settings' => 'fields',
      'header_type' => 'none',
      'view_mode' => 'teaser',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '820a3a68-5ef4-46e8-a854-e2989a374e08';
    $display->content['new-820a3a68-5ef4-46e8-a854-e2989a374e08'] = $pane;
    $display->panels['contentmain'][0] = 'new-820a3a68-5ef4-46e8-a854-e2989a374e08';
    $pane = new stdClass();
    $pane->pid = 'new-fc7d4146-1f94-4b76-8494-6c0c97cf6aad';
    $pane->panel = 'sidebar';
    $pane->type = 'search_box';
    $pane->subtype = 'search_box';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'type' => 'node',
      'form' => 'simple',
      'path_type' => 'default',
      'path' => '',
      'override_prompt' => 0,
      'prompt' => '',
      'override_title' => 0,
      'override_title_text' => 'Search the Site',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'fc7d4146-1f94-4b76-8494-6c0c97cf6aad';
    $display->content['new-fc7d4146-1f94-4b76-8494-6c0c97cf6aad'] = $pane;
    $display->panels['sidebar'][0] = 'new-fc7d4146-1f94-4b76-8494-6c0c97cf6aad';
    $pane = new stdClass();
    $pane->pid = 'new-1d671c35-93e9-4992-8811-12fe45ac4ed4';
    $pane->panel = 'sidebar';
    $pane->type = 'views_panes';
    $pane->subtype = 'panopoly_event_demo_views-panel_pane_2';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_settings' => 'fields',
      'header_type' => 'none',
      'view_mode' => 'teaser',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '1d671c35-93e9-4992-8811-12fe45ac4ed4';
    $display->content['new-1d671c35-93e9-4992-8811-12fe45ac4ed4'] = $pane;
    $display->panels['sidebar'][1] = 'new-1d671c35-93e9-4992-8811-12fe45ac4ed4';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['event_homepage_demo'] = $page;

  return $pages;

}
