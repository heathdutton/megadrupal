<?php
/**
 * @file
 * sprout_admin.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function sprout_admin_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_edit_panel_context';
  $handler->task = 'node_edit';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Default',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'logic' => 'and',
      'plugins' => array(
        0 => array(
          'name' => 'php',
          'settings' => array(
            'description' => 'Admin theme active',
            'php' => 'if (path_is_admin(current_path())) { return TRUE; }',
          ),
          'not' => FALSE,
        ),
      ),
    ),
  );
  $display = new panels_display();
  $display->layout = 'twocol_65_35_stacked';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'left' => NULL,
      'right' => NULL,
      'top' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'e37cbe38-61e0-4ff5-98bf-eb3829ffd12a';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-4872919e-f57e-4942-816c-3936464fe26f';
    $pane->panel = 'left';
    $pane->type = 'node_form_title';
    $pane->subtype = 'node_form_title';
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
    $pane->uuid = '4872919e-f57e-4942-816c-3936464fe26f';
    $display->content['new-4872919e-f57e-4942-816c-3936464fe26f'] = $pane;
    $display->panels['left'][0] = 'new-4872919e-f57e-4942-816c-3936464fe26f';
    $pane = new stdClass();
    $pane->pid = 'new-f3d2a096-dfd3-47ae-aade-dca73cc13951';
    $pane->panel = 'left';
    $pane->type = 'node_form_path';
    $pane->subtype = 'node_form_path';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'return' => 'Finish',
      'cancel' => 'Cancel',
      'context' => 'argument_node_edit_1',
      'override_title' => 1,
      'override_title_text' => '',
      'display_in_fieldset' => 0,
      'collapsible' => 1,
      'collapsed' => 1,
      'form_build_id' => 'form-_7OfmWpYD8j9hmoPRE-iOhJ32AARKddiE1u0c_mQOgo',
      'form_token' => 'PPFpTAoaqK5mGwh_dTdIk7UubiSPn1Pr6yx5ad4BhpU',
      'form_id' => 'fuse_plugins_node_form_content_type_edit_form',
      'op' => 'Finish',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'f3d2a096-dfd3-47ae-aade-dca73cc13951';
    $display->content['new-f3d2a096-dfd3-47ae-aade-dca73cc13951'] = $pane;
    $display->panels['left'][1] = 'new-f3d2a096-dfd3-47ae-aade-dca73cc13951';
    $pane = new stdClass();
    $pane->pid = 'new-3e54a54c-2c09-43c8-b31f-d27bb714bad8';
    $pane->panel = 'left';
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
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '3e54a54c-2c09-43c8-b31f-d27bb714bad8';
    $display->content['new-3e54a54c-2c09-43c8-b31f-d27bb714bad8'] = $pane;
    $display->panels['left'][2] = 'new-3e54a54c-2c09-43c8-b31f-d27bb714bad8';
    $pane = new stdClass();
    $pane->pid = 'new-b841cbf0-0710-4971-abc7-b59967f8f34c';
    $pane->panel = 'right';
    $pane->type = 'node_form_summary';
    $pane->subtype = 'node_form_summary';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'return' => 'Finish',
      'cancel' => 'Cancel',
      'context' => 'argument_node_edit_1',
      'override_title' => 1,
      'override_title_text' => '',
      'published' => 1,
      'authored_by' => 1,
      'authored_on' => 1,
      'scheduler' => 1,
      'display_in_fieldset' => 0,
      'collapsible' => 1,
      'collapsed' => 1,
      'form_build_id' => 'form-IXGfqEJ7oSKz3FPbS3gqOib8oIx2Aqu-P8S2YVJKEvU',
      'form_token' => 'LGs9wvnxdJVGdCOpHTLLu3Yh_ocDkSn_Z0nkrhC5egY',
      'form_id' => 'node_form_panes_node_form_summary_content_type_edit_form',
      'op' => 'Finish',
      'buttons' => 1,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'b841cbf0-0710-4971-abc7-b59967f8f34c';
    $display->content['new-b841cbf0-0710-4971-abc7-b59967f8f34c'] = $pane;
    $display->panels['right'][0] = 'new-b841cbf0-0710-4971-abc7-b59967f8f34c';
    $pane = new stdClass();
    $pane->pid = 'new-726e7e31-d6f0-4537-b2e1-5e39319fc0a3';
    $pane->panel = 'right';
    $pane->type = 'node_form_metatag';
    $pane->subtype = 'node_form_metatag';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'return' => 'Finish',
      'cancel' => 'Cancel',
      'context' => 'argument_node_edit_1',
      'override_title' => 0,
      'override_title_text' => '',
      'display_in_fieldset' => 1,
      'collapsible' => 1,
      'collapsed' => 1,
      'form_build_id' => 'form-pjsX2hXeeLkC_VbZi9I02yLrgwn30nF8Kx-a4iR378Q',
      'form_token' => 'PPFpTAoaqK5mGwh_dTdIk7UubiSPn1Pr6yx5ad4BhpU',
      'form_id' => 'fuse_plugins_node_form_content_type_edit_form',
      'op' => 'Finish',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '726e7e31-d6f0-4537-b2e1-5e39319fc0a3';
    $display->content['new-726e7e31-d6f0-4537-b2e1-5e39319fc0a3'] = $pane;
    $display->panels['right'][1] = 'new-726e7e31-d6f0-4537-b2e1-5e39319fc0a3';
    $pane = new stdClass();
    $pane->pid = 'new-104a2a75-d900-4ff2-8571-8ec9353f2d7a';
    $pane->panel = 'right';
    $pane->type = 'node_form_rabbit_hole';
    $pane->subtype = 'node_form_rabbit_hole';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'return' => 'Finish',
      'cancel' => 'Cancel',
      'context' => 'argument_node_edit_1',
      'override_title' => 0,
      'override_title_text' => '',
      'display_in_fieldset' => 1,
      'collapsible' => 1,
      'collapsed' => 1,
      'form_build_id' => 'form-XxsHOM-fa1SKYUJAtgvnbb9-qynk42vl-3fWyjxc3rE',
      'form_token' => 'PPFpTAoaqK5mGwh_dTdIk7UubiSPn1Pr6yx5ad4BhpU',
      'form_id' => 'fuse_plugins_node_form_content_type_edit_form',
      'op' => 'Finish',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '104a2a75-d900-4ff2-8571-8ec9353f2d7a';
    $display->content['new-104a2a75-d900-4ff2-8571-8ec9353f2d7a'] = $pane;
    $display->panels['right'][2] = 'new-104a2a75-d900-4ff2-8571-8ec9353f2d7a';
    $pane = new stdClass();
    $pane->pid = 'new-fe262b02-ab18-43f6-86b2-8da036e007fa';
    $pane->panel = 'right';
    $pane->type = 'node_form_log';
    $pane->subtype = 'node_form_log';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'return' => 'Finish',
      'cancel' => 'Cancel',
      'context' => 'argument_node_edit_1',
      'override_title' => 0,
      'override_title_text' => '',
      'display_in_fieldset' => 1,
      'collapsible' => 1,
      'collapsed' => 1,
      'form_build_id' => 'form-SsW9wD0QxUvZYYU_9CbA5kDFwHCtPTDt51TsXFCuRxE',
      'form_token' => 'PPFpTAoaqK5mGwh_dTdIk7UubiSPn1Pr6yx5ad4BhpU',
      'form_id' => 'fuse_plugins_node_form_content_type_edit_form',
      'op' => 'Finish',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = 'fe262b02-ab18-43f6-86b2-8da036e007fa';
    $display->content['new-fe262b02-ab18-43f6-86b2-8da036e007fa'] = $pane;
    $display->panels['right'][3] = 'new-fe262b02-ab18-43f6-86b2-8da036e007fa';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['node_edit_panel_context'] = $handler;

  return $export;
}
