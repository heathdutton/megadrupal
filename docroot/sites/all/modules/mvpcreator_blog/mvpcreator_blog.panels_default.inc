<?php
/**
 * @file
 * mvpcreator_blog.panels_default.inc
 */

/**
 * Implements hook_default_panels_mini().
 */
function mvpcreator_blog_default_panels_mini() {
  $export = array();

  $mini = new stdClass();
  $mini->disabled = FALSE; /* Edit this to true to make a default mini disabled initially */
  $mini->api_version = 1;
  $mini->name = 'mvpcreator_blog_author';
  $mini->category = 'Blog';
  $mini->admin_title = 'About the author';
  $mini->admin_description = 'Shows the author of the current node.';
  $mini->requiredcontexts = array(
    1 => array(
      'identifier' => 'Author',
      'keyword' => 'user',
      'name' => 'user',
      'type' => 'select',
      'uid' => '',
      'optional' => 0,
      'id' => 1,
    ),
  );
  $mini->contexts = array();
  $mini->relationships = array();
  $display = new panels_display();
  $display->layout = 'onecol';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'contentmain' => NULL,
      'sidebar' => NULL,
      'middle' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'About %user:name';
  $display->uuid = '5d6eff4a-1a71-4468-9029-55722219565f';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-15b2aa02-b1f6-4c9d-b7b9-b9401ebed3c8';
  $pane->panel = 'middle';
  $pane->type = 'entity_field';
  $pane->subtype = 'user:field_user_picture';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'hidden',
    'formatter' => 'image',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter_settings' => array(
      'image_style' => 'panopoly_image_full',
      'image_link' => 'content',
    ),
    'context' => 'requiredcontext_user_1',
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
  $pane->uuid = '15b2aa02-b1f6-4c9d-b7b9-b9401ebed3c8';
  $display->content['new-15b2aa02-b1f6-4c9d-b7b9-b9401ebed3c8'] = $pane;
  $display->panels['middle'][0] = 'new-15b2aa02-b1f6-4c9d-b7b9-b9401ebed3c8';
  $pane = new stdClass();
  $pane->pid = 'new-1c9a99c6-4269-4fe1-a718-81e156826d4d';
  $pane->panel = 'middle';
  $pane->type = 'entity_field';
  $pane->subtype = 'user:field_user_about';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'hidden',
    'formatter' => 'text_summary_or_trimmed',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter_settings' => array(
      'trim_length' => '300',
    ),
    'context' => 'requiredcontext_user_1',
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
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = '1c9a99c6-4269-4fe1-a718-81e156826d4d';
  $display->content['new-1c9a99c6-4269-4fe1-a718-81e156826d4d'] = $pane;
  $display->panels['middle'][1] = 'new-1c9a99c6-4269-4fe1-a718-81e156826d4d';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $mini->display = $display;
  $export['mvpcreator_blog_author'] = $mini;

  return $export;
}
