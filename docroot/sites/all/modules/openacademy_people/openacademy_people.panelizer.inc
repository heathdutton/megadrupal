<?php
/**
 * @file
 * openacademy_people.panelizer.inc
 */

/**
 * Implements hook_panelizer_defaults().
 */
function openacademy_people_panelizer_defaults() {
  $export = array();

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:openacademy_person:default';
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'openacademy_person';
  $panelizer->no_blocks = FALSE;
  $panelizer->css_id = 'person-page';
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
  $display->layout = 'bartlett';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'sidebar' => NULL,
      'contentheader' => NULL,
      'contentcolumn1' => NULL,
      'contentcolumn2' => NULL,
    ),
    'sidebar' => array(
      'style' => 'background_callout',
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '31365200-b6f0-2f04-3158-59314119b54d';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-49d0991c-aa28-4134-e178-0c6e54e5be1f';
    $pane->panel = 'contentcolumn1';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_person_publications';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'entityreference_label',
      'delta_limit' => '0',
      'delta_offset' => '0',
      'delta_reversed' => 0,
      'formatter_settings' => array(
        'link' => 1,
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
    $pane->uuid = '49d0991c-aa28-4134-e178-0c6e54e5be1f';
    $display->content['new-49d0991c-aa28-4134-e178-0c6e54e5be1f'] = $pane;
    $display->panels['contentcolumn1'][0] = 'new-49d0991c-aa28-4134-e178-0c6e54e5be1f';
    $pane = new stdClass();
    $pane->pid = 'new-be4fe88a-a9c3-78f4-7533-bbd25404db11';
    $pane->panel = 'contentcolumn2';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_person_courses';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'entityreference_label',
      'delta_limit' => '0',
      'delta_offset' => '0',
      'delta_reversed' => 0,
      'formatter_settings' => array(
        'link' => 1,
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
    $pane->uuid = 'be4fe88a-a9c3-78f4-7533-bbd25404db11';
    $display->content['new-be4fe88a-a9c3-78f4-7533-bbd25404db11'] = $pane;
    $display->panels['contentcolumn2'][0] = 'new-be4fe88a-a9c3-78f4-7533-bbd25404db11';
    $pane = new stdClass();
    $pane->pid = 'new-524e0e24-12b9-f6b4-4d08-17d5bf013d34';
    $pane->panel = 'contentheader';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_person_type';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'taxonomy_term_reference_link',
      'delta_limit' => '0',
      'delta_offset' => '0',
      'delta_reversed' => 0,
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
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '524e0e24-12b9-f6b4-4d08-17d5bf013d34';
    $display->content['new-524e0e24-12b9-f6b4-4d08-17d5bf013d34'] = $pane;
    $display->panels['contentheader'][0] = 'new-524e0e24-12b9-f6b4-4d08-17d5bf013d34';
    $pane = new stdClass();
    $pane->pid = 'new-f11b48c0-d74e-9864-d95d-c5f7de640419';
    $pane->panel = 'contentheader';
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'f11b48c0-d74e-9864-d95d-c5f7de640419';
    $display->content['new-f11b48c0-d74e-9864-d95d-c5f7de640419'] = $pane;
    $display->panels['contentheader'][1] = 'new-f11b48c0-d74e-9864-d95d-c5f7de640419';
    $pane = new stdClass();
    $pane->pid = 'new-c0676cd0-0fa7-2d24-99ad-a7ea2d41134d';
    $pane->panel = 'contentheader';
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
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'c0676cd0-0fa7-2d24-99ad-a7ea2d41134d';
    $display->content['new-c0676cd0-0fa7-2d24-99ad-a7ea2d41134d'] = $pane;
    $display->panels['contentheader'][2] = 'new-c0676cd0-0fa7-2d24-99ad-a7ea2d41134d';
    $pane = new stdClass();
    $pane->pid = 'new-2e7cbbc3-3685-a2a4-4915-6705c3c3425f';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_featured_image';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'image',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'image_style' => 'panopoly_image_full',
        'image_link' => '',
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
    $pane->uuid = '2e7cbbc3-3685-a2a4-4915-6705c3c3425f';
    $display->content['new-2e7cbbc3-3685-a2a4-4915-6705c3c3425f'] = $pane;
    $display->panels['sidebar'][0] = 'new-2e7cbbc3-3685-a2a4-4915-6705c3c3425f';
    $pane = new stdClass();
    $pane->pid = 'new-6a13f5e2-1d52-0714-415d-ec29b3815933';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_person_email';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'inline',
      'formatter' => 'email_default',
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
    $pane->uuid = '6a13f5e2-1d52-0714-415d-ec29b3815933';
    $display->content['new-6a13f5e2-1d52-0714-415d-ec29b3815933'] = $pane;
    $display->panels['sidebar'][1] = 'new-6a13f5e2-1d52-0714-415d-ec29b3815933';
    $pane = new stdClass();
    $pane->pid = 'new-7adebce3-79ac-3e14-e11b-ecfed473374c';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_person_office';
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
    $pane->uuid = '7adebce3-79ac-3e14-e11b-ecfed473374c';
    $display->content['new-7adebce3-79ac-3e14-e11b-ecfed473374c'] = $pane;
    $display->panels['sidebar'][2] = 'new-7adebce3-79ac-3e14-e11b-ecfed473374c';
    $pane = new stdClass();
    $pane->pid = 'new-9124662d-f4e6-9454-955a-852a3b06d793';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_person_website';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'inline',
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '9124662d-f4e6-9454-955a-852a3b06d793';
    $display->content['new-9124662d-f4e6-9454-955a-852a3b06d793'] = $pane;
    $display->panels['sidebar'][3] = 'new-9124662d-f4e6-9454-955a-852a3b06d793';
    $pane = new stdClass();
    $pane->pid = 'new-7974aa86-66e4-e994-110e-ba8155c62e73';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_person_phone';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'inline',
      'formatter' => 'phone',
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
    $pane->uuid = '7974aa86-66e4-e994-110e-ba8155c62e73';
    $display->content['new-7974aa86-66e4-e994-110e-ba8155c62e73'] = $pane;
    $display->panels['sidebar'][4] = 'new-7974aa86-66e4-e994-110e-ba8155c62e73';
    $pane = new stdClass();
    $pane->pid = 'new-45cd5fa1-a992-c664-1522-b8e9991ab639';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_person_officehours';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'inline',
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
    $pane->position = 5;
    $pane->locks = array();
    $pane->uuid = '45cd5fa1-a992-c664-1522-b8e9991ab639';
    $display->content['new-45cd5fa1-a992-c664-1522-b8e9991ab639'] = $pane;
    $display->panels['sidebar'][5] = 'new-45cd5fa1-a992-c664-1522-b8e9991ab639';
    $pane = new stdClass();
    $pane->pid = 'new-47b07275-886a-6114-8d1f-34653be754b4';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_tags';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
      'formatter' => 'taxonomy_term_reference_link',
      'delta_limit' => '0',
      'delta_offset' => '0',
      'delta_reversed' => 0,
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
    $pane->position = 6;
    $pane->locks = array();
    $pane->uuid = '47b07275-886a-6114-8d1f-34653be754b4';
    $display->content['new-47b07275-886a-6114-8d1f-34653be754b4'] = $pane;
    $display->panels['sidebar'][6] = 'new-47b07275-886a-6114-8d1f-34653be754b4';
    $pane = new stdClass();
    $pane->pid = 'new-c31f1f32-671f-5f54-45e2-3c5e56f7ac30';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_person_cv';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
      'formatter' => 'media_large_icon',
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
    $pane->position = 7;
    $pane->locks = array();
    $pane->uuid = 'c31f1f32-671f-5f54-45e2-3c5e56f7ac30';
    $display->content['new-c31f1f32-671f-5f54-45e2-3c5e56f7ac30'] = $pane;
    $display->panels['sidebar'][7] = 'new-c31f1f32-671f-5f54-45e2-3c5e56f7ac30';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $panelizer->display = $display;
  $export['node:openacademy_person:default'] = $panelizer;

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'taxonomy_term:openacademy_person_type:default';
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'taxonomy_term';
  $panelizer->panelizer_key = 'openacademy_person_type';
  $panelizer->no_blocks = FALSE;
  $panelizer->css_id = 'person-type-panel';
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
  $display->layout = 'bryant';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'sidebar' => NULL,
      'contentmain' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'e203dc1a-cf0e-3b24-f10f-7a2b302ef66b';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-2f4aabc6-d3be-e474-91be-08c3a23d2e5e';
    $pane->panel = 'contentmain';
    $pane->type = 'views_panes';
    $pane->subtype = 'openacademy_people-people_grid';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'fields_override' => array(
        'field_featured_image' => 1,
        'title' => 1,
        'field_person_position' => 1,
        'field_tags' => 1,
      ),
      'context' => array(
        0 => '',
      ),
      'view_mode' => NULL,
      'widget_title' => '',
      'items_per_page' => NULL,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '2f4aabc6-d3be-e474-91be-08c3a23d2e5e';
    $display->content['new-2f4aabc6-d3be-e474-91be-08c3a23d2e5e'] = $pane;
    $display->panels['contentmain'][0] = 'new-2f4aabc6-d3be-e474-91be-08c3a23d2e5e';
    $pane = new stdClass();
    $pane->pid = 'new-6aa2861c-c0c5-aff4-8571-6100c498c74e';
    $pane->panel = 'sidebar';
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
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '6aa2861c-c0c5-aff4-8571-6100c498c74e';
    $display->content['new-6aa2861c-c0c5-aff4-8571-6100c498c74e'] = $pane;
    $display->panels['sidebar'][0] = 'new-6aa2861c-c0c5-aff4-8571-6100c498c74e';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $panelizer->display = $display;
  $export['taxonomy_term:openacademy_person_type:default'] = $panelizer;

  return $export;
}
