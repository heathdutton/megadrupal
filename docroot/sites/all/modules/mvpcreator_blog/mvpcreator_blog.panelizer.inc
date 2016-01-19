<?php
/**
 * @file
 * mvpcreator_blog.panelizer.inc
 */

/**
 * Implements hook_panelizer_defaults().
 */
function mvpcreator_blog_panelizer_defaults() {
  $export = array();

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:mvpcreator_blog:default';
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'mvpcreator_blog';
  $panelizer->no_blocks = FALSE;
  $panelizer->css_id = '';
  $panelizer->css = '';
  $panelizer->pipeline = 'ipe';
  $panelizer->contexts = array();
  $panelizer->relationships = array(
    0 => array(
      'identifier' => 'User from Node (on node.node_author)',
      'keyword' => 'user',
      'name' => 'entity_from_schema:uid-node-user',
      'context' => 'panelizer',
      'id' => 1,
    ),
    1 => array(
      'identifier' => 'Multiple terms from node',
      'keyword' => 'terms',
      'name' => 'terms_from_node',
      'vocabulary' => array(
        'panopoly_categories' => 'panopoly_categories',
      ),
      'concatenator' => '+',
      'context' => 'panelizer',
      'id' => 1,
    ),
  );
  $panelizer->access = array();
  $panelizer->view_mode = 'page_manager';
  $panelizer->css_class = 'mvpcreator-blog-page';
  $panelizer->title_element = 'H2';
  $panelizer->link_to_entity = TRUE;
  $panelizer->extra = array(
    'panels_breadcrumbs_state' => 1,
    'panels_breadcrumbs_titles' => 'Blog
%node:title',
    'panels_breadcrumbs_paths' => 'blog',
    'panels_breadcrumbs_home' => 1,
  );
  $display = new panels_display();
  $display->layout = 'radix_bryant_flipped';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'sidebar' => NULL,
      'contentmain' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '%node:title';
  $display->uuid = 'f9b7ebb3-a6f2-44f8-8a94-7dc51d51221c';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-e75f69cc-632a-458f-a211-75b0a6833dac';
  $pane->panel = 'contentmain';
  $pane->type = 'node_content';
  $pane->subtype = 'node_content';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'links' => 0,
    'no_extras' => 1,
    'override_title' => 1,
    'override_title_text' => '',
    'identifier' => '',
    'link' => 0,
    'leave_node_title' => 0,
    'build_mode' => 'full',
    'context' => array(),
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = 'e75f69cc-632a-458f-a211-75b0a6833dac';
  $display->content['new-e75f69cc-632a-458f-a211-75b0a6833dac'] = $pane;
  $display->panels['contentmain'][0] = 'new-e75f69cc-632a-458f-a211-75b0a6833dac';
  $pane = new stdClass();
  $pane->pid = 'new-f808cf3a-4265-4583-b189-5d3cec84c748';
  $pane->panel = 'contentmain';
  $pane->type = 'node_comments';
  $pane->subtype = 'node_comments';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'mode' => '1',
    'comments_per_page' => '50',
    'context' => 'panelizer',
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
  $pane->uuid = 'f808cf3a-4265-4583-b189-5d3cec84c748';
  $display->content['new-f808cf3a-4265-4583-b189-5d3cec84c748'] = $pane;
  $display->panels['contentmain'][1] = 'new-f808cf3a-4265-4583-b189-5d3cec84c748';
  $pane = new stdClass();
  $pane->pid = 'new-89d4fadf-6912-4699-a6ac-431a788e7b10';
  $pane->panel = 'contentmain';
  $pane->type = 'node_comment_form';
  $pane->subtype = 'node_comment_form';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'anon_links' => 0,
    'context' => 'panelizer',
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
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = '89d4fadf-6912-4699-a6ac-431a788e7b10';
  $display->content['new-89d4fadf-6912-4699-a6ac-431a788e7b10'] = $pane;
  $display->panels['contentmain'][2] = 'new-89d4fadf-6912-4699-a6ac-431a788e7b10';
  $pane = new stdClass();
  $pane->pid = 'new-5b5d5ee0-f48a-4ce9-86a3-6a5ee3e4ee40';
  $pane->panel = 'sidebar';
  $pane->type = 'panels_mini';
  $pane->subtype = 'mvpcreator_blog_author';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => array(
      0 => 'relationship_entity_from_schema:uid-node-user_1',
    ),
    'override_title' => 0,
    'override_title_text' => '',
    'override_title_heading' => 'h2',
  );
  $pane->cache = array(
    'method' => 0,
  );
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '5b5d5ee0-f48a-4ce9-86a3-6a5ee3e4ee40';
  $display->content['new-5b5d5ee0-f48a-4ce9-86a3-6a5ee3e4ee40'] = $pane;
  $display->panels['sidebar'][0] = 'new-5b5d5ee0-f48a-4ce9-86a3-6a5ee3e4ee40';
  $pane = new stdClass();
  $pane->pid = 'new-f6223cab-ff3d-43db-b535-9c54194068d0';
  $pane->panel = 'sidebar';
  $pane->type = 'views_panes';
  $pane->subtype = 'mvpcreator_blog-articles_by_author';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'items_per_page' => '5',
    'fields_override' => array(
      'field_featured_image' => 0,
      'title' => 1,
      'body' => 0,
      'created' => 0,
    ),
    'exposed' => array(
      'widget_title' => 'More By This Author',
      'sort_by' => 'created',
      'sort_order' => 'DESC',
    ),
    'context' => array(
      0 => 'panelizer',
      1 => 'relationship_entity_from_schema:uid-node-user_1',
    ),
    'override_title' => '',
    'override_title_text' => '',
    'view_mode' => 'teaser',
    'widget_title' => 'More By This Author',
    'use_pager' => NULL,
    'pager_id' => NULL,
    'offset' => NULL,
    'link_to_view' => NULL,
    'more_link' => NULL,
    'path' => NULL,
    'view_settings' => 'fields',
    'header_type' => 'none',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = 'f6223cab-ff3d-43db-b535-9c54194068d0';
  $display->content['new-f6223cab-ff3d-43db-b535-9c54194068d0'] = $pane;
  $display->panels['sidebar'][1] = 'new-f6223cab-ff3d-43db-b535-9c54194068d0';
  $pane = new stdClass();
  $pane->pid = 'new-74354596-091a-4aa7-91ca-330856faa5df';
  $pane->panel = 'sidebar';
  $pane->type = 'views_panes';
  $pane->subtype = 'mvpcreator_blog-related_blog_posts';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'items_per_page' => '5',
    'fields_override' => array(
      'field_featured_image' => 1,
      'title' => 1,
      'body' => 0,
      'created' => 1,
    ),
    'exposed' => array(
      'widget_title' => 'Related Blog Posts',
      'sort_by' => 'created',
      'sort_order' => 'DESC',
    ),
    'context' => array(
      0 => 'panelizer',
      1 => 'relationship_terms_from_node_1',
    ),
    'override_title' => '',
    'override_title_text' => '',
    'view_mode' => 'teaser',
    'widget_title' => 'Related Blog Posts',
    'use_pager' => NULL,
    'pager_id' => NULL,
    'offset' => NULL,
    'link_to_view' => NULL,
    'more_link' => NULL,
    'path' => NULL,
    'view_settings' => 'fields',
    'header_type' => 'none',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = '74354596-091a-4aa7-91ca-330856faa5df';
  $display->content['new-74354596-091a-4aa7-91ca-330856faa5df'] = $pane;
  $display->panels['sidebar'][2] = 'new-74354596-091a-4aa7-91ca-330856faa5df';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $panelizer->display = $display;
  $export['node:mvpcreator_blog:default'] = $panelizer;

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:mvpcreator_blog:default:default';
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'mvpcreator_blog';
  $panelizer->no_blocks = FALSE;
  $panelizer->css_id = '';
  $panelizer->css = '';
  $panelizer->pipeline = 'standard';
  $panelizer->contexts = array();
  $panelizer->relationships = array();
  $panelizer->access = array();
  $panelizer->view_mode = 'default';
  $panelizer->css_class = 'mvpcreator-blog-default';
  $panelizer->title_element = 'H2';
  $panelizer->link_to_entity = TRUE;
  $panelizer->extra = array();
  $display = new panels_display();
  $display->layout = 'radix_boxton';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'contentmain' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '%node:title';
  $display->uuid = 'cdd0ad44-7060-41d5-8203-8114f90f68a1';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-faf884ff-23d6-446f-a791-f56bae9d5aa1';
  $pane->panel = 'contentmain';
  $pane->type = 'custom';
  $pane->subtype = 'custom';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'admin_title' => 'Submission information',
    'title' => '',
    'body' => '<p>by %node:author on %node:created</p>',
    'format' => 'panopoly_wysiwyg_text',
    'substitute' => 1,
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array(
    'css_id' => '',
    'css_class' => 'mvpcreator-blog-submission-info',
  );
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = 'faf884ff-23d6-446f-a791-f56bae9d5aa1';
  $display->content['new-faf884ff-23d6-446f-a791-f56bae9d5aa1'] = $pane;
  $display->panels['contentmain'][0] = 'new-faf884ff-23d6-446f-a791-f56bae9d5aa1';
  $pane = new stdClass();
  $pane->pid = 'new-bc178282-9387-47c4-9a8c-c1eff4238902';
  $pane->panel = 'contentmain';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:field_featured_image';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'hidden',
    'formatter' => 'image',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter_settings' => array(
      'image_style' => 'panopoly_image_quarter',
      'image_link' => '',
    ),
    'context' => 'panelizer',
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
  $pane->uuid = 'bc178282-9387-47c4-9a8c-c1eff4238902';
  $display->content['new-bc178282-9387-47c4-9a8c-c1eff4238902'] = $pane;
  $display->panels['contentmain'][1] = 'new-bc178282-9387-47c4-9a8c-c1eff4238902';
  $pane = new stdClass();
  $pane->pid = 'new-4ea2990c-8ea0-4b61-ac3f-b413b2569ec0';
  $pane->panel = 'contentmain';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:body';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'hidden',
    'formatter' => 'text_default',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter_settings' => array(),
    'context' => 'panelizer',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array(
    'css_id' => '',
    'css_class' => 'clearfix',
  );
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = '4ea2990c-8ea0-4b61-ac3f-b413b2569ec0';
  $display->content['new-4ea2990c-8ea0-4b61-ac3f-b413b2569ec0'] = $pane;
  $display->panels['contentmain'][2] = 'new-4ea2990c-8ea0-4b61-ac3f-b413b2569ec0';
  $pane = new stdClass();
  $pane->pid = 'new-90594bbb-2bfd-44b3-92db-068ac8e119c2';
  $pane->panel = 'contentmain';
  $pane->type = 'node_terms';
  $pane->subtype = 'node_terms';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'vid' => '0',
    'term_format' => 'term-links',
    'link' => 1,
    'term_delimiter' => ', ',
    'context' => 'panelizer',
    'override_title' => 1,
    'override_title_text' => 'Topics:',
    'override_title_heading' => 'div',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 3;
  $pane->locks = array();
  $pane->uuid = '90594bbb-2bfd-44b3-92db-068ac8e119c2';
  $display->content['new-90594bbb-2bfd-44b3-92db-068ac8e119c2'] = $pane;
  $display->panels['contentmain'][3] = 'new-90594bbb-2bfd-44b3-92db-068ac8e119c2';
  $pane = new stdClass();
  $pane->pid = 'new-0dab1974-37b8-4cb8-bc45-32c3711ad7f1';
  $pane->panel = 'contentmain';
  $pane->type = 'node_links';
  $pane->subtype = 'node_links';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => FALSE,
    'override_title_text' => '',
    'build_mode' => 'default',
    'identifier' => '',
    'link' => TRUE,
    'context' => 'panelizer',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array(
    'css_class' => 'link-wrapper',
  );
  $pane->extras = array();
  $pane->position = 4;
  $pane->locks = array();
  $pane->uuid = '0dab1974-37b8-4cb8-bc45-32c3711ad7f1';
  $display->content['new-0dab1974-37b8-4cb8-bc45-32c3711ad7f1'] = $pane;
  $display->panels['contentmain'][4] = 'new-0dab1974-37b8-4cb8-bc45-32c3711ad7f1';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = 'new-bc178282-9387-47c4-9a8c-c1eff4238902';
  $panelizer->display = $display;
  $export['node:mvpcreator_blog:default:default'] = $panelizer;

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:mvpcreator_blog:default:featured';
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'mvpcreator_blog';
  $panelizer->no_blocks = FALSE;
  $panelizer->css_id = '';
  $panelizer->css = '';
  $panelizer->pipeline = 'standard';
  $panelizer->contexts = array();
  $panelizer->relationships = array();
  $panelizer->access = array();
  $panelizer->view_mode = 'featured';
  $panelizer->css_class = 'mvpcreator-blog-featured';
  $panelizer->title_element = 'H2';
  $panelizer->link_to_entity = TRUE;
  $panelizer->extra = array();
  $display = new panels_display();
  $display->layout = 'boxton';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'contentmain' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '%node:title';
  $display->uuid = 'f0c963ec-2930-474f-b2d4-cef557125c7d';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-d4b64e4e-6416-4909-b9e7-130634c738a1';
  $pane->panel = 'contentmain';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:field_featured_image';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'hidden',
    'formatter' => 'image',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter_settings' => array(
      'image_style' => 'panopoly_image_half',
      'image_link' => 'content',
    ),
    'context' => 'panelizer',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = 'd4b64e4e-6416-4909-b9e7-130634c738a1';
  $display->content['new-d4b64e4e-6416-4909-b9e7-130634c738a1'] = $pane;
  $display->panels['contentmain'][0] = 'new-d4b64e4e-6416-4909-b9e7-130634c738a1';
  $pane = new stdClass();
  $pane->pid = 'new-e4a102c1-602a-4083-9e46-958da9748cf0';
  $pane->panel = 'contentmain';
  $pane->type = 'node_title';
  $pane->subtype = 'node_title';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'link' => 1,
    'markup' => 'none',
    'id' => '',
    'class' => '',
    'context' => 'panelizer',
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
  $pane->uuid = 'e4a102c1-602a-4083-9e46-958da9748cf0';
  $display->content['new-e4a102c1-602a-4083-9e46-958da9748cf0'] = $pane;
  $display->panels['contentmain'][1] = 'new-e4a102c1-602a-4083-9e46-958da9748cf0';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = 'new-d4b64e4e-6416-4909-b9e7-130634c738a1';
  $panelizer->display = $display;
  $export['node:mvpcreator_blog:default:featured'] = $panelizer;

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:mvpcreator_blog:default:teaser';
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'mvpcreator_blog';
  $panelizer->no_blocks = FALSE;
  $panelizer->css_id = '';
  $panelizer->css = '';
  $panelizer->pipeline = 'standard';
  $panelizer->contexts = array();
  $panelizer->relationships = array();
  $panelizer->access = array();
  $panelizer->view_mode = 'teaser';
  $panelizer->css_class = 'mvpcreator-blog-teaser';
  $panelizer->title_element = 'H2';
  $panelizer->link_to_entity = TRUE;
  $panelizer->extra = array();
  $display = new panels_display();
  $display->layout = 'radix_boxton';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'contentmain' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '%node:title';
  $display->uuid = 'cc2c786b-d666-4a6e-b320-fdd0ff71f3f7';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-a5ca80ad-9507-42aa-84f2-dde83cee9645';
  $pane->panel = 'contentmain';
  $pane->type = 'custom';
  $pane->subtype = 'custom';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'admin_title' => 'Submission information',
    'title' => '',
    'body' => '<p>by %node:author on %node:created</p>',
    'format' => 'panopoly_wysiwyg_text',
    'substitute' => 1,
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array(
    'css_id' => '',
    'css_class' => 'mvpcreator-blog-submission-info',
  );
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = 'a5ca80ad-9507-42aa-84f2-dde83cee9645';
  $display->content['new-a5ca80ad-9507-42aa-84f2-dde83cee9645'] = $pane;
  $display->panels['contentmain'][0] = 'new-a5ca80ad-9507-42aa-84f2-dde83cee9645';
  $pane = new stdClass();
  $pane->pid = 'new-8c9ff2bc-2950-48e7-90f2-cfcef3789ad4';
  $pane->panel = 'contentmain';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:field_featured_image';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'hidden',
    'formatter' => 'image',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter_settings' => array(
      'image_style' => 'panopoly_image_quarter',
      'image_link' => 'content',
    ),
    'context' => 'panelizer',
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
  $pane->uuid = '8c9ff2bc-2950-48e7-90f2-cfcef3789ad4';
  $display->content['new-8c9ff2bc-2950-48e7-90f2-cfcef3789ad4'] = $pane;
  $display->panels['contentmain'][1] = 'new-8c9ff2bc-2950-48e7-90f2-cfcef3789ad4';
  $pane = new stdClass();
  $pane->pid = 'new-994f23d0-ec23-4045-9911-dd4f8f49c371';
  $pane->panel = 'contentmain';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:body';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'hidden',
    'formatter' => 'text_summary_or_trimmed',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter_settings' => array(
      'trim_length' => 600,
    ),
    'context' => 'panelizer',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = '994f23d0-ec23-4045-9911-dd4f8f49c371';
  $display->content['new-994f23d0-ec23-4045-9911-dd4f8f49c371'] = $pane;
  $display->panels['contentmain'][2] = 'new-994f23d0-ec23-4045-9911-dd4f8f49c371';
  $pane = new stdClass();
  $pane->pid = 'new-7f1aab35-8558-4d4b-adcb-777e64eda26b';
  $pane->panel = 'contentmain';
  $pane->type = 'node_links';
  $pane->subtype = 'node_links';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => FALSE,
    'override_title_text' => '',
    'build_mode' => 'teaser',
    'identifier' => '',
    'link' => TRUE,
    'context' => 'panelizer',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array(
    'css_class' => 'link-wrapper',
  );
  $pane->extras = array();
  $pane->position = 3;
  $pane->locks = array();
  $pane->uuid = '7f1aab35-8558-4d4b-adcb-777e64eda26b';
  $display->content['new-7f1aab35-8558-4d4b-adcb-777e64eda26b'] = $pane;
  $display->panels['contentmain'][3] = 'new-7f1aab35-8558-4d4b-adcb-777e64eda26b';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-8c9ff2bc-2950-48e7-90f2-cfcef3789ad4';
  $panelizer->display = $display;
  $export['node:mvpcreator_blog:default:teaser'] = $panelizer;

  return $export;
}
