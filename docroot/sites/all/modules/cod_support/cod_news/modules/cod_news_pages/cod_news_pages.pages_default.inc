<?php
/**
 * @file
 * cod_news_pages.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function cod_news_pages_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cod_news_pages';
  $page->task = 'page';
  $page->admin_title = 'News';
  $page->admin_description = '';
  $page->path = 'node/%node/news';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'context_exists',
        'settings' => array(
          'exists' => '1',
        ),
        'context' => 'argument_entity_id:node_1',
        'not' => FALSE,
      ),
    ),
    'logic' => 'and',
  );
  $page->menu = array();
  $page->arguments = array(
    'node' => array(
      'id' => 1,
      'identifier' => 'Conference',
      'name' => 'entity_id:node',
      'settings' => array(),
    ),
  );
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_cod_news_pages__panel_context_8cb77836-a44e-4adb-b402-8eca877a4d20';
  $handler->task = 'page';
  $handler->subtask = 'cod_news_pages';
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
    'access' => array(
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'cygnus';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'preface' => NULL,
      'content' => NULL,
      'footer' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'c4f95a4e-d8d4-4a40-90d8-79ba84537648';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-1fa8273f-5b38-4d2e-b5a4-23390f5f04aa';
  $pane->panel = 'content';
  $pane->type = 'views_panes';
  $pane->subtype = 'cod_news-news_conference_page';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'arguments' => array(
      'gid' => '%node:nid',
    ),
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '1fa8273f-5b38-4d2e-b5a4-23390f5f04aa';
  $display->content['new-1fa8273f-5b38-4d2e-b5a4-23390f5f04aa'] = $pane;
  $display->panels['content'][0] = 'new-1fa8273f-5b38-4d2e-b5a4-23390f5f04aa';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-1fa8273f-5b38-4d2e-b5a4-23390f5f04aa';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cod_news_pages'] = $page;

  return $pages;

}
