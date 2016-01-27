<?php
/**
 * @file
 * courseplanner.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function courseplanner_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view_panel_context';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Course outline',
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
          'name' => 'node_type',
          'settings' => array(
            'type' => array(
              'cp_outline' => 'cp_outline',
            ),
          ),
          'context' => 'argument_entity_id:node_1',
          'not' => FALSE,
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
  $display->title = '';
  $display->uuid = '8712376c-318a-4c66-b488-3bda815f4fb2';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-34a33c15-d850-485f-98ea-b835731f95b0';
    $pane->panel = 'middle';
    $pane->type = 'node_content';
    $pane->subtype = 'node_content';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'links' => 1,
      'no_extras' => 0,
      'override_title' => 1,
      'override_title_text' => '',
      'identifier' => '',
      'link' => 0,
      'leave_node_title' => 0,
      'build_mode' => 'full',
      'context' => 'argument_entity_id:node_1',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '34a33c15-d850-485f-98ea-b835731f95b0';
    $display->content['new-34a33c15-d850-485f-98ea-b835731f95b0'] = $pane;
    $display->panels['middle'][0] = 'new-34a33c15-d850-485f-98ea-b835731f95b0';
    $pane = new stdClass();
    $pane->pid = 'new-b916bf8e-474c-4bde-9e61-711aa18fc466';
    $pane->panel = 'middle';
    $pane->type = 'views_panes';
    $pane->subtype = 'cp_outline_sections-cp_outline_sections';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => array(
        0 => 'argument_entity_id:node_1',
      ),
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'b916bf8e-474c-4bde-9e61-711aa18fc466';
    $display->content['new-b916bf8e-474c-4bde-9e61-711aa18fc466'] = $pane;
    $display->panels['middle'][1] = 'new-b916bf8e-474c-4bde-9e61-711aa18fc466';
    $pane = new stdClass();
    $pane->pid = 'new-d5f1c629-2eae-45f3-883a-f943b2dd770a';
    $pane->panel = 'middle';
    $pane->type = 'views_panes';
    $pane->subtype = 'cp_outline_sections-cp_outline_sections_sums';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => array(
        0 => 'argument_entity_id:node_1',
      ),
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'd5f1c629-2eae-45f3-883a-f943b2dd770a';
    $display->content['new-d5f1c629-2eae-45f3-883a-f943b2dd770a'] = $pane;
    $display->panels['middle'][2] = 'new-d5f1c629-2eae-45f3-883a-f943b2dd770a';
    $pane = new stdClass();
    $pane->pid = 'new-781517ab-dbf6-4edd-8aa2-711faf78c16b';
    $pane->panel = 'middle';
    $pane->type = 'rules_panes';
    $pane->subtype = 'rules_cp_create_and_add_sections_to_outline';
    $pane->shown = TRUE;
    $pane->access = array(
      'plugins' => array(
        0 => array(
          'name' => 'node_access',
          'settings' => array(
            'type' => 'update',
          ),
          'context' => array(
            0 => 'logged-in-user',
            1 => 'argument_entity_id:node_1',
          ),
          'not' => FALSE,
        ),
      ),
    );
    $pane->configuration = array(
      'override_title' => 1,
      'button_text' => 'Add sections',
      'help_text' => '',
      'component' => 'rules_cp_create_and_add_sections_to_outline',
      'context' => array(
        0 => 'argument_entity_id:node_1',
        1 => 'empty',
      ),
      'override_title_text' => 'Add sections to outline',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '781517ab-dbf6-4edd-8aa2-711faf78c16b';
    $display->content['new-781517ab-dbf6-4edd-8aa2-711faf78c16b'] = $pane;
    $display->panels['middle'][3] = 'new-781517ab-dbf6-4edd-8aa2-711faf78c16b';
  $display->hide_title = PANELS_TITLE_PANE;
  $display->title_pane = 'new-34a33c15-d850-485f-98ea-b835731f95b0';
  $handler->conf['display'] = $display;
  $export['node_view_panel_context'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view_panel_context_2';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 1;
  $handler->conf = array(
    'title' => 'Course offering',
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
          'name' => 'node_type',
          'settings' => array(
            'type' => array(
              'cp_offering' => 'cp_offering',
            ),
          ),
          'context' => 'argument_entity_id:node_1',
          'not' => FALSE,
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
  $display->title = '';
  $display->uuid = '50afb19d-9724-4499-a7d1-9232fe86b53e';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-20e926ab-4938-444e-8232-bbef556ddd92';
    $pane->panel = 'middle';
    $pane->type = 'node_content';
    $pane->subtype = 'node_content';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'links' => 1,
      'no_extras' => 0,
      'override_title' => 1,
      'override_title_text' => '',
      'identifier' => '',
      'link' => 0,
      'leave_node_title' => 0,
      'build_mode' => 'full',
      'context' => 'argument_entity_id:node_1',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '20e926ab-4938-444e-8232-bbef556ddd92';
    $display->content['new-20e926ab-4938-444e-8232-bbef556ddd92'] = $pane;
    $display->panels['middle'][0] = 'new-20e926ab-4938-444e-8232-bbef556ddd92';
    $pane = new stdClass();
    $pane->pid = 'new-d2eeec7d-65fb-4d00-bc73-84886976b918';
    $pane->panel = 'middle';
    $pane->type = 'views_panes';
    $pane->subtype = 'cp_offering_lessons-cp_lessons_calendar';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => array(
        0 => 'argument_entity_id:node_1',
      ),
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'd2eeec7d-65fb-4d00-bc73-84886976b918';
    $display->content['new-d2eeec7d-65fb-4d00-bc73-84886976b918'] = $pane;
    $display->panels['middle'][1] = 'new-d2eeec7d-65fb-4d00-bc73-84886976b918';
  $display->hide_title = PANELS_TITLE_PANE;
  $display->title_pane = 'new-20e926ab-4938-444e-8232-bbef556ddd92';
  $handler->conf['display'] = $display;
  $export['node_view_panel_context_2'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view_panel_context_3';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 2;
  $handler->conf = array(
    'title' => 'Course section',
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
          'name' => 'node_type',
          'settings' => array(
            'type' => array(
              'cp_section' => 'cp_section',
            ),
          ),
          'context' => 'argument_entity_id:node_1',
          'not' => FALSE,
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
  $display->title = '';
  $display->uuid = 'fd21b6f3-abdb-40d9-9445-311d215c6781';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-12f76fe5-6572-4c12-887f-9c8701ac6467';
    $pane->panel = 'middle';
    $pane->type = 'node_content';
    $pane->subtype = 'node_content';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'links' => 0,
      'no_extras' => 0,
      'override_title' => 1,
      'override_title_text' => '',
      'identifier' => '',
      'link' => 0,
      'leave_node_title' => 0,
      'build_mode' => 'full',
      'context' => 'argument_entity_id:node_1',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '12f76fe5-6572-4c12-887f-9c8701ac6467';
    $display->content['new-12f76fe5-6572-4c12-887f-9c8701ac6467'] = $pane;
    $display->panels['middle'][0] = 'new-12f76fe5-6572-4c12-887f-9c8701ac6467';
    $pane = new stdClass();
    $pane->pid = 'new-551cb4fb-d9f0-45ef-8815-faa4416c9c71';
    $pane->panel = 'middle';
    $pane->type = 'rules_panes';
    $pane->subtype = 'rules_cp_quick_embed_resource';
    $pane->shown = TRUE;
    $pane->access = array(
      'plugins' => array(
        0 => array(
          'name' => 'node_access',
          'settings' => array(
            'type' => 'update',
          ),
          'context' => array(
            0 => 'logged-in-user',
            1 => 'argument_entity_id:node_1',
          ),
          'not' => FALSE,
        ),
      ),
    );
    $pane->configuration = array(
      'override_title' => 0,
      'button_text' => 'Embed resource',
      'help_text' => '',
      'component' => 'rules_cp_quick_embed_resource',
      'context' => array(
        0 => 'argument_entity_id:node_1',
        1 => 'empty',
        2 => 'empty',
        3 => 'empty',
      ),
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
    $pane->uuid = '551cb4fb-d9f0-45ef-8815-faa4416c9c71';
    $display->content['new-551cb4fb-d9f0-45ef-8815-faa4416c9c71'] = $pane;
    $display->panels['middle'][1] = 'new-551cb4fb-d9f0-45ef-8815-faa4416c9c71';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-12f76fe5-6572-4c12-887f-9c8701ac6467';
  $handler->conf['display'] = $display;
  $export['node_view_panel_context_3'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'pm_existing_pages_cp_lesson_panel_context';
  $handler->task = 'pm_existing_pages';
  $handler->subtask = 'cp_lesson';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Lesson',
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
          'name' => 'entity_bundle:field_collection_item',
          'settings' => array(
            'type' => array(
              'cp_offering_lessons' => 'cp_offering_lessons',
            ),
          ),
          'context' => 'argument_entity_id:field_collection_item_1',
          'not' => FALSE,
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
  $display->title = '';
  $display->uuid = '010206d9-ccd6-4ebe-92df-7a014e5fa8c9';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-29446330-116c-4d5c-b5bd-fd62a73109b1';
    $pane->panel = 'middle';
    $pane->type = 'entity_field';
    $pane->subtype = 'field_collection_item:cp_lesson_status';
    $pane->shown = TRUE;
    $pane->access = array(
      'plugins' => array(
        0 => array(
          'name' => 'entity_field_value:field_collection_item:cp_offering_lessons:cp_lesson_status',
          'settings' => array(
            'cp_lesson_status' => array(
              'und' => array(
                0 => array(
                  'value' => '0',
                ),
              ),
            ),
            'cp_lesson_status_value' => '0',
          ),
          'context' => 'argument_entity_id:field_collection_item_1',
          'not' => TRUE,
        ),
      ),
    );
    $pane->configuration = array(
      'label' => 'inline',
      'formatter' => 'list_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
      'context' => 'argument_entity_id:field_collection_item_1',
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
    $pane->uuid = '29446330-116c-4d5c-b5bd-fd62a73109b1';
    $display->content['new-29446330-116c-4d5c-b5bd-fd62a73109b1'] = $pane;
    $display->panels['middle'][0] = 'new-29446330-116c-4d5c-b5bd-fd62a73109b1';
    $pane = new stdClass();
    $pane->pid = 'new-b9fe4387-050e-4ef6-83ea-4f6c7da18855';
    $pane->panel = 'middle';
    $pane->type = 'entity_field';
    $pane->subtype = 'field_collection_item:cp_lesson_date';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'date_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'format_type' => 'long',
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'fromto' => 'both',
      ),
      'context' => 'argument_entity_id:field_collection_item_1',
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
    $pane->uuid = 'b9fe4387-050e-4ef6-83ea-4f6c7da18855';
    $display->content['new-b9fe4387-050e-4ef6-83ea-4f6c7da18855'] = $pane;
    $display->panels['middle'][1] = 'new-b9fe4387-050e-4ef6-83ea-4f6c7da18855';
    $pane = new stdClass();
    $pane->pid = 'new-ec9a7754-34de-4083-8b7d-9c32bae86017';
    $pane->panel = 'middle';
    $pane->type = 'entity_field';
    $pane->subtype = 'field_collection_item:cp_lesson_room';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'inline',
      'formatter' => 'text_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
      'context' => 'argument_entity_id:field_collection_item_1',
      'override_title' => 0,
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
    $pane->uuid = 'ec9a7754-34de-4083-8b7d-9c32bae86017';
    $display->content['new-ec9a7754-34de-4083-8b7d-9c32bae86017'] = $pane;
    $display->panels['middle'][2] = 'new-ec9a7754-34de-4083-8b7d-9c32bae86017';
    $pane = new stdClass();
    $pane->pid = 'new-da750c7b-40c1-491f-b6f7-cee5d915b5ba';
    $pane->panel = 'middle';
    $pane->type = 'entity_field';
    $pane->subtype = 'field_collection_item:cp_lesson_notes';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'text_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
      'context' => 'argument_entity_id:field_collection_item_1',
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
    $pane->uuid = 'da750c7b-40c1-491f-b6f7-cee5d915b5ba';
    $display->content['new-da750c7b-40c1-491f-b6f7-cee5d915b5ba'] = $pane;
    $display->panels['middle'][3] = 'new-da750c7b-40c1-491f-b6f7-cee5d915b5ba';
    $pane = new stdClass();
    $pane->pid = 'new-e6c4d8b8-6152-4115-8fa5-d56229f52196';
    $pane->panel = 'middle';
    $pane->type = 'entity_field';
    $pane->subtype = 'field_collection_item:cp_lesson_section';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'entityreference_entity_view',
      'delta_limit' => '0',
      'delta_offset' => '0',
      'delta_reversed' => 0,
      'formatter_settings' => array(
        'view_mode' => 'full',
        'links' => 1,
      ),
      'context' => 'argument_entity_id:field_collection_item_1',
      'override_title' => 1,
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
    $pane->uuid = 'e6c4d8b8-6152-4115-8fa5-d56229f52196';
    $display->content['new-e6c4d8b8-6152-4115-8fa5-d56229f52196'] = $pane;
    $display->panels['middle'][4] = 'new-e6c4d8b8-6152-4115-8fa5-d56229f52196';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-b9fe4387-050e-4ef6-83ea-4f6c7da18855';
  $handler->conf['display'] = $display;
  $export['pm_existing_pages_cp_lesson_panel_context'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'user_view_panel_context';
  $handler->task = 'user_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = -29;
  $handler->conf = array(
    'title' => 'Profile page',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
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
  $display->title = '';
  $display->uuid = '92325933-a39b-4e61-98eb-08e247bd1516';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-96bacf45-6648-4d68-8847-0ce59d96839d';
    $pane->panel = 'middle';
    $pane->type = 'views_panes';
    $pane->subtype = 'cp_user_schedule-cp_lessons_calendar';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => array(
        0 => 'argument_entity_id:user_1',
      ),
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '96bacf45-6648-4d68-8847-0ce59d96839d';
    $display->content['new-96bacf45-6648-4d68-8847-0ce59d96839d'] = $pane;
    $display->panels['middle'][0] = 'new-96bacf45-6648-4d68-8847-0ce59d96839d';
    $pane = new stdClass();
    $pane->pid = 'new-8bfd818c-8248-4ad2-80c5-1aac8041353d';
    $pane->panel = 'middle';
    $pane->type = 'views_panes';
    $pane->subtype = 'cp_item_lists-cp_my_favourites';
    $pane->shown = TRUE;
    $pane->access = array(
      'plugins' => array(
        0 => array(
          'name' => 'compare_users',
          'settings' => array(
            'equality' => '1',
          ),
          'context' => array(
            0 => 'argument_entity_id:user_1',
            1 => 'logged-in-user',
          ),
          'not' => FALSE,
        ),
      ),
    );
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '8bfd818c-8248-4ad2-80c5-1aac8041353d';
    $display->content['new-8bfd818c-8248-4ad2-80c5-1aac8041353d'] = $pane;
    $display->panels['middle'][1] = 'new-8bfd818c-8248-4ad2-80c5-1aac8041353d';
    $pane = new stdClass();
    $pane->pid = 'new-de2907d6-a7db-45aa-a08d-530d6bc9a019';
    $pane->panel = 'middle';
    $pane->type = 'views_panes';
    $pane->subtype = 'cp_item_lists-cp_items_per_user';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => array(
        1 => 'argument_entity_id:user_1',
      ),
      'override_title' => 1,
      'override_title_text' => 'Items by %user:name',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'de2907d6-a7db-45aa-a08d-530d6bc9a019';
    $display->content['new-de2907d6-a7db-45aa-a08d-530d6bc9a019'] = $pane;
    $display->panels['middle'][2] = 'new-de2907d6-a7db-45aa-a08d-530d6bc9a019';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['user_view_panel_context'] = $handler;

  return $export;
}

/**
 * Implements hook_default_page_manager_pages().
 */
