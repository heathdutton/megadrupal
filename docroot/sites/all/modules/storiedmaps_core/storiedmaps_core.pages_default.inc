<?php
/**
 * @file
 * storiedmaps_core.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function storiedmaps_core_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view_panel_context';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Location',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => 'div.pane-node-field-content-image {
  float: right;
}',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'node_type',
          'settings' => array(
            'type' => array(
              'location' => 'location',
            ),
          ),
          'context' => 'argument_entity_id:node_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'two_brick';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'three_25_50_25_top' => NULL,
      'three_25_50_25_first' => NULL,
      'three_25_50_25_second' => NULL,
      'three_25_50_25_third' => NULL,
      'three_25_50_25_bottom' => NULL,
      'two_brick_top' => NULL,
      'two_brick_left_above' => NULL,
      'two_brick_right_above' => NULL,
      'two_brick_middle' => NULL,
      'two_brick_left_below' => NULL,
      'two_brick_right_below' => NULL,
      'two_brick_bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '%node:title';
  $display->uuid = '8b0aa865-29af-43db-b154-682ec3a00873';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-3a3ec760-af5b-434d-9884-90bb6aa27324';
    $pane->panel = 'two_brick_bottom';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_media';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'file_rendered',
      'delta_limit' => '0',
      'delta_offset' => '0',
      'delta_reversed' => 0,
      'formatter_settings' => array(
        'file_view_mode' => 'preview',
      ),
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = '3a3ec760-af5b-434d-9884-90bb6aa27324';
    $display->content['new-3a3ec760-af5b-434d-9884-90bb6aa27324'] = $pane;
    $display->panels['two_brick_bottom'][0] = 'new-3a3ec760-af5b-434d-9884-90bb6aa27324';
    $pane = new stdClass();
    $pane->pid = 'new-ae5e1846-176f-42b9-b86e-546548efc74f';
    $pane->panel = 'two_brick_bottom';
    $pane->type = 'node_comments';
    $pane->subtype = 'node_comments';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'mode' => '1',
      'comments_per_page' => '50',
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = 'ae5e1846-176f-42b9-b86e-546548efc74f';
    $display->content['new-ae5e1846-176f-42b9-b86e-546548efc74f'] = $pane;
    $display->panels['two_brick_bottom'][1] = 'new-ae5e1846-176f-42b9-b86e-546548efc74f';
    $pane = new stdClass();
    $pane->pid = 'new-5848a051-e513-4811-a332-95a3e1e6e9e5';
    $pane->panel = 'two_brick_bottom';
    $pane->type = 'node_links';
    $pane->subtype = 'node_links';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 0,
      'override_title_text' => '',
      'build_mode' => 'full',
      'identifier' => '',
      'link' => 1,
      'context' => 'argument_entity_id:node_1',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '5848a051-e513-4811-a332-95a3e1e6e9e5';
    $display->content['new-5848a051-e513-4811-a332-95a3e1e6e9e5'] = $pane;
    $display->panels['two_brick_bottom'][2] = 'new-5848a051-e513-4811-a332-95a3e1e6e9e5';
    $pane = new stdClass();
    $pane->pid = 'new-ddf6a766-3235-4252-ab2e-23dec96ff7f3';
    $pane->panel = 'two_brick_left_above';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_geolocation';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'geofield_openlayers',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'data' => 'full',
        'map_preset' => 'geofield_formatter_map',
      ),
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = 'ddf6a766-3235-4252-ab2e-23dec96ff7f3';
    $display->content['new-ddf6a766-3235-4252-ab2e-23dec96ff7f3'] = $pane;
    $display->panels['two_brick_left_above'][0] = 'new-ddf6a766-3235-4252-ab2e-23dec96ff7f3';
    $pane = new stdClass();
    $pane->pid = 'new-97d9b690-8cc6-4f6f-ac5d-ed5298773013';
    $pane->panel = 'two_brick_right_above';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_location_region';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
      'formatter' => 'taxonomy_term_reference_link',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = '97d9b690-8cc6-4f6f-ac5d-ed5298773013';
    $display->content['new-97d9b690-8cc6-4f6f-ac5d-ed5298773013'] = $pane;
    $display->panels['two_brick_right_above'][0] = 'new-97d9b690-8cc6-4f6f-ac5d-ed5298773013';
    $pane = new stdClass();
    $pane->pid = 'new-7838f830-a6b4-43bd-933b-b2763f6a329f';
    $pane->panel = 'two_brick_right_above';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_location_type';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
      'formatter' => 'taxonomy_term_reference_link',
      'delta_limit' => '0',
      'delta_offset' => '0',
      'delta_reversed' => 0,
      'formatter_settings' => array(),
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = '7838f830-a6b4-43bd-933b-b2763f6a329f';
    $display->content['new-7838f830-a6b4-43bd-933b-b2763f6a329f'] = $pane;
    $display->panels['two_brick_right_above'][1] = 'new-7838f830-a6b4-43bd-933b-b2763f6a329f';
    $pane = new stdClass();
    $pane->pid = 'new-59f5fa48-923d-44a8-99be-51d951b82c5c';
    $pane->panel = 'two_brick_right_above';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_organization_name';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'text_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = '59f5fa48-923d-44a8-99be-51d951b82c5c';
    $display->content['new-59f5fa48-923d-44a8-99be-51d951b82c5c'] = $pane;
    $display->panels['two_brick_right_above'][2] = 'new-59f5fa48-923d-44a8-99be-51d951b82c5c';
    $pane = new stdClass();
    $pane->pid = 'new-f7403e06-1116-418d-a7f5-92dbf6f42504';
    $pane->panel = 'two_brick_right_above';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_address';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
      'formatter' => 'addressfield_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'use_widget_handlers' => 1,
        'format_handlers' => array(
          'address' => 'address',
        ),
      ),
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = 'f7403e06-1116-418d-a7f5-92dbf6f42504';
    $display->content['new-f7403e06-1116-418d-a7f5-92dbf6f42504'] = $pane;
    $display->panels['two_brick_right_above'][3] = 'new-f7403e06-1116-418d-a7f5-92dbf6f42504';
    $pane = new stdClass();
    $pane->pid = 'new-1abc8107-a97c-410b-be71-54443d868115';
    $pane->panel = 'two_brick_right_above';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_website';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
      'formatter' => 'link_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = '1abc8107-a97c-410b-be71-54443d868115';
    $display->content['new-1abc8107-a97c-410b-be71-54443d868115'] = $pane;
    $display->panels['two_brick_right_above'][4] = 'new-1abc8107-a97c-410b-be71-54443d868115';
    $pane = new stdClass();
    $pane->pid = 'new-980abbd1-c986-4bd5-8e34-5d6594e16a3f';
    $pane->panel = 'two_brick_right_above';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_email';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
      'formatter' => 'email_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = '980abbd1-c986-4bd5-8e34-5d6594e16a3f';
    $display->content['new-980abbd1-c986-4bd5-8e34-5d6594e16a3f'] = $pane;
    $display->panels['two_brick_right_above'][5] = 'new-980abbd1-c986-4bd5-8e34-5d6594e16a3f';
    $pane = new stdClass();
    $pane->pid = 'new-9a890da8-cf0b-4af7-83bd-5443a4d9020f';
    $pane->panel = 'two_brick_right_above';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_phone_number';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'above',
      'formatter' => 'text_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
      'context' => 'argument_entity_id:node_1',
      'override_title' => 0,
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
    $pane->uuid = '9a890da8-cf0b-4af7-83bd-5443a4d9020f';
    $display->content['new-9a890da8-cf0b-4af7-83bd-5443a4d9020f'] = $pane;
    $display->panels['two_brick_right_above'][6] = 'new-9a890da8-cf0b-4af7-83bd-5443a4d9020f';
    $pane = new stdClass();
    $pane->pid = 'new-d8ffe77c-abea-4221-876d-f06617a90c89';
    $pane->panel = 'two_brick_top';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_content_image';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'image',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'file_view_mode' => 'full',
        'image_style' => 'large',
        'image_link' => 'file',
      ),
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
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'd8ffe77c-abea-4221-876d-f06617a90c89';
    $display->content['new-d8ffe77c-abea-4221-876d-f06617a90c89'] = $pane;
    $display->panels['two_brick_top'][0] = 'new-d8ffe77c-abea-4221-876d-f06617a90c89';
    $pane = new stdClass();
    $pane->pid = 'new-d801655f-02ad-4e1f-9ccf-056c6a86d581';
    $pane->panel = 'two_brick_top';
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
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = 'd801655f-02ad-4e1f-9ccf-056c6a86d581';
    $display->content['new-d801655f-02ad-4e1f-9ccf-056c6a86d581'] = $pane;
    $display->panels['two_brick_top'][1] = 'new-d801655f-02ad-4e1f-9ccf-056c6a86d581';
    $pane = new stdClass();
    $pane->pid = 'new-c2e9c38d-7bd9-4534-b65d-c86d9b23a07c';
    $pane->panel = 'two_brick_top';
    $pane->type = 'views_panes';
    $pane->subtype = 'story-panel_pane_2';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'c2e9c38d-7bd9-4534-b65d-c86d9b23a07c';
    $display->content['new-c2e9c38d-7bd9-4534-b65d-c86d9b23a07c'] = $pane;
    $display->panels['two_brick_top'][2] = 'new-c2e9c38d-7bd9-4534-b65d-c86d9b23a07c';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['node_view_panel_context'] = $handler;

  return $export;
}

/**
 * Implements hook_default_page_manager_pages().
 */
