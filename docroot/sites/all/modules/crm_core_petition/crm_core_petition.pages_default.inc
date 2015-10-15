<?php
/**
 * @file
 * crm_core_petition.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function crm_core_petition_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cmcp_petition_overview';
  $page->task = 'page';
  $page->admin_title = 'Petitions Overview Report';
  $page->admin_description = '';
  $page->path = 'crm-core/reports/cmcp/petition-overview';
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
    'weight' => '0',
    'parent' => array(
      'type' => 'normal',
      'title' => 'Petitions',
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
  $handler->name = 'page_cmcp_petition_overview_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cmcp_petition_overview';
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
      'top' => NULL,
      'left' => NULL,
      'right' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Petition reports';
  $display->uuid = '330690a4-f386-415c-8572-24bd2d4a4e76';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-d88b43bf-5106-4460-8ff8-593d26684e65';
    $pane->panel = 'header';
    $pane->type = 'cmcp_overview_metrics';
    $pane->subtype = 'cmcp_overview_metrics';
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
    $pane->uuid = 'd88b43bf-5106-4460-8ff8-593d26684e65';
    $display->content['new-d88b43bf-5106-4460-8ff8-593d26684e65'] = $pane;
    $display->panels['header'][0] = 'new-d88b43bf-5106-4460-8ff8-593d26684e65';
    $pane = new stdClass();
    $pane->pid = 'new-33de21c3-3849-4818-bcbc-35dd1b7f7805';
    $pane->panel = 'left_1';
    $pane->type = 'cmcp_signature_activity';
    $pane->subtype = 'cmcp_signature_activity';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'time_period' => '90',
      'height' => 0,
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
    $pane->uuid = '33de21c3-3849-4818-bcbc-35dd1b7f7805';
    $display->content['new-33de21c3-3849-4818-bcbc-35dd1b7f7805'] = $pane;
    $display->panels['left_1'][0] = 'new-33de21c3-3849-4818-bcbc-35dd1b7f7805';
    $pane = new stdClass();
    $pane->pid = 'new-d642a2f4-f235-4637-a761-e8ec653cc680';
    $pane->panel = 'left_2';
    $pane->type = 'views_panes';
    $pane->subtype = 'cmcp_recent_signatures-panel_pane_1';
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
    $pane->uuid = 'd642a2f4-f235-4637-a761-e8ec653cc680';
    $display->content['new-d642a2f4-f235-4637-a761-e8ec653cc680'] = $pane;
    $display->panels['left_2'][0] = 'new-d642a2f4-f235-4637-a761-e8ec653cc680';
    $pane = new stdClass();
    $pane->pid = 'new-d2e7c36c-f997-4ee9-ab15-6a158c8c260e';
    $pane->panel = 'right_1';
    $pane->type = 'cmcp_source_chart';
    $pane->subtype = 'cmcp_source_chart';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'color_series' => '',
      'display_labels' => '',
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
    $pane->uuid = 'd2e7c36c-f997-4ee9-ab15-6a158c8c260e';
    $display->content['new-d2e7c36c-f997-4ee9-ab15-6a158c8c260e'] = $pane;
    $display->panels['right_1'][0] = 'new-d2e7c36c-f997-4ee9-ab15-6a158c8c260e';
    $pane = new stdClass();
    $pane->pid = 'new-2ccd16e4-753d-4138-9d73-2d576f1ff3cb';
    $pane->panel = 'right_2';
    $pane->type = 'views_panes';
    $pane->subtype = 'petition_top_sources-panel_pane_1';
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
    $pane->uuid = '2ccd16e4-753d-4138-9d73-2d576f1ff3cb';
    $display->content['new-2ccd16e4-753d-4138-9d73-2d576f1ff3cb'] = $pane;
    $display->panels['right_2'][0] = 'new-2ccd16e4-753d-4138-9d73-2d576f1ff3cb';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cmcp_petition_overview'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cmcp_petitions_page_summary';
  $page->task = 'page';
  $page->admin_title = 'Petitions Page Summary';
  $page->admin_description = 'This is the summary report for petition pages. ';
  $page->path = 'crm-core/reports/cmcp/petition-report/summary';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'view petitions report',
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
    'title' => 'Petition Page Summary',
    'name' => 'navigation',
    'weight' => '0',
    'parent' => array(
      'type' => 'tab',
      'title' => 'Petitions',
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
  $handler->name = 'page_cmcp_petitions_page_summary_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cmcp_petitions_page_summary';
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
          1 => 'right',
        ),
        'parent' => 'main',
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
        'class' => '',
      ),
      'left' => array(
        'type' => 'region',
        'title' => 'Left',
        'width' => '49.95076554759328',
        'width_type' => '%',
        'parent' => 'main-row',
        'class' => '',
      ),
      'right' => array(
        'type' => 'region',
        'title' => 'Right',
        'width' => '50.04923445244912',
        'width_type' => '%',
        'parent' => 'main-row',
        'class' => '',
      ),
    ),
  );
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center_' => NULL,
      'center__' => NULL,
      'left' => NULL,
      'center' => NULL,
      'right' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'ecccfdbd-c57a-4fcf-b4bc-e542354a1064';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-6305e6fc-e393-488b-b3e8-eaa6ce7aa494';
    $pane->panel = 'center_';
    $pane->type = 'cmcp_petition_page_pp_chart';
    $pane->subtype = 'cmcp_petition_page_pp_chart';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'xaxis_tickmarks' => '',
      'chart_color' => '',
      'line_color' => '',
      'background_color' => '',
      'include_legend' => 0,
      'time_period' => '90',
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
    $pane->uuid = '6305e6fc-e393-488b-b3e8-eaa6ce7aa494';
    $display->content['new-6305e6fc-e393-488b-b3e8-eaa6ce7aa494'] = $pane;
    $display->panels['center_'][0] = 'new-6305e6fc-e393-488b-b3e8-eaa6ce7aa494';
    $pane = new stdClass();
    $pane->pid = 'new-f1b5010c-c56f-4d35-b1a9-f1324ba369a0';
    $pane->panel = 'center__';
    $pane->type = 'views_panes';
    $pane->subtype = 'petition_page_performance_summary-panel_pane_1';
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
    $pane->uuid = 'f1b5010c-c56f-4d35-b1a9-f1324ba369a0';
    $display->content['new-f1b5010c-c56f-4d35-b1a9-f1324ba369a0'] = $pane;
    $display->panels['center__'][0] = 'new-f1b5010c-c56f-4d35-b1a9-f1324ba369a0';
    $pane = new stdClass();
    $pane->pid = 'new-a81a08a0-71dd-45bc-9e98-02c4966c75ed';
    $pane->panel = 'center__';
    $pane->type = 'cmcp_set_page_page_title';
    $pane->subtype = 'cmcp_set_page_page_title';
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
    $pane->uuid = 'a81a08a0-71dd-45bc-9e98-02c4966c75ed';
    $display->content['new-a81a08a0-71dd-45bc-9e98-02c4966c75ed'] = $pane;
    $display->panels['center__'][1] = 'new-a81a08a0-71dd-45bc-9e98-02c4966c75ed';
    $pane = new stdClass();
    $pane->pid = 'new-342159a9-26dd-41af-94f8-c834e2ada7ef';
    $pane->panel = 'left';
    $pane->type = 'cmcp_page_source_summary';
    $pane->subtype = 'cmcp_page_source_summary';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'color_series' => '',
      'display_labels' => '',
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
    $pane->uuid = '342159a9-26dd-41af-94f8-c834e2ada7ef';
    $display->content['new-342159a9-26dd-41af-94f8-c834e2ada7ef'] = $pane;
    $display->panels['left'][0] = 'new-342159a9-26dd-41af-94f8-c834e2ada7ef';
    $pane = new stdClass();
    $pane->pid = 'new-4ce4fab4-674e-481d-8f99-587f903d9555';
    $pane->panel = 'right';
    $pane->type = 'cmcp_other_petitions';
    $pane->subtype = 'cmcp_other_petitions';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'color_series' => '',
      'display_labels' => '',
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
    $pane->uuid = '4ce4fab4-674e-481d-8f99-587f903d9555';
    $display->content['new-4ce4fab4-674e-481d-8f99-587f903d9555'] = $pane;
    $display->panels['right'][0] = 'new-4ce4fab4-674e-481d-8f99-587f903d9555';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cmcp_petitions_page_summary'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cmcp_signatories_single_view';
  $page->task = 'page';
  $page->admin_title = 'Petition Signatories Report';
  $page->admin_description = '';
  $page->path = 'node/%node/signatories';
  $page->access = array(
    'logic' => 'and',
    'plugins' => array(
      2 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'view petition signatories',
        ),
        'context' => 'logged-in-user',
        'not' => FALSE,
      ),
      3 => array(
        'name' => 'php',
        'settings' => array(
          'description' => 'Checks petition configuration',
          'php' => 'return _crm_core_petition_signatories_access($contexts);',
        ),
        'not' => FALSE,
      ),
    ),
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'tab',
    'title' => 'Signatories',
    'name' => 'navigation',
    'weight' => '5',
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
  $handler->name = 'page_cmcp_signatories_single_view_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cmcp_signatories_single_view';
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
  $display->title = 'Signatories for %node:title';
  $display->uuid = 'a5f590be-9e3b-49d7-9154-b7f1cbff3ca2';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-c31d9487-33c9-49c8-9817-38de74ba8fa4';
    $pane->panel = 'center';
    $pane->type = 'block';
    $pane->subtype = 'crm_core_petition-petition_signatories_summary';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
      'override_title_text' => 'Signatories',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'c31d9487-33c9-49c8-9817-38de74ba8fa4';
    $display->content['new-c31d9487-33c9-49c8-9817-38de74ba8fa4'] = $pane;
    $display->panels['center'][0] = 'new-c31d9487-33c9-49c8-9817-38de74ba8fa4';
    $pane = new stdClass();
    $pane->pid = 'new-efe788f1-160f-4d7d-a8fd-aa743b57fbf8';
    $pane->panel = 'center';
    $pane->type = 'views';
    $pane->subtype = 'cmcp_signatories_single_view';
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
      'args' => '',
      'url' => '',
      'display' => 'default',
      'context' => array(
        0 => '',
      ),
      'override_title' => 1,
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
    $pane->uuid = 'efe788f1-160f-4d7d-a8fd-aa743b57fbf8';
    $display->content['new-efe788f1-160f-4d7d-a8fd-aa743b57fbf8'] = $pane;
    $display->panels['center'][1] = 'new-efe788f1-160f-4d7d-a8fd-aa743b57fbf8';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-efe788f1-160f-4d7d-a8fd-aa743b57fbf8';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cmcp_signatories_single_view'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cmcp_source_sammary_report';
  $page->task = 'page';
  $page->admin_title = 'CRM Core Petitions - Source Summary Report';
  $page->admin_description = '';
  $page->path = 'crm-core/reports/cmcp/sources-report/summary';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'tab',
    'title' => 'Summary',
    'name' => 'navigation',
    'weight' => '0',
    'parent' => array(
      'type' => 'tab',
      'title' => 'Summary',
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
  $handler->name = 'page_cmcp_source_sammary_report_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cmcp_source_sammary_report';
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
        'title' => 'TOP',
        'width' => 100,
        'width_type' => '%',
        'parent' => 'main-row',
        'class' => '',
      ),
      1 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'middle',
        ),
        'parent' => 'main',
        'class' => '',
      ),
      2 => array(
        'type' => 'row',
        'contains' => 'column',
        'children' => array(
          0 => 5,
          1 => 4,
          2 => 3,
        ),
        'parent' => 'main',
        'class' => '',
      ),
      3 => array(
        'type' => 'column',
        'width' => '33.74265531128276',
        'width_type' => '%',
        'parent' => '2',
        'children' => array(
          0 => 8,
        ),
        'class' => '',
      ),
      4 => array(
        'type' => 'column',
        'width' => '33.08741004819437',
        'width_type' => '%',
        'parent' => '2',
        'children' => array(
          0 => 7,
        ),
        'class' => '',
      ),
      5 => array(
        'type' => 'column',
        'width' => '33.169934640522875',
        'width_type' => '%',
        'parent' => '2',
        'children' => array(
          0 => 6,
        ),
        'class' => '',
      ),
      'middle' => array(
        'type' => 'region',
        'title' => 'Middle',
        'width' => 100,
        'width_type' => '%',
        'parent' => '1',
        'class' => '',
      ),
      6 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'bottom_left',
        ),
        'parent' => '5',
        'class' => '',
      ),
      'bottom_left' => array(
        'type' => 'region',
        'title' => 'Bottom Left',
        'width' => 100,
        'width_type' => '%',
        'parent' => '6',
        'class' => '',
      ),
      7 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'bottom_center',
        ),
        'parent' => '4',
        'class' => '',
      ),
      'bottom_center' => array(
        'type' => 'region',
        'title' => 'Bottom Center',
        'width' => 100,
        'width_type' => '%',
        'parent' => '7',
        'class' => '',
      ),
      8 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'bottom_right',
        ),
        'parent' => '3',
        'class' => '',
      ),
      'bottom_right' => array(
        'type' => 'region',
        'title' => 'Bottom Right',
        'width' => 100,
        'width_type' => '%',
        'parent' => '8',
        'class' => '',
      ),
    ),
  );
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'middle' => NULL,
      'bottom_left' => NULL,
      'bottom_center' => NULL,
      'bottom_right' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'bd64d8fc-4026-42d0-bee0-f18d47e865e7';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-feae97e2-7162-4064-89ca-85dfd6f8213e';
    $pane->panel = 'bottom_center';
    $pane->type = 'views';
    $pane->subtype = 'cmcp_source_summary_top_petitions';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 0,
      'nodes_per_page' => '10',
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
    $pane->uuid = 'feae97e2-7162-4064-89ca-85dfd6f8213e';
    $display->content['new-feae97e2-7162-4064-89ca-85dfd6f8213e'] = $pane;
    $display->panels['bottom_center'][0] = 'new-feae97e2-7162-4064-89ca-85dfd6f8213e';
    $pane = new stdClass();
    $pane->pid = 'new-8760ba72-f505-4c36-a01d-6714c74dac4c';
    $pane->panel = 'bottom_left';
    $pane->type = 'views';
    $pane->subtype = 'cmcp_source_summary_top_signatories';
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
    $pane->uuid = '8760ba72-f505-4c36-a01d-6714c74dac4c';
    $display->content['new-8760ba72-f505-4c36-a01d-6714c74dac4c'] = $pane;
    $display->panels['bottom_left'][0] = 'new-8760ba72-f505-4c36-a01d-6714c74dac4c';
    $pane = new stdClass();
    $pane->pid = 'new-6dfceab0-37a7-4a53-b4ca-3b3ee9019d2f';
    $pane->panel = 'bottom_right';
    $pane->type = 'views';
    $pane->subtype = 'cmcp_source_summary_top_targets';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 0,
      'nodes_per_page' => '10',
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
    $pane->uuid = '6dfceab0-37a7-4a53-b4ca-3b3ee9019d2f';
    $display->content['new-6dfceab0-37a7-4a53-b4ca-3b3ee9019d2f'] = $pane;
    $display->panels['bottom_right'][0] = 'new-6dfceab0-37a7-4a53-b4ca-3b3ee9019d2f';
    $pane = new stdClass();
    $pane->pid = 'new-07841101-8813-494d-9ca6-db31f0cd2e2c';
    $pane->panel = 'center';
    $pane->type = 'cmcp_set_page_title';
    $pane->subtype = 'cmcp_set_page_title';
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
    $pane->uuid = '07841101-8813-494d-9ca6-db31f0cd2e2c';
    $display->content['new-07841101-8813-494d-9ca6-db31f0cd2e2c'] = $pane;
    $display->panels['center'][0] = 'new-07841101-8813-494d-9ca6-db31f0cd2e2c';
    $pane = new stdClass();
    $pane->pid = 'new-57a60182-d28f-44bd-a923-a3dc2599d5b2';
    $pane->panel = 'center';
    $pane->type = 'views';
    $pane->subtype = 'cmcp_source_summary_header';
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '57a60182-d28f-44bd-a923-a3dc2599d5b2';
    $display->content['new-57a60182-d28f-44bd-a923-a3dc2599d5b2'] = $pane;
    $display->panels['center'][1] = 'new-57a60182-d28f-44bd-a923-a3dc2599d5b2';
    $pane = new stdClass();
    $pane->pid = 'new-2f9bc4fc-8de9-4bbf-80e9-35bf39ca1a37';
    $pane->panel = 'middle';
    $pane->type = 'cmcp_source_signature_history';
    $pane->subtype = 'cmcp_source_signature_history';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'text' => '',
      'time_period' => '90',
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
    $pane->uuid = '2f9bc4fc-8de9-4bbf-80e9-35bf39ca1a37';
    $display->content['new-2f9bc4fc-8de9-4bbf-80e9-35bf39ca1a37'] = $pane;
    $display->panels['middle'][0] = 'new-2f9bc4fc-8de9-4bbf-80e9-35bf39ca1a37';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cmcp_source_sammary_report'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'petitions_report';
  $page->task = 'page';
  $page->admin_title = 'Petitions Report';
  $page->admin_description = 'Details on the performance of individual petitions.';
  $page->path = 'crm-core/reports/cmcp/petition-report';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'view petitions report',
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
    'title' => 'Petitions',
    'name' => 'navigation',
    'weight' => '0',
    'parent' => array(
      'type' => 'normal',
      'title' => 'Petition Pages',
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
  $handler->name = 'page_petitions_report_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'petitions_report';
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
  $display->title = 'Petition pages';
  $display->uuid = '5cf8e583-c090-4ace-bad9-0a09d4ea2c11';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-6d81c42a-6b80-44be-b93b-cf198e12d3dc';
    $pane->panel = 'center';
    $pane->type = 'views_panes';
    $pane->subtype = 'petition_page_report-panel_pane_1';
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
    $pane->uuid = '6d81c42a-6b80-44be-b93b-cf198e12d3dc';
    $display->content['new-6d81c42a-6b80-44be-b93b-cf198e12d3dc'] = $pane;
    $display->panels['center'][0] = 'new-6d81c42a-6b80-44be-b93b-cf198e12d3dc';
    $pane = new stdClass();
    $pane->pid = 'new-09893880-86dd-48b4-9707-373d7f5191a8';
    $pane->panel = 'center_';
    $pane->type = 'cmcp_petition_page_chart';
    $pane->subtype = 'cmcp_petition_page_chart';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'color_series' => '',
      'display_labels' => '',
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
    $pane->uuid = '09893880-86dd-48b4-9707-373d7f5191a8';
    $display->content['new-09893880-86dd-48b4-9707-373d7f5191a8'] = $pane;
    $display->panels['center_'][0] = 'new-09893880-86dd-48b4-9707-373d7f5191a8';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['petitions_report'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'petitions_sources_report';
  $page->task = 'page';
  $page->admin_title = 'Petitions: Sources Report';
  $page->admin_description = 'This is the sources report for petitions.';
  $page->path = 'crm-core/reports/cmcp/sources-report';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'view petition sources report',
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
  $handler->name = 'page_petitions_sources_report_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'petitions_sources_report';
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
  $display->title = 'Petition Sources';
  $display->uuid = '289f613f-3720-44bc-9697-ec56dda4d8d9';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-d6e427a3-fde6-41df-8e5f-ce688a066d97';
    $pane->panel = 'center';
    $pane->type = 'views_panes';
    $pane->subtype = 'petition_source_report-panel_pane_1';
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
    $pane->uuid = 'd6e427a3-fde6-41df-8e5f-ce688a066d97';
    $display->content['new-d6e427a3-fde6-41df-8e5f-ce688a066d97'] = $pane;
    $display->panels['center'][0] = 'new-d6e427a3-fde6-41df-8e5f-ce688a066d97';
    $pane = new stdClass();
    $pane->pid = 'new-7f4096bb-b8e5-4225-864d-6fffc1ab1dc6';
    $pane->panel = 'center_';
    $pane->type = 'cmcp_source_chart';
    $pane->subtype = 'cmcp_source_chart';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'height' => '',
      'color_series' => '',
      'display_labels' => '',
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
    $pane->uuid = '7f4096bb-b8e5-4225-864d-6fffc1ab1dc6';
    $display->content['new-7f4096bb-b8e5-4225-864d-6fffc1ab1dc6'] = $pane;
    $display->panels['center_'][0] = 'new-7f4096bb-b8e5-4225-864d-6fffc1ab1dc6';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['petitions_sources_report'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'petitions_targets_report';
  $page->task = 'page';
  $page->admin_title = 'Petitions: Targets Report';
  $page->admin_description = 'This is the targets report';
  $page->path = 'crm-core/reports/cmcp/target-report';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'view targets report',
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
    'title' => 'Targets',
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
  $handler->name = 'page_petitions_targets_report_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'petitions_targets_report';
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
  $display->title = 'Petition targets';
  $display->uuid = '215ccc80-1436-4b87-a634-8596a4b975b5';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-ff9ba6fb-0ba1-459d-8ceb-84548c40e94a';
    $pane->panel = 'center';
    $pane->type = 'views_panes';
    $pane->subtype = 'cmcp_petition_targets_report-panel_pane_1';
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
    $pane->uuid = 'ff9ba6fb-0ba1-459d-8ceb-84548c40e94a';
    $display->content['new-ff9ba6fb-0ba1-459d-8ceb-84548c40e94a'] = $pane;
    $display->panels['center'][0] = 'new-ff9ba6fb-0ba1-459d-8ceb-84548c40e94a';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['petitions_targets_report'] = $page;

  return $pages;

}
