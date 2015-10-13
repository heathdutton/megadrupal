<?php
/**
 * @file
 * openacademy_news.panelizer.inc
 */

/**
 * Implements hook_panelizer_defaults().
 */
function openacademy_news_panelizer_defaults() {
  $export = array();

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:openacademy_news:default';
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'openacademy_news';
  $panelizer->no_blocks = FALSE;
  $panelizer->css_id = 'news-page';
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
  $display->layout = 'burr_flipped';
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
  $display->uuid = '955f12f8-40f8-0754-550e-03a42958f1ba';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-30d8863c-967f-5f44-cda1-dddd1cf817d0';
    $pane->panel = 'contentmain';
    $pane->type = 'node_title';
    $pane->subtype = 'node_title';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'link' => 0,
      'context' => array(),
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
    $pane->uuid = '30d8863c-967f-5f44-cda1-dddd1cf817d0';
    $display->content['new-30d8863c-967f-5f44-cda1-dddd1cf817d0'] = $pane;
    $display->panels['contentmain'][0] = 'new-30d8863c-967f-5f44-cda1-dddd1cf817d0';
    $pane = new stdClass();
    $pane->pid = 'new-2c1764da-95f4-e244-4133-162619173214';
    $pane->panel = 'contentmain';
    $pane->type = 'node_created';
    $pane->subtype = 'node_created';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'format' => 'small',
      'context' => array(),
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
    $pane->uuid = '2c1764da-95f4-e244-4133-162619173214';
    $display->content['new-2c1764da-95f4-e244-4133-162619173214'] = $pane;
    $display->panels['contentmain'][1] = 'new-2c1764da-95f4-e244-4133-162619173214';
    $pane = new stdClass();
    $pane->pid = 'new-6472bf36-d440-6154-dd94-5e8da60366bf';
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
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '6472bf36-d440-6154-dd94-5e8da60366bf';
    $display->content['new-6472bf36-d440-6154-dd94-5e8da60366bf'] = $pane;
    $display->panels['contentmain'][2] = 'new-6472bf36-d440-6154-dd94-5e8da60366bf';
    $pane = new stdClass();
    $pane->pid = 'new-6a8c696d-e85b-0b74-1d1c-b389e68213c0';
    $pane->panel = 'sidebar';
    $pane->type = 'views_panes';
    $pane->subtype = 'openacademy_news-news_listing';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'items_per_page' => '3',
      'fields_override' => array(
        'title' => 1,
        'field_featured_image' => 1,
        'body' => 1,
        'field_tags' => 0,
      ),
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
    $pane->uuid = '6a8c696d-e85b-0b74-1d1c-b389e68213c0';
    $display->content['new-6a8c696d-e85b-0b74-1d1c-b389e68213c0'] = $pane;
    $display->panels['sidebar'][0] = 'new-6a8c696d-e85b-0b74-1d1c-b389e68213c0';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $panelizer->display = $display;
  $export['node:openacademy_news:default'] = $panelizer;

  return $export;
}
