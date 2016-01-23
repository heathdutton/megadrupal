<?php
/**
 * @file
 * crm_core_event.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function crm_core_event_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cmcev_event_summary_report';
  $page->task = 'page';
  $page->admin_title = 'CRM Core Event: Event summary report';
  $page->admin_description = '';
  $page->path = 'crm-core/reports/cmcev/events/summary';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'view event overview',
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
  $handler->name = 'page_cmcev_event_summary_report_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cmcev_event_summary_report';
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
          2 => 2,
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
        'title' => 'Metrics',
        'width' => 100,
        'width_type' => '%',
        'parent' => 'main-row',
        'class' => '',
      ),
      1 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'chart',
        ),
        'parent' => 'main',
        'class' => '',
      ),
      'chart' => array(
        'type' => 'region',
        'title' => 'Chart',
        'width' => 100,
        'width_type' => '%',
        'parent' => '1',
        'class' => '',
      ),
      2 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'view',
        ),
        'parent' => 'main',
        'class' => '',
      ),
      'view' => array(
        'type' => 'region',
        'title' => 'View',
        'width' => 100,
        'width_type' => '%',
        'parent' => '2',
        'class' => '',
      ),
    ),
  );
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'chart' => NULL,
      'view' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '2078b910-dcde-469b-949f-356eabaedaa4';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-3e8fc473-7a91-46a6-bcb2-569f3b50bb44';
    $pane->panel = 'center';
    $pane->type = 'views_panes';
    $pane->subtype = 'cmcev_event_summary_metrics-panel_pane_1';
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
    $pane->uuid = '3e8fc473-7a91-46a6-bcb2-569f3b50bb44';
    $display->content['new-3e8fc473-7a91-46a6-bcb2-569f3b50bb44'] = $pane;
    $display->panels['center'][0] = 'new-3e8fc473-7a91-46a6-bcb2-569f3b50bb44';
    $pane = new stdClass();
    $pane->pid = 'new-e00126b5-b339-4a1f-95c1-57afe82c89df';
    $pane->panel = 'center';
    $pane->type = 'cmcev_set_page_title';
    $pane->subtype = 'cmcev_set_page_title';
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'e00126b5-b339-4a1f-95c1-57afe82c89df';
    $display->content['new-e00126b5-b339-4a1f-95c1-57afe82c89df'] = $pane;
    $display->panels['center'][1] = 'new-e00126b5-b339-4a1f-95c1-57afe82c89df';
    $pane = new stdClass();
    $pane->pid = 'new-65010176-a726-4bf9-a18c-a8682de32799';
    $pane->panel = 'chart';
    $pane->type = 'event_summary_registration';
    $pane->subtype = 'event_summary_registration';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'text' => '',
      'time_period' => '30',
      'override_title' => 0,
      'override_title_text' => '',
      'include_legend' => 0,
      'height' => '',
      'xaxis_tickmarks' => '',
      'line_color' => '',
      'chart_color' => '',
      'background_color' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '65010176-a726-4bf9-a18c-a8682de32799';
    $display->content['new-65010176-a726-4bf9-a18c-a8682de32799'] = $pane;
    $display->panels['chart'][0] = 'new-65010176-a726-4bf9-a18c-a8682de32799';
    $pane = new stdClass();
    $pane->pid = 'new-c2ad779c-c1f4-4dd5-a963-618bdc5c21bd';
    $pane->panel = 'view';
    $pane->type = 'views_panes';
    $pane->subtype = 'cmcev_event_summary_attendees-panel_pane_1';
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
    $pane->uuid = 'c2ad779c-c1f4-4dd5-a963-618bdc5c21bd';
    $display->content['new-c2ad779c-c1f4-4dd5-a963-618bdc5c21bd'] = $pane;
    $display->panels['view'][0] = 'new-c2ad779c-c1f4-4dd5-a963-618bdc5c21bd';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cmcev_event_summary_report'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cmcev_overview_report';
  $page->task = 'page';
  $page->admin_title = 'Events Overview Report';
  $page->admin_description = '';
  $page->path = 'crm-core/reports/cmcev/overview';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'view overview report',
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
      'title' => 'Events Reports',
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
  $handler->name = 'page_cmcev_overview_report_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cmcev_overview_report';
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
          2 => 6,
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
          0 => 3,
          1 => 2,
        ),
        'parent' => 'main',
        'class' => '',
      ),
      2 => array(
        'type' => 'column',
        'width' => '50',
        'width_type' => '%',
        'parent' => '1',
        'children' => array(
          0 => 5,
        ),
        'class' => '',
      ),
      3 => array(
        'type' => 'column',
        'width' => '50',
        'width_type' => '%',
        'parent' => '1',
        'children' => array(
          0 => 4,
        ),
        'class' => '',
      ),
      4 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'left_up',
        ),
        'parent' => '3',
        'class' => '',
      ),
      'left_up' => array(
        'type' => 'region',
        'title' => 'Left up',
        'width' => 100,
        'width_type' => '%',
        'parent' => '4',
        'class' => '',
      ),
      5 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'right_up',
        ),
        'parent' => '2',
        'class' => '',
      ),
      'right_up' => array(
        'type' => 'region',
        'title' => 'Right up',
        'width' => 100,
        'width_type' => '%',
        'parent' => '5',
        'class' => '',
      ),
      6 => array(
        'type' => 'row',
        'contains' => 'column',
        'children' => array(
          0 => 8,
          1 => 7,
        ),
        'parent' => 'main',
        'class' => '',
      ),
      7 => array(
        'type' => 'column',
        'width' => 50,
        'width_type' => '%',
        'parent' => '6',
        'children' => array(
          0 => 10,
        ),
        'class' => '',
      ),
      8 => array(
        'type' => 'column',
        'width' => 50,
        'width_type' => '%',
        'parent' => '6',
        'children' => array(
          0 => 9,
        ),
        'class' => '',
      ),
      9 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'left_down',
        ),
        'parent' => '8',
        'class' => '',
      ),
      'left_down' => array(
        'type' => 'region',
        'title' => 'Left down',
        'width' => 100,
        'width_type' => '%',
        'parent' => '9',
        'class' => '',
      ),
      10 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'right_down',
        ),
        'parent' => '7',
        'class' => '',
      ),
      'right_down' => array(
        'type' => 'region',
        'title' => 'Right down',
        'width' => 100,
        'width_type' => '%',
        'parent' => '10',
        'class' => '',
      ),
    ),
  );
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'left_up' => NULL,
      'right_up' => NULL,
      'left_down' => NULL,
      'right_down' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Event reports';
  $display->uuid = 'ff05d7c0-5bf4-4c03-952d-3e64687037d6';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-9a184b71-a014-412b-ac8d-d9ecaf347ca7';
    $pane->panel = 'center';
    $pane->type = 'event_metrics';
    $pane->subtype = 'event_metrics';
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
    $pane->css = array(
      'css_id' => '',
      'css_class' => 'summary-table',
    );
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '9a184b71-a014-412b-ac8d-d9ecaf347ca7';
    $display->content['new-9a184b71-a014-412b-ac8d-d9ecaf347ca7'] = $pane;
    $display->panels['center'][0] = 'new-9a184b71-a014-412b-ac8d-d9ecaf347ca7';
    $pane = new stdClass();
    $pane->pid = 'new-3d06f5e8-f844-4471-9cc0-e10520630282';
    $pane->panel = 'left_down';
    $pane->type = 'views_panes';
    $pane->subtype = 'cmcev_overview_signups-panel_pane_1';
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
    $pane->uuid = '3d06f5e8-f844-4471-9cc0-e10520630282';
    $display->content['new-3d06f5e8-f844-4471-9cc0-e10520630282'] = $pane;
    $display->panels['left_down'][0] = 'new-3d06f5e8-f844-4471-9cc0-e10520630282';
    $pane = new stdClass();
    $pane->pid = 'new-f7a31e24-aa52-4078-a0f9-a934cfd74e2f';
    $pane->panel = 'left_up';
    $pane->type = 'event_registration';
    $pane->subtype = 'event_registration';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'time_period' => '30',
      'height' => '',
      'xaxis_tickmarks' => '',
      'chart_color' => '',
      'line_color' => '',
      'background_color' => '',
      'include_legend' => 0,
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
    $pane->uuid = 'f7a31e24-aa52-4078-a0f9-a934cfd74e2f';
    $display->content['new-f7a31e24-aa52-4078-a0f9-a934cfd74e2f'] = $pane;
    $display->panels['left_up'][0] = 'new-f7a31e24-aa52-4078-a0f9-a934cfd74e2f';
    $pane = new stdClass();
    $pane->pid = 'new-4885306b-ad03-4f5d-8e05-13e8eb7fe713';
    $pane->panel = 'right_down';
    $pane->type = 'event_attendance';
    $pane->subtype = 'event_attendance';
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
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '4885306b-ad03-4f5d-8e05-13e8eb7fe713';
    $display->content['new-4885306b-ad03-4f5d-8e05-13e8eb7fe713'] = $pane;
    $display->panels['right_down'][0] = 'new-4885306b-ad03-4f5d-8e05-13e8eb7fe713';
    $pane = new stdClass();
    $pane->pid = 'new-25652dd0-bc6a-4b72-bbd6-3ca2ecf757ab';
    $pane->panel = 'right_up';
    $pane->type = 'event_source';
    $pane->subtype = 'event_source';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'color_series' => '',
      'display_labels' => 0,
      'include_legend' => 0,
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
    $pane->uuid = '25652dd0-bc6a-4b72-bbd6-3ca2ecf757ab';
    $display->content['new-25652dd0-bc6a-4b72-bbd6-3ca2ecf757ab'] = $pane;
    $display->panels['right_up'][0] = 'new-25652dd0-bc6a-4b72-bbd6-3ca2ecf757ab';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cmcev_overview_report'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cmcev_revenues_summary_report';
  $page->task = 'page';
  $page->admin_title = 'CRM Core Event: Revenues summary report';
  $page->admin_description = '';
  $page->path = 'crm-core/reports/cmcev/revenues/summary';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'view revenues report',
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
  $handler->name = 'page_cmcev_revenues_summary_report_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cmcev_revenues_summary_report';
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
          2 => 2,
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
        'title' => 'Metrics',
        'width' => 100,
        'width_type' => '%',
        'parent' => 'main-row',
        'class' => '',
      ),
      1 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'registrations',
        ),
        'parent' => 'main',
        'class' => '',
      ),
      'registrations' => array(
        'type' => 'region',
        'title' => 'Registrations',
        'width' => 100,
        'width_type' => '%',
        'parent' => '1',
        'class' => '',
      ),
      2 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'events',
        ),
        'parent' => 'main',
        'class' => '',
      ),
      'events' => array(
        'type' => 'region',
        'title' => 'Events',
        'width' => 100,
        'width_type' => '%',
        'parent' => '2',
        'class' => '',
      ),
    ),
  );
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'registrations' => NULL,
      'events' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'ccfcd9a8-98dc-481c-b330-0ab95ec504f4';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-ffea5c8b-316d-4921-b99f-c6d6a94b42d5';
    $pane->panel = 'center';
    $pane->type = 'views_panes';
    $pane->subtype = 'cmcev_revenues_summary_report_metrics-panel_pane_1';
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
    $pane->uuid = 'ffea5c8b-316d-4921-b99f-c6d6a94b42d5';
    $display->content['new-ffea5c8b-316d-4921-b99f-c6d6a94b42d5'] = $pane;
    $display->panels['center'][0] = 'new-ffea5c8b-316d-4921-b99f-c6d6a94b42d5';
    $pane = new stdClass();
    $pane->pid = 'new-7713a2bc-2174-4982-b745-a1453938f72a';
    $pane->panel = 'events';
    $pane->type = 'views_panes';
    $pane->subtype = 'cmcev_revenues_summary_report-panel_pane_1';
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
    $pane->uuid = '7713a2bc-2174-4982-b745-a1453938f72a';
    $display->content['new-7713a2bc-2174-4982-b745-a1453938f72a'] = $pane;
    $display->panels['events'][0] = 'new-7713a2bc-2174-4982-b745-a1453938f72a';
    $pane = new stdClass();
    $pane->pid = 'new-1a16431e-5526-4249-b3bd-6ed1d2777f9f';
    $pane->panel = 'registrations';
    $pane->type = 'revenues_sammary_registration';
    $pane->subtype = 'revenues_sammary_registration';
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
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '1a16431e-5526-4249-b3bd-6ed1d2777f9f';
    $display->content['new-1a16431e-5526-4249-b3bd-6ed1d2777f9f'] = $pane;
    $display->panels['registrations'][0] = 'new-1a16431e-5526-4249-b3bd-6ed1d2777f9f';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-7713a2bc-2174-4982-b745-a1453938f72a';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cmcev_revenues_summary_report'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cmcev_source_summary_report';
  $page->task = 'page';
  $page->admin_title = 'CRM Core Event: Source summary report';
  $page->admin_description = '';
  $page->path = 'crm-core/reports/cmcev/sources/summary';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'view registration sources report',
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
  $handler->name = 'page_cmcev_source_summary_report_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cmcev_source_summary_report';
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
          2 => 2,
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
        'title' => 'Metrics',
        'width' => 100,
        'width_type' => '%',
        'parent' => 'main-row',
        'class' => '',
      ),
      1 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'chart',
        ),
        'parent' => 'main',
        'class' => '',
      ),
      'chart' => array(
        'type' => 'region',
        'title' => 'Chart',
        'width' => 100,
        'width_type' => '%',
        'parent' => '1',
        'class' => '',
      ),
      2 => array(
        'type' => 'row',
        'contains' => 'column',
        'children' => array(
          0 => 4,
          1 => 3,
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
          0 => 6,
        ),
        'class' => '',
      ),
      4 => array(
        'type' => 'column',
        'width' => '50',
        'width_type' => '%',
        'parent' => '2',
        'children' => array(
          0 => 5,
        ),
        'class' => '',
      ),
      5 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'left',
        ),
        'parent' => '4',
        'class' => '',
      ),
      'left' => array(
        'type' => 'region',
        'title' => 'Left',
        'width' => 100,
        'width_type' => '%',
        'parent' => '5',
        'class' => '',
      ),
      6 => array(
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
        'parent' => '6',
        'class' => '',
      ),
    ),
  );
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'chart' => NULL,
      'left' => NULL,
      'right' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '10217cf0-30e9-46ca-9b9b-aee8f6109e6e';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-091d1915-9238-4216-9ead-75a93bd6389e';
    $pane->panel = 'center';
    $pane->type = 'views_panes';
    $pane->subtype = 'cmcev_sources_summary_report-panel_pane_1';
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
    $pane->uuid = '091d1915-9238-4216-9ead-75a93bd6389e';
    $display->content['new-091d1915-9238-4216-9ead-75a93bd6389e'] = $pane;
    $display->panels['center'][0] = 'new-091d1915-9238-4216-9ead-75a93bd6389e';
    $pane = new stdClass();
    $pane->pid = 'new-b8654826-f4e1-4fd8-9f93-2f0718ced677';
    $pane->panel = 'center';
    $pane->type = 'cmcev_set_page_title';
    $pane->subtype = 'cmcev_set_page_title';
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'b8654826-f4e1-4fd8-9f93-2f0718ced677';
    $display->content['new-b8654826-f4e1-4fd8-9f93-2f0718ced677'] = $pane;
    $display->panels['center'][1] = 'new-b8654826-f4e1-4fd8-9f93-2f0718ced677';
    $pane = new stdClass();
    $pane->pid = 'new-c23c1801-e1cb-4f8e-bb07-519f81252eb3';
    $pane->panel = 'chart';
    $pane->type = 'source_summary_registration';
    $pane->subtype = 'source_summary_registration';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'time_period' => '30',
      'height' => '',
      'xaxis_tickmarks' => '',
      'chart_color' => '',
      'line_color' => '',
      'background_color' => '',
      'include_legend' => 0,
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
    $pane->uuid = 'c23c1801-e1cb-4f8e-bb07-519f81252eb3';
    $display->content['new-c23c1801-e1cb-4f8e-bb07-519f81252eb3'] = $pane;
    $display->panels['chart'][0] = 'new-c23c1801-e1cb-4f8e-bb07-519f81252eb3';
    $pane = new stdClass();
    $pane->pid = 'new-84b06c8b-300a-4844-a91b-7cc5d6382167';
    $pane->panel = 'left';
    $pane->type = 'source_summary_frequency';
    $pane->subtype = 'source_summary_frequency';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'color_series' => '',
      'display_labels' => 0,
      'include_legend' => 0,
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
    $pane->uuid = '84b06c8b-300a-4844-a91b-7cc5d6382167';
    $display->content['new-84b06c8b-300a-4844-a91b-7cc5d6382167'] = $pane;
    $display->panels['left'][0] = 'new-84b06c8b-300a-4844-a91b-7cc5d6382167';
    $pane = new stdClass();
    $pane->pid = 'new-bea4497f-bc9a-47e5-82d3-dc4a0a39da17';
    $pane->panel = 'right';
    $pane->type = 'source_summary_attendance';
    $pane->subtype = 'source_summary_attendance';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'color_series' => '',
      'display_labels' => 0,
      'include_legend' => 0,
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
    $pane->uuid = 'bea4497f-bc9a-47e5-82d3-dc4a0a39da17';
    $display->content['new-bea4497f-bc9a-47e5-82d3-dc4a0a39da17'] = $pane;
    $display->panels['right'][0] = 'new-bea4497f-bc9a-47e5-82d3-dc4a0a39da17';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cmcev_source_summary_report'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cmcev_sources_report';
  $page->task = 'page';
  $page->admin_title = 'CRM Core Event: Sources report';
  $page->admin_description = '';
  $page->path = 'crm-core/reports/cmcev/sources';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'view registration sources report',
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
  $handler->name = 'page_cmcev_sources_report_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cmcev_sources_report';
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
        'title' => 'Chart',
        'width' => 100,
        'width_type' => '%',
        'parent' => 'main-row',
        'class' => '',
      ),
      1 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'view',
        ),
        'parent' => 'main',
        'class' => '',
      ),
      'view' => array(
        'type' => 'region',
        'title' => 'View',
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
      'view' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Registration sources';
  $display->uuid = '03c716c2-faea-47d5-bc79-f385024ba7fb';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-f13bea30-abf7-4bf0-9e48-42d10480573f';
    $pane->panel = 'center';
    $pane->type = 'source_chart';
    $pane->subtype = 'source_chart';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'color_series' => '',
      'display_labels' => 0,
      'chart_color' => '',
      'column_width' => '',
      'include_legend' => 0,
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
    $pane->uuid = 'f13bea30-abf7-4bf0-9e48-42d10480573f';
    $display->content['new-f13bea30-abf7-4bf0-9e48-42d10480573f'] = $pane;
    $display->panels['center'][0] = 'new-f13bea30-abf7-4bf0-9e48-42d10480573f';
    $pane = new stdClass();
    $pane->pid = 'new-d10e7527-d2cb-49e2-91f2-0f9cca4cc4cc';
    $pane->panel = 'view';
    $pane->type = 'views_panes';
    $pane->subtype = 'cmcev_sources_report-panel_pane_1';
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
    $pane->uuid = 'd10e7527-d2cb-49e2-91f2-0f9cca4cc4cc';
    $display->content['new-d10e7527-d2cb-49e2-91f2-0f9cca4cc4cc'] = $pane;
    $display->panels['view'][0] = 'new-d10e7527-d2cb-49e2-91f2-0f9cca4cc4cc';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cmcev_sources_report'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'event_registration';
  $page->task = 'page';
  $page->admin_title = 'Event Registrants';
  $page->admin_description = 'Displays a list of people who have registered for an event.';
  $page->path = 'node/%node/registration';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'entity_bundle:node',
        'settings' => array(
          'type' => array(
            'cmcev_event' => 'cmcev_event',
          ),
        ),
        'context' => 'argument_entity_id:node_1',
        'not' => FALSE,
      ),
      1 => array(
        'name' => 'php',
        'settings' => array(
          'description' => 'Calls access callback for this panel',
          'php' => 'return crm_core_event_access_registrations($contexts);',
        ),
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
  $handler->name = 'page_event_registration_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'event_registration';
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
    'access' => array(
      'plugins' => array(),
      'logic' => 'and',
    ),
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
  $display->title = 'Registrations for %node:title';
  $display->uuid = '97483a70-a2c7-4dca-9b49-347d48983a38';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-dfde15b6-f703-49e0-9117-43f3acd9ff60';
    $pane->panel = 'center';
    $pane->type = 'views_panes';
    $pane->subtype = 'cmcev_registrants-panel_pane_1';
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
    $pane->uuid = 'dfde15b6-f703-49e0-9117-43f3acd9ff60';
    $display->content['new-dfde15b6-f703-49e0-9117-43f3acd9ff60'] = $pane;
    $display->panels['center'][0] = 'new-dfde15b6-f703-49e0-9117-43f3acd9ff60';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['event_registration'] = $page;

  return $pages;

}
