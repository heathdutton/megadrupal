<?php
/**
 * @file
 * openacademy_courses.panelizer.inc
 */

/**
 * Implements hook_panelizer_defaults().
 */
function openacademy_courses_panelizer_defaults() {
  $export = array();

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:openacademy_course:default';
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'openacademy_course';
  $panelizer->no_blocks = FALSE;
  $panelizer->css_id = 'course-page';
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
  $display->layout = 'mccoppin';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'column1' => NULL,
      'column2' => NULL,
      'column3' => NULL,
    ),
    'column1' => array(
      'style' => 'background_callout',
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '360c361e-37f8-0914-d191-6a8d7853b6ce';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-145ae47a-14d6-f264-2d67-fc67a8acb8e8';
    $pane->panel = 'column1';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_course_number';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'inline',
      'formatter' => 'text_default',
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
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '145ae47a-14d6-f264-2d67-fc67a8acb8e8';
    $display->content['new-145ae47a-14d6-f264-2d67-fc67a8acb8e8'] = $pane;
    $display->panels['column1'][0] = 'new-145ae47a-14d6-f264-2d67-fc67a8acb8e8';
    $pane = new stdClass();
    $pane->pid = 'new-58d58643-d0ce-4314-b151-3b90e0a5d8f6';
    $pane->panel = 'column1';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_course_term';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'inline',
      'formatter' => 'taxonomy_term_reference_plain',
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '58d58643-d0ce-4314-b151-3b90e0a5d8f6';
    $display->content['new-58d58643-d0ce-4314-b151-3b90e0a5d8f6'] = $pane;
    $display->panels['column1'][1] = 'new-58d58643-d0ce-4314-b151-3b90e0a5d8f6';
    $pane = new stdClass();
    $pane->pid = 'new-8a962dd3-1d46-3114-c913-c6c73fb94db0';
    $pane->panel = 'column1';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_course_credits';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'inline',
      'formatter' => 'text_default',
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
    $pane->uuid = '8a962dd3-1d46-3114-c913-c6c73fb94db0';
    $display->content['new-8a962dd3-1d46-3114-c913-c6c73fb94db0'] = $pane;
    $display->panels['column1'][2] = 'new-8a962dd3-1d46-3114-c913-c6c73fb94db0';
    $pane = new stdClass();
    $pane->pid = 'new-0d460573-8ba7-74c4-0174-729b771a9fcc';
    $pane->panel = 'column1';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_course_time';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
      'formatter' => 'office_hours',
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '0d460573-8ba7-74c4-0174-729b771a9fcc';
    $display->content['new-0d460573-8ba7-74c4-0174-729b771a9fcc'] = $pane;
    $display->panels['column1'][3] = 'new-0d460573-8ba7-74c4-0174-729b771a9fcc';
    $pane = new stdClass();
    $pane->pid = 'new-9d22b146-4d74-fdf4-ddcd-2d809c63f8e4';
    $pane->panel = 'column1';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_course_location';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
      'formatter' => 'text_default',
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
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = '9d22b146-4d74-fdf4-ddcd-2d809c63f8e4';
    $display->content['new-9d22b146-4d74-fdf4-ddcd-2d809c63f8e4'] = $pane;
    $display->panels['column1'][4] = 'new-9d22b146-4d74-fdf4-ddcd-2d809c63f8e4';
    $pane = new stdClass();
    $pane->pid = 'new-f38f9170-8f79-b354-7510-ab5bd8472a12';
    $pane->panel = 'column2';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_course_type';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'inline',
      'formatter' => 'taxonomy_term_reference_plain',
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
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'f38f9170-8f79-b354-7510-ab5bd8472a12';
    $display->content['new-f38f9170-8f79-b354-7510-ab5bd8472a12'] = $pane;
    $display->panels['column2'][0] = 'new-f38f9170-8f79-b354-7510-ab5bd8472a12';
    $pane = new stdClass();
    $pane->pid = 'new-4a72e247-d6ff-19c4-4124-8d7fa347e251';
    $pane->panel = 'column2';
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '4a72e247-d6ff-19c4-4124-8d7fa347e251';
    $display->content['new-4a72e247-d6ff-19c4-4124-8d7fa347e251'] = $pane;
    $display->panels['column2'][1] = 'new-4a72e247-d6ff-19c4-4124-8d7fa347e251';
    $pane = new stdClass();
    $pane->pid = 'new-e4e70099-33c0-ec04-c53b-2c3d6428ee27';
    $pane->panel = 'column3';
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
    $pane->uuid = 'e4e70099-33c0-ec04-c53b-2c3d6428ee27';
    $display->content['new-e4e70099-33c0-ec04-c53b-2c3d6428ee27'] = $pane;
    $display->panels['column3'][0] = 'new-e4e70099-33c0-ec04-c53b-2c3d6428ee27';
    $pane = new stdClass();
    $pane->pid = 'new-34c8038a-bf8a-8f24-4178-963495ac53e1';
    $pane->panel = 'column3';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_course_people';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
      'formatter' => 'entityreference_entity_view',
      'delta_limit' => '0',
      'delta_offset' => '0',
      'delta_reversed' => 0,
      'formatter_settings' => array(
        'link' => 0,
        'view_mode' => 'teaser',
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '34c8038a-bf8a-8f24-4178-963495ac53e1';
    $display->content['new-34c8038a-bf8a-8f24-4178-963495ac53e1'] = $pane;
    $display->panels['column3'][1] = 'new-34c8038a-bf8a-8f24-4178-963495ac53e1';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $panelizer->display = $display;
  $export['node:openacademy_course:default'] = $panelizer;

  return $export;
}
