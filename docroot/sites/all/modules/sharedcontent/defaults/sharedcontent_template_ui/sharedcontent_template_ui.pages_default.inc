<?php
/**
 * @file
 * sharedcontent_template_ui.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function sharedcontent_template_ui_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'shared_content_dashboard';
  $page->task = 'page';
  $page->admin_title = 'ShareBoard';
  $page->admin_description = 'Shared Content Dashboard';
  $page->path = 'admin/content/sharedcontent/dashboard';
  $page->access = array(
    'logic' => 'and',
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'access sharedcontent ui',
        ),
        'context' => 'logged-in-user',
        'not' => FALSE,
      ),
    ),
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'default tab',
    'title' => 'ShareBoard',
    'name' => 'management',
    'weight' => '0',
    'parent' => array(
      'type' => 'tab',
      'title' => 'Shared Content',
      'name' => 'navigation',
      'weight' => '0',
    ),
  );
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => TRUE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_shared_content_dashboard_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'shared_content_dashboard';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'twocol_stacked';
  $display->layout_settings = array(
    'items' => array(
      'canvas' => array(
        'type' => 'row',
        'contains' => 'column',
        'children' => array(
          0 => 'main',
        ),
        'parent' => NULL,
      ),
      'main' => array(
        'type' => 'column',
        'width' => 100,
        'width_type' => '%',
        'children' => array(
          0 => 'main-row',
        ),
        'parent' => 'canvas',
      ),
      'main-row' => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'center',
          1 => 'right_region',
        ),
        'parent' => 'main',
      ),
      'center' => array(
        'type' => 'region',
        'title' => 'Left column',
        'width' => '50',
        'width_type' => '%',
        'parent' => 'main-row',
        'class' => '',
      ),
      'right_region' => array(
        'type' => 'region',
        'title' => 'Right region',
        'width' => '50',
        'width_type' => '%',
        'parent' => 'main-row',
        'class' => '',
      ),
    ),
  );
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'right_region' => NULL,
      'left' => NULL,
      'right' => NULL,
      'top' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'ShareBoard';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-1';
  $pane->panel = 'left';
  $pane->type = 'views_panes';
  $pane->subtype = 'sharedcontent_local_content-all';
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
  $display->content['new-1'] = $pane;
  $display->panels['left'][0] = 'new-1';
  $pane = new stdClass();
  $pane->pid = 'new-2';
  $pane->panel = 'left';
  $pane->type = 'views_panes';
  $pane->subtype = 'sharedcontent_local_content-queue';
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
  $display->content['new-2'] = $pane;
  $display->panels['left'][1] = 'new-2';
  $pane = new stdClass();
  $pane->pid = 'new-3';
  $pane->panel = 'right';
  $pane->type = 'views_panes';
  $pane->subtype = 'sharedcontent_remote_content-all';
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
  $display->content['new-3'] = $pane;
  $display->panels['right'][0] = 'new-3';
  $pane = new stdClass();
  $pane->pid = 'new-4';
  $pane->panel = 'right';
  $pane->type = 'views_panes';
  $pane->subtype = 'sharedcontent_remote_content-queue';
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
  $display->content['new-4'] = $pane;
  $display->panels['right'][1] = 'new-4';
  $pane = new stdClass();
  $pane->pid = 'new-5';
  $pane->panel = 'right';
  $pane->type = 'views_panes';
  $pane->subtype = 'sharedcontent_remote_content-suggestions';
  $pane->shown = TRUE;
  $pane->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'search_api_server_support',
        'settings' => array(
          'index' => 'sharedcontent',
          'feature' => 'search_api_mlt',
        ),
        'not' => FALSE,
      ),
    ),
  );
  $pane->configuration = array();
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $display->content['new-5'] = $pane;
  $display->panels['right'][2] = 'new-5';
  $pane = new stdClass();
  $pane->pid = 'new-6';
  $pane->panel = 'right';
  $pane->type = 'views_panes';
  $pane->subtype = 'sharedcontent_remote_content-linked';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array();
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 3;
  $pane->locks = array();
  $display->content['new-6'] = $pane;
  $display->panels['right'][3] = 'new-6';
  $pane = new stdClass();
  $pane->pid = 'new-7';
  $pane->panel = 'right';
  $pane->type = 'block';
  $pane->subtype = 'sharedcontent_ui-sharedcontent_ui_index_refresh';
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
  $pane->css = array(
    'css_id' => 'sharedcontent_ui_refresh_index_pane',
    'css_class' => '',
  );
  $pane->extras = array();
  $pane->position = 4;
  $pane->locks = array();
  $display->content['new-7'] = $pane;
  $display->panels['right'][4] = 'new-7';
  $pane = new stdClass();
  $pane->pid = 'new-8';
  $pane->panel = 'top';
  $pane->type = 'views_panes';
  $pane->subtype = 'sharedcontent_local_content-selected';
  $pane->shown = TRUE;
  $pane->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'sharedcontent_active_selected_local_content',
        'settings' => array(
          'present' => 1,
        ),
        'not' => FALSE,
      ),
    ),
  );
  $pane->configuration = array();
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $display->content['new-8'] = $pane;
  $display->panels['top'][0] = 'new-8';
  $pane = new stdClass();
  $pane->pid = 'new-9';
  $pane->panel = 'top';
  $pane->type = 'views_panes';
  $pane->subtype = 'sharedcontent_references-current';
  $pane->shown = TRUE;
  $pane->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'sharedcontent_active_selected_local_content',
        'settings' => array(
          'present' => 1,
        ),
        'not' => FALSE,
      ),
    ),
  );
  $pane->configuration = array();
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $display->content['new-9'] = $pane;
  $display->panels['top'][1] = 'new-9';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['shared_content_dashboard'] = $page;

  return $pages;

}
