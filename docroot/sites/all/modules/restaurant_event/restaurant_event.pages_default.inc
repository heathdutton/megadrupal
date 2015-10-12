<?php
/**
 * @file
 * restaurant_event.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function restaurant_event_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_edit_panel_context_4';
  $handler->task = 'node_edit';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = -29;
  $handler->conf = array(
    'title' => 'Event',
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
          'name' => 'node_type',
          'settings' => array(
            'type' => array(
              'event' => 'event',
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
  $display->layout = 'radix_burr_flipped';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'sidebar' => NULL,
      'contentmain' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Add/Edit Event';
  $display->uuid = '8fb4e3db-16a7-4512-a6fa-bbe1b1b389b3';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-ea094c4d-041e-4205-a039-ed02013ddfaf';
    $pane->panel = 'contentmain';
    $pane->type = 'form';
    $pane->subtype = 'form';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => 'argument_node_edit_1',
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
    $pane->uuid = 'ea094c4d-041e-4205-a039-ed02013ddfaf';
    $display->content['new-ea094c4d-041e-4205-a039-ed02013ddfaf'] = $pane;
    $display->panels['contentmain'][0] = 'new-ea094c4d-041e-4205-a039-ed02013ddfaf';
    $pane = new stdClass();
    $pane->pid = 'new-4d0e9d52-0750-40b5-a311-d7f44f20b1be';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_event_categories';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
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
    $pane->uuid = '4d0e9d52-0750-40b5-a311-d7f44f20b1be';
    $display->content['new-4d0e9d52-0750-40b5-a311-d7f44f20b1be'] = $pane;
    $display->panels['sidebar'][0] = 'new-4d0e9d52-0750-40b5-a311-d7f44f20b1be';
    $pane = new stdClass();
    $pane->pid = 'new-e64a2f08-89f1-4afd-ac45-a1573e3445a0';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_event_featured';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
      'override_title' => 0,
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
    $pane->uuid = 'e64a2f08-89f1-4afd-ac45-a1573e3445a0';
    $display->content['new-e64a2f08-89f1-4afd-ac45-a1573e3445a0'] = $pane;
    $display->panels['sidebar'][1] = 'new-e64a2f08-89f1-4afd-ac45-a1573e3445a0';
    $pane = new stdClass();
    $pane->pid = 'new-50bb954e-8938-4515-a283-38958d67d452';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_event_featured_image';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
      'override_title' => 0,
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
    $pane->uuid = '50bb954e-8938-4515-a283-38958d67d452';
    $display->content['new-50bb954e-8938-4515-a283-38958d67d452'] = $pane;
    $display->panels['sidebar'][2] = 'new-50bb954e-8938-4515-a283-38958d67d452';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-ea094c4d-041e-4205-a039-ed02013ddfaf';
  $handler->conf['display'] = $display;
  $export['node_edit_panel_context_4'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'pm_existing_pages_events_calendar_day_page_panel_context';
  $handler->task = 'pm_existing_pages';
  $handler->subtask = 'events_calendar_day_page';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Default',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'panels_breadcrumbs_state' => 1,
    'panels_breadcrumbs_titles' => 'Events
Calendar',
    'panels_breadcrumbs_paths' => 'events
events/calendar/month',
    'panels_breadcrumbs_home' => 1,
  );
  $display = new panels_display();
  $display->layout = 'radix_boxton';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'contentmain' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Events Calendar';
  $display->uuid = '2890f9b6-d529-4dcd-837d-4c78db3f29da';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-46c686bb-b9a9-45c3-9964-49a93520f03c';
    $pane->panel = 'contentmain';
    $pane->type = 'menu_tree';
    $pane->subtype = 'main-menu';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'menu_name' => 'main-menu',
      'parent_mlid' => 'main-menu:0',
      'title_link' => 0,
      'admin_title' => '',
      'level' => '2',
      'follow' => 0,
      'depth' => 0,
      'expanded' => 0,
      'sort' => 0,
      'override_title' => 1,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => array(
        'nav_type' => 'tabs',
        'nav_display' => 'default',
      ),
      'style' => 'nav',
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '46c686bb-b9a9-45c3-9964-49a93520f03c';
    $display->content['new-46c686bb-b9a9-45c3-9964-49a93520f03c'] = $pane;
    $display->panels['contentmain'][0] = 'new-46c686bb-b9a9-45c3-9964-49a93520f03c';
    $pane = new stdClass();
    $pane->pid = 'new-9ab86316-bdc6-4a23-98f7-43d61865d8bb';
    $pane->panel = 'contentmain';
    $pane->type = 'pm_existing_pages';
    $pane->subtype = 'pm_existing_pages';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      0 => 'task_id',
      'task_id' => 'events_calendar_day_page',
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
    $pane->uuid = '9ab86316-bdc6-4a23-98f7-43d61865d8bb';
    $display->content['new-9ab86316-bdc6-4a23-98f7-43d61865d8bb'] = $pane;
    $display->panels['contentmain'][1] = 'new-9ab86316-bdc6-4a23-98f7-43d61865d8bb';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-46c686bb-b9a9-45c3-9964-49a93520f03c';
  $handler->conf['display'] = $display;
  $export['pm_existing_pages_events_calendar_day_page_panel_context'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'pm_existing_pages_events_calendar_month_page_panel_context';
  $handler->task = 'pm_existing_pages';
  $handler->subtask = 'events_calendar_month_page';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Default',
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
  $display->layout = 'radix_boxton';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'contentmain' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Events Calendar';
  $display->uuid = 'e65a00cf-e7c5-472b-adb1-ada7f8aac919';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-da3b30f8-5f73-4b89-9403-063cfcfc920b';
    $pane->panel = 'contentmain';
    $pane->type = 'menu_tree';
    $pane->subtype = 'main-menu';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'menu_name' => 'main-menu',
      'parent_mlid' => 'main-menu:0',
      'title_link' => 0,
      'admin_title' => '',
      'level' => '2',
      'follow' => 0,
      'depth' => 0,
      'expanded' => 0,
      'sort' => 0,
      'override_title' => 1,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => array(
        'nav_type' => 'tabs',
        'nav_display' => 'default',
      ),
      'style' => 'nav',
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'da3b30f8-5f73-4b89-9403-063cfcfc920b';
    $display->content['new-da3b30f8-5f73-4b89-9403-063cfcfc920b'] = $pane;
    $display->panels['contentmain'][0] = 'new-da3b30f8-5f73-4b89-9403-063cfcfc920b';
    $pane = new stdClass();
    $pane->pid = 'new-748da449-6a13-4a20-bfe1-93bfcf4c0e68';
    $pane->panel = 'contentmain';
    $pane->type = 'pm_existing_pages';
    $pane->subtype = 'pm_existing_pages';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      0 => 'task_id',
      'task_id' => 'events_calendar_month_page',
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
    $pane->uuid = '748da449-6a13-4a20-bfe1-93bfcf4c0e68';
    $display->content['new-748da449-6a13-4a20-bfe1-93bfcf4c0e68'] = $pane;
    $display->panels['contentmain'][1] = 'new-748da449-6a13-4a20-bfe1-93bfcf4c0e68';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['pm_existing_pages_events_calendar_month_page_panel_context'] = $handler;

  return $export;
}

/**
 * Implements hook_default_page_manager_pages().
 */
