<?php
/**
 * @file
 * oa_folders.panelizer.inc
 */

/**
 * Implements hook_panelizer_defaults().
 */
function oa_folders_panelizer_defaults() {
  $export = array();

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:oa_folder:default';
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'oa_folder';
  $panelizer->no_blocks = FALSE;
  $panelizer->css_id = '';
  $panelizer->css = '';
  $panelizer->pipeline = 'ipe_node_access';
  $panelizer->contexts = array();
  $panelizer->relationships = array(
    0 => array(
      'identifier' => 'Folder Author',
      'keyword' => 'user',
      'name' => 'entity_from_schema:uid-node-user',
      'context' => 'panelizer',
      'id' => 1,
    ),
    1 => array(
      'identifier' => 'Space',
      'keyword' => 'space',
      'name' => 'entity_from_field:og_group_ref-node-node',
      'delta' => 0,
      'context' => 'panelizer',
      'id' => 1,
    ),
    2 => array(
      'identifier' => 'Section',
      'keyword' => 'space',
      'name' => 'entity_from_field:oa_section_ref-node-node',
      'delta' => 0,
      'context' => 'panelizer',
      'id' => 1,
    ),
  );
  $panelizer->access = array();
  $panelizer->view_mode = 'page_manager';
  $panelizer->css_class = '';
  $panelizer->title_element = 'H2';
  $panelizer->link_to_entity = TRUE;
  $panelizer->extra = array();
  $display = new panels_display();
  $display->layout = 'folders';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'sidebar' => NULL,
      'contentmain' => NULL,
      'column1' => NULL,
      'column2' => NULL,
      'column3' => NULL,
      'folderlayout' => NULL,
      'content' => NULL,
    ),
    'sidebar' => array(
      'style' => 'oa_styles_oa_pane',
    ),
  );
  $display->cache = array();
  $display->title = '%node:title';
  $display->uuid = 'bb6a940f-7670-401e-a236-7ab1c5798250';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-6bb5471e-c643-404b-a085-97270ebeec07';
    $pane->panel = 'content';
    $pane->type = 'views_panes';
    $pane->subtype = 'oa_folders-subfolder_pane';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => array(
        0 => 'panelizer',
      ),
      'view_settings' => 'fields',
      'header_type' => 'none',
      'view_mode' => 'teaser',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '6bb5471e-c643-404b-a085-97270ebeec07';
    $display->content['new-6bb5471e-c643-404b-a085-97270ebeec07'] = $pane;
    $display->panels['content'][0] = 'new-6bb5471e-c643-404b-a085-97270ebeec07';
    $pane = new stdClass();
    $pane->pid = 'new-cad82dbe-f70c-4c3a-acbe-60faab9e34c6';
    $pane->panel = 'content';
    $pane->type = 'views_panes';
    $pane->subtype = 'oa_folders-content_pane';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => array(
        0 => 'panelizer',
      ),
      'view_settings' => 'fields',
      'header_type' => 'none',
      'view_mode' => 'teaser',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'cad82dbe-f70c-4c3a-acbe-60faab9e34c6';
    $display->content['new-cad82dbe-f70c-4c3a-acbe-60faab9e34c6'] = $pane;
    $display->panels['content'][1] = 'new-cad82dbe-f70c-4c3a-acbe-60faab9e34c6';
    $pane = new stdClass();
    $pane->pid = 'new-a811ab4b-b391-4bac-aee2-f0b6bdb5a650';
    $pane->panel = 'content';
    $pane->type = 'node_links';
    $pane->subtype = 'node_links';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => FALSE,
      'override_title_text' => '',
      'build_mode' => 'page_manager',
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
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'a811ab4b-b391-4bac-aee2-f0b6bdb5a650';
    $display->content['new-a811ab4b-b391-4bac-aee2-f0b6bdb5a650'] = $pane;
    $display->panels['content'][2] = 'new-a811ab4b-b391-4bac-aee2-f0b6bdb5a650';
    $pane = new stdClass();
    $pane->pid = 'new-32c308fc-ae57-43df-a4fa-8667a747ea60';
    $pane->panel = 'content';
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
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '32c308fc-ae57-43df-a4fa-8667a747ea60';
    $display->content['new-32c308fc-ae57-43df-a4fa-8667a747ea60'] = $pane;
    $display->panels['content'][3] = 'new-32c308fc-ae57-43df-a4fa-8667a747ea60';
    $pane = new stdClass();
    $pane->pid = 'new-fd50a4cf-8298-4584-8504-b21e0e215e70';
    $pane->panel = 'content';
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
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = 'fd50a4cf-8298-4584-8504-b21e0e215e70';
    $display->content['new-fd50a4cf-8298-4584-8504-b21e0e215e70'] = $pane;
    $display->panels['content'][4] = 'new-fd50a4cf-8298-4584-8504-b21e0e215e70';
    $pane = new stdClass();
    $pane->pid = 'new-e46086d2-3bda-405b-b9b1-6d44d79ea780';
    $pane->panel = 'content';
    $pane->type = 'node_comment_form';
    $pane->subtype = 'node_comment_form';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'anon_links' => 0,
      'context' => 'panelizer',
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
    $pane->uuid = 'e46086d2-3bda-405b-b9b1-6d44d79ea780';
    $display->content['new-e46086d2-3bda-405b-b9b1-6d44d79ea780'] = $pane;
    $display->panels['content'][5] = 'new-e46086d2-3bda-405b-b9b1-6d44d79ea780';
    $pane = new stdClass();
    $pane->pid = 'new-f0599495-0162-4c98-8fa2-62126036f54c';
    $pane->panel = 'folderlayout';
    $pane->type = 'oa_folders_subfolder_outline';
    $pane->subtype = 'oa_folders_subfolder_outline';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'return' => 'Finish',
      'cancel' => 'Cancel',
      'context' => 'panelizer',
      'override_title' => 0,
      'override_title_text' => '',
      'form_build_id' => 'form-xaOxAPDg8fJ1XPLMu4IJ4Gk6oBCsdWcA5bHxfqbVM54',
      'form_token' => 'H55NLVoctM9_h2Hq6pM3swTvYdQjM61DzGYS0PIvMRs',
      'form_id' => 'oa_folders_subfolder_outline_settings_form',
      'op' => 'Finish',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'f0599495-0162-4c98-8fa2-62126036f54c';
    $display->content['new-f0599495-0162-4c98-8fa2-62126036f54c'] = $pane;
    $display->panels['folderlayout'][0] = 'new-f0599495-0162-4c98-8fa2-62126036f54c';
    $pane = new stdClass();
    $pane->pid = 'new-7ddbd76b-3d46-436a-b91b-5078220845a0';
    $pane->panel = 'sidebar';
    $pane->type = 'panels_mini';
    $pane->subtype = 'section_sidebar_top';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => array(
        0 => 'panelizer',
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
    $pane->uuid = '7ddbd76b-3d46-436a-b91b-5078220845a0';
    $display->content['new-7ddbd76b-3d46-436a-b91b-5078220845a0'] = $pane;
    $display->panels['sidebar'][0] = 'new-7ddbd76b-3d46-436a-b91b-5078220845a0';
    $pane = new stdClass();
    $pane->pid = 'new-ef3d38d4-45e0-4b40-8e1a-e5b1d41d0cb0';
    $pane->panel = 'sidebar';
    $pane->type = 'og_menu_single_menu';
    $pane->subtype = 'og_menu_single_menu';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'parent' => 0,
      'return' => 'Finish',
      'cancel' => 'Cancel',
      'override_title' => 0,
      'override_title_text' => '',
      'og_menu_single_depth' => '0',
      'og_menu_single_parent' => 'auto',
      'form_build_id' => 'form-8ZJxgyniQAos_O59gzys75sTbJUdXCm_LUdnj6dYFJk',
      'form_token' => 'UzJG_y4NiC-PmQnUegNcr3aM7-vMnWV9mccSTyYUvUA',
      'form_id' => 'og_menu_single_menu_edit_form',
      'op' => 'Finish',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'ef3d38d4-45e0-4b40-8e1a-e5b1d41d0cb0';
    $display->content['new-ef3d38d4-45e0-4b40-8e1a-e5b1d41d0cb0'] = $pane;
    $display->panels['sidebar'][1] = 'new-ef3d38d4-45e0-4b40-8e1a-e5b1d41d0cb0';
    $pane = new stdClass();
    $pane->pid = 'new-7746f122-1e4b-445f-8cf4-d15ef627d710';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:body';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'text_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
      'context' => 'panelizer',
      'override_title' => 1,
      'override_title_text' => 'Folder Description',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '7746f122-1e4b-445f-8cf4-d15ef627d710';
    $display->content['new-7746f122-1e4b-445f-8cf4-d15ef627d710'] = $pane;
    $display->panels['sidebar'][2] = 'new-7746f122-1e4b-445f-8cf4-d15ef627d710';
    $pane = new stdClass();
    $pane->pid = 'new-726f799f-b273-4c70-bb8e-8fa078660e61';
    $pane->panel = 'sidebar';
    $pane->type = 'panels_mini';
    $pane->subtype = 'default_section_sidebar';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => array(
        0 => 'panelizer',
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '726f799f-b273-4c70-bb8e-8fa078660e61';
    $display->content['new-726f799f-b273-4c70-bb8e-8fa078660e61'] = $pane;
    $display->panels['sidebar'][3] = 'new-726f799f-b273-4c70-bb8e-8fa078660e61';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $panelizer->display = $display;
  $export['node:oa_folder:default'] = $panelizer;

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->name = 'node:oa_section:oa_section_folder';
  $panelizer->title = 'Files Section';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'oa_section';
  $panelizer->no_blocks = FALSE;
  $panelizer->css_id = '';
  $panelizer->css = '';
  $panelizer->pipeline = 'ipe_node_access';
  $panelizer->contexts = array();
  $panelizer->relationships = array(
    0 => array(
      'identifier' => 'Space',
      'keyword' => 'space',
      'name' => 'entity_from_field:og_group_ref-node-node',
      'delta' => 0,
      'context' => 'panelizer',
      'id' => 1,
    ),
  );
  $panelizer->access = array();
  $panelizer->view_mode = 'page_manager';
  $panelizer->css_class = '';
  $panelizer->title_element = 'H2';
  $panelizer->link_to_entity = TRUE;
  $panelizer->extra = array();
  $display = new panels_display();
  $display->layout = 'bryant_flipped_flipped';
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
  $display->uuid = '9c775bb2-49c2-4331-b091-b6bfe6978657';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-4f5c640e-3205-486b-b2ba-692d1441e553';
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
    $pane->uuid = '4f5c640e-3205-486b-b2ba-692d1441e553';
    $display->content['new-4f5c640e-3205-486b-b2ba-692d1441e553'] = $pane;
    $display->panels['contentmain'][0] = 'new-4f5c640e-3205-486b-b2ba-692d1441e553';
    $pane = new stdClass();
    $pane->pid = 'new-2908c2d1-1afa-40ef-ba29-bed463f987d3';
    $pane->panel = 'contentmain';
    $pane->type = 'node_links';
    $pane->subtype = 'node_links';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 0,
      'override_title_text' => '',
      'build_mode' => 'full',
      'identifier' => '',
      'link' => 1,
      'context' => 'panelizer',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '2908c2d1-1afa-40ef-ba29-bed463f987d3';
    $display->content['new-2908c2d1-1afa-40ef-ba29-bed463f987d3'] = $pane;
    $display->panels['contentmain'][1] = 'new-2908c2d1-1afa-40ef-ba29-bed463f987d3';
    $pane = new stdClass();
    $pane->pid = 'new-fd082bf1-2eee-40d9-a321-2e779273f43e';
    $pane->panel = 'contentmain';
    $pane->type = 'views_panes';
    $pane->subtype = 'oa_folders-root_pane';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => array(
        0 => 'panelizer',
      ),
      'view_settings' => 'fields',
      'header_type' => 'none',
      'view_mode' => 'teaser',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'fd082bf1-2eee-40d9-a321-2e779273f43e';
    $display->content['new-fd082bf1-2eee-40d9-a321-2e779273f43e'] = $pane;
    $display->panels['contentmain'][2] = 'new-fd082bf1-2eee-40d9-a321-2e779273f43e';
    $pane = new stdClass();
    $pane->pid = 'new-cc589739-e925-4526-b430-b020cabfafb9';
    $pane->panel = 'sidebar';
    $pane->type = 'oa_widgets_content_visibility';
    $pane->subtype = 'oa_widgets_content_visibility';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'return' => 'Finish',
      'cancel' => 'Cancel',
      'context' => 'panelizer',
      'override_title' => 0,
      'override_title_text' => '',
      'form_build_id' => 'form-AeA9pIsETCRxGGR03bAnPTUOMsegMDJFU87Lj9MPvuE',
      'form_token' => 'VqDG1snLTs-rXlSoAelXX6ZAEbEU1iLWdhhUvM2Lx7I',
      'form_id' => 'oa_widgets_content_visibility_edit_form',
      'op' => 'Finish',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'cc589739-e925-4526-b430-b020cabfafb9';
    $display->content['new-cc589739-e925-4526-b430-b020cabfafb9'] = $pane;
    $display->panels['sidebar'][0] = 'new-cc589739-e925-4526-b430-b020cabfafb9';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $panelizer->display = $display;
  $export['node:oa_section:oa_section_folder'] = $panelizer;

  return $export;
}
