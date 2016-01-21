<?php
/**
 * @file
 * cod_bof_pages.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function cod_bof_pages_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cod_bof_pages';
  $page->task = 'page';
  $page->admin_title = 'cod_bof_pages';
  $page->admin_description = '';
  $page->path = 'node/%node/bofs/!day';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array();
  $page->arguments = array(
    'node' => array(
      'id' => 1,
      'identifier' => 'Conference',
      'name' => 'entity_id:node',
      'settings' => array(),
    ),
    'day' => array(
      'id' => 1,
      'identifier' => 'Schedule Day',
      'name' => 'string',
      'settings' => array(
        'use_tail' => 0,
      ),
    ),
  );
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page-cod_bof_pages__bof_schedule_default_day';
  $handler->task = 'page';
  $handler->subtask = 'cod_bof_pages';
  $handler->handler = 'http_response';
  $handler->weight = -30;
  $handler->conf = array(
    'title' => 'Bof Schedule Default day',
    'contexts' => array(
      0 => array(
        'identifier' => 'Schedule Default Day',
        'keyword' => 'cod_schedule_default_day',
        'name' => 'cod_schedule_day',
        'id' => 1,
        'day' => '',
        'content_type' => 'bof_session',
      ),
    ),
    'relationships' => array(),
    'code' => '301',
    'destination' => 'node/%node:nid/bofs/%cod_schedule_default_day',
    'name' => 'bof_schedule_default_day',
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'context_exists',
          'settings' => array(
            'exists' => '0',
          ),
          'context' => 'argument_string_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $page->default_handlers[$handler->name] = $handler;
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_cod_bof_pages__panel_context_d1b7e4a1-edf8-4c94-bc18-5b7362409cc6';
  $handler->task = 'page';
  $handler->subtask = 'cod_bof_pages';
  $handler->handler = 'panel_context';
  $handler->weight = -29;
  $handler->conf = array(
    'title' => 'Bof Schedule',
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
  $display->title = 'Birds of a Feather';
  $display->uuid = '83c104ac-f931-4d19-980e-937e84900cee';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-9498e9f1-adf3-4345-af97-3c7c784dbc7b';
    $pane->panel = 'content';
    $pane->type = 'views_panes';
    $pane->subtype = 'cod_bof_schedule_timeslots-cod_bof_session_time_pane';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'arguments' => array(
        'nid' => '%node:nid',
        'field_evttime_date_value' => '%day',
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
    $pane->uuid = '9498e9f1-adf3-4345-af97-3c7c784dbc7b';
    $display->content['new-9498e9f1-adf3-4345-af97-3c7c784dbc7b'] = $pane;
    $display->panels['content'][0] = 'new-9498e9f1-adf3-4345-af97-3c7c784dbc7b';
    $pane = new stdClass();
    $pane->pid = 'new-4c14bf58-31f4-45d4-812a-44119df82554';
    $pane->panel = 'preface';
    $pane->type = 'cod_schedule_menu_pane';
    $pane->subtype = 'cod_schedule_menu_pane';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'content_type' => 'bof_session',
      'override_title' => 0,
      'override_title_text' => '',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '4c14bf58-31f4-45d4-812a-44119df82554';
    $display->content['new-4c14bf58-31f4-45d4-812a-44119df82554'] = $pane;
    $display->panels['preface'][0] = 'new-4c14bf58-31f4-45d4-812a-44119df82554';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cod_bof_pages'] = $page;

  return $pages;

}
