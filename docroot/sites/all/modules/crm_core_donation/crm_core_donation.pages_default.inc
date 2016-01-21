<?php
/**
 * @file
 * crm_core_donation.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function crm_core_donation_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cmcd_donation_source_report';
  $page->task = 'page';
  $page->admin_title = 'Donation Source Report';
  $page->admin_description = '';
  $page->path = 'crm-core/reports/cmcd/donation-sources';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'tab',
    'title' => 'Donation Sources',
    'name' => 'navigation',
    'weight' => '-9',
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
  $handler->name = 'page_cmcd_donation_source_report_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cmcd_donation_source_report';
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
  $display->title = 'Donation Sources ';
  $display->uuid = '75934044-8ac7-45bb-adaf-21c2b5cba5cc';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-71b684b0-2c70-499f-a45b-2e9acedcdfab';
    $pane->panel = 'center';
    $pane->type = 'donation_by_source_bar_chart';
    $pane->subtype = 'donation_by_source_bar_chart';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'color_series' => '',
      'display_labels' => 0,
      'chart_color' => '',
      'include_legend' => 0,
      'override_title' => 0,
      'override_title_text' => '',
      'column_width' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '71b684b0-2c70-499f-a45b-2e9acedcdfab';
    $display->content['new-71b684b0-2c70-499f-a45b-2e9acedcdfab'] = $pane;
    $display->panels['center'][0] = 'new-71b684b0-2c70-499f-a45b-2e9acedcdfab';
    $pane = new stdClass();
    $pane->pid = 'new-3a2504a7-87ee-44c0-aa05-3b22dd66ad48';
    $pane->panel = 'center_';
    $pane->type = 'views_panes';
    $pane->subtype = 'cmcd_source_report-panel_pane_1';
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
    $pane->uuid = '3a2504a7-87ee-44c0-aa05-3b22dd66ad48';
    $display->content['new-3a2504a7-87ee-44c0-aa05-3b22dd66ad48'] = $pane;
    $display->panels['center_'][0] = 'new-3a2504a7-87ee-44c0-aa05-3b22dd66ad48';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cmcd_donation_source_report'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cmcd_overview';
  $page->task = 'page';
  $page->admin_title = 'Donations Overview';
  $page->admin_description = 'This is the donations overview report. It is a dashboard for understanding donation activity.';
  $page->path = 'crm-core/reports/cmcd/overview';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'view donation summary report',
        ),
        'context' => 'logged-in-user',
        'not' => FALSE,
      ),
      1 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'view donation reports',
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
    'title' => 'Donation Overview',
    'name' => 'navigation',
    'weight' => '-10',
    'parent' => array(
      'type' => 'normal',
      'title' => 'Donations Reports',
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
  $handler->name = 'page_cmcd_overview_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cmcd_overview';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Able Organizer Report Overview Panel Template',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
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
        'class' => 'container-12',
      ),
      'main-row' => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'left_region',
          1 => 'center',
        ),
        'parent' => 'main',
      ),
      'center' => array(
        'type' => 'region',
        'title' => 'Right region',
        'width' => 50,
        'width_type' => '%',
        'parent' => 'main-row',
        'class' => 'grid-6',
      ),
      'left_region' => array(
        'type' => 'region',
        'title' => 'Left region',
        'width' => 50,
        'width_type' => '%',
        'parent' => 'main-row',
        'class' => 'grid-6',
      ),
      1 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'left_region_',
          1 => 'region_region',
        ),
        'parent' => 'main',
        'class' => '',
      ),
      'left_region_' => array(
        'type' => 'region',
        'title' => 'Left region',
        'width' => 50,
        'width_type' => '%',
        'parent' => '1',
        'class' => 'grid-6',
      ),
      'region_region' => array(
        'type' => 'region',
        'title' => 'Right Region',
        'width' => 50,
        'width_type' => '%',
        'parent' => '1',
        'class' => 'grid-6',
      ),
    ),
  );
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'left_region' => NULL,
      'left_region_' => NULL,
      'region_region' => NULL,
    ),
    'left_region' => array(
      'style' => '-1',
    ),
  );
  $display->cache = array();
  $display->title = 'Donations Overview';
  $display->uuid = 'f349612d-b83e-4945-9175-0f5c9844a552';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-81680325-d04e-4637-a07b-339231115391';
    $pane->panel = 'center';
    $pane->type = 'donation_by_source_chart';
    $pane->subtype = 'donation_by_source_chart';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'days_to_display' => '90',
      'height' => '',
      'xaxis_tickmarks' => '',
      'chart_color' => '',
      'line_color' => '',
      'background_color' => '',
      'include_legend' => 1,
      'override_title' => 0,
      'override_title_text' => '',
      'color_series' => '',
      'display_labels' => 0,
      'include_tooltip' => 1,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '81680325-d04e-4637-a07b-339231115391';
    $display->content['new-81680325-d04e-4637-a07b-339231115391'] = $pane;
    $display->panels['center'][0] = 'new-81680325-d04e-4637-a07b-339231115391';
    $pane = new stdClass();
    $pane->pid = 'new-727f9e09-ed19-41ba-802d-5f90ccf1acf5';
    $pane->panel = 'left_region';
    $pane->type = 'donation_history_chart';
    $pane->subtype = 'donation_history_chart';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 0,
      'override_title_text' => '',
      'donors_to_display' => NULL,
      'days_to_display' => '30',
      'xaxis_tickmarks' => '',
      'line_color' => '',
      'background_color' => '',
      'chart_color' => '',
      'height' => '',
      'include_legend' => 0,
      'include_tooltip' => 1,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '727f9e09-ed19-41ba-802d-5f90ccf1acf5';
    $display->content['new-727f9e09-ed19-41ba-802d-5f90ccf1acf5'] = $pane;
    $display->panels['left_region'][0] = 'new-727f9e09-ed19-41ba-802d-5f90ccf1acf5';
    $pane = new stdClass();
    $pane->pid = 'new-b74ba2ee-ebb3-4e6a-8323-c33adb26929d';
    $pane->panel = 'left_region_';
    $pane->type = 'views_panes';
    $pane->subtype = 'cmcd_details_callouts-panel_pane_1';
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
    $pane->uuid = 'b74ba2ee-ebb3-4e6a-8323-c33adb26929d';
    $display->content['new-b74ba2ee-ebb3-4e6a-8323-c33adb26929d'] = $pane;
    $display->panels['left_region_'][0] = 'new-b74ba2ee-ebb3-4e6a-8323-c33adb26929d';
    $pane = new stdClass();
    $pane->pid = 'new-ae15fa72-3933-4e64-9242-d8deeeb244d0';
    $pane->panel = 'region_region';
    $pane->type = 'views_panes';
    $pane->subtype = 'cmcd_source_callouts-panel_pane_1';
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
    $pane->uuid = 'ae15fa72-3933-4e64-9242-d8deeeb244d0';
    $display->content['new-ae15fa72-3933-4e64-9242-d8deeeb244d0'] = $pane;
    $display->panels['region_region'][0] = 'new-ae15fa72-3933-4e64-9242-d8deeeb244d0';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cmcd_overview'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'donation_overview';
  $page->task = 'page';
  $page->admin_title = 'Donation Overview';
  $page->admin_description = 'Overview of donation page activity in the system.';
  $page->path = 'crm-core/reports/cmcd/donation-pages';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'view donation details report',
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
    'title' => 'Online Donation Pages',
    'name' => 'navigation',
    'weight' => '-8',
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
  $handler->name = 'page_donation_overview_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'donation_overview';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'logic' => 'and',
    ),
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
      'left' => NULL,
      'region_right' => NULL,
      'center_' => NULL,
      'center__' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Online donation pages';
  $display->uuid = 'dc80e795-e8f6-4e7e-90b5-b8f212be6aa4';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-127f0771-a9b2-4040-af18-65a8d8c61153';
    $pane->panel = 'center';
    $pane->type = 'donation_page_overview_graph';
    $pane->subtype = 'donation_page_overview_graph';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'text' => '',
      'donation_levels' => '1,10|11,20|21,50|51,100|101,250|251,500|501,1000',
      'override_title' => 0,
      'override_title_text' => '',
      'include_legend' => 0,
      'height' => '',
      'column_width' => '',
      'color_series' => '',
      'display_labels' => 0,
      'chart_color' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '127f0771-a9b2-4040-af18-65a8d8c61153';
    $display->content['new-127f0771-a9b2-4040-af18-65a8d8c61153'] = $pane;
    $display->panels['center'][0] = 'new-127f0771-a9b2-4040-af18-65a8d8c61153';
    $pane = new stdClass();
    $pane->pid = 'new-6d4f93a6-6af1-4e51-b6d7-e0eb46a54a8a';
    $pane->panel = 'center_';
    $pane->type = 'views';
    $pane->subtype = 'donation_page_overview';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 0,
      'nodes_per_page' => '0',
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
    $pane->uuid = '6d4f93a6-6af1-4e51-b6d7-e0eb46a54a8a';
    $display->content['new-6d4f93a6-6af1-4e51-b6d7-e0eb46a54a8a'] = $pane;
    $display->panels['center_'][0] = 'new-6d4f93a6-6af1-4e51-b6d7-e0eb46a54a8a';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['donation_overview'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'donation_page_summary';
  $page->task = 'page';
  $page->admin_title = 'Donation Page Summary';
  $page->admin_description = 'This is a summary report for a selected donation page.';
  $page->path = 'crm-core/reports/cmcd/donation-pages/summary';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'view donation page details report',
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
    'title' => 'Donation Page Summary',
    'name' => 'navigation',
    'weight' => '0',
    'parent' => array(
      'type' => 'tab',
      'title' => 'Donation Page Summary',
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
  $handler->name = 'page_donation_page_summary_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'donation_page_summary';
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
      2 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'left',
          1 => 'right',
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
      'left' => array(
        'type' => 'region',
        'title' => 'Left',
        'width' => 50,
        'width_type' => '%',
        'parent' => '2',
        'class' => '',
      ),
      'right' => array(
        'type' => 'region',
        'title' => 'Right',
        'width' => 50,
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
      'left' => NULL,
      'region_right' => NULL,
      'center_' => NULL,
      'center__' => NULL,
      'right' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'b921e7d8-065f-4430-812b-45e6fb146113';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-a8a56ac1-7e42-4c52-b626-3b6d9c15a98e';
    $pane->panel = 'center';
    $pane->type = 'views_panes';
    $pane->subtype = 'donations_page_total_figures-panel_pane_1';
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
    $pane->uuid = 'a8a56ac1-7e42-4c52-b626-3b6d9c15a98e';
    $display->content['new-a8a56ac1-7e42-4c52-b626-3b6d9c15a98e'] = $pane;
    $display->panels['center'][0] = 'new-a8a56ac1-7e42-4c52-b626-3b6d9c15a98e';
    $pane = new stdClass();
    $pane->pid = 'new-d4671325-7ba8-43a4-b6e7-48a538e38947';
    $pane->panel = 'center';
    $pane->type = 'donation_set_page_title';
    $pane->subtype = 'donation_set_page_title';
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
    $pane->uuid = 'd4671325-7ba8-43a4-b6e7-48a538e38947';
    $display->content['new-d4671325-7ba8-43a4-b6e7-48a538e38947'] = $pane;
    $display->panels['center'][1] = 'new-d4671325-7ba8-43a4-b6e7-48a538e38947';
    $pane = new stdClass();
    $pane->pid = 'new-0e126add-acce-4600-981b-8b4fa3db1f7d';
    $pane->panel = 'center_';
    $pane->type = 'donation_page_history_chart';
    $pane->subtype = 'donation_page_history_chart';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'text' => '',
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
    $pane->uuid = '0e126add-acce-4600-981b-8b4fa3db1f7d';
    $display->content['new-0e126add-acce-4600-981b-8b4fa3db1f7d'] = $pane;
    $display->panels['center_'][0] = 'new-0e126add-acce-4600-981b-8b4fa3db1f7d';
    $pane = new stdClass();
    $pane->pid = 'new-49ddad1d-edf4-45b4-a8eb-26341d98c6c6';
    $pane->panel = 'left';
    $pane->type = 'donation_page_day_table';
    $pane->subtype = 'donation_page_day_table';
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
    $pane->uuid = '49ddad1d-edf4-45b4-a8eb-26341d98c6c6';
    $display->content['new-49ddad1d-edf4-45b4-a8eb-26341d98c6c6'] = $pane;
    $display->panels['left'][0] = 'new-49ddad1d-edf4-45b4-a8eb-26341d98c6c6';
    $pane = new stdClass();
    $pane->pid = 'new-c1c9acce-3d83-4449-87b1-fec3e99402c5';
    $pane->panel = 'right';
    $pane->type = 'donation_page_time_table';
    $pane->subtype = 'donation_page_time_table';
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
    $pane->uuid = 'c1c9acce-3d83-4449-87b1-fec3e99402c5';
    $display->content['new-c1c9acce-3d83-4449-87b1-fec3e99402c5'] = $pane;
    $display->panels['right'][0] = 'new-c1c9acce-3d83-4449-87b1-fec3e99402c5';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['donation_page_summary'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'donation_source_summary';
  $page->task = 'page';
  $page->admin_title = 'Donation Source Summary';
  $page->admin_description = 'Provides summary figures and metrics for a specific donation source.';
  $page->path = 'crm-core/reports/cmcd/donation-sources/summary';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'view donation source summary report',
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
    'title' => 'Source Summary',
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
  $handler->name = 'page_donation_source_summary_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'donation_source_summary';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Donation Source Summary',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
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
          0 => 2,
          1 => 1,
          2 => 'main-row',
        ),
        'parent' => 'canvas',
      ),
      'main-row' => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'left',
          1 => 'center',
          2 => 'region_right',
        ),
        'parent' => 'main',
      ),
      'center' => array(
        'type' => 'region',
        'title' => 'Center',
        'width' => '33.35516342047596',
        'width_type' => '%',
        'parent' => 'main-row',
        'class' => 'grid-4',
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
      'left' => array(
        'type' => 'region',
        'title' => 'Left',
        'width' => '33.305444309616',
        'width_type' => '%',
        'parent' => 'main-row',
        'class' => 'grid-4',
      ),
      'region_right' => array(
        'type' => 'region',
        'title' => 'Right',
        'width' => '33.33939226990804',
        'width_type' => '%',
        'parent' => 'main-row',
        'class' => 'grid-4',
      ),
      'center_' => array(
        'type' => 'region',
        'title' => 'Center',
        'width' => 100,
        'width_type' => '%',
        'parent' => '1',
        'class' => 'grid-12',
      ),
      2 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'center__',
        ),
        'parent' => 'main',
        'class' => '',
      ),
      'center__' => array(
        'type' => 'region',
        'title' => 'Center',
        'width' => 100,
        'width_type' => '%',
        'parent' => '2',
        'class' => 'grid-12',
      ),
    ),
  );
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'left' => NULL,
      'region_right' => NULL,
      'center_' => NULL,
      'center__' => NULL,
    ),
    'left' => array(
      'style' => 'naked',
    ),
    'center' => array(
      'style' => 'naked',
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '841e0363-2c5b-4601-83d5-0034c5903cb0';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-156e0d36-6fc5-45da-8528-2a6fb4b9a1f1';
    $pane->panel = 'center';
    $pane->type = 'donation_source_average_graph';
    $pane->subtype = 'donation_source_average_graph';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'text' => '',
      'donation_levels' => '1,10|11,20|21,50|51,100|101,250|251,500|501,1000',
      'override_title' => 0,
      'override_title_text' => '',
      'include_legend' => 0,
      'height' => '',
      'color_series' => '',
      'display_labels' => 0,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '156e0d36-6fc5-45da-8528-2a6fb4b9a1f1';
    $display->content['new-156e0d36-6fc5-45da-8528-2a6fb4b9a1f1'] = $pane;
    $display->panels['center'][0] = 'new-156e0d36-6fc5-45da-8528-2a6fb4b9a1f1';
    $pane = new stdClass();
    $pane->pid = 'new-3d36a372-e583-4b5c-9072-89a40c6dcdc0';
    $pane->panel = 'center_';
    $pane->type = 'donation_source_summary_chart';
    $pane->subtype = 'donation_source_summary_chart';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'text' => '',
      'override_title' => 0,
      'override_title_text' => '',
      'days_to_display' => '90',
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
    $pane->uuid = '3d36a372-e583-4b5c-9072-89a40c6dcdc0';
    $display->content['new-3d36a372-e583-4b5c-9072-89a40c6dcdc0'] = $pane;
    $display->panels['center_'][0] = 'new-3d36a372-e583-4b5c-9072-89a40c6dcdc0';
    $pane = new stdClass();
    $pane->pid = 'new-9eb9f1a6-62f3-473e-806a-583dbad5fd5b';
    $pane->panel = 'center__';
    $pane->type = 'donation_set_page_title';
    $pane->subtype = 'donation_set_page_title';
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
    $pane->uuid = '9eb9f1a6-62f3-473e-806a-583dbad5fd5b';
    $display->content['new-9eb9f1a6-62f3-473e-806a-583dbad5fd5b'] = $pane;
    $display->panels['center__'][0] = 'new-9eb9f1a6-62f3-473e-806a-583dbad5fd5b';
    $pane = new stdClass();
    $pane->pid = 'new-c561a5d2-1798-4211-94a6-51833f54d12d';
    $pane->panel = 'center__';
    $pane->type = 'views_panes';
    $pane->subtype = 'cmcd_source_total_figures-panel_pane_1';
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
    $pane->uuid = 'c561a5d2-1798-4211-94a6-51833f54d12d';
    $display->content['new-c561a5d2-1798-4211-94a6-51833f54d12d'] = $pane;
    $display->panels['center__'][1] = 'new-c561a5d2-1798-4211-94a6-51833f54d12d';
    $pane = new stdClass();
    $pane->pid = 'new-33272808-5c5a-4c08-8ec0-a64764bedad0';
    $pane->panel = 'left';
    $pane->type = 'donation_source_frequency_graph';
    $pane->subtype = 'donation_source_frequency_graph';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'text' => '',
      'override_title' => 0,
      'override_title_text' => '',
      'include_legend' => 0,
      'height' => '',
      'color_series' => '',
      'display_labels' => 0,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '33272808-5c5a-4c08-8ec0-a64764bedad0';
    $display->content['new-33272808-5c5a-4c08-8ec0-a64764bedad0'] = $pane;
    $display->panels['left'][0] = 'new-33272808-5c5a-4c08-8ec0-a64764bedad0';
    $pane = new stdClass();
    $pane->pid = 'new-85384aac-c5f8-4150-9de1-30fe96deaf3e';
    $pane->panel = 'region_right';
    $pane->type = 'donation_source_os_graph';
    $pane->subtype = 'donation_source_os_graph';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'text' => '',
      'override_title' => 0,
      'override_title_text' => '',
      'include_legend' => 0,
      'height' => '',
      'color_series' => '',
      'display_labels' => 0,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '85384aac-c5f8-4150-9de1-30fe96deaf3e';
    $display->content['new-85384aac-c5f8-4150-9de1-30fe96deaf3e'] = $pane;
    $display->panels['region_right'][0] = 'new-85384aac-c5f8-4150-9de1-30fe96deaf3e';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['donation_source_summary'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'donor_overview';
  $page->task = 'page';
  $page->admin_title = 'Donor Overview';
  $page->admin_description = 'This is the donor overview page. It displays some basic information about donor activity, and includes links to additional reports.';
  $page->path = 'crm-core/reports/cmcd-donors/donors';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'view donor reports',
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
    'title' => 'Donor Overview',
    'name' => 'navigation',
    'weight' => '-10',
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
  $handler->name = 'page_donor_overview_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'donor_overview';
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
          1 => 'top_right',
        ),
        'parent' => 'main',
      ),
      'center' => array(
        'type' => 'region',
        'title' => 'Top left',
        'width' => 50,
        'width_type' => '%',
        'parent' => 'main-row',
        'class' => '',
      ),
      'top_right' => array(
        'type' => 'region',
        'title' => 'Top right',
        'width' => 50,
        'width_type' => '%',
        'parent' => 'main-row',
        'class' => '',
      ),
      1 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'bottom_left',
          1 => 'bottom_right',
        ),
        'parent' => 'main',
        'class' => '',
      ),
      'bottom_left' => array(
        'type' => 'region',
        'title' => 'Bottom left',
        'width' => 50,
        'width_type' => '%',
        'parent' => '1',
        'class' => '',
      ),
      'bottom_right' => array(
        'type' => 'region',
        'title' => 'Bottom right',
        'width' => 50,
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
      'left_region' => NULL,
      'left_region_' => NULL,
      'region_region' => NULL,
      'top_right' => NULL,
      'bottom_left' => NULL,
      'bottom_right' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Donor Overview';
  $display->uuid = 'ac41da64-161c-4afd-9cc8-837a6962cf1c';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-36f3ecc5-0468-45a0-8b27-bb9c19dad4b5';
    $pane->panel = 'bottom_left';
    $pane->type = 'donor_recent_donors_table';
    $pane->subtype = 'donor_recent_donors_table';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'donors_to_display' => '10',
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
    $pane->uuid = '36f3ecc5-0468-45a0-8b27-bb9c19dad4b5';
    $display->content['new-36f3ecc5-0468-45a0-8b27-bb9c19dad4b5'] = $pane;
    $display->panels['bottom_left'][0] = 'new-36f3ecc5-0468-45a0-8b27-bb9c19dad4b5';
    $pane = new stdClass();
    $pane->pid = 'new-07e6b5d2-5aac-4158-802b-836215cfc3cc';
    $pane->panel = 'bottom_right';
    $pane->type = 'views_panes';
    $pane->subtype = 'donors_top_donors-panel_pane_1';
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
    $pane->uuid = '07e6b5d2-5aac-4158-802b-836215cfc3cc';
    $display->content['new-07e6b5d2-5aac-4158-802b-836215cfc3cc'] = $pane;
    $display->panels['bottom_right'][0] = 'new-07e6b5d2-5aac-4158-802b-836215cfc3cc';
    $pane = new stdClass();
    $pane->pid = 'new-74f418b2-dc89-45e8-adb7-e2ad9ba9defb';
    $pane->panel = 'center';
    $pane->type = 'donor_history_chart';
    $pane->subtype = 'donor_history_chart';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'xaxis_tickmarks' => '',
      'chart_color' => '',
      'line_color' => '',
      'background_color' => '',
      'include_legend' => 0,
      'override_title' => 0,
      'override_title_text' => '',
      'include_tooltip' => 1,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '74f418b2-dc89-45e8-adb7-e2ad9ba9defb';
    $display->content['new-74f418b2-dc89-45e8-adb7-e2ad9ba9defb'] = $pane;
    $display->panels['center'][0] = 'new-74f418b2-dc89-45e8-adb7-e2ad9ba9defb';
    $pane = new stdClass();
    $pane->pid = 'new-d6a90923-d930-4456-8082-3346250e7b94';
    $pane->panel = 'top_right';
    $pane->type = 'donors_by_source';
    $pane->subtype = 'donors_by_source';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'color_series' => '',
      'display_labels' => 0,
      'include_legend' => 1,
      'override_title' => 0,
      'override_title_text' => '',
      'include_tooltip' => 1,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'd6a90923-d930-4456-8082-3346250e7b94';
    $display->content['new-d6a90923-d930-4456-8082-3346250e7b94'] = $pane;
    $display->panels['top_right'][0] = 'new-d6a90923-d930-4456-8082-3346250e7b94';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['donor_overview'] = $page;

  return $pages;

}
