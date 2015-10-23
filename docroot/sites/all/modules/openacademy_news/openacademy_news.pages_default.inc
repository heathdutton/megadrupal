<?php
/**
 * @file
 * openacademy_news.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function openacademy_news_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'openacademy_news';
  $page->task = 'page';
  $page->admin_title = 'News';
  $page->admin_description = '';
  $page->path = 'news';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'access content',
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
    'title' => 'News',
    'name' => 'main-menu',
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
  $handler->name = 'page_openacademy_news_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'openacademy_news';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'News',
    'no_blocks' => 1,
    'pipeline' => 'ipe',
    'css_id' => 'news-panel',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'brenham_flipped';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'left' => NULL,
      'right' => NULL,
      'sidebar' => NULL,
      'contentmain' => NULL,
      'header' => NULL,
    ),
    'sidebar' => array(
      'style' => 'background_callout',
    ),
  );
  $display->cache = array();
  $display->title = 'News';
  $display->uuid = '9a51ce7c-4059-2c24-8995-48d577000ea7';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-33e41fb4-a4e0-87b4-1d62-cbe15beb5c70';
    $pane->panel = 'contentmain';
    $pane->type = 'views_panes';
    $pane->subtype = 'openacademy_news-all_news';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'items_per_page' => '7',
      'fields_override' => array(
        'title' => 1,
        'name' => 0,
        'created' => 0,
        'nothing' => 1,
        'field_featured_image' => 1,
        'body' => 1,
      ),
      'override_title' => 0,
      'override_title_text' => '',
      'view_mode' => 'teaser',
      'widget_title' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '33e41fb4-a4e0-87b4-1d62-cbe15beb5c70';
    $display->content['new-33e41fb4-a4e0-87b4-1d62-cbe15beb5c70'] = $pane;
    $display->panels['contentmain'][0] = 'new-33e41fb4-a4e0-87b4-1d62-cbe15beb5c70';
    $pane = new stdClass();
    $pane->pid = 'new-d0fb0a5e-cef5-9d24-7d0c-0bc60f94fc23';
    $pane->panel = 'header';
    $pane->type = 'views_panes';
    $pane->subtype = 'openacademy_news-featured_news';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'exposed' => array(
        'title' => 'Historical Documents on Display',
        'widget_title' => '',
      ),
      'override_title' => 0,
      'override_title_text' => '',
      'view_mode' => 'featured',
      'widget_title' => '',
      'items_per_page' => NULL,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'd0fb0a5e-cef5-9d24-7d0c-0bc60f94fc23';
    $display->content['new-d0fb0a5e-cef5-9d24-7d0c-0bc60f94fc23'] = $pane;
    $display->panels['header'][0] = 'new-d0fb0a5e-cef5-9d24-7d0c-0bc60f94fc23';
    $pane = new stdClass();
    $pane->pid = 'new-a1ae30ea-910b-7b94-1da8-9216ceb56fa0';
    $pane->panel = 'sidebar';
    $pane->type = 'views_panes';
    $pane->subtype = 'openacademy_events-upcoming_events';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'items_per_page' => '3',
      'fields_override' => array(
        'field_featured_image' => 1,
        'field_event_date' => 1,
        'title' => 1,
        'body' => 1,
      ),
      'view_mode' => NULL,
      'widget_title' => 'Upcoming Events',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'a1ae30ea-910b-7b94-1da8-9216ceb56fa0';
    $display->content['new-a1ae30ea-910b-7b94-1da8-9216ceb56fa0'] = $pane;
    $display->panels['sidebar'][0] = 'new-a1ae30ea-910b-7b94-1da8-9216ceb56fa0';
    $pane = new stdClass();
    $pane->pid = 'new-80ba15dd-437d-7614-0d53-50687390ddd9';
    $pane->panel = 'sidebar';
    $pane->type = 'views_panes';
    $pane->subtype = 'openacademy_publications-publication_listing';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'items_per_page' => '3',
      'fields_override' => array(
        'field_featured_image' => 1,
        'title' => 1,
        'body' => 1,
      ),
      'view_mode' => NULL,
      'widget_title' => 'Recent Publications',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '80ba15dd-437d-7614-0d53-50687390ddd9';
    $display->content['new-80ba15dd-437d-7614-0d53-50687390ddd9'] = $pane;
    $display->panels['sidebar'][1] = 'new-80ba15dd-437d-7614-0d53-50687390ddd9';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-33e41fb4-a4e0-87b4-1d62-cbe15beb5c70';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['openacademy_news'] = $page;

  return $pages;

}
