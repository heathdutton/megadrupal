<?php
/**
 * @file
 * crm_core_volunteer.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function crm_core_volunteer_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cmcv_opportunity_attendance_report';
  $page->task = 'page';
  $page->admin_title = 'Volunteer Opportunity Attendance Report';
  $page->admin_description = '';
  $page->path = 'crm-core/reports/cmcv/opportunities/attendance/%node';
  $page->access = array(
    'logic' => 'and',
    'plugins' => array(
      0 => array(
        'name' => 'php',
        'settings' => array(
          'description' => 'Access check callback',
          'php' => 'return crm_core_volunteer_access_my_opportunity($contexts);',
        ),
        'not' => FALSE,
      ),
      1 => array(
        'name' => 'node_type',
        'settings' => array(
          'type' => array(
            'cmcv_volunteer' => 'cmcv_volunteer',
          ),
        ),
        'context' => 'argument_entity_id:node_1',
        'not' => FALSE,
      ),
    ),
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'tab',
    'title' => 'Attendance',
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
    'node' => array(
      'id' => 1,
      'identifier' => 'Node: ID',
      'name' => 'entity_id:node',
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
  $handler->name = 'page_cmcv_opportunity_attendance_report_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cmcv_opportunity_attendance_report';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Pager',
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
          'name' => 'php',
          'settings' => array(
            'description' => '',
            'php' => 'return !isset($_GET[\'pager\']) || $_GET[\'pager\'] != 0;',
          ),
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
  $display->title = 'Attendance for %node:title';
  $display->uuid = '1fae9048-c3b6-4ccc-9847-60a283cb8ea9';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-a9f6bf38-35c7-4bbd-a392-0369943a3d71';
    $pane->panel = 'middle';
    $pane->type = 'views';
    $pane->subtype = 'cmcv_opportunity_attendance_report';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 1,
      'nodes_per_page' => '50',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '@0',
      'url' => '',
      'display' => 'default',
      'context' => array(
        0 => '',
      ),
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
    $pane->uuid = 'a9f6bf38-35c7-4bbd-a392-0369943a3d71';
    $display->content['new-a9f6bf38-35c7-4bbd-a392-0369943a3d71'] = $pane;
    $display->panels['middle'][0] = 'new-a9f6bf38-35c7-4bbd-a392-0369943a3d71';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-a9f6bf38-35c7-4bbd-a392-0369943a3d71';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_cmcv_opportunity_attendance_report_panel_context_2';
  $handler->task = 'page';
  $handler->subtask = 'cmcv_opportunity_attendance_report';
  $handler->handler = 'panel_context';
  $handler->weight = 1;
  $handler->conf = array(
    'title' => 'No Pager',
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
          'name' => 'php',
          'settings' => array(
            'description' => '',
            'php' => 'return !isset($_GET[\'pager\']) || $_GET[\'pager\'] == 0;',
          ),
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
  $display->title = 'Attendance for %node:title';
  $display->uuid = 'b92a983a-61cc-4965-a76d-63b8e5c6b8fe';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-a1cbb369-cd58-46fb-b00d-bd7a5430648c';
    $pane->panel = 'middle';
    $pane->type = 'views';
    $pane->subtype = 'cmcv_opportunity_attendance_report';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 1,
      'use_pager' => 0,
      'nodes_per_page' => '0',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '@0',
      'url' => '',
      'display' => 'default',
      'context' => array(
        0 => '',
      ),
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
    $pane->uuid = 'a1cbb369-cd58-46fb-b00d-bd7a5430648c';
    $display->content['new-a1cbb369-cd58-46fb-b00d-bd7a5430648c'] = $pane;
    $display->panels['middle'][0] = 'new-a1cbb369-cd58-46fb-b00d-bd7a5430648c';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-a1cbb369-cd58-46fb-b00d-bd7a5430648c';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cmcv_opportunity_attendance_report'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cmcv_opportunity_signups';
  $page->task = 'page';
  $page->admin_title = 'Volunteer opportunity signups';
  $page->admin_description = '';
  $page->path = 'node/%node/volunteer-signups';
  $page->access = array(
    'plugins' => array(
      2 => array(
        'name' => 'php',
        'settings' => array(
          'description' => 'Access check callback',
          'php' => 'return crm_core_volunteer_access_my_opportunity($contexts);',
        ),
        'not' => FALSE,
      ),
      3 => array(
        'name' => 'entity_bundle:node',
        'settings' => array(
          'type' => array(
            'cmcv_volunteer' => 'cmcv_volunteer',
          ),
        ),
        'context' => 'argument_entity_id:node_1',
        'not' => FALSE,
      ),
    ),
    'logic' => 'and',
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'tab',
    'title' => 'Registration',
    'name' => 'navigation',
    'weight' => '10',
    'parent' => array(
      'type' => 'none',
      'title' => '',
      'name' => 'navigation',
      'weight' => '0',
    ),
  );
  $page->arguments = array(
    'node' => array(
      'id' => 1,
      'identifier' => 'Node: ID',
      'name' => 'entity_id:node',
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
  $handler->name = 'page_cmcv_opportunity_signups_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cmcv_opportunity_signups';
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
  $display->layout = 'flexible';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Volunteer Signups';
  $display->uuid = 'caf6ff76-6baa-4d45-8808-cfb6edb7c06d';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-04e3a25e-b0b8-40e6-942b-3e915daf106f';
    $pane->panel = 'center';
    $pane->type = 'views';
    $pane->subtype = 'cmcv_opportunity_signups';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 1,
      'nodes_per_page' => '0',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 1,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'default',
      'context' => array(
        0 => '',
      ),
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
    $pane->uuid = '04e3a25e-b0b8-40e6-942b-3e915daf106f';
    $display->content['new-04e3a25e-b0b8-40e6-942b-3e915daf106f'] = $pane;
    $display->panels['center'][0] = 'new-04e3a25e-b0b8-40e6-942b-3e915daf106f';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-04e3a25e-b0b8-40e6-942b-3e915daf106f';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cmcv_opportunity_signups'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cmcv_opportunity_summary_report';
  $page->task = 'page';
  $page->admin_title = 'CRM Core Volunteer: Opportunity Summary Report';
  $page->admin_description = '';
  $page->path = 'crm-core/reports/cmcv/opportunities/summary';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'access opportunities report',
        ),
        'context' => 'logged-in-user',
        'not' => FALSE,
      ),
    ),
    'logic' => 'and',
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'tab',
    'title' => 'Summary',
    'name' => 'navigation',
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
  $handler->name = 'page_cmcv_opportunity_summary_report_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cmcv_opportunity_summary_report';
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
  $display->layout = 'flexible';
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
          0 => 15,
          1 => 'main-row',
          2 => 1,
          3 => 5,
        ),
        'parent' => 'canvas',
      ),
      'main-row' => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'center',
        ),
        'parent' => 'main',
      ),
      'center' => array(
        'type' => 'region',
        'title' => 'Center',
        'width' => 100,
        'width_type' => '%',
        'parent' => 'main-row',
      ),
      1 => array(
        'type' => 'row',
        'contains' => 'column',
        'children' => array(
          0 => 4,
          1 => 3,
          2 => 2,
        ),
        'parent' => 'main',
        'class' => '',
      ),
      2 => array(
        'type' => 'column',
        'width' => '33.330451183526236',
        'width_type' => '%',
        'parent' => '1',
        'children' => array(
          0 => 11,
        ),
        'class' => '',
      ),
      3 => array(
        'type' => 'column',
        'width' => '33.33435967559419',
        'width_type' => '%',
        'parent' => '1',
        'children' => array(
          0 => 10,
        ),
        'class' => '',
      ),
      4 => array(
        'type' => 'column',
        'width' => '33.33518914087958',
        'width_type' => '%',
        'parent' => '1',
        'children' => array(
          0 => 9,
        ),
        'class' => '',
      ),
      5 => array(
        'type' => 'row',
        'contains' => 'column',
        'children' => array(
          0 => 8,
          1 => 7,
          2 => 6,
        ),
        'parent' => 'main',
        'class' => '',
      ),
      6 => array(
        'type' => 'column',
        'width' => '33.33003234964019',
        'width_type' => '%',
        'parent' => '5',
        'children' => array(
          0 => 14,
        ),
        'class' => '',
      ),
      7 => array(
        'type' => 'column',
        'width' => '33.334983825179904',
        'width_type' => '%',
        'parent' => '5',
        'children' => array(
          0 => 13,
        ),
        'class' => '',
      ),
      8 => array(
        'type' => 'column',
        'width' => '33.334983825179904',
        'width_type' => '%',
        'parent' => '5',
        'children' => array(
          0 => 12,
        ),
        'class' => '',
      ),
      9 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => '2_row_left',
        ),
        'parent' => '4',
        'class' => '',
      ),
      10 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => '2_row_center',
        ),
        'parent' => '3',
        'class' => '',
      ),
      11 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => '2_row_right',
        ),
        'parent' => '2',
        'class' => '',
      ),
      12 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => '3_row_left',
        ),
        'parent' => '8',
        'class' => '',
      ),
      13 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => '3_row_center',
        ),
        'parent' => '7',
        'class' => '',
      ),
      14 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => '3_row_right',
        ),
        'parent' => '6',
        'class' => '',
      ),
      '3_row_left' => array(
        'type' => 'region',
        'title' => '3 row left',
        'width' => 100,
        'width_type' => '%',
        'parent' => '12',
        'class' => '',
      ),
      '3_row_center' => array(
        'type' => 'region',
        'title' => '3 row center',
        'width' => 100,
        'width_type' => '%',
        'parent' => '13',
        'class' => '',
      ),
      '3_row_right' => array(
        'type' => 'region',
        'title' => '3 row right',
        'width' => 100,
        'width_type' => '%',
        'parent' => '14',
        'class' => '',
      ),
      '2_row_right' => array(
        'type' => 'region',
        'title' => '2 row right',
        'width' => 100,
        'width_type' => '%',
        'parent' => '11',
        'class' => '',
      ),
      '2_row_center' => array(
        'type' => 'region',
        'title' => '2 row center',
        'width' => 100,
        'width_type' => '%',
        'parent' => '10',
        'class' => '',
      ),
      '2_row_left' => array(
        'type' => 'region',
        'title' => '2 row left',
        'width' => 100,
        'width_type' => '%',
        'parent' => '9',
        'class' => '',
      ),
      15 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'metrics',
        ),
        'parent' => 'main',
        'class' => '',
      ),
      'metrics' => array(
        'type' => 'region',
        'title' => 'Metrics',
        'width' => 100,
        'width_type' => '%',
        'parent' => '15',
        'class' => '',
      ),
    ),
  );
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      '3_row_left' => NULL,
      '3_row_center' => NULL,
      '3_row_right' => NULL,
      '2_row_right' => NULL,
      '2_row_center' => NULL,
      '2_row_left' => NULL,
      'metrics' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'f5b7cc50-444d-48e8-a9d8-be990425135b';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-28521772-3098-4015-9f39-b9c9bdf61d6c';
    $pane->panel = '2_row_center';
    $pane->type = 'opportunity_summary_frequency';
    $pane->subtype = 'opportunity_summary_frequency';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'color_series' => '',
      'display_labels' => '',
      'include_legend' => '',
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
    $pane->uuid = '28521772-3098-4015-9f39-b9c9bdf61d6c';
    $display->content['new-28521772-3098-4015-9f39-b9c9bdf61d6c'] = $pane;
    $display->panels['2_row_center'][0] = 'new-28521772-3098-4015-9f39-b9c9bdf61d6c';
    $pane = new stdClass();
    $pane->pid = 'new-df981335-b7d7-41a8-867a-bf55e4a469ab';
    $pane->panel = '2_row_left';
    $pane->type = 'opportunity_summary_attendance';
    $pane->subtype = 'opportunity_summary_attendance';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'color_series' => '',
      'display_labels' => '',
      'include_legend' => '',
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
    $pane->uuid = 'df981335-b7d7-41a8-867a-bf55e4a469ab';
    $display->content['new-df981335-b7d7-41a8-867a-bf55e4a469ab'] = $pane;
    $display->panels['2_row_left'][0] = 'new-df981335-b7d7-41a8-867a-bf55e4a469ab';
    $pane = new stdClass();
    $pane->pid = 'new-56cbd4b3-f83b-435f-8c4b-2d306e134223';
    $pane->panel = '2_row_right';
    $pane->type = 'opportunity_summary_sources';
    $pane->subtype = 'opportunity_summary_sources';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'color_series' => '',
      'display_labels' => '',
      'include_legend' => '',
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
    $pane->uuid = '56cbd4b3-f83b-435f-8c4b-2d306e134223';
    $display->content['new-56cbd4b3-f83b-435f-8c4b-2d306e134223'] = $pane;
    $display->panels['2_row_right'][0] = 'new-56cbd4b3-f83b-435f-8c4b-2d306e134223';
    $pane = new stdClass();
    $pane->pid = 'new-faa6a411-5a7d-46c6-8f4b-071aea6182b9';
    $pane->panel = 'center';
    $pane->type = 'cmcv_summary_registration';
    $pane->subtype = 'cmcv_summary_registration';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'time_period' => 'All',
      'height' => '',
      'xaxis_tickmarks' => '',
      'chart_color' => '',
      'line_color' => '',
      'background_color' => '',
      'include_legend' => '',
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
    $pane->uuid = 'faa6a411-5a7d-46c6-8f4b-071aea6182b9';
    $display->content['new-faa6a411-5a7d-46c6-8f4b-071aea6182b9'] = $pane;
    $display->panels['center'][0] = 'new-faa6a411-5a7d-46c6-8f4b-071aea6182b9';
    $pane = new stdClass();
    $pane->pid = 'new-6326a54a-2ab7-4da0-bb12-e86ca718bf36';
    $pane->panel = 'metrics';
    $pane->type = 'cmcv_set_page_title';
    $pane->subtype = 'cmcv_set_page_title';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'color_series' => '',
      'display_labels' => '',
      'include_legend' => '0',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '6326a54a-2ab7-4da0-bb12-e86ca718bf36';
    $display->content['new-6326a54a-2ab7-4da0-bb12-e86ca718bf36'] = $pane;
    $display->panels['metrics'][0] = 'new-6326a54a-2ab7-4da0-bb12-e86ca718bf36';
    $pane = new stdClass();
    $pane->pid = 'new-1a3f5513-213b-4dfa-830d-b023878c36a2';
    $pane->panel = 'metrics';
    $pane->type = 'cmcv_opportunity_metrics';
    $pane->subtype = 'cmcv_opportunity_metrics';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '1a3f5513-213b-4dfa-830d-b023878c36a2';
    $display->content['new-1a3f5513-213b-4dfa-830d-b023878c36a2'] = $pane;
    $display->panels['metrics'][1] = 'new-1a3f5513-213b-4dfa-830d-b023878c36a2';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cmcv_opportunity_summary_report'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cmcv_overview_report';
  $page->task = 'page';
  $page->admin_title = 'Volunteers: Overview Report';
  $page->admin_description = 'This is overview report for volunteers.';
  $page->path = 'crm-core/reports/cmcv/overview';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'view crm report index',
        ),
        'context' => 'logged-in-user',
        'not' => FALSE,
      ),
    ),
    'logic' => 'and',
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'default tab',
    'title' => 'Overview',
    'name' => 'navigation',
    'weight' => '-15',
    'parent' => array(
      'type' => 'normal',
      'title' => 'Volunteers Reports',
      'name' => 'features',
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
  $handler->name = 'page_cmcv_overview_report_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cmcv_overview_report';
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
  $display->layout = 'flexible';
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
          0 => 1,
          1 => 2,
          2 => 5,
        ),
        'parent' => 'canvas',
      ),
      1 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'header',
        ),
        'parent' => 'main',
        'class' => '',
      ),
      2 => array(
        'type' => 'row',
        'contains' => 'column',
        'children' => array(
          0 => 3,
          1 => 4,
        ),
        'parent' => 'main',
        'class' => '',
      ),
      3 => array(
        'type' => 'column',
        'width' => '50',
        'width_type' => '%',
        'parent' => '2',
        'children' => array(
          0 => 8,
        ),
        'class' => '',
      ),
      4 => array(
        'type' => 'column',
        'width' => '50',
        'width_type' => '%',
        'parent' => '2',
        'children' => array(
          0 => 9,
        ),
        'class' => '',
      ),
      5 => array(
        'type' => 'row',
        'contains' => 'column',
        'children' => array(
          0 => 6,
          1 => 7,
        ),
        'parent' => 'main',
        'class' => '',
      ),
      6 => array(
        'type' => 'column',
        'width' => '50',
        'width_type' => '%',
        'parent' => '5',
        'children' => array(
          0 => 10,
        ),
        'class' => '',
      ),
      7 => array(
        'type' => 'column',
        'width' => '50',
        'width_type' => '%',
        'parent' => '5',
        'children' => array(
          0 => 11,
        ),
        'class' => '',
      ),
      'header' => array(
        'type' => 'region',
        'title' => 'Header',
        'width' => 100,
        'width_type' => '%',
        'parent' => '1',
        'class' => '',
      ),
      8 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'left_1',
        ),
        'parent' => '3',
        'class' => '',
      ),
      'left_1' => array(
        'type' => 'region',
        'title' => 'Left up',
        'width' => 100,
        'width_type' => '%',
        'parent' => '8',
        'class' => '',
      ),
      9 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'right_1',
        ),
        'parent' => '4',
        'class' => '',
      ),
      'right_1' => array(
        'type' => 'region',
        'title' => 'Right up',
        'width' => 100,
        'width_type' => '%',
        'parent' => '9',
        'class' => '',
      ),
      10 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'left_2',
        ),
        'parent' => '6',
        'class' => '',
      ),
      'left_2' => array(
        'type' => 'region',
        'title' => 'Left down',
        'width' => 100,
        'width_type' => '%',
        'parent' => '10',
        'class' => '',
      ),
      11 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'right_2',
        ),
        'parent' => '7',
        'class' => '',
      ),
      'right_2' => array(
        'type' => 'region',
        'title' => 'Right down',
        'width' => 100,
        'width_type' => '%',
        'parent' => '11',
        'class' => '',
      ),
    ),
  );
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'header' => NULL,
      'left_1' => NULL,
      'right_1' => NULL,
      'left_2' => NULL,
      'right_2' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Volunteer reports';
  $display->uuid = 'be0123a9-accc-49f3-bd69-1832a531f0d6';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-4c20baa6-12a7-4c93-989f-2c59dd4a8d87';
    $pane->panel = 'header';
    $pane->type = 'cmcv_overview_metrics';
    $pane->subtype = 'cmcv_overview_metrics';
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
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '4c20baa6-12a7-4c93-989f-2c59dd4a8d87';
    $display->content['new-4c20baa6-12a7-4c93-989f-2c59dd4a8d87'] = $pane;
    $display->panels['header'][0] = 'new-4c20baa6-12a7-4c93-989f-2c59dd4a8d87';
    $pane = new stdClass();
    $pane->pid = 'new-a91d14ab-aed7-4f85-97c8-3705ee37520a';
    $pane->panel = 'left_1';
    $pane->type = 'cmcv_registration';
    $pane->subtype = 'cmcv_registration';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'time_period' => '90',
      'height' => '',
      'xaxis_tickmarks' => '',
      'chart_color' => '',
      'line_color' => '',
      'background_color' => '',
      'include_legend' => '',
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
    $pane->uuid = 'a91d14ab-aed7-4f85-97c8-3705ee37520a';
    $display->content['new-a91d14ab-aed7-4f85-97c8-3705ee37520a'] = $pane;
    $display->panels['left_1'][0] = 'new-a91d14ab-aed7-4f85-97c8-3705ee37520a';
    $pane = new stdClass();
    $pane->pid = 'new-ebec137d-2b06-40d3-a8e3-1284cc29f42c';
    $pane->panel = 'left_2';
    $pane->type = 'views_panes';
    $pane->subtype = 'cmcv_recent_signups-panel_pane_1';
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
    $pane->uuid = 'ebec137d-2b06-40d3-a8e3-1284cc29f42c';
    $display->content['new-ebec137d-2b06-40d3-a8e3-1284cc29f42c'] = $pane;
    $display->panels['left_2'][0] = 'new-ebec137d-2b06-40d3-a8e3-1284cc29f42c';
    $pane = new stdClass();
    $pane->pid = 'new-feeb9c2a-170c-499f-b0bd-c094f6a38c59';
    $pane->panel = 'right_1';
    $pane->type = 'sources_overview';
    $pane->subtype = 'sources_overview';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'color_series' => '',
      'display_labels' => '',
      'include_legend' => '',
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
    $pane->uuid = 'feeb9c2a-170c-499f-b0bd-c094f6a38c59';
    $display->content['new-feeb9c2a-170c-499f-b0bd-c094f6a38c59'] = $pane;
    $display->panels['right_1'][0] = 'new-feeb9c2a-170c-499f-b0bd-c094f6a38c59';
    $pane = new stdClass();
    $pane->pid = 'new-dd6ff414-bd99-4206-b11a-86f4b7b3e96b';
    $pane->panel = 'right_2';
    $pane->type = 'cmcv_opportunity_attendance';
    $pane->subtype = 'cmcv_opportunity_attendance';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'dd6ff414-bd99-4206-b11a-86f4b7b3e96b';
    $display->content['new-dd6ff414-bd99-4206-b11a-86f4b7b3e96b'] = $pane;
    $display->panels['right_2'][0] = 'new-dd6ff414-bd99-4206-b11a-86f4b7b3e96b';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cmcv_overview_report'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cmcv_source_summary';
  $page->task = 'page';
  $page->admin_title = 'CRM Core Volunteer: Source summary';
  $page->admin_description = '';
  $page->path = 'crm-core/reports/cmcv/source/summary';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'tab',
    'title' => 'Source summary',
    'name' => 'navigation',
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
  $handler->name = 'page_cmcv_source_summary_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cmcv_source_summary';
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
  $display->layout = 'flexible';
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
          1 => 1,
        ),
        'parent' => 'canvas',
      ),
      'main-row' => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'center',
        ),
        'parent' => 'main',
      ),
      'center' => array(
        'type' => 'region',
        'title' => 'Center',
        'width' => 100,
        'width_type' => '%',
        'parent' => 'main-row',
      ),
      1 => array(
        'type' => 'row',
        'contains' => 'column',
        'children' => array(
          0 => 2,
          1 => 3,
        ),
        'parent' => 'main',
        'class' => '',
      ),
      2 => array(
        'type' => 'column',
        'width' => 50,
        'width_type' => '%',
        'parent' => '1',
        'children' => array(
          0 => 4,
        ),
        'class' => '',
      ),
      3 => array(
        'type' => 'column',
        'width' => 50,
        'width_type' => '%',
        'parent' => '1',
        'children' => array(
          0 => 5,
        ),
        'class' => '',
      ),
      4 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'left',
        ),
        'parent' => '2',
        'class' => '',
      ),
      'left' => array(
        'type' => 'region',
        'title' => 'Left',
        'width' => 100,
        'width_type' => '%',
        'parent' => '4',
        'class' => '',
      ),
      5 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'right',
        ),
        'parent' => '3',
        'class' => '',
      ),
      'right' => array(
        'type' => 'region',
        'title' => 'Right',
        'width' => 100,
        'width_type' => '%',
        'parent' => '5',
        'class' => '',
      ),
    ),
  );
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'left' => NULL,
      'right' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'e2a7c3a4-c332-45d1-a475-3b69951d2b74';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-b88db042-984f-4149-9eb8-db865a47679d';
    $pane->panel = 'center';
    $pane->type = 'cmcv_set_page_title';
    $pane->subtype = 'cmcv_set_page_title';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'color_series' => '',
      'display_labels' => '',
      'include_legend' => '0',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'b88db042-984f-4149-9eb8-db865a47679d';
    $display->content['new-b88db042-984f-4149-9eb8-db865a47679d'] = $pane;
    $display->panels['center'][0] = 'new-b88db042-984f-4149-9eb8-db865a47679d';
    $pane = new stdClass();
    $pane->pid = 'new-8a4c4be3-bf7e-4c9e-824d-5277fbb59f0d';
    $pane->panel = 'center';
    $pane->type = 'cmcv_source_summary_totals';
    $pane->subtype = 'cmcv_source_summary_totals';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'text' => '',
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
    $pane->uuid = '8a4c4be3-bf7e-4c9e-824d-5277fbb59f0d';
    $display->content['new-8a4c4be3-bf7e-4c9e-824d-5277fbb59f0d'] = $pane;
    $display->panels['center'][1] = 'new-8a4c4be3-bf7e-4c9e-824d-5277fbb59f0d';
    $pane = new stdClass();
    $pane->pid = 'new-7ea38817-6a77-4562-b19e-8736782ccd51';
    $pane->panel = 'center';
    $pane->type = 'cmcv_source_summary_chart';
    $pane->subtype = 'cmcv_source_summary_chart';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'xaxis_tickmarks' => '',
      'chart_color' => '',
      'line_color' => '',
      'background_color' => '',
      'include_legend' => '',
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
    $pane->uuid = '7ea38817-6a77-4562-b19e-8736782ccd51';
    $display->content['new-7ea38817-6a77-4562-b19e-8736782ccd51'] = $pane;
    $display->panels['center'][2] = 'new-7ea38817-6a77-4562-b19e-8736782ccd51';
    $pane = new stdClass();
    $pane->pid = 'new-3b4bf14c-33fb-4801-a4cd-fffc9a7d78e0';
    $pane->panel = 'left';
    $pane->type = 'cmcv_source_summary_attendance';
    $pane->subtype = 'cmcv_source_summary_attendance';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'color_series' => '',
      'display_labels' => '',
      'include_legend' => '',
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
    $pane->uuid = '3b4bf14c-33fb-4801-a4cd-fffc9a7d78e0';
    $display->content['new-3b4bf14c-33fb-4801-a4cd-fffc9a7d78e0'] = $pane;
    $display->panels['left'][0] = 'new-3b4bf14c-33fb-4801-a4cd-fffc9a7d78e0';
    $pane = new stdClass();
    $pane->pid = 'new-23451d48-0d8e-45d4-908c-58b6bc3c0c9e';
    $pane->panel = 'right';
    $pane->type = 'cmcv_source_summary_frequency';
    $pane->subtype = 'cmcv_source_summary_frequency';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'text' => '',
      'override_title' => 0,
      'override_title_text' => '',
      'include_legend' => '',
      'height' => '',
      'color_series' => '',
      'display_labels' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '23451d48-0d8e-45d4-908c-58b6bc3c0c9e';
    $display->content['new-23451d48-0d8e-45d4-908c-58b6bc3c0c9e'] = $pane;
    $display->panels['right'][0] = 'new-23451d48-0d8e-45d4-908c-58b6bc3c0c9e';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-8a4c4be3-bf7e-4c9e-824d-5277fbb59f0d';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cmcv_source_summary'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cmcv_sources_report';
  $page->task = 'page';
  $page->admin_title = 'Volunteers: Sources Report';
  $page->admin_description = 'This is the sources report for volunteers.';
  $page->path = 'crm-core/reports/cmcv/source';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'view volunteer sources report',
        ),
        'context' => 'logged-in-user',
        'not' => FALSE,
      ),
    ),
    'logic' => 'and',
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'tab',
    'title' => 'Sources',
    'name' => 'navigation',
    'weight' => '-7',
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
  $handler->name = 'page_cmcv_sources_report_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cmcv_sources_report';
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
  $display->layout = 'flexible';
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
          0 => 1,
          1 => 'main-row',
        ),
        'parent' => 'canvas',
      ),
      'main-row' => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'center',
        ),
        'parent' => 'main',
      ),
      'center' => array(
        'type' => 'region',
        'title' => 'Center',
        'width' => 100,
        'width_type' => '%',
        'parent' => 'main-row',
      ),
      1 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'center_',
        ),
        'parent' => 'main',
        'class' => '',
      ),
      'center_' => array(
        'type' => 'region',
        'title' => 'Center',
        'width' => 100,
        'width_type' => '%',
        'parent' => '1',
        'class' => '',
      ),
    ),
  );
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'center_' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Volunteer commitment sources';
  $display->uuid = 'c96b759d-e298-41d9-a086-dab2aec924c7';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-194b2467-ad38-4fa5-8a46-b3c1b4044cea';
    $pane->panel = 'center';
    $pane->type = 'views';
    $pane->subtype = 'cmcv_source_report';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 1,
      'nodes_per_page' => '25',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'default',
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
    $pane->uuid = '194b2467-ad38-4fa5-8a46-b3c1b4044cea';
    $display->content['new-194b2467-ad38-4fa5-8a46-b3c1b4044cea'] = $pane;
    $display->panels['center'][0] = 'new-194b2467-ad38-4fa5-8a46-b3c1b4044cea';
    $pane = new stdClass();
    $pane->pid = 'new-ccd8a90a-8cc4-4c55-8b8b-e49fdfbd2fe5';
    $pane->panel = 'center_';
    $pane->type = 'cmcv_source_chart';
    $pane->subtype = 'cmcv_source_chart';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'color_series' => '',
      'display_labels' => '',
      'chart_color' => '',
      'column_width' => '',
      'include_legend' => '',
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
    $pane->uuid = 'ccd8a90a-8cc4-4c55-8b8b-e49fdfbd2fe5';
    $display->content['new-ccd8a90a-8cc4-4c55-8b8b-e49fdfbd2fe5'] = $pane;
    $display->panels['center_'][0] = 'new-ccd8a90a-8cc4-4c55-8b8b-e49fdfbd2fe5';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cmcv_sources_report'] = $page;

  return $pages;

}
