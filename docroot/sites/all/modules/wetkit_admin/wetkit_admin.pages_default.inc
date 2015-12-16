<?php
/**
 * @file
 * wetkit_admin.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function wetkit_admin_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_edit_panel_context';
  $handler->task = 'node_edit';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 100;
  $handler->conf = array(
    'title' => 'Node Edit Page',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'css_id' => 'node-edit',
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
      'contentmain' => NULL,
      'sidebar' => NULL,
    ),
    'sidebar' => array(
      'style' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'fb7f3c23-61e8-0294-b9c0-340267aa4f4a';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-ff97b876-c055-7ce4-8137-20c523445df9';
    $pane->panel = 'contentmain';
    $pane->type = 'node_form_title';
    $pane->subtype = 'node_form_title';
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
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'ff97b876-c055-7ce4-8137-20c523445df9';
    $display->content['new-ff97b876-c055-7ce4-8137-20c523445df9'] = $pane;
    $display->panels['contentmain'][0] = 'new-ff97b876-c055-7ce4-8137-20c523445df9';
    $pane = new stdClass();
    $pane->pid = 'new-b5f50cf2-e574-1354-a547-4d25cb9695d2';
    $pane->panel = 'contentmain';
    $pane->type = 'node_form_path';
    $pane->subtype = 'node_form_path';
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'b5f50cf2-e574-1354-a547-4d25cb9695d2';
    $display->content['new-b5f50cf2-e574-1354-a547-4d25cb9695d2'] = $pane;
    $display->panels['contentmain'][1] = 'new-b5f50cf2-e574-1354-a547-4d25cb9695d2';
    $pane = new stdClass();
    $pane->pid = 'new-758fc8ab-2f84-4004-e173-0bedcc1d349e';
    $pane->panel = 'contentmain';
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
    $pane->uuid = '758fc8ab-2f84-4004-e173-0bedcc1d349e';
    $display->content['new-758fc8ab-2f84-4004-e173-0bedcc1d349e'] = $pane;
    $display->panels['contentmain'][2] = 'new-758fc8ab-2f84-4004-e173-0bedcc1d349e';
    $pane = new stdClass();
    $pane->pid = 'new-95564a77-8b5b-4c74-0dfa-5117da097884';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_featured_image';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
      'override_title' => 1,
      'override_title_text' => 'Featured image',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '95564a77-8b5b-4c74-0dfa-5117da097884';
    $display->content['new-95564a77-8b5b-4c74-0dfa-5117da097884'] = $pane;
    $display->panels['sidebar'][0] = 'new-95564a77-8b5b-4c74-0dfa-5117da097884';
    $pane = new stdClass();
    $pane->pid = 'new-084cd973-73fa-87d4-9dc4-d1e195a45dfd';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_featured_categories';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
      'override_title' => 1,
      'override_title_text' => 'Content category',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '084cd973-73fa-87d4-9dc4-d1e195a45dfd';
    $display->content['new-084cd973-73fa-87d4-9dc4-d1e195a45dfd'] = $pane;
    $display->panels['sidebar'][1] = 'new-084cd973-73fa-87d4-9dc4-d1e195a45dfd';
    $pane = new stdClass();
    $pane->pid = 'new-15d0ad18-f2b8-9c64-7573-176bf8bc58dc';
    $pane->panel = 'sidebar';
    $pane->type = 'panelizer_form_default';
    $pane->subtype = 'panelizer_form_default';
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
    $pane->uuid = '15d0ad18-f2b8-9c64-7573-176bf8bc58dc';
    $display->content['new-15d0ad18-f2b8-9c64-7573-176bf8bc58dc'] = $pane;
    $display->panels['sidebar'][2] = 'new-15d0ad18-f2b8-9c64-7573-176bf8bc58dc';
    $pane = new stdClass();
    $pane->pid = 'new-d8ca4dfe-15a7-4ae4-451e-a0a39e495ada';
    $pane->panel = 'sidebar';
    $pane->type = 'node_form_menu';
    $pane->subtype = 'node_form_menu';
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = 'd8ca4dfe-15a7-4ae4-451e-a0a39e495ada';
    $display->content['new-d8ca4dfe-15a7-4ae4-451e-a0a39e495ada'] = $pane;
    $display->panels['sidebar'][3] = 'new-d8ca4dfe-15a7-4ae4-451e-a0a39e495ada';
    $pane = new stdClass();
    $pane->pid = 'new-8ca4ffdd-3548-2854-69f4-cc530df7cd6b';
    $pane->panel = 'sidebar';
    $pane->type = 'node_form_publishing';
    $pane->subtype = 'node_form_publishing';
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
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = '8ca4ffdd-3548-2854-69f4-cc530df7cd6b';
    $display->content['new-8ca4ffdd-3548-2854-69f4-cc530df7cd6b'] = $pane;
    $display->panels['sidebar'][4] = 'new-8ca4ffdd-3548-2854-69f4-cc530df7cd6b';
    $pane = new stdClass();
    $pane->pid = 'new-8f2de99f-296c-e514-c143-e283b888ebf8';
    $pane->panel = 'sidebar';
    $pane->type = 'node_form_author';
    $pane->subtype = 'node_form_author';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => array(),
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
    $pane->uuid = '8f2de99f-296c-e514-c143-e283b888ebf8';
    $display->content['new-8f2de99f-296c-e514-c143-e283b888ebf8'] = $pane;
    $display->panels['sidebar'][5] = 'new-8f2de99f-296c-e514-c143-e283b888ebf8';
    $pane = new stdClass();
    $pane->pid = 'new-3fbeea9b-b02b-7ed4-41ec-378fb72d541e';
    $pane->panel = 'sidebar';
    $pane->type = 'node_form_buttons';
    $pane->subtype = 'node_form_buttons';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => array(),
      'override_title' => 1,
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
    $pane->uuid = '3fbeea9b-b02b-7ed4-41ec-378fb72d541e';
    $display->content['new-3fbeea9b-b02b-7ed4-41ec-378fb72d541e'] = $pane;
    $display->panels['sidebar'][6] = 'new-3fbeea9b-b02b-7ed4-41ec-378fb72d541e';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-758fc8ab-2f84-4004-e173-0bedcc1d349e';
  $handler->conf['display'] = $display;
  $export['node_edit_panel_context'] = $handler;

  return $export;
}

/**
 * Implements hook_default_page_manager_pages().
 */