function storiedmaps_core_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'location_listing_landing';
  $page->task = 'page';
  $page->admin_title = 'Location listing landing';
  $page->admin_description = 'Landing page for text based listing of locations.';
  $page->path = 'location-landing';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'normal',
    'title' => 'Featured Sites',
    'name' => 'main-menu',
    'weight' => '0',
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
  $handler->name = 'page_location_listing_landing_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'location_listing_landing';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 1,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'two_66_33';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'two_66_33_top' => NULL,
      'two_66_33_first' => NULL,
      'two_66_33_second' => NULL,
      'two_66_33_bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Featured sites';
  $display->uuid = 'd748a345-bc1c-46a4-a127-002cd8369023';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-f27da7ac-f6aa-49d5-b624-d381f2e2ad7d';
    $pane->panel = 'two_66_33_first';
    $pane->type = 'views_panes';
    $pane->subtype = 'location_listing-panel_pane_2';
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
    $pane->uuid = 'f27da7ac-f6aa-49d5-b624-d381f2e2ad7d';
    $display->content['new-f27da7ac-f6aa-49d5-b624-d381f2e2ad7d'] = $pane;
    $display->panels['two_66_33_first'][0] = 'new-f27da7ac-f6aa-49d5-b624-d381f2e2ad7d';
    $pane = new stdClass();
    $pane->pid = 'new-c3d2c0d8-2437-4644-81c8-cdf3e750923f';
    $pane->panel = 'two_66_33_first';
    $pane->type = 'views_panes';
    $pane->subtype = 'location_listing-panel_pane_3';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'c3d2c0d8-2437-4644-81c8-cdf3e750923f';
    $display->content['new-c3d2c0d8-2437-4644-81c8-cdf3e750923f'] = $pane;
    $display->panels['two_66_33_first'][1] = 'new-c3d2c0d8-2437-4644-81c8-cdf3e750923f';
    $pane = new stdClass();
    $pane->pid = 'new-eac5b4a2-b0e2-47fa-8e7b-affb71c33af3';
    $pane->panel = 'two_66_33_second';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => 'Location by category help text',
      'title' => 'Explore locations by category',
      'body' => '<p>You can explore locations that belong to a particular category by clicking on the icons below. Hover over the icon to learn what it signifies.</p>',
      'format' => 'wysiwyg_editor',
      'substitute' => TRUE,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'eac5b4a2-b0e2-47fa-8e7b-affb71c33af3';
    $display->content['new-eac5b4a2-b0e2-47fa-8e7b-affb71c33af3'] = $pane;
    $display->panels['two_66_33_second'][0] = 'new-eac5b4a2-b0e2-47fa-8e7b-affb71c33af3';
    $pane = new stdClass();
    $pane->pid = 'new-aba61d8e-a127-463e-9eaf-1fc83006e7d0';
    $pane->panel = 'two_66_33_second';
    $pane->type = 'views_panes';
    $pane->subtype = 'location_terms-panel_pane_sidebar';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
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
    $pane->uuid = 'aba61d8e-a127-463e-9eaf-1fc83006e7d0';
    $display->content['new-aba61d8e-a127-463e-9eaf-1fc83006e7d0'] = $pane;
    $display->panels['two_66_33_second'][1] = 'new-aba61d8e-a127-463e-9eaf-1fc83006e7d0';
    $pane = new stdClass();
    $pane->pid = 'new-0c4d0ebd-f54b-4f8e-ab5b-83fa47e9d970';
    $pane->panel = 'two_66_33_second';
    $pane->type = 'views_panes';
    $pane->subtype = 'location_terms-panel_pane_1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '0c4d0ebd-f54b-4f8e-ab5b-83fa47e9d970';
    $display->content['new-0c4d0ebd-f54b-4f8e-ab5b-83fa47e9d970'] = $pane;
    $display->panels['two_66_33_second'][2] = 'new-0c4d0ebd-f54b-4f8e-ab5b-83fa47e9d970';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['location_listing_landing'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'storiedmaps_home';
  $page->task = 'page';
  $page->admin_title = 'StoriedMaps Home';
  $page->admin_description = 'A custom panels page for the home page of  StoriedMaps.';
  $page->path = 'storiedmaps-home';
  $page->access = array();
  $page->menu = array();
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_storiedmaps_home_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'storiedmaps_home';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 1,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'two_brick';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'two_brick_top' => NULL,
      'two_brick_left_above' => NULL,
      'two_brick_right_above' => NULL,
      'two_brick_middle' => NULL,
      'two_brick_left_below' => NULL,
      'two_brick_right_below' => NULL,
      'two_brick_bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '6fbcb9c2-46b4-4792-bca4-50b36b5f486b';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-92ea8c06-03bd-46bc-ae51-f71585243c4f';
    $pane->panel = 'two_brick_bottom';
    $pane->type = 'views';
    $pane->subtype = 'media';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 0,
      'nodes_per_page' => '9',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'block_1',
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
    $pane->uuid = '92ea8c06-03bd-46bc-ae51-f71585243c4f';
    $display->content['new-92ea8c06-03bd-46bc-ae51-f71585243c4f'] = $pane;
    $display->panels['two_brick_bottom'][0] = 'new-92ea8c06-03bd-46bc-ae51-f71585243c4f';
    $pane = new stdClass();
    $pane->pid = 'new-f97a6f74-c735-45f2-8b35-e30cc9e3153c';
    $pane->panel = 'two_brick_left_below';
    $pane->type = 'views_panes';
    $pane->subtype = 'story-panel_pane_6';
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
    $pane->uuid = 'f97a6f74-c735-45f2-8b35-e30cc9e3153c';
    $display->content['new-f97a6f74-c735-45f2-8b35-e30cc9e3153c'] = $pane;
    $display->panels['two_brick_left_below'][0] = 'new-f97a6f74-c735-45f2-8b35-e30cc9e3153c';
    $pane = new stdClass();
    $pane->pid = 'new-61a6bb92-1d63-45c8-93be-ff7aab72da90';
    $pane->panel = 'two_brick_middle';
    $pane->type = 'views_panes';
    $pane->subtype = 'location_terms-panel_pane_3';
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
    $pane->uuid = '61a6bb92-1d63-45c8-93be-ff7aab72da90';
    $display->content['new-61a6bb92-1d63-45c8-93be-ff7aab72da90'] = $pane;
    $display->panels['two_brick_middle'][0] = 'new-61a6bb92-1d63-45c8-93be-ff7aab72da90';
    $pane = new stdClass();
    $pane->pid = 'new-46c74b8f-61ac-477a-a9cc-9f6d60fa6844';
    $pane->panel = 'two_brick_right_below';
    $pane->type = 'views_panes';
    $pane->subtype = 'location_listing-panel_pane_4';
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
    $pane->uuid = '46c74b8f-61ac-477a-a9cc-9f6d60fa6844';
    $display->content['new-46c74b8f-61ac-477a-a9cc-9f6d60fa6844'] = $pane;
    $display->panels['two_brick_right_below'][0] = 'new-46c74b8f-61ac-477a-a9cc-9f6d60fa6844';
    $pane = new stdClass();
    $pane->pid = 'new-88bc7dd9-554e-45f6-ae2c-54f0ba40bcaf';
    $pane->panel = 'two_brick_top';
    $pane->type = 'views';
    $pane->subtype = 'highlighted';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 0,
      'nodes_per_page' => '5',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'block_full',
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
    $pane->uuid = '88bc7dd9-554e-45f6-ae2c-54f0ba40bcaf';
    $display->content['new-88bc7dd9-554e-45f6-ae2c-54f0ba40bcaf'] = $pane;
    $display->panels['two_brick_top'][0] = 'new-88bc7dd9-554e-45f6-ae2c-54f0ba40bcaf';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['storiedmaps_home'] = $page;

  return $pages;

}
