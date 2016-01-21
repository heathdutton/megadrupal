<?php
/**
 * @file
 * oa_site_layout_defaults.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function oa_site_layout_defaults_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'site_template__panel_context_a4d57c32-e3d0-4611-96eb-3777dd5d594e';
  $handler->task = 'site_template';
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
    'name' => '',
  );
  $display = new panels_display();
  $display->layout = 'oa_site_layout';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'header' => NULL,
      'sidebar' => array(
        'position' => 'right',
        'css_classes' => '',
      ),
      'contentmain' => NULL,
      'secondarycolumn2' => array(
        'position' => 'center',
        'css_classes' => '',
      ),
      'column1' => NULL,
      'column2' => array(
        'position' => 'top',
        'css_classes' => '',
      ),
      'column3' => NULL,
      'secondarycolumn1' => array(
        'position' => 'left',
        'fixed' => 0,
        'mobile_only' => 1,
        'expand_always' => 0,
        'css_classes' => '',
      ),
      'secondarycolumn3' => array(
        'position' => 'right',
        'fixed' => 0,
        'mobile_only' => 0,
        'expand_always' => 0,
        'css_classes' => '',
      ),
      'sidebar1' => array(
        'position' => 'left',
        'fixed' => 0,
        'mobile_only' => 0,
        'expand_always' => 0,
        'css_classes' => '',
      ),
      'sidebar2' => array(
        'position' => 'right',
        'fixed' => 0,
        'mobile_only' => 0,
        'expand_always' => 0,
        'css_classes' => '',
      ),
      'contentfooter' => NULL,
      'content' => NULL,
      'traytop' => array(
        'position' => 'top',
        'fixed' => 1,
        'mobile_only' => 0,
        'expand_always' => 1,
        'css_classes' => '',
      ),
      'traybottom' => array(
        'position' => 'bottom',
        'fixed' => 1,
        'mobile_only' => 0,
        'expand_always' => 1,
        'css_classes' => '',
      ),
      'footer' => NULL,
      'contentheader' => NULL,
    ),
    'sidebar' => array(
      'style' => 'oa_responsive_region',
    ),
    'contentmain' => array(
      'style' => '0',
    ),
    'column1' => array(
      'style' => '0',
    ),
    'column3' => array(
      'style' => '0',
    ),
    'secondarycolumn1' => array(
      'style' => 'oa_responsive_region',
    ),
    'secondarycolumn3' => array(
      'style' => 'oa_responsive_region',
    ),
    'column2' => array(
      'style' => 'oa_responsive_region',
    ),
    'header' => array(
      'style' => '0',
    ),
    'secondarycolumn2' => array(
      'style' => 'oa_responsive_region',
    ),
    'footer' => array(
      'style' => '0',
    ),
    'sidebar1' => array(
      'style' => 'oa_responsive_region',
    ),
    'sidebar2' => array(
      'style' => 'oa_responsive_region',
    ),
    'contentheader' => array(
      'style' => '0',
    ),
    'content' => array(
      'style' => '0',
    ),
    'traytop' => array(
      'style' => 'oa_responsive_region',
    ),
    'traybottom' => array(
      'style' => 'oa_responsive_region',
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '092d1ae4-83ab-471f-8c35-a169a16bdb6a';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-0540fb6c-37a4-4fc4-80d2-3afcc52dea13';
  $pane->panel = 'content';
  $pane->type = 'pane_messages';
  $pane->subtype = 'pane_messages';
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
  $pane->uuid = '0540fb6c-37a4-4fc4-80d2-3afcc52dea13';
  $display->content['new-0540fb6c-37a4-4fc4-80d2-3afcc52dea13'] = $pane;
  $display->panels['content'][0] = 'new-0540fb6c-37a4-4fc4-80d2-3afcc52dea13';
  $pane = new stdClass();
  $pane->pid = 'new-06387b2b-0035-49aa-beb1-9b16e7048e18';
  $pane->panel = 'content';
  $pane->type = 'contextual_tabs';
  $pane->subtype = 'contextual_tabs';
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
  $pane->uuid = '06387b2b-0035-49aa-beb1-9b16e7048e18';
  $display->content['new-06387b2b-0035-49aa-beb1-9b16e7048e18'] = $pane;
  $display->panels['content'][1] = 'new-06387b2b-0035-49aa-beb1-9b16e7048e18';
  $pane = new stdClass();
  $pane->pid = 'new-fcc6b1ff-541b-4fe3-82d2-ea79c4e53895';
  $pane->panel = 'content';
  $pane->type = 'page_content';
  $pane->subtype = 'page_content';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => 'argument_page_content_1',
    'override_title' => 0,
    'override_title_text' => '',
    'override_title_heading' => 'h2',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => array(),
    'style' => 'oa_horizontal_slice',
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = 'fcc6b1ff-541b-4fe3-82d2-ea79c4e53895';
  $display->content['new-fcc6b1ff-541b-4fe3-82d2-ea79c4e53895'] = $pane;
  $display->panels['content'][2] = 'new-fcc6b1ff-541b-4fe3-82d2-ea79c4e53895';
  $pane = new stdClass();
  $pane->pid = 'new-45d40ae9-3122-4b39-afba-5547f5915f56';
  $pane->panel = 'footer';
  $pane->type = 'panels_mini';
  $pane->subtype = 'oa_footer_panel';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 0,
    'override_title_text' => '',
    'override_title_heading' => 'h2',
  );
  $pane->cache = array();
  $pane->style = array(
    'style' => 'oa_horizontal_slice',
    'settings' => array(
      'position' => 0,
      'toolbar' => 0,
      'margins' => 'oa-fullwidth',
    ),
  );
  $pane->css = array(
    'css_id' => '',
    'css_class' => 'oa-navbar',
  );
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '45d40ae9-3122-4b39-afba-5547f5915f56';
  $display->content['new-45d40ae9-3122-4b39-afba-5547f5915f56'] = $pane;
  $display->panels['footer'][0] = 'new-45d40ae9-3122-4b39-afba-5547f5915f56';
  $pane = new stdClass();
  $pane->pid = 'new-e1c95efc-282c-468f-8763-dbf09477df3a';
  $pane->panel = 'header';
  $pane->type = 'panels_mini';
  $pane->subtype = 'oa_toolbar_panel';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 0,
    'override_title_text' => '',
    'override_title_heading' => 'h2',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => array(
      'position' => 1,
      'toolbar' => 1,
      'margins' => 'oa-fullwidth',
    ),
    'style' => 'oa_horizontal_slice',
  );
  $pane->css = array(
    'css_id' => '',
    'css_class' => 'oa-navbar',
  );
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = 'e1c95efc-282c-468f-8763-dbf09477df3a';
  $display->content['new-e1c95efc-282c-468f-8763-dbf09477df3a'] = $pane;
  $display->panels['header'][0] = 'new-e1c95efc-282c-468f-8763-dbf09477df3a';
  $pane = new stdClass();
  $pane->pid = 'new-29f92506-351a-4b10-9a44-227fb1afe2d9';
  $pane->panel = 'header';
  $pane->type = 'oa_space_banner';
  $pane->subtype = 'oa_space_banner';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'banner_position' => '2',
    'override_title' => 0,
    'override_title_text' => '',
    'override_title_heading' => 'h2',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => array(
      'position' => 0,
      'toolbar' => 0,
      'margins' => 'oa-fullwidth',
    ),
    'style' => 'oa_horizontal_slice',
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = '29f92506-351a-4b10-9a44-227fb1afe2d9';
  $display->content['new-29f92506-351a-4b10-9a44-227fb1afe2d9'] = $pane;
  $display->panels['header'][1] = 'new-29f92506-351a-4b10-9a44-227fb1afe2d9';
  $pane = new stdClass();
  $pane->pid = 'new-84c9284a-2356-4395-ab70-1d7d876f02a6';
  $pane->panel = 'header';
  $pane->type = 'oa_space_menu';
  $pane->subtype = 'oa_space_menu';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 0,
    'override_title_text' => '',
    'override_title_heading' => 'h2',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => array(
      'position' => 0,
      'toolbar' => 0,
      'margins' => 'oa-fullwidth',
    ),
    'style' => 'oa_horizontal_slice',
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = '84c9284a-2356-4395-ab70-1d7d876f02a6';
  $display->content['new-84c9284a-2356-4395-ab70-1d7d876f02a6'] = $pane;
  $display->panels['header'][2] = 'new-84c9284a-2356-4395-ab70-1d7d876f02a6';
  $display->hide_title = PANELS_TITLE_PANE;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['site_template__panel_context_a4d57c32-e3d0-4611-96eb-3777dd5d594e'] = $handler;

  return $export;
}