function restaurant_event_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'events';
  $page->task = 'page';
  $page->admin_title = 'Events';
  $page->admin_description = '';
  $page->path = 'events';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'normal',
    'title' => 'Events',
    'name' => 'main-menu',
    'weight' => '5',
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
  $handler->name = 'page_events_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'events';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => 'events-page',
    'css_id' => 'events-page',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'radix_burr';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'sidebar' => NULL,
      'contentmain' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Upcoming Events';
  $display->uuid = 'b0a17d9d-6ef1-47c9-b829-50eef126af9a';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-3c1a3ac9-912d-4b31-b6c6-4ba29bbfd172';
    $pane->panel = 'contentmain';
    $pane->type = 'menu_tree';
    $pane->subtype = 'main-menu';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'menu_name' => 'main-menu',
      'parent_mlid' => 'main-menu:0',
      'title_link' => 0,
      'admin_title' => '',
      'level' => '2',
      'follow' => 0,
      'depth' => 0,
      'expanded' => 1,
      'sort' => 0,
      'override_title' => 1,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => array(
        'nav_type' => 'tabs',
        'nav_display' => 'default',
      ),
      'style' => 'nav',
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '3c1a3ac9-912d-4b31-b6c6-4ba29bbfd172';
    $display->content['new-3c1a3ac9-912d-4b31-b6c6-4ba29bbfd172'] = $pane;
    $display->panels['contentmain'][0] = 'new-3c1a3ac9-912d-4b31-b6c6-4ba29bbfd172';
    $pane = new stdClass();
    $pane->pid = 'new-99a4420f-144b-470e-aed8-1ddf1f40f574';
    $pane->panel = 'contentmain';
    $pane->type = 'block';
    $pane->subtype = 'views--exp-events-upcoming_events';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'inherit_path' => 1,
      'override_title' => 0,
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
    $pane->uuid = '99a4420f-144b-470e-aed8-1ddf1f40f574';
    $display->content['new-99a4420f-144b-470e-aed8-1ddf1f40f574'] = $pane;
    $display->panels['contentmain'][1] = 'new-99a4420f-144b-470e-aed8-1ddf1f40f574';
    $pane = new stdClass();
    $pane->pid = 'new-57fb3028-c367-4ea1-8da6-f7b5794a9c3b';
    $pane->panel = 'contentmain';
    $pane->type = 'views_panes';
    $pane->subtype = 'events-upcoming_events';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_mode' => 'teaser',
      'widget_title' => '',
      'items_per_page' => NULL,
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
      'view_settings' => 'nodes',
      'header_type' => 'none',
      'override_title' => '',
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
    $pane->uuid = '57fb3028-c367-4ea1-8da6-f7b5794a9c3b';
    $display->content['new-57fb3028-c367-4ea1-8da6-f7b5794a9c3b'] = $pane;
    $display->panels['contentmain'][2] = 'new-57fb3028-c367-4ea1-8da6-f7b5794a9c3b';
    $pane = new stdClass();
    $pane->pid = 'new-6913c7f3-8fc8-48ac-adb0-d77acbfac443';
    $pane->panel = 'sidebar';
    $pane->type = 'views_panes';
    $pane->subtype = 'events_calendar-events_calendar_widget';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_mode' => 'teaser',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '6913c7f3-8fc8-48ac-adb0-d77acbfac443';
    $display->content['new-6913c7f3-8fc8-48ac-adb0-d77acbfac443'] = $pane;
    $display->panels['sidebar'][0] = 'new-6913c7f3-8fc8-48ac-adb0-d77acbfac443';
    $pane = new stdClass();
    $pane->pid = 'new-06c442ad-d4f5-454e-bd01-a7b2f6540f2c';
    $pane->panel = 'sidebar';
    $pane->type = 'views_panes';
    $pane->subtype = 'event_categories-event_categories';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_settings' => 'fields',
      'header_type' => 'none',
      'view_mode' => 'teaser',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '06c442ad-d4f5-454e-bd01-a7b2f6540f2c';
    $display->content['new-06c442ad-d4f5-454e-bd01-a7b2f6540f2c'] = $pane;
    $display->panels['sidebar'][1] = 'new-06c442ad-d4f5-454e-bd01-a7b2f6540f2c';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-57fb3028-c367-4ea1-8da6-f7b5794a9c3b';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['events'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'past_events';
  $page->task = 'page';
  $page->admin_title = 'Past Events';
  $page->admin_description = '';
  $page->path = 'events/past';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'normal',
    'title' => 'Past Events',
    'name' => 'main-menu',
    'weight' => '5',
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
  $handler->name = 'page_past_events_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'past_events';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => 'events-page',
    'css_id' => 'events-page',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'panels_breadcrumbs_state' => 1,
    'panels_breadcrumbs_titles' => 'Events
