<?php
/**
 * @file
 * storiedmaps_story.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function storiedmaps_story_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'story_landing_page';
  $page->task = 'page';
  $page->admin_title = 'Story landing page';
  $page->admin_description = '';
  $page->path = 'story';
  $page->access = array();
  $page->menu = array(
    'type' => 'normal',
    'title' => 'Stories',
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
  $handler->name = 'page_story_landing_page_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'story_landing_page';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 1,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'two_brick';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'two_brick_top' => NULL,
      'two_brick_left_above' => NULL,
      'two_brick_right_above' => NULL,
      'two_brick_middle' => NULL,
      'two_brick_left_below' => NULL,
      'two_brick_right_below' => NULL,
      'two_brick_bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Stories';
  $display->uuid = '30faeb18-44f9-4e06-b228-067d6f7c93b1';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-dfee102f-a7f1-41b3-a07d-32a5318fe563';
    $pane->panel = 'two_brick_bottom';
    $pane->type = 'views';
    $pane->subtype = 'media';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 0,
      'nodes_per_page' => '9',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'block_1',
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
    $pane->uuid = 'dfee102f-a7f1-41b3-a07d-32a5318fe563';
    $display->content['new-dfee102f-a7f1-41b3-a07d-32a5318fe563'] = $pane;
    $display->panels['two_brick_bottom'][0] = 'new-dfee102f-a7f1-41b3-a07d-32a5318fe563';
    $pane = new stdClass();
    $pane->pid = 'new-c4cd3ccb-be32-4753-9be4-dcfa2784755b';
    $pane->panel = 'two_brick_middle';
    $pane->type = 'views_panes';
    $pane->subtype = 'story-panel_pane_5';
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
    $pane->uuid = 'c4cd3ccb-be32-4753-9be4-dcfa2784755b';
    $display->content['new-c4cd3ccb-be32-4753-9be4-dcfa2784755b'] = $pane;
    $display->panels['two_brick_middle'][0] = 'new-c4cd3ccb-be32-4753-9be4-dcfa2784755b';
    $pane = new stdClass();
    $pane->pid = 'new-10bb14b1-6fed-4d86-9a63-50d98fb6355b';
    $pane->panel = 'two_brick_top';
    $pane->type = 'views_panes';
    $pane->subtype = 'story-panel_pane_3';
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
    $pane->uuid = '10bb14b1-6fed-4d86-9a63-50d98fb6355b';
    $display->content['new-10bb14b1-6fed-4d86-9a63-50d98fb6355b'] = $pane;
    $display->panels['two_brick_top'][0] = 'new-10bb14b1-6fed-4d86-9a63-50d98fb6355b';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-10bb14b1-6fed-4d86-9a63-50d98fb6355b';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['story_landing_page'] = $page;

  return $pages;

}
