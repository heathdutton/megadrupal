<?php
/**
 * @file
 * recruiter_menu.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function recruiter_menu_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'pm_existing_pages_manage_jobs_panel_context';
  $handler->task = 'pm_existing_pages';
  $handler->subtask = 'manage_jobs';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Anonymou',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'role',
          'settings' => array(
            'rids' => array(
              0 => 30037204,
              1 => 100482362,
              2 => 139881272,
              3 => 29617163,
            ),
          ),
          'context' => 'logged-in-user',
          'not' => TRUE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'onecol';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'middle' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Manage jobs';
  $display->uuid = 'ad1e6165-7dd2-7314-e553-4825ebcf4ce8';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-81e227dc-51e6-cad4-4d82-90c278ab1031';
    $pane->panel = 'middle';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => '',
      'title' => '',
      'body' => 'You need to login as recruiter in order to publish jobs.',
      'format' => 'full_html',
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
    $pane->uuid = '81e227dc-51e6-cad4-4d82-90c278ab1031';
    $display->content['new-81e227dc-51e6-cad4-4d82-90c278ab1031'] = $pane;
    $display->panels['middle'][0] = 'new-81e227dc-51e6-cad4-4d82-90c278ab1031';
    $pane = new stdClass();
    $pane->pid = 'new-5aefceae-d1e6-3e84-716c-b4e4088cc500';
    $pane->panel = 'middle';
    $pane->type = 'block';
    $pane->subtype = 'user-login';
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
    $pane->uuid = '5aefceae-d1e6-3e84-716c-b4e4088cc500';
    $display->content['new-5aefceae-d1e6-3e84-716c-b4e4088cc500'] = $pane;
    $display->panels['middle'][1] = 'new-5aefceae-d1e6-3e84-716c-b4e4088cc500';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-81e227dc-51e6-cad4-4d82-90c278ab1031';
  $handler->conf['display'] = $display;
  $export['pm_existing_pages_manage_jobs_panel_context'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'pm_existing_pages_resume_panel_context';
  $handler->task = 'pm_existing_pages';
  $handler->subtask = 'resume';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Anonymous',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'role',
          'settings' => array(
            'rids' => array(
              0 => 30037204,
              1 => 115362423,
              2 => 29617163,
            ),
          ),
          'context' => 'logged-in-user',
          'not' => TRUE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'onecol';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'middle' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Resume';
  $display->uuid = '11c8a598-30c1-2d34-e925-832e68b7157c';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-1af54f59-9d7c-5584-edef-a308c1e70a97';
    $pane->panel = 'middle';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => 'Info',
      'title' => '',
      'body' => 'You need to login as applicant in order the create your resume!',
      'format' => 'plain_text',
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
    $pane->uuid = '1af54f59-9d7c-5584-edef-a308c1e70a97';
    $display->content['new-1af54f59-9d7c-5584-edef-a308c1e70a97'] = $pane;
    $display->panels['middle'][0] = 'new-1af54f59-9d7c-5584-edef-a308c1e70a97';
    $pane = new stdClass();
    $pane->pid = 'new-9a569094-217c-3e04-ed95-080a27fcf078';
    $pane->panel = 'middle';
    $pane->type = 'block';
    $pane->subtype = 'user-login';
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
    $pane->uuid = '9a569094-217c-3e04-ed95-080a27fcf078';
    $display->content['new-9a569094-217c-3e04-ed95-080a27fcf078'] = $pane;
    $display->panels['middle'][1] = 'new-9a569094-217c-3e04-ed95-080a27fcf078';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['pm_existing_pages_resume_panel_context'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'pm_existing_pages_resume_search_panel_context';
  $handler->task = 'pm_existing_pages';
  $handler->subtask = 'resume_search';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Anonymous',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'role',
          'settings' => array(
            'rids' => array(
              0 => 30037204,
              1 => 100482362,
              2 => 29617163,
            ),
          ),
          'context' => 'logged-in-user',
          'not' => TRUE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'onecol';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'middle' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Resume search';
  $display->uuid = '63f902b1-6bdb-44d4-855d-f6a9095cdc88';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-629539b7-b38d-3b14-fdd5-ae13a9412bcc';
    $pane->panel = 'middle';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => 'Info',
      'title' => '',
      'body' => 'You need to login as recruiter in order to search for resumes!',
      'format' => 'full_html',
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
    $pane->uuid = '629539b7-b38d-3b14-fdd5-ae13a9412bcc';
    $display->content['new-629539b7-b38d-3b14-fdd5-ae13a9412bcc'] = $pane;
    $display->panels['middle'][0] = 'new-629539b7-b38d-3b14-fdd5-ae13a9412bcc';
    $pane = new stdClass();
    $pane->pid = 'new-0fc7c354-cca0-bc54-c958-06da1b576529';
    $pane->panel = 'middle';
    $pane->type = 'block';
    $pane->subtype = 'user-login';
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
    $pane->uuid = '0fc7c354-cca0-bc54-c958-06da1b576529';
    $display->content['new-0fc7c354-cca0-bc54-c958-06da1b576529'] = $pane;
    $display->panels['middle'][1] = 'new-0fc7c354-cca0-bc54-c958-06da1b576529';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-629539b7-b38d-3b14-fdd5-ae13a9412bcc';
  $handler->conf['display'] = $display;
  $export['pm_existing_pages_resume_search_panel_context'] = $handler;

  return $export;
}

/**
 * Implements hook_default_page_manager_pages().
 */
function recruiter_menu_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'recruiter_home';
  $page->task = 'page';
  $page->admin_title = 'Recruiter home';
  $page->admin_description = '';
  $page->path = 'recruiter';
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
  $handler->name = 'page_recruiter_home_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'recruiter_home';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => 'page-recruiter-frontpage',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'onecol';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'middle' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Recruiter';
  $display->uuid = '9efd52d2-e254-7a34-75ea-5b293c6a53df';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-542ddc72-6e09-b964-c541-c8adb373bfab';
    $pane->panel = 'middle';
    $pane->type = 'views_panes';
    $pane->subtype = 'recruiter_frontpage-panel_pane_1';
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
    $pane->uuid = '542ddc72-6e09-b964-c541-c8adb373bfab';
    $display->content['new-542ddc72-6e09-b964-c541-c8adb373bfab'] = $pane;
    $display->panels['middle'][0] = 'new-542ddc72-6e09-b964-c541-c8adb373bfab';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['recruiter_home'] = $page;

  return $pages;

}
