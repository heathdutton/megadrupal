<?php
/**
 * @file
 * wetkit_pages.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function wetkit_pages_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_edit_panel_context_2';
  $handler->task = 'node_edit';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = -1;
  $handler->conf = array(
    'title' => 'Basic Page',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'css_id' => 'node-edit',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
  );
  $display = new panels_display();
  $display->layout = 'burr_flipped';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'contentmain' => NULL,
      'sidebar' => NULL,
    ),
    'sidebar' => array(
      'style' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'e721f600-7c2f-9fd4-dd0a-eee48ee14010';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-23b4d0bf-7df5-fcb4-9977-4dec1058582e';
    $pane->panel = 'contentmain';
    $pane->type = 'page_title';
    $pane->subtype = 'page_title';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'markup' => 'h1',
      'class' => 'node-edit-title',
      'id' => 'wb-cont',
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
    $pane->uuid = '23b4d0bf-7df5-fcb4-9977-4dec1058582e';
    $display->content['new-23b4d0bf-7df5-fcb4-9977-4dec1058582e'] = $pane;
    $display->panels['contentmain'][0] = 'new-23b4d0bf-7df5-fcb4-9977-4dec1058582e';
    $pane = new stdClass();
    $pane->pid = 'new-fdd6faee-963e-6534-0d08-4db1cfcf0818';
    $pane->panel = 'contentmain';
    $pane->type = 'page_tabs';
    $pane->subtype = 'page_tabs';
    $pane->shown = TRUE;
    $pane->access = array(
      'plugins' => array(),
    );
    $pane->configuration = array(
      'type' => 'both',
      'id' => 'tabs',
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
    $pane->uuid = 'fdd6faee-963e-6534-0d08-4db1cfcf0818';
    $display->content['new-fdd6faee-963e-6534-0d08-4db1cfcf0818'] = $pane;
    $display->panels['contentmain'][1] = 'new-fdd6faee-963e-6534-0d08-4db1cfcf0818';
    $pane = new stdClass();
    $pane->pid = 'new-10639cba-6d6b-b744-8d64-a9324e2b636c';
    $pane->panel = 'contentmain';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:title_field';
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
    $pane->uuid = '10639cba-6d6b-b744-8d64-a9324e2b636c';
    $display->content['new-10639cba-6d6b-b744-8d64-a9324e2b636c'] = $pane;
    $display->panels['contentmain'][2] = 'new-10639cba-6d6b-b744-8d64-a9324e2b636c';
    $pane = new stdClass();
    $pane->pid = 'new-4430e6b4-5128-8344-0d3f-3a0021249c28';
    $pane->panel = 'contentmain';
    $pane->type = 'node_form_title';
    $pane->subtype = 'node_form_title';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => 'argument_node_edit_1',
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
    $pane->uuid = '4430e6b4-5128-8344-0d3f-3a0021249c28';
    $display->content['new-4430e6b4-5128-8344-0d3f-3a0021249c28'] = $pane;
    $display->panels['contentmain'][3] = 'new-4430e6b4-5128-8344-0d3f-3a0021249c28';
    $pane = new stdClass();
    $pane->pid = 'new-3fbfca82-5e81-bb54-0d91-209b9d4a0df1';
    $pane->panel = 'contentmain';
    $pane->type = 'node_form_path';
    $pane->subtype = 'node_form_path';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => 'argument_node_edit_1',
      'override_title' => 1,
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
    $pane->uuid = '3fbfca82-5e81-bb54-0d91-209b9d4a0df1';
    $display->content['new-3fbfca82-5e81-bb54-0d91-209b9d4a0df1'] = $pane;
    $display->panels['contentmain'][4] = 'new-3fbfca82-5e81-bb54-0d91-209b9d4a0df1';
    $pane = new stdClass();
    $pane->pid = 'new-853205b5-8c9f-dfc4-71de-72d9424ab6e5';
    $pane->panel = 'contentmain';
    $pane->type = 'node_form_language';
    $pane->subtype = 'node_form_language';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => 'argument_node_edit_1',
      'override_title' => 1,
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
    $pane->uuid = '853205b5-8c9f-dfc4-71de-72d9424ab6e5';
    $display->content['new-853205b5-8c9f-dfc4-71de-72d9424ab6e5'] = $pane;
    $display->panels['contentmain'][5] = 'new-853205b5-8c9f-dfc4-71de-72d9424ab6e5';
    $pane = new stdClass();
    $pane->pid = 'new-36b06ca5-b71c-0e74-3160-71bbf898c299';
    $pane->panel = 'contentmain';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:body';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
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
    $pane->uuid = '36b06ca5-b71c-0e74-3160-71bbf898c299';
    $display->content['new-36b06ca5-b71c-0e74-3160-71bbf898c299'] = $pane;
    $display->panels['contentmain'][6] = 'new-36b06ca5-b71c-0e74-3160-71bbf898c299';
    $pane = new stdClass();
    $pane->pid = 'new-696d9b71-2e82-1414-19af-2f65bf35a787';
    $pane->panel = 'contentmain';
    $pane->type = 'node_form_menu';
    $pane->subtype = 'node_form_menu';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => 'argument_node_edit_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => array(
        'pane_collapsed' => 1,
      ),
      'style' => 'collapsible',
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 7;
    $pane->locks = array();
    $pane->uuid = '696d9b71-2e82-1414-19af-2f65bf35a787';
    $display->content['new-696d9b71-2e82-1414-19af-2f65bf35a787'] = $pane;
    $display->panels['contentmain'][7] = 'new-696d9b71-2e82-1414-19af-2f65bf35a787';
    $pane = new stdClass();
    $pane->pid = 'new-390b7bd0-1989-7964-7106-2e29ba6076d4';
    $pane->panel = 'contentmain';
    $pane->type = 'form';
    $pane->subtype = 'form';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => 'argument_node_edit_1',
      'override_title' => 1,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 8;
    $pane->locks = array();
    $pane->uuid = '390b7bd0-1989-7964-7106-2e29ba6076d4';
    $display->content['new-390b7bd0-1989-7964-7106-2e29ba6076d4'] = $pane;
    $display->panels['contentmain'][8] = 'new-390b7bd0-1989-7964-7106-2e29ba6076d4';
    $pane = new stdClass();
    $pane->pid = 'new-3004d634-c79e-8764-79b3-bd0a1e5892d4';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_featured_status';
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
    $pane->uuid = '3004d634-c79e-8764-79b3-bd0a1e5892d4';
    $display->content['new-3004d634-c79e-8764-79b3-bd0a1e5892d4'] = $pane;
    $display->panels['sidebar'][0] = 'new-3004d634-c79e-8764-79b3-bd0a1e5892d4';
    $pane = new stdClass();
    $pane->pid = 'new-857367a5-b834-50d4-5d10-bf15f424854e';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_featured_image';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
      'override_title' => 1,
      'override_title_text' => 'Featured image',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '857367a5-b834-50d4-5d10-bf15f424854e';
    $display->content['new-857367a5-b834-50d4-5d10-bf15f424854e'] = $pane;
    $display->panels['sidebar'][1] = 'new-857367a5-b834-50d4-5d10-bf15f424854e';
    $pane = new stdClass();
    $pane->pid = 'new-23b65da0-7fa6-91b4-c94f-dbd30ba5aed6';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_featured_categories';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
      'override_title' => 1,
      'override_title_text' => 'Content category',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '23b65da0-7fa6-91b4-c94f-dbd30ba5aed6';
    $display->content['new-23b65da0-7fa6-91b4-c94f-dbd30ba5aed6'] = $pane;
    $display->panels['sidebar'][2] = 'new-23b65da0-7fa6-91b4-c94f-dbd30ba5aed6';
    $pane = new stdClass();
    $pane->pid = 'new-380364ad-a36f-13e4-e521-b2be36733db6';
    $pane->panel = 'sidebar';
    $pane->type = 'node_form_publishing';
    $pane->subtype = 'node_form_publishing';
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '380364ad-a36f-13e4-e521-b2be36733db6';
    $display->content['new-380364ad-a36f-13e4-e521-b2be36733db6'] = $pane;
    $display->panels['sidebar'][3] = 'new-380364ad-a36f-13e4-e521-b2be36733db6';
    $pane = new stdClass();
    $pane->pid = 'new-3223a34b-bf87-b1d4-b5fb-425a5ce8d892';
    $pane->panel = 'sidebar';
    $pane->type = 'node_form_author';
    $pane->subtype = 'node_form_author';
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
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = '3223a34b-bf87-b1d4-b5fb-425a5ce8d892';
    $display->content['new-3223a34b-bf87-b1d4-b5fb-425a5ce8d892'] = $pane;
    $display->panels['sidebar'][4] = 'new-3223a34b-bf87-b1d4-b5fb-425a5ce8d892';
    $pane = new stdClass();
    $pane->pid = 'new-b4789c8a-2356-2ad4-2d39-3f1722eb448c';
    $pane->panel = 'sidebar';
    $pane->type = 'node_form_buttons';
    $pane->subtype = 'node_form_buttons';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
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
    $pane->position = 5;
    $pane->locks = array();
    $pane->uuid = 'b4789c8a-2356-2ad4-2d39-3f1722eb448c';
    $display->content['new-b4789c8a-2356-2ad4-2d39-3f1722eb448c'] = $pane;
    $display->panels['sidebar'][5] = 'new-b4789c8a-2356-2ad4-2d39-3f1722eb448c';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-390b7bd0-1989-7964-7106-2e29ba6076d4';
  $handler->conf['display'] = $display;
  $export['node_edit_panel_context_2'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view_panel_context_3';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 100;
  $handler->conf = array(
    'title' => 'Node',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => 'page-page',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'logic' => 'and',
      'plugins' => array(),
    ),
  );
  $display = new panels_display();
  $display->layout = 'pearson';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'pearson_top' => NULL,
      'pearson_first' => NULL,
      'pearson_second' => NULL,
      'pearson_third' => NULL,
      'pearson_bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '4286442d-61e8-8974-c145-399811c5794b';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-e4ef5c53-ce91-4ea4-cd0b-fecf62de32cd';
    $pane->panel = 'pearson_first';
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
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'e4ef5c53-ce91-4ea4-cd0b-fecf62de32cd';
    $display->content['new-e4ef5c53-ce91-4ea4-cd0b-fecf62de32cd'] = $pane;
    $display->panels['pearson_first'][0] = 'new-e4ef5c53-ce91-4ea4-cd0b-fecf62de32cd';
    $pane = new stdClass();
    $pane->pid = 'new-6c1a548a-cc6a-5004-51f6-a92b7785bde7';
    $pane->panel = 'pearson_first';
    $pane->type = 'page_tabs';
    $pane->subtype = 'page_tabs';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'type' => 'both',
      'id' => 'tabs',
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
    $pane->uuid = '6c1a548a-cc6a-5004-51f6-a92b7785bde7';
    $display->content['new-6c1a548a-cc6a-5004-51f6-a92b7785bde7'] = $pane;
    $display->panels['pearson_first'][1] = 'new-6c1a548a-cc6a-5004-51f6-a92b7785bde7';
    $pane = new stdClass();
    $pane->pid = 'new-1460bc1b-ea01-7f64-21a0-fa85274d6a3d';
    $pane->panel = 'pearson_first';
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
    $pane->uuid = '1460bc1b-ea01-7f64-21a0-fa85274d6a3d';
    $display->content['new-1460bc1b-ea01-7f64-21a0-fa85274d6a3d'] = $pane;
    $display->panels['pearson_first'][2] = 'new-1460bc1b-ea01-7f64-21a0-fa85274d6a3d';
    $pane = new stdClass();
    $pane->pid = 'new-fd9f2848-da1e-9d94-51d8-f93ecfb80106';
    $pane->panel = 'pearson_first';
    $pane->type = 'node_updated';
    $pane->subtype = 'node_updated';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'format' => 'short',
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = 'fd9f2848-da1e-9d94-51d8-f93ecfb80106';
    $display->content['new-fd9f2848-da1e-9d94-51d8-f93ecfb80106'] = $pane;
    $display->panels['pearson_first'][3] = 'new-fd9f2848-da1e-9d94-51d8-f93ecfb80106';
    $pane = new stdClass();
    $pane->pid = 'new-6e3e36c9-61b2-a8f4-3daa-ded1011baa06';
    $pane->panel = 'pearson_second';
    $pane->type = 'menu_tree';
    $pane->subtype = 'sidebar';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'menu_name' => 'sidebar',
      'parent_mlid' => 0,
      'show_menu_items' => array(),
      'parent' => 'sidebar:0',
      'title_link' => 0,
      'admin_title' => '',
      'level' => '1',
      'follow' => 0,
      'depth' => '0',
      'expanded' => 1,
      'sort' => 0,
      'override_title' => 1,
      'override_title_text' => '',
      'title_embed' => 0,
      'theme' => 'menu_link__menu_block__wxt_sidebar',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '6e3e36c9-61b2-a8f4-3daa-ded1011baa06';
    $display->content['new-6e3e36c9-61b2-a8f4-3daa-ded1011baa06'] = $pane;
    $display->panels['pearson_second'][0] = 'new-6e3e36c9-61b2-a8f4-3daa-ded1011baa06';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-6e3e36c9-61b2-a8f4-3daa-ded1011baa06';
  $handler->conf['display'] = $display;
  $export['node_view_panel_context_3'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view_panel_context_4';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 99;
  $handler->conf = array(
    'title' => 'Node (WxT)',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => 'page-page',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'logic' => 'and',
      'plugins' => array(
        0 => array(
          'name' => 'node_type',
          'settings' => array(
            'type' => array(
              'wetkit_page' => 'wetkit_page',
            ),
          ),
          'context' => 'argument_entity_id:node_1',
          'not' => FALSE,
        ),
      ),
    ),
  );
  $display = new panels_display();
  $display->layout = 'pearson';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'pearson_top' => NULL,
      'pearson_first' => NULL,
      'pearson_second' => NULL,
      'pearson_third' => NULL,
      'pearson_bottom' => NULL,
    ),
    'pearson_first' => array(
      'style' => '0',
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '4286442d-61e8-8974-c145-399811c5794b';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-e4ef5c53-ce91-4ea4-cd0b-fecf62de32cd';
    $pane->panel = 'pearson_first';
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
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'e4ef5c53-ce91-4ea4-cd0b-fecf62de32cd';
    $display->content['new-e4ef5c53-ce91-4ea4-cd0b-fecf62de32cd'] = $pane;
    $display->panels['pearson_first'][0] = 'new-e4ef5c53-ce91-4ea4-cd0b-fecf62de32cd';
    $pane = new stdClass();
    $pane->pid = 'new-6c1a548a-cc6a-5004-51f6-a92b7785bde7';
    $pane->panel = 'pearson_first';
    $pane->type = 'page_tabs';
    $pane->subtype = 'page_tabs';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'type' => 'both',
      'id' => 'tabs',
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
    $pane->uuid = '6c1a548a-cc6a-5004-51f6-a92b7785bde7';
    $display->content['new-6c1a548a-cc6a-5004-51f6-a92b7785bde7'] = $pane;
    $display->panels['pearson_first'][1] = 'new-6c1a548a-cc6a-5004-51f6-a92b7785bde7';
    $pane = new stdClass();
    $pane->pid = 'new-06f702e1-7dcb-4859-8646-e196e4c81a5d';
    $pane->panel = 'pearson_first';
    $pane->type = 'archived_widget';
    $pane->subtype = 'archived_widget';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => 'argument_entity_id:node_1',
      'span_item' => 'span-6',
      'form_build_id' => 'form-xLkW9cJZ71w6g_X2K4LiskkkbULD0eXhQJkexWvx0Ls',
      'form_token' => 'XFsHblzUTyYAErcjF3uZ-shvhu2EtcziOaPfmRs3W30',
      'form_id' => 'wetkit_wetboew_archived_widget_edit_form',
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
    $pane->uuid = '06f702e1-7dcb-4859-8646-e196e4c81a5d';
    $display->content['new-06f702e1-7dcb-4859-8646-e196e4c81a5d'] = $pane;
    $display->panels['pearson_first'][2] = 'new-06f702e1-7dcb-4859-8646-e196e4c81a5d';
    $pane = new stdClass();
    $pane->pid = 'new-505f19a8-a0c9-4c36-ab92-711267c3cfa9';
    $pane->panel = 'pearson_first';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_featured_image';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'wetkit_widgets_lightbox',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'file_view_mode' => 'default',
        'image_link' => '',
        'picture_group' => 'wxt_responsive_image',
        'fallback_image_style' => '',
        'colorbox' => 'wxt_responsive_image',
        'image_style_destination' => 'wetkit_image_half',
        'image_style_source' => 'wetkit_image_half',
        'custom_class' => '',
      ),
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '505f19a8-a0c9-4c36-ab92-711267c3cfa9';
    $display->content['new-505f19a8-a0c9-4c36-ab92-711267c3cfa9'] = $pane;
    $display->panels['pearson_first'][3] = 'new-505f19a8-a0c9-4c36-ab92-711267c3cfa9';
    $pane = new stdClass();
    $pane->pid = 'new-d7ae79c9-99c4-4b11-85be-f01600ba49fe';
    $pane->panel = 'pearson_first';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:body';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'text_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
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
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = 'd7ae79c9-99c4-4b11-85be-f01600ba49fe';
    $display->content['new-d7ae79c9-99c4-4b11-85be-f01600ba49fe'] = $pane;
    $display->panels['pearson_first'][4] = 'new-d7ae79c9-99c4-4b11-85be-f01600ba49fe';
    $pane = new stdClass();
    $pane->pid = 'new-6cba6356-6fca-4013-b37f-586ecc43e30a';
    $pane->panel = 'pearson_first';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_featured_categories';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'taxonomy_term_reference_link',
      'delta_limit' => '0',
      'delta_offset' => '0',
      'delta_reversed' => 0,
      'formatter_settings' => array(),
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
    $pane->position = 5;
    $pane->locks = array();
    $pane->uuid = '6cba6356-6fca-4013-b37f-586ecc43e30a';
    $display->content['new-6cba6356-6fca-4013-b37f-586ecc43e30a'] = $pane;
    $display->panels['pearson_first'][5] = 'new-6cba6356-6fca-4013-b37f-586ecc43e30a';
    $pane = new stdClass();
    $pane->pid = 'new-ddb8bcf4-4eb0-4c2d-99bb-5a361a034cc1';
    $pane->panel = 'pearson_first';
    $pane->type = 'date_modified';
    $pane->subtype = 'date_modified';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 6;
    $pane->locks = array();
    $pane->uuid = 'ddb8bcf4-4eb0-4c2d-99bb-5a361a034cc1';
    $display->content['new-ddb8bcf4-4eb0-4c2d-99bb-5a361a034cc1'] = $pane;
    $display->panels['pearson_first'][6] = 'new-ddb8bcf4-4eb0-4c2d-99bb-5a361a034cc1';
    $pane = new stdClass();
    $pane->pid = 'new-6e3e36c9-61b2-a8f4-3daa-ded1011baa06';
    $pane->panel = 'pearson_second';
    $pane->type = 'menu_tree';
    $pane->subtype = 'sidebar';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'menu_name' => 'sidebar',
      'parent_mlid' => 0,
      'show_menu_items' => array(),
      'parent' => 'sidebar:0',
      'title_link' => 0,
      'admin_title' => '',
      'level' => '1',
      'follow' => 0,
      'depth' => '0',
      'expanded' => 1,
      'sort' => 0,
      'override_title' => 1,
      'override_title_text' => '',
      'title_embed' => 0,
      'theme' => 'menu_link__menu_block__wxt_sidebar',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '6e3e36c9-61b2-a8f4-3daa-ded1011baa06';
    $display->content['new-6e3e36c9-61b2-a8f4-3daa-ded1011baa06'] = $pane;
    $display->panels['pearson_second'][0] = 'new-6e3e36c9-61b2-a8f4-3daa-ded1011baa06';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-6e3e36c9-61b2-a8f4-3daa-ded1011baa06';
  $handler->conf['display'] = $display;
  $export['node_view_panel_context_4'] = $handler;

  return $export;
}
