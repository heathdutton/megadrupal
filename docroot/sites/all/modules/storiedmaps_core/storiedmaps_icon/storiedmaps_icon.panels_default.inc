<?php
/**
 * @file
 * storiedmaps_icon.panels_default.inc
 */

/**
 * Implements hook_default_panels_mini().
 */
function storiedmaps_icon_default_panels_mini() {
  $export = array();

  $mini = new stdClass();
  $mini->disabled = FALSE; /* Edit this to true to make a default mini disabled initially */
  $mini->api_version = 1;
  $mini->name = 'icons';
  $mini->category = '';
  $mini->admin_title = 'Icons';
  $mini->admin_description = 'Three column listing of all icon categories.';
  $mini->requiredcontexts = array();
  $mini->contexts = array();
  $mini->relationships = array();
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
  $display->title = 'Explore more icons';
  $display->uuid = '305c2804-cce7-4838-8514-0fa9f5cf7988';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-f832899e-5d9a-49e9-936e-33acf5a5d269';
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
    $pane->uuid = 'f832899e-5d9a-49e9-936e-33acf5a5d269';
    $display->content['new-f832899e-5d9a-49e9-936e-33acf5a5d269'] = $pane;
    $display->panels['three_33_first'][0] = 'new-f832899e-5d9a-49e9-936e-33acf5a5d269';
    $pane = new stdClass();
    $pane->pid = 'new-9eca3878-7a1d-4a52-bb13-541cfbe52a7a';
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
    $pane->uuid = '9eca3878-7a1d-4a52-bb13-541cfbe52a7a';
    $display->content['new-9eca3878-7a1d-4a52-bb13-541cfbe52a7a'] = $pane;
    $display->panels['three_33_second'][0] = 'new-9eca3878-7a1d-4a52-bb13-541cfbe52a7a';
    $pane = new stdClass();
    $pane->pid = 'new-ed3284dc-521d-4109-8ec8-69c6af413e5a';
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
    $pane->uuid = 'ed3284dc-521d-4109-8ec8-69c6af413e5a';
    $display->content['new-ed3284dc-521d-4109-8ec8-69c6af413e5a'] = $pane;
    $display->panels['three_33_third'][0] = 'new-ed3284dc-521d-4109-8ec8-69c6af413e5a';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-9eca3878-7a1d-4a52-bb13-541cfbe52a7a';
  $mini->display = $display;
  $export['icons'] = $mini;

  return $export;
}
