<?php
/**
 * @file
 * cod_front_page.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function cod_front_page_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cod_front_page';
  $page->task = 'page';
  $page->admin_title = 'cod_front_page';
  $page->admin_description = 'Front page';
  $page->path = 'front';
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
  $handler->name = 'page_cod_front_page__front_page';
  $handler->task = 'page';
  $handler->subtask = 'cod_front_page';
  $handler->handler = 'panel_context';
  $handler->weight = 1;
  $handler->conf = array(
    'title' => 'Front Page',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'name' => 'front_page',
  );
  $display = new panels_display();
  $display->layout = 'orion';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'banner' => NULL,
      'left_header' => NULL,
      'right_header' => NULL,
      'preface' => NULL,
      'left_content' => NULL,
      'right_content' => NULL,
      'footer' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '92d70791-409f-4b4b-b6ed-83fb0594ea26';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-b93f6002-e000-4864-81a2-7ea8daaa37ea';
    $pane->panel = 'preface';
    $pane->type = 'views_panes';
    $pane->subtype = 'cod_front_page_conferences-panel_pane_1';
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
    $pane->uuid = 'b93f6002-e000-4864-81a2-7ea8daaa37ea';
    $display->content['new-b93f6002-e000-4864-81a2-7ea8daaa37ea'] = $pane;
    $display->panels['preface'][0] = 'new-b93f6002-e000-4864-81a2-7ea8daaa37ea';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = 'new-b93f6002-e000-4864-81a2-7ea8daaa37ea';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cod_front_page'] = $page;

  return $pages;

}
