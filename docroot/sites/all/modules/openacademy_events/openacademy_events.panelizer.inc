<?php
/**
 * @file
 * openacademy_events.panelizer.inc
 */

/**
 * Implements hook_panelizer_defaults().
 */
function openacademy_events_panelizer_defaults() {
  $export = array();

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:openacademy_event:default';
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'openacademy_event';
  $panelizer->no_blocks = FALSE;
  $panelizer->css_id = 'events-page';
  $panelizer->css = '';
  $panelizer->pipeline = 'ipe';
  $panelizer->contexts = array();
  $panelizer->relationships = array();
  $panelizer->access = '';
  $panelizer->view_mode = '';
  $panelizer->css_class = '';
  $panelizer->title_element = 'H2';
  $panelizer->link_to_entity = TRUE;
  $panelizer->extra = '';
  $display = new panels_display();
  $display->layout = 'burr';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'sidebar' => NULL,
      'contentmain' => NULL,
    ),
    'sidebar' => array(
      'style' => 'background_callout',
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'c8a7f109-bbfb-5664-99a8-574cf6597203';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-68146bee-e1e5-8f04-c152-51f4d9f6e43e';
    $pane->panel = 'contentmain';
    $pane->type = 'node_content';
    $pane->subtype = 'node_content';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'links' => 1,
      'no_extras' => 1,
      'override_title' => 1,
      'override_title_text' => '',
      'identifier' => '',
      'link' => 0,
      'leave_node_title' => 0,
      'build_mode' => 'full',
      'context' => array(),
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '68146bee-e1e5-8f04-c152-51f4d9f6e43e';
    $display->content['new-68146bee-e1e5-8f04-c152-51f4d9f6e43e'] = $pane;
    $display->panels['contentmain'][0] = 'new-68146bee-e1e5-8f04-c152-51f4d9f6e43e';
    $pane = new stdClass();
    $pane->pid = 'new-ce95942f-a4ce-33f4-01f9-200e9af04810';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_event_date';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'date_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'format_type' => 'panopoly_day',
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'fromto' => 'both',
      ),
      'context' => 'panelizer',
      'override_title' => 1,
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
    $pane->uuid = 'ce95942f-a4ce-33f4-01f9-200e9af04810';
    $display->content['new-ce95942f-a4ce-33f4-01f9-200e9af04810'] = $pane;
    $display->panels['sidebar'][0] = 'new-ce95942f-a4ce-33f4-01f9-200e9af04810';
    $pane = new stdClass();
    $pane->pid = 'new-04d5f093-7583-1ca4-e17c-2b688d4b70a5';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_event_date';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'date_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'format_type' => 'panopoly_time',
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'fromto' => 'both',
      ),
      'context' => 'panelizer',
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
    $pane->uuid = '04d5f093-7583-1ca4-e17c-2b688d4b70a5';
    $display->content['new-04d5f093-7583-1ca4-e17c-2b688d4b70a5'] = $pane;
    $display->panels['sidebar'][1] = 'new-04d5f093-7583-1ca4-e17c-2b688d4b70a5';
    $pane = new stdClass();
    $pane->pid = 'new-5e66c167-c32b-6654-853d-b387c97b87ff';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_event_location';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'text_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
      'context' => 'panelizer',
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
    $pane->uuid = '5e66c167-c32b-6654-853d-b387c97b87ff';
    $display->content['new-5e66c167-c32b-6654-853d-b387c97b87ff'] = $pane;
    $display->panels['sidebar'][2] = 'new-5e66c167-c32b-6654-853d-b387c97b87ff';
    $pane = new stdClass();
    $pane->pid = 'new-d52545d1-a655-f8d4-a55a-c73fa16778bc';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_event_address';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'simple_gmap',
      'delta_limit' => '0',
      'delta_offset' => '0',
      'delta_reversed' => 0,
      'formatter_settings' => array(
        'include_map' => 1,
        'include_link' => 1,
        'include_text' => 0,
        'iframe_height' => 200,
        'iframe_width' => 320,
        'zoom_level' => 15,
        'information_bubble' => 0,
        'link_text' => 'Get Directions',
      ),
      'context' => 'panelizer',
      'override_title' => 1,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = 'd52545d1-a655-f8d4-a55a-c73fa16778bc';
    $display->content['new-d52545d1-a655-f8d4-a55a-c73fa16778bc'] = $pane;
    $display->panels['sidebar'][3] = 'new-d52545d1-a655-f8d4-a55a-c73fa16778bc';
    $pane = new stdClass();
    $pane->pid = 'new-34d99894-dd64-4834-856a-d52e6b4c5f2a';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_event_moreinfo';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
      'formatter' => 'link_default',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = '34d99894-dd64-4834-856a-d52e6b4c5f2a';
    $display->content['new-34d99894-dd64-4834-856a-d52e6b4c5f2a'] = $pane;
    $display->panels['sidebar'][4] = 'new-34d99894-dd64-4834-856a-d52e6b4c5f2a';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $panelizer->display = $display;
  $export['node:openacademy_event:default'] = $panelizer;

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'taxonomy_term:openacademy_event_type:default';
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'taxonomy_term';
  $panelizer->panelizer_key = 'openacademy_event_type';
  $panelizer->no_blocks = FALSE;
  $panelizer->css_id = '';
  $panelizer->css = '';
  $panelizer->pipeline = 'ipe';
  $panelizer->contexts = array();
  $panelizer->relationships = array();
  $panelizer->access = '';
  $panelizer->view_mode = '';
  $panelizer->css_class = '';
  $panelizer->title_element = 'H2';
  $panelizer->link_to_entity = TRUE;
  $panelizer->extra = '';
  $display = new panels_display();
  $display->layout = 'burr';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'sidebar' => NULL,
      'default' => NULL,
      'contentmain' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '%term:name';
  $display->uuid = '71cf48cc-1337-9dd4-2da9-0fc5e1fab7db';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-4d6e1c4d-bddf-7924-c912-237a8fc9c417';
    $pane->panel = 'contentmain';
    $pane->type = 'views_panes';
    $pane->subtype = 'openacademy_events-all_events';
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
    $pane->uuid = '4d6e1c4d-bddf-7924-c912-237a8fc9c417';
    $display->content['new-4d6e1c4d-bddf-7924-c912-237a8fc9c417'] = $pane;
    $display->panels['contentmain'][0] = 'new-4d6e1c4d-bddf-7924-c912-237a8fc9c417';
    $pane = new stdClass();
    $pane->pid = 'new-d61412f2-cb54-3ec4-f9a3-8950c20d754b';
    $pane->panel = 'sidebar';
    $pane->type = 'views_panes';
    $pane->subtype = 'openacademy_events-monthly_mini_calendar';
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
    $pane->uuid = 'd61412f2-cb54-3ec4-f9a3-8950c20d754b';
    $display->content['new-d61412f2-cb54-3ec4-f9a3-8950c20d754b'] = $pane;
    $display->panels['sidebar'][0] = 'new-d61412f2-cb54-3ec4-f9a3-8950c20d754b';
    $pane = new stdClass();
    $pane->pid = 'new-0b343d0d-2242-77b4-0d4b-b4921c5d98bb';
    $pane->panel = 'sidebar';
    $pane->type = 'menu_tree';
    $pane->subtype = 'main-menu';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'menu_name' => 'main-menu',
      'parent_mlid' => 'main-menu:369',
      'title_link' => 0,
      'admin_title' => 'Event Types',
      'level' => '2',
      'follow' => 0,
      'depth' => 0,
      'expanded' => 0,
      'sort' => 0,
      'override_title' => 1,
      'override_title_text' => 'Event Types',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '0b343d0d-2242-77b4-0d4b-b4921c5d98bb';
    $display->content['new-0b343d0d-2242-77b4-0d4b-b4921c5d98bb'] = $pane;
    $display->panels['sidebar'][1] = 'new-0b343d0d-2242-77b4-0d4b-b4921c5d98bb';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-d61412f2-cb54-3ec4-f9a3-8950c20d754b';
  $panelizer->display = $display;
  $export['taxonomy_term:openacademy_event_type:default'] = $panelizer;

  return $export;
}
