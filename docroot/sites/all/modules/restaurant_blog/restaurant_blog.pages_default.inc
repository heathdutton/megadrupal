<?php
/**
 * @file
 * restaurant_blog.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function restaurant_blog_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'news';
  $page->task = 'page';
  $page->admin_title = 'News';
  $page->admin_description = '';
  $page->path = 'news';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'normal',
    'title' => 'News',
    'name' => 'main-menu',
    'weight' => '3',
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
  $handler->name = 'page_news__news';
  $handler->task = 'page';
  $handler->subtask = 'news';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'News',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'name' => 'news',
  );
  $display = new panels_display();
  $display->layout = 'layout_news';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'top' => NULL,
      'contentmain' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'News';
  $display->uuid = 'f4525f8c-9249-4869-b5cf-90742b4f4e6b';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-95f4ce96-3bf6-4f59-97c8-3b5fb4382ca5';
  $pane->panel = 'contentmain';
  $pane->type = 'views_panes';
  $pane->subtype = 'blog-blog';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'view_mode' => 'teaser',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '95f4ce96-3bf6-4f59-97c8-3b5fb4382ca5';
  $display->content['new-95f4ce96-3bf6-4f59-97c8-3b5fb4382ca5'] = $pane;
  $display->panels['contentmain'][0] = 'new-95f4ce96-3bf6-4f59-97c8-3b5fb4382ca5';
  $pane = new stdClass();
  $pane->pid = 'new-33c269f9-f5b5-46da-be7e-ecf23a8af71a';
  $pane->panel = 'slideshow';
  $pane->type = 'fieldable_panels_pane';
  $pane->subtype = 'fpid:5';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'view_mode' => 'full',
    'fieldable_panels_pane' => (object) array(
      'bundle' => 'slideshow',
      'language' => '',
      'is_new' => TRUE,
      'title' => 'News',
      'link' => 0,
      'path' => '',
      'reusable' => 1,
      'admin_title' => 'News slideshow',
      'admin_description' => '',
      'category' => 'Reusable Content',
      'vid' => '16',
      'current_vid' => '',
      'revision' => 1,
      'fpid' => '5',
      'field_slideshow_slides' => array(
        'und' => array(
          0 => array(
            'value' => '13',
          ),
        ),
      ),
      'log' => '',
      'created' => 1436899735,
      'timestamp' => 1436899735,
      'changed' => 1436899735,
      'uuid' => '',
      'uid' => '1',
      'vuuid' => '',
    ),
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '33c269f9-f5b5-46da-be7e-ecf23a8af71a';
  $display->content['new-33c269f9-f5b5-46da-be7e-ecf23a8af71a'] = $pane;
  $display->panels['slideshow'][0] = 'new-33c269f9-f5b5-46da-be7e-ecf23a8af71a';
  $pane = new stdClass();
  $pane->pid = 'new-4a4ae7ea-471b-47be-830b-4b16966f1e6d';
  $pane->panel = 'top';
  $pane->type = 'fieldable_panels_pane';
  $pane->subtype = 'fpid:8';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'view_mode' => 'full',
    'fieldable_panels_pane' => (object) array(
      'timestamp' => 1436909468,
      'uid' => '1',
      'title' => '',
      'log' => '',
      'vuuid' => '',
      'fpid' => '8',
      'bundle' => 'text',
      'link' => 0,
      'path' => '',
      'reusable' => 1,
      'admin_title' => 'News top text',
      'admin_description' => '',
      'category' => 'Reusable Content',
      'view_access' => NULL,
      'edit_access' => NULL,
      'created' => '1436909439',
      'changed' => 1436909468,
      'uuid' => '',
      'language' => 'und',
      'current_vid' => '22',
      'field_basic_text_text' => array(
        'und' => array(
          0 => array(
            'value' => '<h3 class="text--light">Our News &amp; Blog</h3>',
            'format' => 'panopoly_html_text',
          ),
        ),
      ),
      'revision' => 1,
      'is_new' => FALSE,
      'original' => (object) array(
        'vid' => '22',
        'timestamp' => '1436909439',
        'uid' => '1',
        'title' => '',
        'log' => '',
        'vuuid' => '',
        'fpid' => '8',
        'bundle' => 'text',
        'link' => '0',
        'path' => '',
        'reusable' => '0',
        'admin_title' => '',
        'admin_description' => '',
        'category' => 'Reusable Content',
        'view_access' => NULL,
        'edit_access' => NULL,
        'created' => '1436909439',
        'changed' => '1436909439',
        'uuid' => '',
        'language' => 'und',
        'current_vid' => '22',
        'field_basic_text_text' => array(
          'und' => array(
            0 => array(
              'value' => '<h3 class="text--light">Our News &amp; Blog</h3>',
              'format' => 'panopoly_html_text',
              'safe_value' => '<h3 class="text--light">Our News &amp; Blog</h3>
',
            ),
          ),
        ),
      ),
      'old_vid' => '22',
      'vid' => '23',
    ),
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '4a4ae7ea-471b-47be-830b-4b16966f1e6d';
  $display->content['new-4a4ae7ea-471b-47be-830b-4b16966f1e6d'] = $pane;
  $display->panels['top'][0] = 'new-4a4ae7ea-471b-47be-830b-4b16966f1e6d';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['news'] = $page;

  return $pages;

}
