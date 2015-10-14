<?php
/**
 * @file
 * wetkit_users.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function wetkit_users_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'pm_existing_pages_user_login_panel_context';
  $handler->task = 'pm_existing_pages';
  $handler->subtask = 'user_login';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'User Login Page',
    'no_blocks' => 1,
    'pipeline' => 'standard',
    'css_id' => 'user-login-panel',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'role',
          'settings' => array(
            'rids' => array(
              0 => 1,
            ),
          ),
          'context' => 'argument_entity_id:user_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'burr_flipped';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'sidebar' => NULL,
      'contentmain' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Log in';
  $display->uuid = '7f016b0e-91ae-0974-b5cd-6f19a14c991e';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-4dc85e6a-9552-4124-810b-10c71802b1da';
    $pane->panel = 'contentmain';
    $pane->type = 'pm_existing_pages';
    $pane->subtype = 'pm_existing_pages';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      0 => 'task_id',
      'task_id' => 'user_login',
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
    $pane->uuid = '4dc85e6a-9552-4124-810b-10c71802b1da';
    $display->content['new-4dc85e6a-9552-4124-810b-10c71802b1da'] = $pane;
    $display->panels['contentmain'][0] = 'new-4dc85e6a-9552-4124-810b-10c71802b1da';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['pm_existing_pages_user_login_panel_context'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'pm_existing_pages_user_password_panel_context';
  $handler->task = 'pm_existing_pages';
  $handler->subtask = 'user_password';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'User Password Page',
    'no_blocks' => 1,
    'pipeline' => 'standard',
    'css_id' => 'user-password-panel',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'role',
          'settings' => array(
            'rids' => array(
              0 => 1,
            ),
          ),
          'context' => 'argument_entity_id:user_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'burr_flipped';
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
  $display->uuid = 'b9c0d611-eb01-3b84-15c8-4b343c3e746b';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-82bb39a3-ba5b-ff44-5922-32904dc0653c';
    $pane->panel = 'contentmain';
    $pane->type = 'pm_existing_pages';
    $pane->subtype = 'pm_existing_pages';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      0 => 'task_id',
      'task_id' => 'user_password',
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
    $pane->uuid = '82bb39a3-ba5b-ff44-5922-32904dc0653c';
    $display->content['new-82bb39a3-ba5b-ff44-5922-32904dc0653c'] = $pane;
    $display->panels['contentmain'][0] = 'new-82bb39a3-ba5b-ff44-5922-32904dc0653c';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['pm_existing_pages_user_password_panel_context'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'pm_existing_pages_user_register_panel_context';
  $handler->task = 'pm_existing_pages';
  $handler->subtask = 'user_register';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'User Register Page',
    'no_blocks' => 1,
    'pipeline' => 'ipe',
    'css_id' => 'user-register-panel',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'role',
          'settings' => array(
            'rids' => array(
              0 => 1,
            ),
          ),
          'context' => 'argument_entity_id:user_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'burr_flipped';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'sidebar' => NULL,
      'contentmain' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Create an Account';
  $display->uuid = 'd1fd1426-0cbf-c0e4-bd6a-4f9bc58d845a';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-6b6f207f-8585-fbc4-519b-b6e03a01c9ba';
    $pane->panel = 'contentmain';
    $pane->type = 'pm_existing_pages';
    $pane->subtype = 'pm_existing_pages';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      0 => 'task_id',
      'task_id' => 'user_register',
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
    $pane->uuid = '6b6f207f-8585-fbc4-519b-b6e03a01c9ba';
    $display->content['new-6b6f207f-8585-fbc4-519b-b6e03a01c9ba'] = $pane;
    $display->panels['contentmain'][0] = 'new-6b6f207f-8585-fbc4-519b-b6e03a01c9ba';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['pm_existing_pages_user_register_panel_context'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'user_edit_panel_context';
  $handler->task = 'user_edit';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'User Edit Page',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => 'user-edit',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'burr_flipped';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'sidebar' => NULL,
      'contentmain' => NULL,
      'column1' => NULL,
      'column2' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '4d0e02d9-bf3b-3be4-4159-f564718f52c1';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-89168d29-da90-5784-9524-3ff245745e25';
    $pane->panel = 'contentmain';
    $pane->type = 'form';
    $pane->subtype = 'form';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => 'argument_user_edit_1',
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
    $pane->uuid = '89168d29-da90-5784-9524-3ff245745e25';
    $display->content['new-89168d29-da90-5784-9524-3ff245745e25'] = $pane;
    $display->panels['contentmain'][0] = 'new-89168d29-da90-5784-9524-3ff245745e25';
    $pane = new stdClass();
    $pane->pid = 'new-542794a8-1a12-89e4-6df1-99eed7a63e15';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'user:field_user_picture';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_user_edit_1',
      'override_title' => 1,
      'override_title_text' => 'User Picture',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '542794a8-1a12-89e4-6df1-99eed7a63e15';
    $display->content['new-542794a8-1a12-89e4-6df1-99eed7a63e15'] = $pane;
    $display->panels['sidebar'][0] = 'new-542794a8-1a12-89e4-6df1-99eed7a63e15';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-89168d29-da90-5784-9524-3ff245745e25';
  $handler->conf['display'] = $display;
  $export['user_edit_panel_context'] = $handler;

  return $export;
}
