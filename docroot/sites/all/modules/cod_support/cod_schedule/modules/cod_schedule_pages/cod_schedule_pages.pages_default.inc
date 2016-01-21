<?php
/**
 * @file
 * cod_schedule_pages.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function cod_schedule_pages_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cod_schedule_admin_page';
  $page->task = 'page';
  $page->admin_title = 'Schedule Admin Page';
  $page->admin_description = 'Cod Scheduler';
  $page->path = 'admin/conference/%node/schedule/!day';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'node_type',
        'settings' => array(
          'type' => array(
            'event' => 'event',
          ),
        ),
        'context' => 'argument_entity_id:node_1',
        'not' => FALSE,
      ),
      1 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'administer schedule',
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
    'type' => 'tab',
    'title' => 'Scheduler',
    'name' => 'navigation',
    'weight' => '-2',
    'parent' => array(
      'type' => 'none',
      'title' => '',
      'name' => 'navigation',
      'weight' => '0',
    ),
  );
  $page->arguments = array(
    'node' => array(
      'id' => 1,
      'identifier' => 'Event',
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
  $handler->name = 'page_cod_schedule_admin_page_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cod_schedule_admin_page';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Schedule Panel',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => 'cod-schedule-wrapper',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'context_exists',
          'settings' => array(
            'exists' => '1',
          ),
          'context' => 'argument_string_1',
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
      'center' => NULL,
      'left' => NULL,
      'right' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Schedule';
  $display->uuid = 'f606f0b5-c491-418c-97f0-cbdaac8b8561';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-c16b040e-aa16-4c47-9b3d-4a6b1b9b40d4';
    $pane->panel = 'left';
    $pane->type = 'cod_schedule_menu_pane';
    $pane->subtype = 'cod_schedule_menu_pane';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'content_type' => '',
      'override_title' => 0,
      'override_title_text' => '',
      'override_title_heading' => 'h2',
      'path_prefix' => '',
      'date_format' => 'Y-m-d',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'c16b040e-aa16-4c47-9b3d-4a6b1b9b40d4';
    $display->content['new-c16b040e-aa16-4c47-9b3d-4a6b1b9b40d4'] = $pane;
    $display->panels['left'][0] = 'new-c16b040e-aa16-4c47-9b3d-4a6b1b9b40d4';
    $pane = new stdClass();
    $pane->pid = 'new-e960910c-0102-4b1c-96de-e59a3496beb8';
    $pane->panel = 'left';
    $pane->type = 'cod_schedule_scheduler_pane';
    $pane->subtype = 'cod_schedule_scheduler_pane';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'e960910c-0102-4b1c-96de-e59a3496beb8';
    $display->content['new-e960910c-0102-4b1c-96de-e59a3496beb8'] = $pane;
    $display->panels['left'][1] = 'new-e960910c-0102-4b1c-96de-e59a3496beb8';
    $pane = new stdClass();
    $pane->pid = 'new-e1cc7db0-4390-432a-ab4b-1e9611d50f4e';
    $pane->panel = 'right';
    $pane->type = 'cod_schedule_sessions_pane';
    $pane->subtype = 'cod_schedule_sessions_pane';
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
    $pane->uuid = 'e1cc7db0-4390-432a-ab4b-1e9611d50f4e';
    $display->content['new-e1cc7db0-4390-432a-ab4b-1e9611d50f4e'] = $pane;
    $display->panels['right'][0] = 'new-e1cc7db0-4390-432a-ab4b-1e9611d50f4e';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page-cod_schedule_admin_page__schedule_default_day';
  $handler->task = 'page';
  $handler->subtask = 'cod_schedule_admin_page';
  $handler->handler = 'http_response';
  $handler->weight = 1;
  $handler->conf = array(
    'title' => 'Schedule Default day',
    'contexts' => array(
      0 => array(
        'identifier' => 'Schedule Default Day',
        'keyword' => 'cod_schedule_default_day',
        'name' => 'cod_schedule_day',
        'day' => '',
        'content_type' => '',
        'id' => 1,
      ),
    ),
    'relationships' => array(),
    'code' => '301',
    'destination' => 'admin/conference/%node:nid/schedule/%cod_schedule_default_day',
    'name' => 'schedule_default_day',
    'access' => array(
      'plugins' => array(),
      'logic' => 'and',
    ),
  );
  $page->default_handlers[$handler->name] = $handler;
  $pages['cod_schedule_admin_page'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cod_schedule_mine';
  $page->task = 'page';
  $page->admin_title = 'My Schedule';
  $page->admin_description = '';
  $page->path = 'node/%node/schedule/mine';
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
  );
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_cod_schedule_mine__panel_context_4c6ae307-de5a-4064-8903-97f7c9c284a5';
  $handler->task = 'page';
  $handler->subtask = 'cod_schedule_mine';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'My Schedule',
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
  $display->title = 'My Schedule';
  $display->uuid = 'e37336dc-989b-45cf-b43f-1c64b64773b9';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-ca1f880f-8cf9-4310-8e74-5858614cd653';
    $pane->panel = 'content';
    $pane->type = 'views_panes';
    $pane->subtype = 'cod_session_sessions-pane_sessions_mine';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'arguments' => array(
        'og_group_ref_target_id' => '%node:nid',
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
    $pane->uuid = 'ca1f880f-8cf9-4310-8e74-5858614cd653';
    $display->content['new-ca1f880f-8cf9-4310-8e74-5858614cd653'] = $pane;
    $display->panels['content'][0] = 'new-ca1f880f-8cf9-4310-8e74-5858614cd653';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-ca1f880f-8cf9-4310-8e74-5858614cd653';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cod_schedule_mine'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cod_schedule_public';
  $page->task = 'page';
  $page->admin_title = 'Public Schedule page';
  $page->admin_description = '';
  $page->path = 'node/%node/schedule/!day';
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
  $handler->name = 'page-cod_schedule_public__schedule_default_day';
  $handler->task = 'page';
  $handler->subtask = 'cod_schedule_public';
  $handler->handler = 'http_response';
  $handler->weight = -30;
  $handler->conf = array(
    'title' => 'Schedule Default day',
    'contexts' => array(
      0 => array(
        'identifier' => 'Schedule Default Day',
        'keyword' => 'cod_schedule_default_day',
        'name' => 'cod_schedule_day',
        'day' => '',
        'id' => 1,
        'content_type' => 'session+schedule_item',
      ),
    ),
    'relationships' => array(),
    'code' => '301',
    'destination' => 'node/%node:nid/schedule/%cod_schedule_default_day',
    'name' => 'schedule_default_day',
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
  $handler->name = 'page_cod_schedule_public__panel_context_931cc9dd-ba60-4165-9530-0abad52c7c4b';
  $handler->task = 'page';
  $handler->subtask = 'cod_schedule_public';
  $handler->handler = 'panel_context';
  $handler->weight = -29;
  $handler->conf = array(
    'title' => 'Schedule Page',
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
  $display->title = 'Schedule';
  $display->uuid = '77ac51b8-f86c-4684-8b15-b3f0ea5cdd0c';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-220151d2-a724-460e-8851-715d41070cc8';
    $pane->panel = 'content';
    $pane->type = 'views_panes';
    $pane->subtype = 'cod_schedule_content-cod_schedule_session_content_pane';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'arguments' => array(
        'gid' => '%node:nid',
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
    $pane->uuid = '220151d2-a724-460e-8851-715d41070cc8';
    $display->content['new-220151d2-a724-460e-8851-715d41070cc8'] = $pane;
    $display->panels['content'][0] = 'new-220151d2-a724-460e-8851-715d41070cc8';
    $pane = new stdClass();
    $pane->pid = 'new-9e717097-25dc-48fc-b8bc-65212ec84f12';
    $pane->panel = 'preface';
    $pane->type = 'cod_schedule_menu_pane';
    $pane->subtype = 'cod_schedule_menu_pane';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'content_type' => 'session+schedule_item',
      'path_prefix' => '',
      'date_format' => '',
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
    $pane->uuid = '9e717097-25dc-48fc-b8bc-65212ec84f12';
    $display->content['new-9e717097-25dc-48fc-b8bc-65212ec84f12'] = $pane;
    $display->panels['preface'][0] = 'new-9e717097-25dc-48fc-b8bc-65212ec84f12';
    $pane = new stdClass();
    $pane->pid = 'new-f71d9b52-8e00-4059-9b26-297422799348';
    $pane->panel = 'preface';
    $pane->type = 'views_panes';
    $pane->subtype = 'cod_session_tracks-schedule_session_tracks';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'arguments' => array(
        'machine_name' => 'session_tracks_%node:nid',
        'null_1' => '',
        'null' => '%day',
      ),
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'f71d9b52-8e00-4059-9b26-297422799348';
    $display->content['new-f71d9b52-8e00-4059-9b26-297422799348'] = $pane;
    $display->panels['preface'][1] = 'new-f71d9b52-8e00-4059-9b26-297422799348';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cod_schedule_public'] = $page;

  return $pages;

}
