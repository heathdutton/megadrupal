<?php
/**
 * @file
 * cod_events_pages.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function cod_events_pages_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'conference_admin';
  $page->task = 'page';
  $page->admin_title = 'Event Administration Listing';
  $page->admin_description = '';
  $page->path = 'admin/conference/list';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'administer nodes',
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
    'type' => 'default tab',
    'title' => 'List',
    'name' => 'navigation',
    'weight' => '-10',
    'parent' => array(
      'type' => 'normal',
      'title' => 'Conference',
      'name' => 'management',
      'weight' => '-7',
    ),
  );
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => TRUE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_page_conference_listing_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'conference_admin';
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
  $display->layout = 'flexible';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '8ef4038d-f915-48ec-9600-79d48ecba5ee';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-c021d888-805c-47a3-903b-9785df04eeb8';
    $pane->panel = 'center';
    $pane->type = 'views_panes';
    $pane->subtype = 'cod_events_admin-panel_pane_1';
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
    $pane->uuid = 'c021d888-805c-47a3-903b-9785df04eeb8';
    $display->content['new-c021d888-805c-47a3-903b-9785df04eeb8'] = $pane;
    $display->panels['center'][0] = 'new-c021d888-805c-47a3-903b-9785df04eeb8';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['conference_admin'] = $page;

  return $pages;

}
