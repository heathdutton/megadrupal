<?php
/**
 * @file
 * wetkit_deployment_source.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function wetkit_deployment_source_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'wetkit_deployment_source';
  $page->task = 'page';
  $page->admin_title = 'WetKit Deployment';
  $page->admin_description = 'Deployment Administrative Screen';
  $page->path = 'admin/dashboard/deployment';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'administer deployments',
        ),
        'context' => 'logged-in-user',
        'not' => FALSE,
      ),
    ),
    'logic' => 'and',
  );
  $page->menu = array(
    'type' => 'tab',
    'title' => 'Deployment',
    'name' => 'navigation',
    'weight' => '-1',
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
  $handler->name = 'page_wetkit_deployment_source_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'wetkit_deployment_source';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Deployment Staging Area',
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
  $display->layout = 'boxton';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'contentmain' => NULL,
      'header' => NULL,
      'sidebar' => NULL,
      'footer' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Deployment';
  $display->uuid = '1dd0ab5c-44b0-4db4-257a-8e42c5430520';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-b9044f55-b545-448d-bfea-68b21eb33b98';
    $pane->panel = 'contentmain';
    $pane->type = 'page_title';
    $pane->subtype = 'page_title';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'markup' => 'h1',
      'class' => '',
      'id' => 'wb-cont',
      'override_title' => 0,
      'override_title_text' => '',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array();
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'b9044f55-b545-448d-bfea-68b21eb33b98';
    $display->content['new-b9044f55-b545-448d-bfea-68b21eb33b98'] = $pane;
    $display->panels['contentmain'][0] = 'new-b9044f55-b545-448d-bfea-68b21eb33b98';
    $pane = new stdClass();
    $pane->pid = 'new-a107804d-565f-7de4-65bd-c3f17d14fedf';
    $pane->panel = 'contentmain';
    $pane->type = 'queue';
    $pane->subtype = 'queue';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'a107804d-565f-7de4-65bd-c3f17d14fedf';
    $display->content['new-a107804d-565f-7de4-65bd-c3f17d14fedf'] = $pane;
    $display->panels['contentmain'][1] = 'new-a107804d-565f-7de4-65bd-c3f17d14fedf';
    $pane = new stdClass();
    $pane->pid = 'new-b147ce67-10f1-6b84-8131-ae1b5ce05e56';
    $pane->panel = 'contentmain';
    $pane->type = 'block';
    $pane->subtype = 'quicktabs-deployment_quicktabs';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'b147ce67-10f1-6b84-8131-ae1b5ce05e56';
    $display->content['new-b147ce67-10f1-6b84-8131-ae1b5ce05e56'] = $pane;
    $display->panels['contentmain'][2] = 'new-b147ce67-10f1-6b84-8131-ae1b5ce05e56';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['wetkit_deployment_source'] = $page;

  return $pages;

}
