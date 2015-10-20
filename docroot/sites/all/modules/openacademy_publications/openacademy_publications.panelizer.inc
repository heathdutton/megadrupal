<?php
/**
 * @file
 * openacademy_publications.panelizer.inc
 */

/**
 * Implements hook_panelizer_defaults().
 */
function openacademy_publications_panelizer_defaults() {
  $export = array();

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:openacademy_publication:default';
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'openacademy_publication';
  $panelizer->no_blocks = FALSE;
  $panelizer->css_id = 'publication-page';
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
  $display->layout = 'whelan';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'column1' => NULL,
      'column2' => NULL,
      'contentmain' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '4bbf7419-5f80-aff4-4d7c-6d6fa3b445da';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-2f564a7d-59c8-e6b4-d912-c16200901f52';
    $pane->panel = 'column1';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_featured_image';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'image',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'image_style' => 'panopoly_image_full',
        'image_link' => '',
      ),
      'context' => 'panelizer',
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
    $pane->uuid = '2f564a7d-59c8-e6b4-d912-c16200901f52';
    $display->content['new-2f564a7d-59c8-e6b4-d912-c16200901f52'] = $pane;
    $display->panels['column1'][0] = 'new-2f564a7d-59c8-e6b4-d912-c16200901f52';
    $pane = new stdClass();
    $pane->pid = 'new-1048a573-a96d-2424-8dbf-fd8e879c1e3b';
    $pane->panel = 'column1';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_tags';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'inline',
      'formatter' => 'taxonomy_term_reference_link',
      'delta_limit' => '0',
      'delta_offset' => '0',
      'delta_reversed' => 0,
      'formatter_settings' => array(),
      'context' => 'panelizer',
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
    $pane->uuid = '1048a573-a96d-2424-8dbf-fd8e879c1e3b';
    $display->content['new-1048a573-a96d-2424-8dbf-fd8e879c1e3b'] = $pane;
    $display->panels['column1'][1] = 'new-1048a573-a96d-2424-8dbf-fd8e879c1e3b';
    $pane = new stdClass();
    $pane->pid = 'new-82fbabca-99f8-d894-e1b8-91cfc9d7cdc4';
    $pane->panel = 'column1';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_publication_action';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'link_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
      'context' => 'panelizer',
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
    $pane->uuid = '82fbabca-99f8-d894-e1b8-91cfc9d7cdc4';
    $display->content['new-82fbabca-99f8-d894-e1b8-91cfc9d7cdc4'] = $pane;
    $display->panels['column1'][2] = 'new-82fbabca-99f8-d894-e1b8-91cfc9d7cdc4';
    $pane = new stdClass();
    $pane->pid = 'new-615a1c61-fd1e-8d94-f5c6-39a449bb884c';
    $pane->panel = 'column2';
    $pane->type = 'views_panes';
    $pane->subtype = 'openacademy_publications-publication_listing';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'items_per_page' => '3',
      'fields_override' => array(
        'field_featured_image' => 1,
        'title' => 1,
        'body' => 1,
      ),
      'override_title' => 0,
      'override_title_text' => '',
      'view_mode' => NULL,
      'widget_title' => 'Publications',
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
    $pane->uuid = '615a1c61-fd1e-8d94-f5c6-39a449bb884c';
    $display->content['new-615a1c61-fd1e-8d94-f5c6-39a449bb884c'] = $pane;
    $display->panels['column2'][0] = 'new-615a1c61-fd1e-8d94-f5c6-39a449bb884c';
    $pane = new stdClass();
    $pane->pid = 'new-0af16ced-33bf-5324-bd84-639c192c11ef';
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
    $pane->uuid = '0af16ced-33bf-5324-bd84-639c192c11ef';
    $display->content['new-0af16ced-33bf-5324-bd84-639c192c11ef'] = $pane;
    $display->panels['contentmain'][0] = 'new-0af16ced-33bf-5324-bd84-639c192c11ef';
    $pane = new stdClass();
    $pane->pid = 'new-ff39af94-b4c3-0134-81e0-6e67ae5b5d03';
    $pane->panel = 'contentmain';
    $pane->type = 'node_content';
    $pane->subtype = 'node_content';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'links' => 1,
      'no_extras' => 0,
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'ff39af94-b4c3-0134-81e0-6e67ae5b5d03';
    $display->content['new-ff39af94-b4c3-0134-81e0-6e67ae5b5d03'] = $pane;
    $display->panels['contentmain'][1] = 'new-ff39af94-b4c3-0134-81e0-6e67ae5b5d03';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $panelizer->display = $display;
  $export['node:openacademy_publication:default'] = $panelizer;

  return $export;
}