function courseplanner_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cp_course_offering_admin';
  $page->task = 'page';
  $page->admin_title = 'Admin page for course offering';
  $page->admin_description = 'A page used for creating, updating and managing lessons that belong to a course offering. Only accessible for those who can edit the course offering.';
  $page->path = 'node/%offering/admin';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'node_type',
        'settings' => array(
          'type' => array(
            'cp_offering' => 'cp_offering',
          ),
        ),
        'context' => 'argument_entity_id:node_1',
        'not' => FALSE,
      ),
      1 => array(
        'name' => 'node_access',
        'settings' => array(
          'type' => 'update',
        ),
        'context' => array(
          0 => 'logged-in-user',
          1 => 'argument_entity_id:node_1',
        ),
        'not' => FALSE,
      ),
    ),
    'logic' => 'and',
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'tab',
    'title' => 'Administer',
    'name' => 'navigation',
    'weight' => '5',
    'parent' => array(
      'type' => 'none',
      'title' => '',
      'name' => 'navigation',
      'weight' => '0',
    ),
  );
  $page->arguments = array(
    'offering' => array(
      'id' => 1,
      'identifier' => 'Course offering',
      'name' => 'entity_id:node',
      'settings' => array(),
    ),
  );
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_cp_course_offering_admin_panel_context_2';
  $handler->task = 'page';
  $handler->subtask = 'cp_course_offering_admin';
  $handler->handler = 'panel_context';
  $handler->weight = -30;
  $handler->conf = array(
    'title' => 'Empty course offering',
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
          'name' => 'courseplanner_offering_empty',
          'settings' => NULL,
          'context' => 'argument_entity_id:node_1',
          'not' => FALSE,
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
  $display->title = '%offering:title: administer lessons';
  $display->uuid = 'cde8343e-3119-447c-98cd-b5c3489ec165';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-6f466df0-d14e-4645-abee-34a2a35b08cb';
    $pane->panel = 'middle';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => 'Help text',
      'title' => 'First step: Add lessons to this course offering',
      'body' => 'You now have an empty course offering. This course offering will have a calendar and a (printable) week list, which you can find as tabs on this page.

Before the calendar and week list is useful, however, you\'ll have to add lessons to this course offering. The lessons constitute the schedule for the course offering, and can be filled with content either manually, by connecting them with a course section, or by using a course outline to fill a lot of lessons at the same time.

You can use the box below to add the first lessons. Once lessons are added, you will get options to repeat lessons over several weeks – and other options for managing the lessons.',
      'format' => 'plain_text',
      'substitute' => 0,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '6f466df0-d14e-4645-abee-34a2a35b08cb';
    $display->content['new-6f466df0-d14e-4645-abee-34a2a35b08cb'] = $pane;
    $display->panels['middle'][0] = 'new-6f466df0-d14e-4645-abee-34a2a35b08cb';
    $pane = new stdClass();
    $pane->pid = 'new-ad4ac44c-daeb-45c6-be83-cf7a68f8bffe';
    $pane->panel = 'middle';
    $pane->type = 'rules_panes';
    $pane->subtype = 'rules_cp_lessons_import';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
      'button_text' => 'Add',
      'help_text' => '',
      'component' => 'rules_cp_lessons_import',
      'context' => array(
        0 => 'argument_entity_id:node_1',
        1 => 'empty',
      ),
      'override_title_text' => 'Add lessons',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'ad4ac44c-daeb-45c6-be83-cf7a68f8bffe';
    $display->content['new-ad4ac44c-daeb-45c6-be83-cf7a68f8bffe'] = $pane;
    $display->panels['middle'][1] = 'new-ad4ac44c-daeb-45c6-be83-cf7a68f8bffe';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-ad4ac44c-daeb-45c6-be83-cf7a68f8bffe';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_cp_course_offering_admin_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cp_course_offering_admin';
  $handler->handler = 'panel_context';
  $handler->weight = -29;
  $handler->conf = array(
    'title' => 'Course offering with lessons',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
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
      'top' => NULL,
      'left' => NULL,
      'right' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '%offering:title: administer lessons';
  $display->uuid = '64b0e0c4-db6b-4c9f-96ff-5be10fdb44ed';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-78f713e1-7b88-4715-a2c4-fb890adefd04';
    $pane->panel = 'middle';
    $pane->type = 'views_panes';
    $pane->subtype = 'cp_offering_lessons-cp_lessons_admin';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => array(
        0 => 'argument_entity_id:node_1',
      ),
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '78f713e1-7b88-4715-a2c4-fb890adefd04';
    $display->content['new-78f713e1-7b88-4715-a2c4-fb890adefd04'] = $pane;
    $display->panels['middle'][0] = 'new-78f713e1-7b88-4715-a2c4-fb890adefd04';
    $pane = new stdClass();
    $pane->pid = 'new-23348141-54ca-4ba0-bfa0-c8ee78de1823';
    $pane->panel = 'middle';
    $pane->type = 'views_panes';
    $pane->subtype = 'cp_offering_lessons-cp_lessons_count';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => array(
        0 => 'argument_entity_id:node_1',
      ),
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '23348141-54ca-4ba0-bfa0-c8ee78de1823';
    $display->content['new-23348141-54ca-4ba0-bfa0-c8ee78de1823'] = $pane;
    $display->panels['middle'][1] = 'new-23348141-54ca-4ba0-bfa0-c8ee78de1823';
    $pane = new stdClass();
    $pane->pid = 'new-a9a9b939-8a9c-4a6e-9cac-224f5885064f';
    $pane->panel = 'middle';
    $pane->type = 'views_panes';
    $pane->subtype = 'cp_offering_lessons-cp_lesson_count_empty';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => array(
        0 => 'argument_entity_id:node_1',
      ),
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'a9a9b939-8a9c-4a6e-9cac-224f5885064f';
    $display->content['new-a9a9b939-8a9c-4a6e-9cac-224f5885064f'] = $pane;
    $display->panels['middle'][2] = 'new-a9a9b939-8a9c-4a6e-9cac-224f5885064f';
    $pane = new stdClass();
    $pane->pid = 'new-d40f77d2-7758-4d51-ae04-1f41cc2eae6b';
    $pane->panel = 'middle';
    $pane->type = 'rules_panes';
    $pane->subtype = 'rules_cp_lessons_import';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
      'button_text' => 'Add',
      'help_text' => '',
      'component' => 'rules_cp_lessons_import',
      'context' => array(
        0 => 'argument_entity_id:node_1',
        1 => 'empty',
      ),
      'override_title_text' => 'Add lessons',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => array(
        'pane_collapsed' => 1,
      ),
      'style' => 'collapsible',
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = 'd40f77d2-7758-4d51-ae04-1f41cc2eae6b';
    $display->content['new-d40f77d2-7758-4d51-ae04-1f41cc2eae6b'] = $pane;
    $display->panels['middle'][3] = 'new-d40f77d2-7758-4d51-ae04-1f41cc2eae6b';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cp_course_offering_admin'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cp_course_offering_weekly';
  $page->task = 'page';
  $page->admin_title = 'Weekly lesson list';
  $page->admin_description = 'A list of lessons in a course offering, grouped by week. Suitable for print-outs.';
  $page->path = 'node/%offering/weekly';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'node_type',
        'settings' => array(
          'type' => array(
            'cp_offering' => 'cp_offering',
          ),
        ),
        'context' => 'argument_entity_id:node_1',
        'not' => FALSE,
      ),
    ),
    'logic' => 'and',
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'tab',
    'title' => 'Week list',
    'name' => 'navigation',
    'weight' => '3',
    'parent' => array(
      'type' => 'none',
      'title' => '',
      'name' => 'navigation',
      'weight' => '0',
    ),
  );
  $page->arguments = array(
    'offering' => array(
      'id' => 1,
      'identifier' => 'Course offering',
      'name' => 'entity_id:node',
      'settings' => array(),
    ),
  );
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_cp_course_offering_weekly_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'cp_course_offering_weekly';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
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
  $display->title = '%offering:title: week list';
  $display->uuid = 'f43e6a2e-d06c-4778-b1a6-647e5defc6de';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-02f5d8b4-2964-4e62-adc5-28fa2a697153';
    $pane->panel = 'middle';
    $pane->type = 'views_panes';
    $pane->subtype = 'cp_offering_lessons-cp_lessons_week';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => array(
        0 => 'argument_entity_id:node_1',
      ),
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '02f5d8b4-2964-4e62-adc5-28fa2a697153';
    $display->content['new-02f5d8b4-2964-4e62-adc5-28fa2a697153'] = $pane;
    $display->panels['middle'][0] = 'new-02f5d8b4-2964-4e62-adc5-28fa2a697153';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['cp_course_offering_weekly'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'cp_embed_resource';
  $page->task = 'page';
  $page->admin_title = 'Embed resource';
  $page->admin_description = 'A local action with a redirect allowing embedding resources in a section.';
  $page->path = 'node/%section/embed-resource';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'node_type',
        'settings' => array(
          'type' => array(
            'cp_section' => 'cp_section',
          ),
        ),
        'context' => 'argument_entity_id:node_1',
        'not' => FALSE,
      ),
      1 => array(
        'name' => 'node_access',
        'settings' => array(
          'type' => 'update',
        ),
        'context' => array(
          0 => 'logged-in-user',
          1 => 'argument_entity_id:node_1',
        ),
        'not' => FALSE,
      ),
    ),
    'logic' => 'and',
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'action',
    'title' => 'Embed resource',
    'name' => 'navigation',
    'weight' => '3',
    'parent' => array(
      'type' => 'none',
      'title' => '',
      'name' => 'navigation',
      'weight' => '0',
    ),
  );
  $page->arguments = array(
    'section' => array(
      'id' => 1,
      'identifier' => 'Section',
      'name' => 'entity_id:node',
      'settings' => array(),
    ),
  );
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_cp_embed_resource_http_response';
  $handler->task = 'page';
  $handler->subtask = 'cp_embed_resource';
  $handler->handler = 'http_response';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'HTTP response code',
    'contexts' => array(),
    'relationships' => array(),
    'code' => '301',
    'destination' => 'field-collection/cp-section-resources-embedded/add/node/%section:nid?destination=node/%section:nid',
  );
  $page->default_handlers[$handler->name] = $handler;
  $pages['cp_embed_resource'] = $page;

  return $pages;

}
