<?php
/**
 * @file
 * openacademy_courses.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function openacademy_courses_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'openacademy_courses';
  $page->task = 'page';
  $page->admin_title = 'Courses';
  $page->admin_description = '';
  $page->path = 'courses';
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
    'title' => 'Courses',
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
  $handler->name = 'page_openacademy_courses_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'openacademy_courses';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Courses',
    'no_blocks' => 1,
    'pipeline' => 'ipe',
    'css_id' => 'courses-panel',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'boxton';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'left' => NULL,
      'right' => NULL,
      'sidebar' => NULL,
      'contentmain' => NULL,
      'header' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Courses';
  $display->uuid = 'f53f389a-1214-d9a4-a591-2bb351bc696c';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-29fd5442-0a9d-8134-c92b-b354a001f2b3';
    $pane->panel = 'contentmain';
    $pane->type = 'views_panes';
    $pane->subtype = 'openacademy_courses-courses_table';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'items_per_page' => '0',
      'fields_override' => array(
        'title' => 1,
        'field_course_number' => 1,
        'field_course_term' => 1,
        'field_course_time' => 1,
      ),
      'view_mode' => 'teaser',
      'widget_title' => '',
    );
    $pane->cache = array();
    $pane->style = array();
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '29fd5442-0a9d-8134-c92b-b354a001f2b3';
    $display->content['new-29fd5442-0a9d-8134-c92b-b354a001f2b3'] = $pane;
    $display->panels['contentmain'][0] = 'new-29fd5442-0a9d-8134-c92b-b354a001f2b3';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['openacademy_courses'] = $page;

  return $pages;

}
