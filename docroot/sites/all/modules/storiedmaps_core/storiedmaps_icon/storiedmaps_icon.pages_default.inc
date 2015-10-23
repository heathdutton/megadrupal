<?php
/**
 * @file
 * storiedmaps_icon.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function storiedmaps_icon_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'term_view_panel_context';
  $handler->task = 'term_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Location type term',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => 'div.pane-taxonomy-term-field-location-icon,
div.views-field-field-content-image {
 float: left;
 margin-right: 10px;
 margin-bottom: 10px;
}
div.views-row {
  clear: both;
}',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'term_vocabulary',
          'settings' => array(
            'machine_name' => array(
              'location_type' => 'location_type',
            ),
          ),
          'context' => 'argument_term_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'three_3x33';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'one_main' => NULL,
      'three_33_top' => NULL,
      'three_33_second' => NULL,
      'three_33_third' => NULL,
      'three_33_first' => NULL,
      'three_33_bottom' => NULL,
    ),
    'three_33_first' => array(
      'style' => '-1',
    ),
    'three_33_bottom' => array(
      'style' => '-1',
    ),
  );
  $display->cache = array();
  $display->title = '%term:name';
  $display->uuid = '2a7f6aa1-c5ec-478b-8b74-dc28663488b7';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-1578d98d-36a6-45ee-88d6-0a3a9ba67172';
    $pane->panel = 'three_33_top';
    $pane->type = 'entity_field';
    $pane->subtype = 'taxonomy_term:field_location_icon';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'image',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'image_style' => 'icon-large',
        'image_link' => '',
      ),
      'context' => 'argument_term_1',
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
    $pane->uuid = '1578d98d-36a6-45ee-88d6-0a3a9ba67172';
    $display->content['new-1578d98d-36a6-45ee-88d6-0a3a9ba67172'] = $pane;
    $display->panels['three_33_top'][0] = 'new-1578d98d-36a6-45ee-88d6-0a3a9ba67172';
    $pane = new stdClass();
    $pane->pid = 'new-dc0504e2-ea61-4180-9979-bee7a6dfce05';
    $pane->panel = 'three_33_top';
    $pane->type = 'term_description';
    $pane->subtype = 'term_description';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => 'argument_term_1',
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
    $pane->uuid = 'dc0504e2-ea61-4180-9979-bee7a6dfce05';
    $display->content['new-dc0504e2-ea61-4180-9979-bee7a6dfce05'] = $pane;
    $display->panels['three_33_top'][1] = 'new-dc0504e2-ea61-4180-9979-bee7a6dfce05';
    $pane = new stdClass();
    $pane->pid = 'new-7c3bce81-e01f-4d47-a690-c5c292cb919c';
    $pane->panel = 'three_33_top';
    $pane->type = 'views_panes';
    $pane->subtype = 'location_term_page-panel_pane_1';
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
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '7c3bce81-e01f-4d47-a690-c5c292cb919c';
    $display->content['new-7c3bce81-e01f-4d47-a690-c5c292cb919c'] = $pane;
    $display->panels['three_33_top'][2] = 'new-7c3bce81-e01f-4d47-a690-c5c292cb919c';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['term_view_panel_context'] = $handler;

  return $export;
}

/**
 * Implements hook_default_page_manager_pages().
 */
function storiedmaps_icon_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'location_icons';
  $page->task = 'page';
  $page->admin_title = 'Location icons';
  $page->admin_description = '';
  $page->path = 'location-icons';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array();
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_location_icons_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'location_icons';
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
  $display->layout = 'three_3x33';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'one_main' => NULL,
      'three_33_top' => NULL,
      'three_33_first' => NULL,
      'three_33_second' => NULL,
      'three_33_third' => NULL,
      'three_33_bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '7c5d8f62-290b-443f-b412-c815036d1bf6';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-efbe9252-faa8-42a6-8d64-3c5a092175a9';
    $pane->panel = 'three_33_bottom';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => 'Icon page footer',
      'title' => '',
      'body' => '<pre wrap="">