function wetkit_admin_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'wetkit_admin_layout_library';
  $page->task = 'page';
  $page->admin_title = 'WetKit Layouts';
  $page->admin_description = 'This is the page library that manages page layouts.';
  $page->path = 'admin/wetkit/layouts';
  $page->access = array(
    'plugins' => array(
      1 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'access administration pages',
        ),
        'context' => 'logged-in-user',
        'not' => FALSE,
      ),
      2 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'administer panels layouts',
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
    'type' => 'normal',
    'title' => 'Layouts',
    'name' => 'management',
    'weight' => '0',
    'parent' => array(
      'type' => 'normal',
      'title' => 'Dashboard',
      'name' => 'management',
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
  $handler->name = 'page_wetkit_admin_layout_library_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'wetkit_admin_layout_library';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Layout Library',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'css_id' => 'layout-library',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
  );
  $display = new panels_display();
  $display->layout = 'boxton';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'header' => NULL,
      'column1' => NULL,
      'column2' => NULL,
      'footer' => NULL,
      'contentmain' => NULL,
      'sidebar' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Layout Library';
  $display->uuid = '98ffc912-d16f-3924-7980-a008c64b3a21';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-ee25c05a-a5fc-b454-1d9c-138d753eb125';
    $pane->panel = 'contentmain';
    $pane->type = 'panels_layouts';
    $pane->subtype = 'panels_layouts';
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
    $pane->uuid = 'ee25c05a-a5fc-b454-1d9c-138d753eb125';
    $display->content['new-ee25c05a-a5fc-b454-1d9c-138d753eb125'] = $pane;
    $display->panels['contentmain'][0] = 'new-ee25c05a-a5fc-b454-1d9c-138d753eb125';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['wetkit_admin_layout_library'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'wetkit_admin_page_library';
  $page->task = 'page';
  $page->admin_title = 'WetKit Pages';
  $page->admin_description = 'This is the page library that manages page templates, landing pages, and overridden pages.';
  $page->path = 'admin/wetkit/pages';
  $page->access = array(
    'plugins' => array(
      1 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'access administration pages',
        ),
        'context' => 'logged-in-user',
        'not' => FALSE,
      ),
      2 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'use page manager',
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
    'type' => 'normal',
    'title' => 'Pages',
    'name' => 'management',
    'weight' => '0',
    'parent' => array(
      'type' => 'normal',
      'title' => 'Dashboard',
      'name' => 'management',
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
  $handler->name = 'page_wetkit_admin_page_library_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'wetkit_admin_page_library';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Page Library',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'css_id' => 'page-library',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
  );
  $display = new panels_display();
  $display->layout = 'boxton';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'header' => NULL,
      'column1' => NULL,
      'column2' => NULL,
      'footer' => NULL,
      'contentmain' => NULL,
      'sidebar' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Page Library';
  $display->uuid = '8c75ee38-3ae8-8464-4525-e30e8d67b462';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-1a75c8f6-fd1b-4862-99e0-1aaab819e260';
    $pane->panel = 'contentmain';
    $pane->type = 'page_title';
    $pane->subtype = 'page_title';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'markup' => 'h1',
      'class' => '',
      'id' => 'wb-cont',
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
    $pane->uuid = '1a75c8f6-fd1b-4862-99e0-1aaab819e260';
    $display->content['new-1a75c8f6-fd1b-4862-99e0-1aaab819e260'] = $pane;
    $display->panels['contentmain'][0] = 'new-1a75c8f6-fd1b-4862-99e0-1aaab819e260';
    $pane = new stdClass();
    $pane->pid = 'new-4f3c50c5-6eaf-e134-8d0b-e6e2cc0dc655';
    $pane->panel = 'contentmain';
    $pane->type = 'landing_pages';
    $pane->subtype = 'landing_pages';
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
    $pane->uuid = '4f3c50c5-6eaf-e134-8d0b-e6e2cc0dc655';
    $display->content['new-4f3c50c5-6eaf-e134-8d0b-e6e2cc0dc655'] = $pane;
    $display->panels['contentmain'][1] = 'new-4f3c50c5-6eaf-e134-8d0b-e6e2cc0dc655';
    $pane = new stdClass();
    $pane->pid = 'new-4debdf1a-8779-eb34-b994-63efbc005454';
    $pane->panel = 'contentmain';
    $pane->type = 'overridden_page_templates';
    $pane->subtype = 'overridden_page_templates';
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
    $pane->uuid = '4debdf1a-8779-eb34-b994-63efbc005454';
    $display->content['new-4debdf1a-8779-eb34-b994-63efbc005454'] = $pane;
    $display->panels['contentmain'][2] = 'new-4debdf1a-8779-eb34-b994-63efbc005454';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['wetkit_admin_page_library'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'wetkit_admin_pane_library';
  $page->task = 'page';
  $page->admin_title = 'WetKit Panes';
  $page->admin_description = 'This is the pane library for the site. See all of the panes you can add to your pages.';
  $page->path = 'admin/wetkit/panes';
  $page->access = array(
    'plugins' => array(
      1 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'access administration pages',
        ),
        'context' => 'logged-in-user',
        'not' => FALSE,
      ),
      2 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'administer page manager',
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
    'type' => 'normal',
    'title' => 'Panes',
    'name' => 'management',
    'weight' => '0',
    'parent' => array(
      'type' => 'normal',
      'title' => 'Dashboard',
      'name' => 'management',
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
  $handler->name = 'page_wetkit_admin_pane_library_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'wetkit_admin_pane_library';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Pane Library',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'css_id' => 'pane-library',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
  );
  $display = new panels_display();
  $display->layout = 'boxton';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'header' => NULL,
      'column1' => NULL,
      'column2' => NULL,
      'footer' => NULL,
      'contentmain' => NULL,
      'sidebar' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Pane Library';
  $display->uuid = '45b222ae-1279-2ba4-d590-a091b11bc4db';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-bc8db260-387b-480f-bc6f-3681980c9360';
    $pane->panel = 'contentmain';
    $pane->type = 'page_title';
    $pane->subtype = 'page_title';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'markup' => 'h1',
      'class' => '',
      'id' => 'wb-cont',
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
    $pane->uuid = 'bc8db260-387b-480f-bc6f-3681980c9360';
    $display->content['new-bc8db260-387b-480f-bc6f-3681980c9360'] = $pane;
    $display->panels['contentmain'][0] = 'new-bc8db260-387b-480f-bc6f-3681980c9360';
    $pane = new stdClass();
    $pane->pid = 'new-679afc53-122a-2f84-e1c8-9265c2717cc0';
    $pane->panel = 'contentmain';
    $pane->type = 'general_panes';
    $pane->subtype = 'general_panes';
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
    $pane->uuid = '679afc53-122a-2f84-e1c8-9265c2717cc0';
    $display->content['new-679afc53-122a-2f84-e1c8-9265c2717cc0'] = $pane;
    $display->panels['contentmain'][1] = 'new-679afc53-122a-2f84-e1c8-9265c2717cc0';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['wetkit_admin_pane_library'] = $page;

  return $pages;

}
