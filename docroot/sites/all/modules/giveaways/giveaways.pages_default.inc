<?php
/**
 * @file
 * giveaways.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function giveaways_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view__key_giveaways';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Key Giveaways',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'name' => 'key_giveaways',
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'entity_bundle:node',
          'settings' => array(
            'type' => array(
              'giveaway' => 'giveaway',
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
  $display->layout = 'onecol';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'middle' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '%node:title';
  $display->uuid = 'bf9bc562-e3d0-4a9f-89cb-2d263a1f9b92';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-de35013a-4da8-4aac-bffb-fadf78f4a937';
    $pane->panel = 'middle';
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
    $pane->uuid = 'de35013a-4da8-4aac-bffb-fadf78f4a937';
    $display->content['new-de35013a-4da8-4aac-bffb-fadf78f4a937'] = $pane;
    $display->panels['middle'][0] = 'new-de35013a-4da8-4aac-bffb-fadf78f4a937';
    $pane = new stdClass();
    $pane->pid = 'new-1f861e88-4afb-4d8d-ad0e-1608ce755133';
    $pane->panel = 'middle';
    $pane->type = 'block';
    $pane->subtype = 'giveaways-giveaways_keys_left_block';
    $pane->shown = TRUE;
    $pane->access = array(
      'plugins' => array(
        0 => array(
          'name' => 'giveaways_keys_are_available',
          'settings' => NULL,
          'context' => 'argument_entity_id:node_1',
          'not' => FALSE,
        ),
      ),
    );
    $pane->configuration = array(
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
    $pane->uuid = '1f861e88-4afb-4d8d-ad0e-1608ce755133';
    $display->content['new-1f861e88-4afb-4d8d-ad0e-1608ce755133'] = $pane;
    $display->panels['middle'][1] = 'new-1f861e88-4afb-4d8d-ad0e-1608ce755133';
    $pane = new stdClass();
    $pane->pid = 'new-9c36f6b4-8a22-43d0-b3f0-0e8881133c0b';
    $pane->panel = 'middle';
    $pane->type = 'block';
    $pane->subtype = 'giveaways-giveaway_claim_block';
    $pane->shown = TRUE;
    $pane->access = array(
      'plugins' => array(
        0 => array(
          'name' => 'giveaways_user_has_claimed',
          'settings' => NULL,
          'context' => array(
            0 => 'logged-in-user',
            1 => 'argument_entity_id:node_1',
          ),
          'not' => TRUE,
        ),
      ),
    );
    $pane->configuration = array(
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
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '9c36f6b4-8a22-43d0-b3f0-0e8881133c0b';
    $display->content['new-9c36f6b4-8a22-43d0-b3f0-0e8881133c0b'] = $pane;
    $display->panels['middle'][2] = 'new-9c36f6b4-8a22-43d0-b3f0-0e8881133c0b';
    $pane = new stdClass();
    $pane->pid = 'new-5768fef3-e20e-47f0-8f8b-b634c3467bc5';
    $pane->panel = 'middle';
    $pane->type = 'block';
    $pane->subtype = 'giveaways-giveaways_user_claimed_key_block';
    $pane->shown = TRUE;
    $pane->access = array(
      'plugins' => array(
        0 => array(
          'name' => 'giveaways_user_has_claimed',
          'settings' => NULL,
          'context' => array(
            0 => 'logged-in-user',
            1 => 'argument_entity_id:node_1',
          ),
          'not' => FALSE,
        ),
      ),
    );
    $pane->configuration = array(
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
    $pane->uuid = '5768fef3-e20e-47f0-8f8b-b634c3467bc5';
    $display->content['new-5768fef3-e20e-47f0-8f8b-b634c3467bc5'] = $pane;
    $display->panels['middle'][3] = 'new-5768fef3-e20e-47f0-8f8b-b634c3467bc5';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-de35013a-4da8-4aac-bffb-fadf78f4a937';
  $handler->conf['display'] = $display;
  $export['node_view__key_giveaways'] = $handler;

  return $export;
}