You can also <a href="/location-terms">explore the icons in a larger format</a>.
</pre>
',
      'format' => 'wysiwyg_editor',
      'substitute' => TRUE,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'efbe9252-faa8-42a6-8d64-3c5a092175a9';
    $display->content['new-efbe9252-faa8-42a6-8d64-3c5a092175a9'] = $pane;
    $display->panels['three_33_bottom'][0] = 'new-efbe9252-faa8-42a6-8d64-3c5a092175a9';
    $pane = new stdClass();
    $pane->pid = 'new-ec960c0f-1db3-42cd-80bd-77408a7c9fe9';
    $pane->panel = 'three_33_first';
    $pane->type = 'views_panes';
    $pane->subtype = 'location_term_parent-panel_pane_2';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'arguments' => array(
        'tid' => '14',
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
    $pane->uuid = 'ec960c0f-1db3-42cd-80bd-77408a7c9fe9';
    $display->content['new-ec960c0f-1db3-42cd-80bd-77408a7c9fe9'] = $pane;
    $display->panels['three_33_first'][0] = 'new-ec960c0f-1db3-42cd-80bd-77408a7c9fe9';
    $pane = new stdClass();
    $pane->pid = 'new-027fa4c2-2ba0-4feb-84e7-33664e070203';
    $pane->panel = 'three_33_second';
    $pane->type = 'views_panes';
    $pane->subtype = 'location_term_parent-panel_pane_2';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'arguments' => array(
        'tid' => '13',
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
    $pane->uuid = '027fa4c2-2ba0-4feb-84e7-33664e070203';
    $display->content['new-027fa4c2-2ba0-4feb-84e7-33664e070203'] = $pane;
    $display->panels['three_33_second'][0] = 'new-027fa4c2-2ba0-4feb-84e7-33664e070203';
    $pane = new stdClass();
    $pane->pid = 'new-15d9f7f8-c44e-4780-9bd0-23d04b204fe6';
    $pane->panel = 'three_33_third';
    $pane->type = 'views_panes';
    $pane->subtype = 'location_term_parent-panel_pane_2';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'arguments' => array(
        'tid' => '25',
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
    $pane->uuid = '15d9f7f8-c44e-4780-9bd0-23d04b204fe6';
    $display->content['new-15d9f7f8-c44e-4780-9bd0-23d04b204fe6'] = $pane;
    $display->panels['three_33_third'][0] = 'new-15d9f7f8-c44e-4780-9bd0-23d04b204fe6';
    $pane = new stdClass();
    $pane->pid = 'new-d51695c4-f79e-4b63-9c74-c6e7cf3c32f1';
    $pane->panel = 'three_33_top';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => 'Icon text',
      'title' => '',
      'body' => '<p>The CRD Community Green Map icons are based on a universal green-map iconography designed by&nbsp;<a href="http://www.greenmap.org/">Green Map&reg; System</a>. The symbols make every green map easy to explore,&nbsp;regardless of the language and cultural orientation of the user. Browse the icons below, and you&#39;ll find symbols designed specifically for the Capital Region, like Camas Meadow, Garry Oak, and Culturally Modified Tree. <a href="http://www.greenmap.org/greenhouse/en/about">&copy; Green Map System, Inc. 2012</a></p><p>When you click on an icon below, the icon&#39;s definition and a list of the sites associated with the icon is displayed.</p>',
      'format' => 'wysiwyg_editor',
      'substitute' => TRUE,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'd51695c4-f79e-4b63-9c74-c6e7cf3c32f1';
    $display->content['new-d51695c4-f79e-4b63-9c74-c6e7cf3c32f1'] = $pane;
    $display->panels['three_33_top'][0] = 'new-d51695c4-f79e-4b63-9c74-c6e7cf3c32f1';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['location_icons'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'location_term_landing';
  $page->task = 'page';
  $page->admin_title = 'Location term landing';
  $page->admin_description = '';
  $page->path = 'location-categories';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'none',
    'title' => '',
    'weight' => '0',
    'name' => 'navigation',
    'parent' => array(
      'type' => 'none',
      'title' => '',
      'weight' => 0,
      'name' => 'navigation',
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
  $handler->name = 'page_location_term_landing_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'location_term_landing';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Landing page',
    'no_blocks' => FALSE,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'three_3x33';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'three_33_top' => NULL,
      'three_33_first' => NULL,
      'three_33_second' => NULL,
      'three_33_third' => NULL,
      'three_33_bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Location term landing';
  $display->uuid = '4d382d68-dd23-4532-941d-b4a643eb921e';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-7d05418f-e0f5-4130-ae6d-df9fc3c1e418';
    $pane->panel = 'three_33_first';
    $pane->type = 'views_panes';
    $pane->subtype = 'location_term_parent-panel_pane_2';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'arguments' => array(
        'tid' => '14',
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
    $pane->uuid = '7d05418f-e0f5-4130-ae6d-df9fc3c1e418';
    $display->content['new-7d05418f-e0f5-4130-ae6d-df9fc3c1e418'] = $pane;
    $display->panels['three_33_first'][0] = 'new-7d05418f-e0f5-4130-ae6d-df9fc3c1e418';
    $pane = new stdClass();
    $pane->pid = 'new-ef186f3d-e507-4c61-bcf9-b9d0f0f4a02d';
    $pane->panel = 'three_33_second';
    $pane->type = 'views_panes';
    $pane->subtype = 'location_term_parent-panel_pane_2';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'arguments' => array(
        'tid' => '13',
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
    $pane->uuid = 'ef186f3d-e507-4c61-bcf9-b9d0f0f4a02d';
    $display->content['new-ef186f3d-e507-4c61-bcf9-b9d0f0f4a02d'] = $pane;
    $display->panels['three_33_second'][0] = 'new-ef186f3d-e507-4c61-bcf9-b9d0f0f4a02d';
    $pane = new stdClass();
    $pane->pid = 'new-046b0758-40b7-4e95-848b-94212c8a9d7f';
    $pane->panel = 'three_33_third';
    $pane->type = 'views_panes';
    $pane->subtype = 'location_term_parent-panel_pane_2';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'arguments' => array(
        'tid' => '25',
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
    $pane->uuid = '046b0758-40b7-4e95-848b-94212c8a9d7f';
    $display->content['new-046b0758-40b7-4e95-848b-94212c8a9d7f'] = $pane;
    $display->panels['three_33_third'][0] = 'new-046b0758-40b7-4e95-848b-94212c8a9d7f';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['location_term_landing'] = $page;

  return $pages;

}
