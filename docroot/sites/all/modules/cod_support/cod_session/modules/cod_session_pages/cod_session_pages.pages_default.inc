<?php
/**
 * @file
 * cod_session_pages.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function cod_session_pages_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'sessions';
  $page->task = 'page';
  $page->admin_title = 'Sessions';
  $page->admin_description = '';
  $page->path = 'node/%node/sessions/!status/!term_machine_name';
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
        'name' => 'node_status',
        'settings' => NULL,
        'context' => 'argument_entity_id:node_1',
        'not' => FALSE,
      ),
    ),
    'logic' => 'and',
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'none',
    'title' => 'Sessions',
    'name' => 'navigation',
    'weight' => '0',
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
      'keyword' => 'node',
    ),
    'status' => array(
      'id' => 1,
      'identifier' => 'Session Status',
      'name' => 'cod_session_status',
      'settings' => array(),
    ),
    'term_machine_name' => array(
      'id' => 1,
      'identifier' => 'Taxonomy Term Machine Name',
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
  $handler->name = 'page_sessions_panel_context_3';
  $handler->task = 'page';
  $handler->subtask = 'sessions';
  $handler->handler = 'panel_context';
  $handler->weight = -30;
  $handler->conf = array(
    'title' => 'Sessions by  Track',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
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
          'context' => 'argument_entity_id:node_1',
          'not' => FALSE,
        ),
        1 => array(
          'name' => 'context_exists',
          'settings' => array(
            'exists' => '1',
          ),
          'context' => 'argument_cod_session_status_1',
          'not' => FALSE,
        ),
        2 => array(
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
  $display->layout = 'cygnus';
  $display->layout_settings = array(
    'items' => array(
      'canvas' => array(
        'type' => 'row',
        'contains' => 'column',
        'children' => array(
          0 => 'main',
          1 => 1,
        ),
        'parent' => NULL,
      ),
      'main' => array(
        'type' => 'column',
        'width' => '66.65375151243254',
        'width_type' => '%',
        'children' => array(
          0 => 'main-row',
        ),
        'parent' => 'canvas',
      ),
      'main-row' => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'center',
        ),
        'parent' => 'main',
      ),
      'center' => array(
        'type' => 'region',
        'title' => 'Center',
        'width' => 100,
        'width_type' => '%',
        'parent' => 'main-row',
      ),
      1 => array(
        'type' => 'column',
        'width' => '33.34624848756747',
        'width_type' => '%',
        'parent' => 'canvas',
        'children' => array(
          0 => 2,
        ),
        'class' => '',
      ),
      2 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'right_sidebar',
        ),
        'parent' => '1',
        'class' => '',
      ),
      'right_sidebar' => array(
        'type' => 'region',
        'title' => 'Right Sidebar',
        'width' => 100,
        'width_type' => '%',
        'parent' => '2',
        'class' => '',
      ),
    ),
  );
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'left' => NULL,
      'right' => NULL,
      'center' => NULL,
      'right_sidebar' => NULL,
      'preface' => NULL,
      'content' => NULL,
      'footer' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '%status Sessions';
  $display->uuid = '0eeda185-558c-46be-bee1-c6c1962556c5';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-26148802-058c-4076-8ebd-050fa51886d2';
  $pane->panel = 'content';
  $pane->type = 'views_panes';
  $pane->subtype = 'cod_session_sessions-panel_sessions_by_track';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'arguments' => array(
      'gid' => '%node:nid',
      'field_accepted_value' => '%status',
      'machine_name' => '%term_machine_name:html_safe',
    ),
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array(
    'css_id' => '',
    'css_class' => '%term_machine_name:html_safe',
  );
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '26148802-058c-4076-8ebd-050fa51886d2';
  $display->content['new-26148802-058c-4076-8ebd-050fa51886d2'] = $pane;
  $display->panels['content'][0] = 'new-26148802-058c-4076-8ebd-050fa51886d2';
  $pane = new stdClass();
  $pane->pid = 'new-84112ab3-bc13-4d12-92a9-248215cf1829';
  $pane->panel = 'preface';
  $pane->type = 'page_title';
  $pane->subtype = 'page_title';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'markup' => 'h1',
    'class' => '',
    'id' => '',
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
  $pane->uuid = '84112ab3-bc13-4d12-92a9-248215cf1829';
  $display->content['new-84112ab3-bc13-4d12-92a9-248215cf1829'] = $pane;
  $display->panels['preface'][0] = 'new-84112ab3-bc13-4d12-92a9-248215cf1829';
  $pane = new stdClass();
  $pane->pid = 'new-7951585b-3121-4440-a3b9-deea160bc05a';
  $pane->panel = 'preface';
  $pane->type = 'views_panes';
  $pane->subtype = 'cod_session_tracks-track_session_filter';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'arguments' => array(
      'machine_name' => 'session_tracks_%node:nid',
      'null' => '%node:nid',
      'null_1' => '%term_machine_name:html_safe',
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
  $pane->uuid = '7951585b-3121-4440-a3b9-deea160bc05a';
  $display->content['new-7951585b-3121-4440-a3b9-deea160bc05a'] = $pane;
  $display->panels['preface'][1] = 'new-7951585b-3121-4440-a3b9-deea160bc05a';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_sessions_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'sessions';
  $handler->handler = 'panel_context';
  $handler->weight = -29;
  $handler->conf = array(
    'title' => 'Sessions Summary',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
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
          'context' => 'argument_entity_id:node_1',
          'not' => FALSE,
        ),
        1 => array(
          'name' => 'context_exists',
          'settings' => array(
            'exists' => '1',
          ),
          'context' => 'argument_cod_session_status_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'cygnus';
  $display->layout_settings = array(
    'items' => array(
      'canvas' => array(
        'type' => 'row',
        'contains' => 'column',
        'children' => array(
          0 => 'main',
          1 => 1,
        ),
        'parent' => NULL,
      ),
      'main' => array(
        'type' => 'column',
        'width' => '66.65375151243254',
        'width_type' => '%',
        'children' => array(
          0 => 'main-row',
        ),
        'parent' => 'canvas',
      ),
      'main-row' => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'center',
        ),
        'parent' => 'main',
      ),
      'center' => array(
        'type' => 'region',
        'title' => 'Center',
        'width' => 100,
        'width_type' => '%',
        'parent' => 'main-row',
      ),
      1 => array(
        'type' => 'column',
        'width' => '33.34624848756747',
        'width_type' => '%',
        'parent' => 'canvas',
        'children' => array(
          0 => 2,
        ),
        'class' => '',
      ),
      2 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'right_sidebar',
        ),
        'parent' => '1',
        'class' => '',
      ),
      'right_sidebar' => array(
        'type' => 'region',
        'title' => 'Right Sidebar',
        'width' => 100,
        'width_type' => '%',
        'parent' => '2',
        'class' => '',
      ),
    ),
  );
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'left' => NULL,
      'right' => NULL,
      'center' => NULL,
      'right_sidebar' => NULL,
      'preface' => NULL,
      'content' => NULL,
      'footer' => NULL,
    ),
    'style' => 'naked',
  );
  $display->cache = array();
  $display->title = '%status Sessions';
  $display->uuid = '0eeda185-558c-46be-bee1-c6c1962556c5';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-0dde2356-8b78-475b-adb8-53483333858e';
  $pane->panel = 'content';
  $pane->type = 'views_panes';
  $pane->subtype = 'cod_session_tracks-track_session_summary';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'arguments' => array(
      'machine_name' => 'session_tracks_%node:nid',
      'null' => '%status',
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
  $pane->uuid = '0dde2356-8b78-475b-adb8-53483333858e';
  $display->content['new-0dde2356-8b78-475b-adb8-53483333858e'] = $pane;
  $display->panels['content'][0] = 'new-0dde2356-8b78-475b-adb8-53483333858e';
  $pane = new stdClass();
  $pane->pid = 'new-560ee2b6-99df-4286-bb7b-82537b54f6c4';
  $pane->panel = 'preface';
  $pane->type = 'page_title';
  $pane->subtype = 'page_title';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'markup' => 'h1',
    'class' => '',
    'id' => '',
    'override_title' => 0,
    'override_title_text' => '%status Sessions',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '560ee2b6-99df-4286-bb7b-82537b54f6c4';
  $display->content['new-560ee2b6-99df-4286-bb7b-82537b54f6c4'] = $pane;
  $display->panels['preface'][0] = 'new-560ee2b6-99df-4286-bb7b-82537b54f6c4';
  $pane = new stdClass();
  $pane->pid = 'new-7951585b-3121-4440-a3b9-deea160bc05a';
  $pane->panel = 'preface';
  $pane->type = 'views_panes';
  $pane->subtype = 'cod_session_tracks-track_session_filter';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'arguments' => array(
      'machine_name' => 'session_tracks_%node:nid',
      'null' => '%node:nid',
      'null_1' => '',
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
  $pane->uuid = '7951585b-3121-4440-a3b9-deea160bc05a';
  $display->content['new-7951585b-3121-4440-a3b9-deea160bc05a'] = $pane;
  $display->panels['preface'][1] = 'new-7951585b-3121-4440-a3b9-deea160bc05a';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_sessions_http_response';
  $handler->task = 'page';
  $handler->subtask = 'sessions';
  $handler->handler = 'http_response';
  $handler->weight = -28;
  $handler->conf = array(
    'title' => 'HTTP response code',
    'contexts' => array(),
    'relationships' => array(),
    'code' => '301',
    'destination' => 'node/%node:nid/sessions/%node:field-event-default-session-view',
    'access' => array(
      'plugins' => array(
        1 => array(
          'name' => 'context_exists',
          'settings' => array(
            'exists' => '1',
          ),
          'context' => 'argument_entity_id:node_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $page->default_handlers[$handler->name] = $handler;
  $pages['sessions'] = $page;

  return $pages;

}