Past Events',
    'panels_breadcrumbs_paths' => 'events',
    'panels_breadcrumbs_home' => 1,
  );
  $display = new panels_display();
  $display->layout = 'radix_burr';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'sidebar' => NULL,
      'contentmain' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Past Events';
  $display->uuid = 'ea8ee5c8-fc49-4112-bc5b-39a5b4310e17';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-be329ca2-bc02-4338-b891-eeca451555a4';
    $pane->panel = 'contentmain';
    $pane->type = 'menu_tree';
    $pane->subtype = 'main-menu';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'menu_name' => 'main-menu',
      'parent_mlid' => 'main-menu:0',
      'title_link' => 0,
      'admin_title' => '',
      'level' => '2',
      'follow' => 0,
      'depth' => 0,
      'expanded' => 1,
      'sort' => 0,
      'override_title' => 1,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => array(
        'nav_type' => 'tabs',
        'nav_display' => 'default',
      ),
      'style' => 'nav',
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'be329ca2-bc02-4338-b891-eeca451555a4';
    $display->content['new-be329ca2-bc02-4338-b891-eeca451555a4'] = $pane;
    $display->panels['contentmain'][0] = 'new-be329ca2-bc02-4338-b891-eeca451555a4';
    $pane = new stdClass();
    $pane->pid = 'new-1ba288b9-424c-497f-94cc-5f765b531bb1';
    $pane->panel = 'contentmain';
    $pane->type = 'views_panes';
    $pane->subtype = 'events-past_events';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_settings' => 'nodes',
      'header_type' => 'none',
      'view_mode' => 'teaser',
      'override_title' => '',
      'override_title_text' => '',
      'widget_title' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '1ba288b9-424c-497f-94cc-5f765b531bb1';
    $display->content['new-1ba288b9-424c-497f-94cc-5f765b531bb1'] = $pane;
    $display->panels['contentmain'][1] = 'new-1ba288b9-424c-497f-94cc-5f765b531bb1';
    $pane = new stdClass();
    $pane->pid = 'new-2a526e4e-b831-4cbb-be74-c9b00d261665';
    $pane->panel = 'sidebar';
    $pane->type = 'views_panes';
    $pane->subtype = 'events_calendar-events_calendar_widget';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_mode' => 'teaser',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '2a526e4e-b831-4cbb-be74-c9b00d261665';
    $display->content['new-2a526e4e-b831-4cbb-be74-c9b00d261665'] = $pane;
    $display->panels['sidebar'][0] = 'new-2a526e4e-b831-4cbb-be74-c9b00d261665';
    $pane = new stdClass();
    $pane->pid = 'new-1791e49e-af2d-49c0-be9b-7b69bdea04ad';
    $pane->panel = 'sidebar';
    $pane->type = 'views_panes';
    $pane->subtype = 'event_categories-event_categories';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_settings' => 'fields',
      'header_type' => 'none',
      'view_mode' => 'teaser',
    );
    $pane->cache = array();
    $pane->style = array();
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '1791e49e-af2d-49c0-be9b-7b69bdea04ad';
    $display->content['new-1791e49e-af2d-49c0-be9b-7b69bdea04ad'] = $pane;
    $display->panels['sidebar'][1] = 'new-1791e49e-af2d-49c0-be9b-7b69bdea04ad';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['past_events'] = $page;

  return $pages;

}
