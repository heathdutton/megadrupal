<?php
/**
 * @file
 * openacademy_publications.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function openacademy_publications_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_edit_panel_context_2';
  $handler->task = 'node_edit';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = -10;
  $handler->conf = array(
    'title' => 'Biblio Edit Page',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'css_id' => 'biblio-edit',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'node_type',
          'settings' => array(
            'type' => array(
              'biblio' => 'biblio',
            ),
          ),
          'context' => 'argument_node_edit_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'boxton';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'contentmain' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '47408196-a9a0-7104-a9bb-6525aa830fef';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-aada347f-e6eb-a704-0d4e-95ed15c85f10';
    $pane->panel = 'contentmain';
    $pane->type = 'form';
    $pane->subtype = 'form';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => array(),
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
    $pane->uuid = 'aada347f-e6eb-a704-0d4e-95ed15c85f10';
    $display->content['new-aada347f-e6eb-a704-0d4e-95ed15c85f10'] = $pane;
    $display->panels['contentmain'][0] = 'new-aada347f-e6eb-a704-0d4e-95ed15c85f10';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-aada347f-e6eb-a704-0d4e-95ed15c85f10';
  $handler->conf['display'] = $display;
  $export['node_edit_panel_context_2'] = $handler;

  return $export;
}

/**
 * Implements hook_default_page_manager_pages().
 */
function openacademy_publications_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'openacademy_publications';
  $page->task = 'page';
  $page->admin_title = 'Publications';
  $page->admin_description = '';
  $page->path = 'publications';
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
    'title' => 'Publications',
    'name' => 'main-menu',
    'weight' => '2',
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
  $handler->name = 'page_openacademy_publications_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'openacademy_publications';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Events',
    'no_blocks' => 1,
    'pipeline' => 'ipe',
    'css_id' => 'events-panel',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'burr_flipped';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'left' => NULL,
      'right' => NULL,
      'sidebar' => NULL,
      'contentmain' => NULL,
    ),
    'sidebar' => array(
      'style' => 'background_callout',
    ),
  );
  $display->cache = array();
  $display->title = 'Publications';
  $display->uuid = 'c29ca1af-e965-1874-39ca-711251dffb3e';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-f3f7b2f8-8c96-04c4-1ddf-cbcbbe9150e2';
    $pane->panel = 'contentmain';
    $pane->type = 'views_panes';
    $pane->subtype = 'openacademy_publications-publication_all';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'items_per_page' => '7',
      'view_mode' => 'teaser',
      'widget_title' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'f3f7b2f8-8c96-04c4-1ddf-cbcbbe9150e2';
    $display->content['new-f3f7b2f8-8c96-04c4-1ddf-cbcbbe9150e2'] = $pane;
    $display->panels['contentmain'][0] = 'new-f3f7b2f8-8c96-04c4-1ddf-cbcbbe9150e2';
    $pane = new stdClass();
    $pane->pid = 'new-a0c999f3-44af-4a94-f943-d647c2ed4605';
    $pane->panel = 'sidebar';
    $pane->type = 'views_panes';
    $pane->subtype = 'openacademy_news-news_listing';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'items_per_page' => '3',
      'fields_override' => array(
        'title' => 1,
        'field_news_image' => 1,
        'body' => 1,
      ),
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
    $pane->uuid = 'a0c999f3-44af-4a94-f943-d647c2ed4605';
    $display->content['new-a0c999f3-44af-4a94-f943-d647c2ed4605'] = $pane;
    $display->panels['sidebar'][0] = 'new-a0c999f3-44af-4a94-f943-d647c2ed4605';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['openacademy_publications'] = $page;

  return $pages;

}
