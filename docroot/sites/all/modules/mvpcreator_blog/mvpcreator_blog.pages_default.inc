<?php
/**
 * @file
 * mvpcreator_blog.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function mvpcreator_blog_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_edit_panel_context_mvpcreator_blog';
  $handler->task = 'node_edit';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = -30;
  $handler->conf = array(
    'title' => 'Blog Edit Page',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'css_id' => 'node-edit',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'entity_bundle:node',
          'settings' => array(
            'type' => array(
              'mvpcreator_blog' => 'mvpcreator_blog',
            ),
          ),
          'context' => 'argument_node_edit_1',
          'not' => FALSE,
        ),
      ),
    ),
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
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
  $display->uuid = 'fadf42a3-1ab7-4910-91b4-f3fce9f3107b';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-ffd91963-cdd5-4f68-a786-87037f523268';
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
  $pane->uuid = 'ffd91963-cdd5-4f68-a786-87037f523268';
  $display->content['new-ffd91963-cdd5-4f68-a786-87037f523268'] = $pane;
  $display->panels['contentmain'][0] = 'new-ffd91963-cdd5-4f68-a786-87037f523268';
  $pane = new stdClass();
  $pane->pid = 'new-7123d684-5ce5-4363-9413-3af9e88aa8a1';
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
  $pane->uuid = '7123d684-5ce5-4363-9413-3af9e88aa8a1';
  $display->content['new-7123d684-5ce5-4363-9413-3af9e88aa8a1'] = $pane;
  $display->panels['contentmain'][1] = 'new-7123d684-5ce5-4363-9413-3af9e88aa8a1';
  $pane = new stdClass();
  $pane->pid = 'new-76ef5856-1261-4133-a0f0-cd9dc709e041';
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
  $pane->uuid = '76ef5856-1261-4133-a0f0-cd9dc709e041';
  $display->content['new-76ef5856-1261-4133-a0f0-cd9dc709e041'] = $pane;
  $display->panels['contentmain'][2] = 'new-76ef5856-1261-4133-a0f0-cd9dc709e041';
  $pane = new stdClass();
  $pane->pid = 'new-0b9944a2-9a6b-476b-b559-d89bab872026';
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
  $pane->uuid = '0b9944a2-9a6b-476b-b559-d89bab872026';
  $display->content['new-0b9944a2-9a6b-476b-b559-d89bab872026'] = $pane;
  $display->panels['sidebar'][0] = 'new-0b9944a2-9a6b-476b-b559-d89bab872026';
  $pane = new stdClass();
  $pane->pid = 'new-c0431fd5-6cd0-494a-9f88-0ea381d38096';
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
  $pane->uuid = 'c0431fd5-6cd0-494a-9f88-0ea381d38096';
  $display->content['new-c0431fd5-6cd0-494a-9f88-0ea381d38096'] = $pane;
  $display->panels['sidebar'][1] = 'new-c0431fd5-6cd0-494a-9f88-0ea381d38096';
  $pane = new stdClass();
  $pane->pid = 'new-7b3ac7d8-4cae-47a7-a707-140d09714319';
  $pane->panel = 'sidebar';
  $pane->type = 'entity_form_field';
  $pane->subtype = 'node:field_mvpcreator_blog_tags';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => '',
    'formatter' => '',
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
  $pane->uuid = '7b3ac7d8-4cae-47a7-a707-140d09714319';
  $display->content['new-7b3ac7d8-4cae-47a7-a707-140d09714319'] = $pane;
  $display->panels['sidebar'][2] = 'new-7b3ac7d8-4cae-47a7-a707-140d09714319';
  $pane = new stdClass();
  $pane->pid = 'new-3b7f9f5e-2b9d-4f5f-b4a7-0d0d152baf69';
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
  $pane->position = 3;
  $pane->locks = array();
  $pane->uuid = '3b7f9f5e-2b9d-4f5f-b4a7-0d0d152baf69';
  $display->content['new-3b7f9f5e-2b9d-4f5f-b4a7-0d0d152baf69'] = $pane;
  $display->panels['sidebar'][3] = 'new-3b7f9f5e-2b9d-4f5f-b4a7-0d0d152baf69';
  $pane = new stdClass();
  $pane->pid = 'new-106a0ca8-f556-4859-aafe-e11a66dea53a';
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
  $pane->position = 4;
  $pane->locks = array();
  $pane->uuid = '106a0ca8-f556-4859-aafe-e11a66dea53a';
  $display->content['new-106a0ca8-f556-4859-aafe-e11a66dea53a'] = $pane;
  $display->panels['sidebar'][4] = 'new-106a0ca8-f556-4859-aafe-e11a66dea53a';
  $pane = new stdClass();
  $pane->pid = 'new-0dd74e6a-04cc-49ea-b677-cd8fb82d37e3';
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
  $pane->position = 5;
  $pane->locks = array();
  $pane->uuid = '0dd74e6a-04cc-49ea-b677-cd8fb82d37e3';
  $display->content['new-0dd74e6a-04cc-49ea-b677-cd8fb82d37e3'] = $pane;
  $display->panels['sidebar'][5] = 'new-0dd74e6a-04cc-49ea-b677-cd8fb82d37e3';
  $pane = new stdClass();
  $pane->pid = 'new-df0ec4e4-e4a7-41c8-80c7-7280d78550b5';
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
  $pane->position = 6;
  $pane->locks = array();
  $pane->uuid = 'df0ec4e4-e4a7-41c8-80c7-7280d78550b5';
  $display->content['new-df0ec4e4-e4a7-41c8-80c7-7280d78550b5'] = $pane;
  $display->panels['sidebar'][6] = 'new-df0ec4e4-e4a7-41c8-80c7-7280d78550b5';
  $pane = new stdClass();
  $pane->pid = 'new-c9b880e2-ceec-486a-8c4e-7c02b7c85b90';
  $pane->panel = 'sidebar';
  $pane->type = 'entity_form_field';
  $pane->subtype = 'node:field_featured_status';
  $pane->shown = TRUE;
  $pane->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'administer nodes',
        ),
        'context' => 'logged-in-user',
        'not' => TRUE,
      ),
    ),
  );
  $pane->configuration = array(
    'label' => '',
    'formatter' => '',
    'context' => 'argument_node_edit_1',
    'override_title' => 1,
    'override_title_text' => 'Publishing options',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 7;
  $pane->locks = array();
  $pane->uuid = 'c9b880e2-ceec-486a-8c4e-7c02b7c85b90';
  $display->content['new-c9b880e2-ceec-486a-8c4e-7c02b7c85b90'] = $pane;
  $display->panels['sidebar'][7] = 'new-c9b880e2-ceec-486a-8c4e-7c02b7c85b90';
  $pane = new stdClass();
  $pane->pid = 'new-b15b2bd2-cf81-4daa-975a-d943d914ce5e';
  $pane->panel = 'sidebar';
  $pane->type = 'node_form_log';
  $pane->subtype = 'node_form_log';
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
  $pane->position = 8;
  $pane->locks = array();
  $pane->uuid = 'b15b2bd2-cf81-4daa-975a-d943d914ce5e';
  $display->content['new-b15b2bd2-cf81-4daa-975a-d943d914ce5e'] = $pane;
  $display->panels['sidebar'][8] = 'new-b15b2bd2-cf81-4daa-975a-d943d914ce5e';
  $pane = new stdClass();
  $pane->pid = 'new-95f0e4cb-847d-46f2-939e-c437c75f42ff';
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
  $pane->position = 9;
  $pane->locks = array();
  $pane->uuid = '95f0e4cb-847d-46f2-939e-c437c75f42ff';
  $display->content['new-95f0e4cb-847d-46f2-939e-c437c75f42ff'] = $pane;
  $display->panels['sidebar'][9] = 'new-95f0e4cb-847d-46f2-939e-c437c75f42ff';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-76ef5856-1261-4133-a0f0-cd9dc709e041';
  $handler->conf['display'] = $display;
  $export['node_edit_panel_context_mvpcreator_blog'] = $handler;

  return $export;
}

/**
 * Implements hook_default_page_manager_pages().
 */
