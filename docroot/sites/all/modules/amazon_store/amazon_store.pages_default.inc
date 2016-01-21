<?php
/**
 * Default panels pages for Amazon Store
 *
 * @return none
 */
function amazon_store_default_page_manager_pages() {
  $page = new stdClass;
  $page->disabled = TRUE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'amazon_item';
  $page->task = 'page';
  $page->admin_title = 'Amazon Store Item Detail Page';
  $page->admin_description = '';
  $page->path = AMAZON_STORE_PATH .'/item/%asin';
  $page->access = array();
  $page->menu = array();
  $page->arguments = array(
      'asin' => array(
      'id' => 1,
      'identifier' => 'ASIN (Amazon item identifier)',
      'name' => 'asin',
      'settings' => array(),
      ),
  );
  $page->conf = array();
  $page->default_handlers = array();
  $handler = new stdClass;
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_testamzn_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'testamzn';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
      'title' => 'Panel',
      'no_blocks' => FALSE,
      'css_id' => '',
      'css' => '',
      'contexts' => array(),
      'relationships' => array(),
  );
  $display = new panels_display;
  $display->layout = 'twocol_stacked';
  $display->layout_settings = array();
  $display->panel_settings = array(
      'style' => 'rounded_corners',
      'style_settings' => array(
      'default' => array(
      'corner_location' => 'pane',
      ),
      ),
  );
  $display->cache = array();
  $display->title = '%asin';
  $display->hide_title = FALSE;
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass;
  $pane->pid = 'new-1';
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
  $pane->position = 0;
  $display->content['new-1'] = $pane;
  $display->panels['left'][0] = 'new-1';
  $pane = new stdClass;
  $pane->pid = 'new-2';
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
  $pane->position = 1;
  $display->content['new-2'] = $pane;
  $display->panels['left'][1] = 'new-2';
  $pane = new stdClass;
  $pane->pid = 'new-3';
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
  $pane->position = 0;
  $display->content['new-3'] = $pane;
  $display->panels['right'][0] = 'new-3';
  $pane = new stdClass;
  $pane->pid = 'new-4';
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
  $pane->position = 1;
  $display->content['new-4'] = $pane;
  $display->panels['right'][1] = 'new-4';
  $pane = new stdClass;
  $pane->pid = 'new-5';
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
  $pane->position = 2;
  $display->content['new-5'] = $pane;
  $display->panels['right'][2] = 'new-5';
  $pane = new stdClass;
  $pane->pid = 'new-6';
  $pane->panel = 'top';
  $pane->type = 'item_title';
  $pane->subtype = 'item_title';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
      'context' => 'argument_asin_1',
      'override_title' => 1,
      'override_title_text' => '%title',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $display->content['new-6'] = $pane;
  $display->panels['top'][0] = 'new-6';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;

  $pages['amazon_store_item_panel'] = $page;

  return $pages;
}
