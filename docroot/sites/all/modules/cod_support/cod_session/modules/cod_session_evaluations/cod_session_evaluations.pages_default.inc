<?php
/**
 * @file
 * cod_session_evaluations.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function cod_session_evaluations_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cod_session_evaluations';
  $page->task = 'page';
  $page->admin_title = 'cod_session_evaluations';
  $page->admin_description = '';
  $page->path = 'node/%node/evaluations';
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
      2 => array(
        'name' => 'user_is_speaker',
        'settings' => NULL,
        'not' => FALSE,
        'context' => 'argument_entity_id:node_1',
      ),
    ),
    'logic' => 'and',
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'tab',
    'title' => 'Evaluations',
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
      'identifier' => 'Session ID',
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
  $handler->name = 'page_cod_session_evaluations__panel_context_59dc1494-6be2-47b7-b11d-8f1727a46661';
  $handler->task = 'page';
  $handler->subtask = 'cod_session_evaluations';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Evaluations Panel',
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
      'plugins' => array(),
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
  $display->uuid = '72b39795-e9c1-4a9b-af84-580a0459abe3';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-b03fad1e-3753-4de9-8ebb-6028dc15a236';
    $pane->panel = 'content';
    $pane->type = 'views_panes';
    $pane->subtype = 'sessions_eval_report-cod_session_evaluations_pane';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'arguments' => array(
        'nid' => '%node:nid',
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
    $pane->uuid = 'b03fad1e-3753-4de9-8ebb-6028dc15a236';
    $display->content['new-b03fad1e-3753-4de9-8ebb-6028dc15a236'] = $pane;
    $display->panels['content'][0] = 'new-b03fad1e-3753-4de9-8ebb-6028dc15a236';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-b03fad1e-3753-4de9-8ebb-6028dc15a236';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page-cod_session_evaluations__access_denied';
  $handler->task = 'page';
  $handler->subtask = 'cod_session_evaluations';
  $handler->handler = 'http_response';
  $handler->weight = 1;
  $handler->conf = array(
    'title' => 'Access Denied',
    'contexts' => array(),
    'relationships' => array(),
    'code' => '403',
    'destination' => '',
    'name' => 'access_denied',
  );
  $page->default_handlers[$handler->name] = $handler;
  $pages['cod_session_evaluations'] = $page;

  return $pages;

}