function mvpcreator_blog_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'mvpcreator_blog_home_page';
  $page->task = 'page';
  $page->admin_title = 'MVPCreator Blog Home Page';
  $page->admin_description = 'The blog front page.';
  $page->path = 'blog';
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
    'title' => 'Blog',
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
  $handler->name = 'page_mvpcreator_blog_home_page_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'mvpcreator_blog_home_page';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => 'mvpcreator-blog-home-page',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'selby_flipped';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'sidebar' => NULL,
      'contentmain' => NULL,
      'contentheader' => NULL,
      'contentcolumn1' => NULL,
      'contentcolumn2' => NULL,
      'contentfooter' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Blog';
  $display->uuid = '92e1bd98-a328-40d3-bc2f-e1e735772d19';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-4366c1c6-ad7a-407b-98f9-1e15663c73c5';
  $pane->panel = 'contentheader';
  $pane->type = 'views_panes';
  $pane->subtype = 'mvpcreator_blog-list_blog_posts';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'items_per_page' => '10',
    'fields_override' => array(
      'field_featured_image' => 1,
      'title' => 1,
      'body' => 1,
      'created' => 1,
    ),
    'exposed' => array(
      'Is Featured' => 'All',
      'widget_title' => '',
      'use_pager' => 1,
      'pager_id' => '',
      'sort_by' => 'created',
      'sort_order' => 'DESC',
    ),
    'context' => array(
      0 => 'empty',
    ),
    'override_title' => '',
    'override_title_text' => '',
    'view_mode' => 'teaser',
    'widget_title' => '',
    'use_pager' => NULL,
    'pager_id' => NULL,
    'offset' => NULL,
    'link_to_view' => NULL,
    'more_link' => NULL,
    'path' => NULL,
    'view_settings' => 'nodes',
    'header_type' => 'none',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '4366c1c6-ad7a-407b-98f9-1e15663c73c5';
  $display->content['new-4366c1c6-ad7a-407b-98f9-1e15663c73c5'] = $pane;
  $display->panels['contentheader'][0] = 'new-4366c1c6-ad7a-407b-98f9-1e15663c73c5';
  $pane = new stdClass();
  $pane->pid = 'new-5222d43a-bff6-4d8e-b5ba-ff968b725f66';
  $pane->panel = 'sidebar';
  $pane->type = 'views_panes';
  $pane->subtype = 'mvpcreator_blog-popular_blog_topics';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'items_per_page' => '10',
    'exposed' => array(
      'widget_title' => 'Popular topics',
      'use_pager' => 0,
      'pager_id' => '0',
      'sort_by' => NULL,
      'sort_order' => NULL,
    ),
    'override_title' => '',
    'override_title_text' => '',
    'view_mode' => 'teaser',
    'widget_title' => 'Popular topics',
    'use_pager' => NULL,
    'pager_id' => NULL,
    'offset' => NULL,
    'link_to_view' => NULL,
    'more_link' => NULL,
    'path' => NULL,
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '5222d43a-bff6-4d8e-b5ba-ff968b725f66';
  $display->content['new-5222d43a-bff6-4d8e-b5ba-ff968b725f66'] = $pane;
  $display->panels['sidebar'][0] = 'new-5222d43a-bff6-4d8e-b5ba-ff968b725f66';
  $pane = new stdClass();
  $pane->pid = 'new-d5641167-536a-4392-8373-e94a2e9016d5';
  $pane->panel = 'sidebar';
  $pane->type = 'mvpcreator_blog_rss';
  $pane->subtype = 'mvpcreator_blog_rss';
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
  $pane->uuid = 'd5641167-536a-4392-8373-e94a2e9016d5';
  $display->content['new-d5641167-536a-4392-8373-e94a2e9016d5'] = $pane;
  $display->panels['sidebar'][1] = 'new-d5641167-536a-4392-8373-e94a2e9016d5';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-4366c1c6-ad7a-407b-98f9-1e15663c73c5';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['mvpcreator_blog_home_page'] = $page;

  return $pages;

}
