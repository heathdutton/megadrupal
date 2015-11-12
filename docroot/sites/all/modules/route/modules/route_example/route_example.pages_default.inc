<?php
/**
 * @file
 * route_example.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function route_example_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'route_example';
  $page->task = 'page';
  $page->admin_title = 'Route example';
  $page->admin_description = '';
  $page->path = 'route-example/!active_step';
  $page->access = array();
  $page->menu = array(
    'type' => 'none',
    'title' => '',
    'name' => 'navigation',
    'weight' => '0',
    'parent' => array(
      'type' => 'none',
      'title' => '',
      'name' => 'navigation',
      'weight' => '0',
    ),
  );
  $page->arguments = array(
    'active_step' => array(
      'id' => 1,
      'identifier' => 'Route: active step',
      'name' => 'route_active_step',
      'settings' => array(),
    ),
  );
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_route_example_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'route_example';
  $handler->handler = 'panel_context';
  $handler->weight = -30;
  $handler->conf = array(
    'title' => 'Step one',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'route_step_name',
          'settings' => array(
            'step_name' => 'step-one',
          ),
          'context' => 'argument_route_active_step_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'threecol_25_50_25';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'left' => NULL,
      'middle' => NULL,
      'right' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Step one of our example route';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-1';
  $pane->panel = 'left';
  $pane->type = 'route_progress_indicator';
  $pane->subtype = 'route_progress_indicator';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'form' => NULL,
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $display->content['new-1'] = $pane;
  $display->panels['left'][0] = 'new-1';
  $pane = new stdClass();
  $pane->pid = 'new-2';
  $pane->panel = 'middle';
  $pane->type = 'capture_personal_data';
  $pane->subtype = 'capture_personal_data';
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
  $display->content['new-2'] = $pane;
  $display->panels['middle'][0] = 'new-2';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_route_example_panel_context_2';
  $handler->task = 'page';
  $handler->subtask = 'route_example';
  $handler->handler = 'panel_context';
  $handler->weight = -29;
  $handler->conf = array(
    'title' => 'Step two',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'route_step_name',
          'settings' => array(
            'step_name' => 'step-two',
          ),
          'context' => 'argument_active_step_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'threecol_25_50_25';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'left' => NULL,
      'middle' => NULL,
      'right' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '%active_step:first_name, behold the step two!';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-3';
  $pane->panel = 'left';
  $pane->type = 'route_progress_indicator';
  $pane->subtype = 'route_progress_indicator';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'form' => NULL,
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $display->content['new-3'] = $pane;
  $display->panels['left'][0] = 'new-3';
  $pane = new stdClass();
  $pane->pid = 'new-4';
  $pane->panel = 'middle';
  $pane->type = 'capture_interests';
  $pane->subtype = 'capture_interests';
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
  $display->content['new-4'] = $pane;
  $display->panels['middle'][0] = 'new-4';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_route_example_panel_context_4';
  $handler->task = 'page';
  $handler->subtask = 'route_example';
  $handler->handler = 'panel_context';
  $handler->weight = -28;
  $handler->conf = array(
    'title' => 'For authenticated users only',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'route_step_name',
          'settings' => array(
            'step_name' => 'step-three',
          ),
          'context' => 'argument_route_active_step_1',
          'not' => FALSE,
        ),
        1 => array(
          'name' => 'role',
          'settings' => array(
            'rids' => array(
              0 => 2,
            ),
          ),
          'context' => 'logged-in-user',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'threecol_25_50_25';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'left' => NULL,
      'middle' => NULL,
      'right' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'As a bonus for being logged in, we\'ve added an extra step for you!';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-5';
  $pane->panel = 'left';
  $pane->type = 'route_progress_indicator';
  $pane->subtype = 'route_progress_indicator';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'form' => NULL,
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $display->content['new-5'] = $pane;
  $display->panels['left'][0] = 'new-5';
  $pane = new stdClass();
  $pane->pid = 'new-6';
  $pane->panel = 'middle';
  $pane->type = 'capture_reasons';
  $pane->subtype = 'capture_reasons';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 0,
    'override_title_text' => 'test',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $display->content['new-6'] = $pane;
  $display->panels['middle'][0] = 'new-6';
  $pane = new stdClass();
  $pane->pid = 'new-7';
  $pane->panel = 'right';
  $pane->type = 'block';
  $pane->subtype = 'user-online';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 0,
    'override_title_text' => 'Check this out! You should be on the list somewhere.',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $display->content['new-7'] = $pane;
  $display->panels['right'][0] = 'new-7';
  $pane = new stdClass();
  $pane->pid = 'new-8';
  $pane->panel = 'right';
  $pane->type = 'block';
  $pane->subtype = 'user-new';
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
  $display->content['new-8'] = $pane;
  $display->panels['right'][1] = 'new-8';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_route_example_panel_context_3';
  $handler->task = 'page';
  $handler->subtask = 'route_example';
  $handler->handler = 'panel_context';
  $handler->weight = -27;
  $handler->conf = array(
    'title' => 'Last step',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'route_step_name',
          'settings' => array(
            'step_name' => 'last-step',
          ),
          'context' => 'argument_active_step_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'threecol_25_50_25';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'left' => NULL,
      'middle' => NULL,
      'right' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Now you can review your data. How awesome is this?';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-9';
  $pane->panel = 'left';
  $pane->type = 'route_progress_indicator';
  $pane->subtype = 'route_progress_indicator';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'form' => NULL,
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $display->content['new-9'] = $pane;
  $display->panels['left'][0] = 'new-9';
  $pane = new stdClass();
  $pane->pid = 'new-10';
  $pane->panel = 'middle';
  $pane->type = 'show_summary';
  $pane->subtype = 'show_summary';
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
  $display->content['new-10'] = $pane;
  $display->panels['middle'][0] = 'new-10';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;

  $pages['route_example'] = $page;

  return $pages;

}
