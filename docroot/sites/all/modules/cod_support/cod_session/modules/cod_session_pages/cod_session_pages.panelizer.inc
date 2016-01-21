<?php
/**
 * @file
 * cod_session_pages.panelizer.inc
 */

/**
 * Implements hook_panelizer_defaults().
 */
function cod_session_pages_panelizer_defaults() {
  $cod_session_video = module_exists('cod_session_video') ? 1 : 0;
  $cod_sponsors = module_exists('cod_sponsors') ? 1 : 0;
  $cod_schedule = module_exists('cod_schedule') ? 1 : 0;

  $export = array();

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:schedule_item:default:default';
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'schedule_item';
  $panelizer->no_blocks = FALSE;
  $panelizer->css_id = '';
  $panelizer->css = '';
  $panelizer->pipeline = 'ipe';
  $panelizer->contexts = array();
  $panelizer->relationships = array();
  $panelizer->access = array();
  $panelizer->view_mode = 'default';
  $panelizer->css_class = '';
  $panelizer->title_element = 'H2';
  $panelizer->link_to_entity = FALSE;
  $panelizer->extra = array();
  $display = new panels_display();
  $display->layout = 'hydra';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'preface' => NULL,
      'left_content' => NULL,
      'top_right_content' => NULL,
      'bottom_right_content' => NULL,
      'footer' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '%node:title';
  $display->uuid = '3603b081-3d55-442f-b360-9ac081bb25ef';
  $display->content = array();
  $display->panels = array();
  if ($cod_sponsors) {
    $pane = new stdClass();
    $pane->pid = 'new-35a9f58d-ea4f-4308-bd0e-bb34ee9c673d';
    $pane->panel = 'bottom_right_content';
    $pane->type = 'views_panes';
    $pane->subtype = 'cod_sponsors-sponsored';
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
    $pane->uuid = '35a9f58d-ea4f-4308-bd0e-bb34ee9c673d';
    $display->content['new-35a9f58d-ea4f-4308-bd0e-bb34ee9c673d'] = $pane;
    $display->panels['bottom_right_content'][0] = 'new-35a9f58d-ea4f-4308-bd0e-bb34ee9c673d';
  }
  $pane = new stdClass();
  $pane->pid = 'new-060e88d3-a530-456b-a153-72c849f61551';
  $pane->panel = 'footer';
  $pane->type = 'node_links';
  $pane->subtype = 'node_links';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => FALSE,
    'override_title_text' => '',
    'build_mode' => 'default',
    'identifier' => '',
    'link' => TRUE,
    'context' => 'panelizer',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array(
    'css_class' => 'link-wrapper',
  );
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '060e88d3-a530-456b-a153-72c849f61551';
  $display->content['new-060e88d3-a530-456b-a153-72c849f61551'] = $pane;
  $display->panels['footer'][0] = 'new-060e88d3-a530-456b-a153-72c849f61551';
  $pane = new stdClass();
  $pane->pid = 'new-1b9909fe-bd2b-4a5f-9d12-179bfb9974ce';
  $pane->panel = 'left_content';
  $pane->type = 'node_title';
  $pane->subtype = 'node_title';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'link' => 0,
    'markup' => 'h1',
    'id' => '',
    'class' => '',
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
  $pane->uuid = '1b9909fe-bd2b-4a5f-9d12-179bfb9974ce';
  $display->content['new-1b9909fe-bd2b-4a5f-9d12-179bfb9974ce'] = $pane;
  $display->panels['left_content'][0] = 'new-1b9909fe-bd2b-4a5f-9d12-179bfb9974ce';
  if ($cod_session_video) {
    $pane = new stdClass();
    $pane->pid = 'new-cb58a482-3454-4fd6-ada8-f4d556002527';
    $pane->panel = 'left_content';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_session_video';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'file_rendered',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'file_view_mode' => 'default',
      ),
      'context' => 'panelizer',
      'override_title' => 0,
      'override_title_text' => '',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'cb58a482-3454-4fd6-ada8-f4d556002527';
    $display->content['new-cb58a482-3454-4fd6-ada8-f4d556002527'] = $pane;
    $display->panels['left_content'][1] = 'new-cb58a482-3454-4fd6-ada8-f4d556002527';
  }
  $pane = new stdClass();
  $pane->pid = 'new-50d086d8-ad1f-41bd-bd3e-ae29daa3ba5d';
  $pane->panel = 'left_content';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:body';
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
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = '50d086d8-ad1f-41bd-bd3e-ae29daa3ba5d';
  $display->content['new-50d086d8-ad1f-41bd-bd3e-ae29daa3ba5d'] = $pane;
  $display->panels['left_content'][2] = 'new-50d086d8-ad1f-41bd-bd3e-ae29daa3ba5d';
  $pane = new stdClass();
  $pane->pid = 'new-4be76145-7e82-4cf5-a24c-bdd7b437ddb1';
  $pane->panel = 'top_right_content';
  $pane->type = 'flag_link';
  $pane->subtype = 'node';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'flag_name' => 'session_schedule',
    'context' => 'panelizer',
    'override_title' => 1,
    'override_title_text' => '',
    'override_title_heading' => 'h2',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '4be76145-7e82-4cf5-a24c-bdd7b437ddb1';
  $display->content['new-4be76145-7e82-4cf5-a24c-bdd7b437ddb1'] = $pane;
  $display->panels['top_right_content'][0] = 'new-4be76145-7e82-4cf5-a24c-bdd7b437ddb1';
  $pane = new stdClass();
  $pane->pid = 'new-7f6be7ab-2a46-4f82-b7b7-d666d640f819';
  $pane->panel = 'top_right_content';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:field_session_timeslot';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'hidden',
    'formatter' => 'entityreference_entity_view',
    'delta_limit' => '0',
    'delta_offset' => '0',
    'delta_reversed' => 0,
    'formatter_settings' => array(
      'links' => 0,
      'view_mode' => 'full',
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
  $pane->uuid = '7f6be7ab-2a46-4f82-b7b7-d666d640f819';
  $display->content['new-7f6be7ab-2a46-4f82-b7b7-d666d640f819'] = $pane;
  $display->panels['top_right_content'][1] = 'new-7f6be7ab-2a46-4f82-b7b7-d666d640f819';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = 'new-50d086d8-ad1f-41bd-bd3e-ae29daa3ba5d';
  $panelizer->display = $display;
  $export['node:schedule_item:default:default'] = $panelizer;

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:session:default:default';
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'session';
  $panelizer->no_blocks = FALSE;
  $panelizer->css_id = '';
  $panelizer->css = '';
  $panelizer->pipeline = 'ipe';
  $panelizer->contexts = array();
  $panelizer->relationships = array();
  $panelizer->access = array();
  $panelizer->view_mode = 'default';
  $panelizer->css_class = '';
  $panelizer->title_element = 'H2';
  $panelizer->link_to_entity = TRUE;
  $panelizer->extra = array();
  $display = new panels_display();
  $display->layout = 'hydra';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'preface' => NULL,
      'left_content' => NULL,
      'top_right_content' => NULL,
      'bottom_right_content' => NULL,
      'footer' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '%node:title';
  $display->uuid = 'a150eb79-129d-4b8e-a4fc-ab5df6eafef3';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-d1c2e7df-53fe-4dc5-9c77-51c7e83580e2';
  $pane->panel = 'bottom_right_content';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:og_vocabulary';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'title',
    'formatter' => 'entityreference_label',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter_settings' => array(
      'concatenate' => FALSE,
      'view_mode' => 'default',
      'links' => 0,
      'link' => 0,
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
  $pane->uuid = 'd1c2e7df-53fe-4dc5-9c77-51c7e83580e2';
  $display->content['new-d1c2e7df-53fe-4dc5-9c77-51c7e83580e2'] = $pane;
  $display->panels['bottom_right_content'][0] = 'new-d1c2e7df-53fe-4dc5-9c77-51c7e83580e2';
  $pane = new stdClass();
  $pane->pid = 'new-b4b99930-c93b-4de8-97f8-08a81413b531';
  $pane->panel = 'bottom_right_content';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:field_experience';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'title',
    'formatter' => 'list_default',
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
  $pane->uuid = 'b4b99930-c93b-4de8-97f8-08a81413b531';
  $display->content['new-b4b99930-c93b-4de8-97f8-08a81413b531'] = $pane;
  $display->panels['bottom_right_content'][1] = 'new-b4b99930-c93b-4de8-97f8-08a81413b531';
  $pane = new stdClass();
  $pane->pid = 'new-0da34661-2455-4e0b-918b-5c11fd44fd6a';
  $pane->panel = 'bottom_right_content';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:field_session_timeslot';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'title',
    'formatter' => 'entityreference_entity_view',
    'delta_limit' => '0',
    'delta_offset' => '0',
    'delta_reversed' => 0,
    'formatter_settings' => array(
      'links' => 1,
      'view_mode' => 'default',
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
  $pane->position = 3;
  $pane->locks = array();
  $pane->uuid = '0da34661-2455-4e0b-918b-5c11fd44fd6a';
  $display->content['new-0da34661-2455-4e0b-918b-5c11fd44fd6a'] = $pane;
  $display->panels['bottom_right_content'][3] = 'new-0da34661-2455-4e0b-918b-5c11fd44fd6a';
  $pane = new stdClass();
  $pane->pid = 'new-152727ea-93d6-4ee6-adf0-d3aea54994c9';
  $pane->panel = 'footer';
  $pane->type = 'node_comment_wrapper';
  $pane->subtype = 'node_comment_wrapper';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'mode' => '1',
    'comments_per_page' => '10',
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
  $pane->uuid = '152727ea-93d6-4ee6-adf0-d3aea54994c9';
  $display->content['new-152727ea-93d6-4ee6-adf0-d3aea54994c9'] = $pane;
  $display->panels['footer'][0] = 'new-152727ea-93d6-4ee6-adf0-d3aea54994c9';
  $pane = new stdClass();
  $pane->pid = 'new-62e658ee-33e0-4efd-b325-fe755cfaf049';
  $pane->panel = 'left_content';
  $pane->type = 'node_title';
  $pane->subtype = 'node_title';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'link' => 0,
    'markup' => 'h1',
    'id' => '',
    'class' => '',
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
  $pane->uuid = '62e658ee-33e0-4efd-b325-fe755cfaf049';
  $display->content['new-62e658ee-33e0-4efd-b325-fe755cfaf049'] = $pane;
  $display->panels['left_content'][0] = 'new-62e658ee-33e0-4efd-b325-fe755cfaf049';
  $pane = new stdClass();
  $pane->pid = 'new-b01a3d0c-16f1-43ec-8d15-a359a49d0ef0';
  $pane->panel = 'left_content';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:field_speakers';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'hidden',
    'formatter' => 'cod_user_label_fullname',
    'delta_limit' => '0',
    'delta_offset' => '0',
    'delta_reversed' => 0,
    'formatter_settings' => array(
      'link' => 1,
    ),
    'context' => 'panelizer',
    'override_title' => 0,
    'override_title_text' => '',
    'override_title_heading' => 'h2',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = 'b01a3d0c-16f1-43ec-8d15-a359a49d0ef0';
  $display->content['new-b01a3d0c-16f1-43ec-8d15-a359a49d0ef0'] = $pane;
  $display->panels['left_content'][1] = 'new-b01a3d0c-16f1-43ec-8d15-a359a49d0ef0';
  $pane = new stdClass();
  $pane->pid = 'new-b42fe3c1-4891-4c9c-bcb0-638cc2f63217';
  $pane->panel = 'left_content';
  $pane->type = 'node_links';
  $pane->subtype = 'node_links';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 0,
    'override_title_text' => '',
    'build_mode' => 'full',
    'identifier' => '',
    'link' => 0,
    'context' => 'panelizer',
    'override_title_heading' => 'h2',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = 'b42fe3c1-4891-4c9c-bcb0-638cc2f63217';
  $display->content['new-b42fe3c1-4891-4c9c-bcb0-638cc2f63217'] = $pane;
  $display->panels['left_content'][2] = 'new-b42fe3c1-4891-4c9c-bcb0-638cc2f63217';
  if ($cod_session_video) {
    $pane = new stdClass();
    $pane->pid = 'new-29a81407-1d8c-45ed-bdc2-e4382e7af28f';
    $pane->panel = 'left_content';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_session_video';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'file_rendered',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'file_view_mode' => 'default',
      ),
      'context' => 'panelizer',
      'override_title' => 0,
      'override_title_text' => '',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '29a81407-1d8c-45ed-bdc2-e4382e7af28f';
    $display->content['new-29a81407-1d8c-45ed-bdc2-e4382e7af28f'] = $pane;
    $display->panels['left_content'][3] = 'new-29a81407-1d8c-45ed-bdc2-e4382e7af28f';
  }
  $pane = new stdClass();
  $pane->pid = 'new-bf3ef643-3409-4b51-a951-2bfc9c548100';
  $pane->panel = 'left_content';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:field_slides';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'above',
    'formatter' => 'file_default',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter_settings' => array(),
    'context' => 'panelizer',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 4;
  $pane->locks = array();
  $pane->uuid = 'bf3ef643-3409-4b51-a951-2bfc9c548100';
  $display->content['new-bf3ef643-3409-4b51-a951-2bfc9c548100'] = $pane;
  $display->panels['left_content'][4] = 'new-bf3ef643-3409-4b51-a951-2bfc9c548100';
  $pane = new stdClass();
  $pane->pid = 'new-7414cec9-2fb3-489f-9af6-c9bc8ea7cc61';
  $pane->panel = 'left_content';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:field_news_image';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'above',
    'formatter' => 'hidden',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter_settings' => array(),
    'context' => 'panelizer',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 5;
  $pane->locks = array();
  $pane->uuid = '7414cec9-2fb3-489f-9af6-c9bc8ea7cc61';
  $display->content['new-7414cec9-2fb3-489f-9af6-c9bc8ea7cc61'] = $pane;
  $display->panels['left_content'][5] = 'new-7414cec9-2fb3-489f-9af6-c9bc8ea7cc61';
  $pane = new stdClass();
  $pane->pid = 'new-e1938ebe-87cf-478b-803b-54f51816a39b';
  $pane->panel = 'left_content';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:body';
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
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 6;
  $pane->locks = array();
  $pane->uuid = 'e1938ebe-87cf-478b-803b-54f51816a39b';
  $display->content['new-e1938ebe-87cf-478b-803b-54f51816a39b'] = $pane;
  $display->panels['left_content'][6] = 'new-e1938ebe-87cf-478b-803b-54f51816a39b';
  if ($cod_schedule) {
    $pane = new stdClass();
    $pane->pid = 'new-d936016a-863a-4689-8a00-53067e63422e';
    $pane->panel = 'top_right_content';
    $pane->type = 'flag_link';
    $pane->subtype = 'node';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'flag_name' => 'session_schedule',
      'context' => 'panelizer',
      'override_title' => 1,
      'override_title_text' => '',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'd936016a-863a-4689-8a00-53067e63422e';
    $display->content['new-d936016a-863a-4689-8a00-53067e63422e'] = $pane;
    $display->panels['top_right_content'][0] = 'new-d936016a-863a-4689-8a00-53067e63422e';
  }
  $display->hide_title = PANELS_TITLE_PANE;
  $display->title_pane = 'new-e1938ebe-87cf-478b-803b-54f51816a39b';
  $panelizer->display = $display;
  $export['node:session:default:default'] = $panelizer;

  return $export;
}
