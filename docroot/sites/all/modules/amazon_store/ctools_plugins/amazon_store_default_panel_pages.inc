<?php
/**
 * @file Default panels for amazon_store
 *
 * Implements hook_default_panel_pages().
 * To offer a default panel for the item detail page
 * @return unknown_type
 */
function amazon_store_default_panel_pages() {
  $page = new stdClass;
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'amazon_item';
  $page->task = 'panel_page';
  $page->admin_title = 'amazon_item';
  $page->path = AMAZON_STORE_PATH . '/item/%asin';
  $page->access = FALSE;
  $page->multiple = FALSE;
  $page->menu = FALSE;
  $page->arguments = array(
  'asin' => array(
    'id' => 1,
    'identifier' => 'ASIN (Amazon item identifier)',
    'name' => 'asin',
    'settings' => array(),
  ),
);
  $page->conf = array(
  'no_blocks' => 0,
  'css_id' => '',
  'css' => '',
  'contexts' => array(),
  'relationships' => array(),
);
  $display = new panels_display;
  $display->layout = 'twocol';
  $display->layout_settings = array();
  $display->panel_settings = array();
  $display->cache = array();
  $display->title = '';
  $display->hide_title = FALSE;
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass;
  $pane->pid = 'new-1';
  $pane->panel = 'left';
  $pane->type = 'item_title';
  $pane->subtype = 'item_title';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => 'argument_asin_1',
    'override_title' => 0,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $display->content['new-1'] = $pane;
  $display->panels['left'][0] = 'new-1';
  $pane = new stdClass;
  $pane->pid = 'new-2';
  $pane->panel = 'left';
  $pane->type = 'item_description';
  $pane->subtype = 'item_description';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => 'argument_asin_1',
    'override_title' => 0,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $display->content['new-2'] = $pane;
  $display->panels['left'][1] = 'new-2';
  $pane = new stdClass;
  $pane->pid = 'new-3';
  $pane->panel = 'left';
  $pane->type = 'item_details';
  $pane->subtype = 'item_details';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => 'argument_asin_1',
    'override_title' => 0,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $display->content['new-3'] = $pane;
  $display->panels['left'][2] = 'new-3';
  $pane = new stdClass;
  $pane->pid = 'new-4';
  $pane->panel = 'right';
  $pane->type = 'custom';
  $pane->subtype = 'custom';
  $pane->shown = TRUE;
  $pane->access = array(
    'logic' => 'and',
  );
  $pane->configuration = array(
    'title' => 'Sample Amazon Store Product Page',
    'body' => 'This is a panel which is a starting place for an Amazon Store item detail page. You can change it in many ways, adding custom content, theming it, changing the borders, moving around the content, etc.',
    'format' => '1',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $display->content['new-4'] = $pane;
  $display->panels['right'][0] = 'new-4';
  $pane = new stdClass;
  $pane->pid = 'new-5';
  $pane->panel = 'right';
  $pane->type = 'item_image';
  $pane->subtype = 'item_image';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'size_selection' => 'MediumImage',
    'context' => 'argument_asin_1',
    'override_title' => 0,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $display->content['new-5'] = $pane;
  $display->panels['right'][1] = 'new-5';
  $pane = new stdClass;
  $pane->pid = 'new-6';
  $pane->panel = 'right';
  $pane->type = 'item_offers';
  $pane->subtype = 'item_offers';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => 'argument_asin_1',
    'override_title' => 0,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $display->content['new-6'] = $pane;
  $display->panels['right'][2] = 'new-6';
  $pane = new stdClass;
  $pane->pid = 'new-7';
  $pane->panel = 'right';
  $pane->type = 'item_reviews';
  $pane->subtype = 'item_reviews';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => 'argument_asin_1',
    'override_title' => 0,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 3;
  $display->content['new-7'] = $pane;
  $display->panels['right'][3] = 'new-7';
  $page->conf['display'] = $display;

  $pages['amazon_store_item_panel'] = $page;
  return $pages;
}
