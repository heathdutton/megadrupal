<?php
/**
 * @file
 * openacademy_core.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function openacademy_core_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'openacademy_front';
  $page->task = 'page';
  $page->admin_title = 'Front Page';
  $page->admin_description = 'This is the front page of the site used to kick things off on a good foot!';
  $page->path = 'front';
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
  $page->menu = array();
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_openacademy_front_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'openacademy_front';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Front Page',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'css_id' => 'front-panel',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'brown';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'top' => NULL,
      'left' => NULL,
      'right' => NULL,
      'bottom' => NULL,
      'slider' => NULL,
      'slidergutter' => NULL,
      'column1' => NULL,
      'column2' => NULL,
      'column3' => NULL,
      'footercolumn1' => NULL,
      'footercolumn2' => NULL,
      'footercolumn3' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '95f5dd59-6d4e-fd74-f154-8f8a8e6c08a9';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-5533b8ed-b577-67d4-21dc-23b674713699';
    $pane->panel = 'column1';
    $pane->type = 'views_panes';
    $pane->subtype = 'openacademy_news-news_listing';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'items_per_page' => '2',
      'fields_override' => array(
        'field_featured_image' => 1,
        'title' => 1,
        'created' => 1,
        'body' => 1,
        'field_tags' => 0,
      ),
      'override_title' => 0,
      'override_title_text' => '',
      'view_mode' => NULL,
      'widget_title' => 'Recent News',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '5533b8ed-b577-67d4-21dc-23b674713699';
    $display->content['new-5533b8ed-b577-67d4-21dc-23b674713699'] = $pane;
    $display->panels['column1'][0] = 'new-5533b8ed-b577-67d4-21dc-23b674713699';
    $pane = new stdClass();
    $pane->pid = 'new-23c58be3-e73a-ea24-a9e1-b5e7400f2518';
    $pane->panel = 'column2';
    $pane->type = 'views_panes';
    $pane->subtype = 'openacademy_events-upcoming_events';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'items_per_page' => '3',
      'fields_override' => array(
        'field_featured_image' => 1,
        'title' => 1,
        'field_event_date' => 0,
        'field_event_date_1' => 0,
        'field_event_date_2' => 1,
        'body' => 0,
      ),
      'override_title' => 0,
      'override_title_text' => '',
      'view_mode' => 'teaser',
      'widget_title' => NULL,
      'exposed' => array(
        'sort_by' => NULL,
        'sort_order' => NULL,
      ),
      'use_pager' => NULL,
      'pager_id' => NULL,
      'offset' => NULL,
      'link_to_view' => NULL,
      'more_link' => NULL,
      'path' => NULL,
      'view_settings' => 'fields',
      'header_type' => 'none',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '23c58be3-e73a-ea24-a9e1-b5e7400f2518';
    $display->content['new-23c58be3-e73a-ea24-a9e1-b5e7400f2518'] = $pane;
    $display->panels['column2'][0] = 'new-23c58be3-e73a-ea24-a9e1-b5e7400f2518';
    $pane = new stdClass();
    $pane->pid = 'new-4ee490d4-e5d0-8224-d960-56b021559d4d';
    $pane->panel = 'column3';
    $pane->type = 'views_panes';
    $pane->subtype = 'openacademy_publications-publication_listing';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'items_per_page' => '2',
      'fields_override' => array(
        'field_featured_image' => 1,
        'title' => 1,
        'body' => 1,
      ),
      'view_mode' => NULL,
      'widget_title' => 'Recent Publications',
    );
    $pane->cache = array();
    $pane->style = array();
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '4ee490d4-e5d0-8224-d960-56b021559d4d';
    $display->content['new-4ee490d4-e5d0-8224-d960-56b021559d4d'] = $pane;
    $display->panels['column3'][0] = 'new-4ee490d4-e5d0-8224-d960-56b021559d4d';
    $pane = new stdClass();
    $pane->pid = 'new-58b3618d-d2d2-84f4-cd66-3cad276c8c8c';
    $pane->panel = 'slider';
    $pane->type = 'fieldable_panels_pane';
    $pane->subtype = 'uuid:3cf85237-de5c-35b4-095e-64447e9ff6bf';
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
    $pane->uuid = '58b3618d-d2d2-84f4-cd66-3cad276c8c8c';
    $display->content['new-58b3618d-d2d2-84f4-cd66-3cad276c8c8c'] = $pane;
    $display->panels['slider'][0] = 'new-58b3618d-d2d2-84f4-cd66-3cad276c8c8c';
    $pane = new stdClass();
    $pane->pid = 'new-95594421-56f5-e984-15e9-f11c9430fe5f';
    $pane->panel = 'slidergutter';
    $pane->type = 'fieldable_panels_pane';
    $pane->subtype = 'uuid:e663c0b4-8de1-1e54-2133-3de6a740bb8c';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_mode' => 'full',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
      'style' => 'background_callout',
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '95594421-56f5-e984-15e9-f11c9430fe5f';
    $display->content['new-95594421-56f5-e984-15e9-f11c9430fe5f'] = $pane;
    $display->panels['slidergutter'][0] = 'new-95594421-56f5-e984-15e9-f11c9430fe5f';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['openacademy_front'] = $page;

  return $pages;

}
