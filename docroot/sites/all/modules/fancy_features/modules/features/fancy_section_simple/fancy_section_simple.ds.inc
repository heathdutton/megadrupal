<?php
/**
 * @file
 * fancy_section_simple.ds.inc
 */

/**
 * Implements hook_ds_layout_settings_info().
 */
function fancy_section_simple_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'fancy_section|simple|default';
  $ds_layout->entity_type = 'fancy_section';
  $ds_layout->bundle = 'simple';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'field_fancy_rel_contents',
      ),
    ),
    'fields' => array(
      'field_fancy_rel_contents' => 'ds_content',
    ),
    'classes' => array(
      'layout_class' => array(),
    ),
    'wrappers' => array(
      'ds_content' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['fancy_section|simple|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'fancy_section|simple|fancy_ds_2';
  $ds_layout->entity_type = 'fancy_section';
  $ds_layout->bundle = 'simple';
  $ds_layout->view_mode = 'fancy_ds_2';
  $ds_layout->layout = 'ds_2col';
  $ds_layout->settings = array(
    'regions' => array(
      'left' => array(
        0 => 'field_fancy_rel_contents',
      ),
      'right' => array(
        1 => 'field_fancy_rel_contents_1',
      ),
    ),
    'fields' => array(
      'field_fancy_rel_contents' => 'left',
      'field_fancy_rel_contents_1' => 'right',
    ),
    'classes' => array(),
    'wrappers' => array(
      'left' => 'div',
      'right' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['fancy_section|simple|fancy_ds_2'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'fancy_section|simple|fancy_ds_2_fluid';
  $ds_layout->entity_type = 'fancy_section';
  $ds_layout->bundle = 'simple';
  $ds_layout->view_mode = 'fancy_ds_2_fluid';
  $ds_layout->layout = 'ds_2col_fluid';
  $ds_layout->settings = array(
    'regions' => array(
      'left' => array(
        0 => 'field_fancy_rel_contents',
      ),
      'right' => array(
        1 => 'field_fancy_rel_contents_1',
      ),
    ),
    'fields' => array(
      'field_fancy_rel_contents' => 'left',
      'field_fancy_rel_contents_1' => 'right',
    ),
    'classes' => array(),
    'wrappers' => array(
      'left' => 'div',
      'right' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['fancy_section|simple|fancy_ds_2_fluid'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'fancy_section|simple|fancy_ds_2_stack';
  $ds_layout->entity_type = 'fancy_section';
  $ds_layout->bundle = 'simple';
  $ds_layout->view_mode = 'fancy_ds_2_stack';
  $ds_layout->layout = 'ds_2col_stacked';
  $ds_layout->settings = array(
    'regions' => array(
      'header' => array(
        0 => 'field_fancy_rel_contents',
      ),
      'left' => array(
        1 => 'field_fancy_rel_contents_1',
      ),
      'right' => array(
        2 => 'field_fancy_rel_contents_2',
      ),
      'footer' => array(
        3 => 'field_fancy_rel_contents_3',
      ),
    ),
    'fields' => array(
      'field_fancy_rel_contents' => 'header',
      'field_fancy_rel_contents_1' => 'left',
      'field_fancy_rel_contents_2' => 'right',
      'field_fancy_rel_contents_3' => 'footer',
    ),
    'classes' => array(),
    'wrappers' => array(
      'header' => 'div',
      'left' => 'div',
      'right' => 'div',
      'footer' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['fancy_section|simple|fancy_ds_2_stack'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'fancy_section|simple|fancy_ds_2_stack_fluid';
  $ds_layout->entity_type = 'fancy_section';
  $ds_layout->bundle = 'simple';
  $ds_layout->view_mode = 'fancy_ds_2_stack_fluid';
  $ds_layout->layout = 'ds_2col_stacked_fluid';
  $ds_layout->settings = array(
    'regions' => array(
      'header' => array(
        0 => 'field_fancy_rel_contents',
      ),
      'left' => array(
        1 => 'field_fancy_rel_contents_1',
      ),
      'right' => array(
        2 => 'field_fancy_rel_contents_2',
      ),
      'footer' => array(
        3 => 'field_fancy_rel_contents_3',
      ),
    ),
    'fields' => array(
      'field_fancy_rel_contents' => 'header',
      'field_fancy_rel_contents_1' => 'left',
      'field_fancy_rel_contents_2' => 'right',
      'field_fancy_rel_contents_3' => 'footer',
    ),
    'classes' => array(),
    'wrappers' => array(
      'header' => 'div',
      'left' => 'div',
      'right' => 'div',
      'footer' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['fancy_section|simple|fancy_ds_2_stack_fluid'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'fancy_section|simple|fancy_ds_3';
  $ds_layout->entity_type = 'fancy_section';
  $ds_layout->bundle = 'simple';
  $ds_layout->view_mode = 'fancy_ds_3';
  $ds_layout->layout = 'ds_3col_equal_width';
  $ds_layout->settings = array(
    'regions' => array(
      'left' => array(
        0 => 'field_fancy_rel_contents',
      ),
      'middle' => array(
        1 => 'field_fancy_rel_contents_1',
      ),
      'right' => array(
        2 => 'field_fancy_rel_contents_2',
      ),
    ),
    'fields' => array(
      'field_fancy_rel_contents' => 'left',
      'field_fancy_rel_contents_1' => 'middle',
      'field_fancy_rel_contents_2' => 'right',
    ),
    'classes' => array(),
    'wrappers' => array(
      'left' => 'div',
      'middle' => 'div',
      'right' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['fancy_section|simple|fancy_ds_3'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'fancy_section|simple|fancy_ds_3_stack';
  $ds_layout->entity_type = 'fancy_section';
  $ds_layout->bundle = 'simple';
  $ds_layout->view_mode = 'fancy_ds_3_stack';
  $ds_layout->layout = 'ds_3col_stacked';
  $ds_layout->settings = array(
    'regions' => array(
      'header' => array(
        0 => 'field_fancy_rel_contents',
      ),
      'left' => array(
        1 => 'field_fancy_rel_contents_1',
      ),
      'middle' => array(
        2 => 'field_fancy_rel_contents_2',
      ),
      'right' => array(
        3 => 'field_fancy_rel_contents_3',
      ),
      'footer' => array(
        4 => 'field_fancy_rel_contents_4',
      ),
    ),
    'fields' => array(
      'field_fancy_rel_contents' => 'header',
      'field_fancy_rel_contents_1' => 'left',
      'field_fancy_rel_contents_2' => 'middle',
      'field_fancy_rel_contents_3' => 'right',
      'field_fancy_rel_contents_4' => 'footer',
    ),
    'classes' => array(),
    'wrappers' => array(
      'header' => 'div',
      'left' => 'div',
      'middle' => 'div',
      'right' => 'div',
      'footer' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['fancy_section|simple|fancy_ds_3_stack'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'fancy_section|simple|fancy_ds_3_stack_fluid';
  $ds_layout->entity_type = 'fancy_section';
  $ds_layout->bundle = 'simple';
  $ds_layout->view_mode = 'fancy_ds_3_stack_fluid';
  $ds_layout->layout = 'ds_3col_stacked_fluid';
  $ds_layout->settings = array(
    'regions' => array(
      'header' => array(
        0 => 'field_fancy_rel_contents',
      ),
      'left' => array(
        1 => 'field_fancy_rel_contents_1',
      ),
      'middle' => array(
        2 => 'field_fancy_rel_contents_2',
      ),
      'right' => array(
        3 => 'field_fancy_rel_contents_3',
      ),
      'footer' => array(
        4 => 'field_fancy_rel_contents_4',
      ),
    ),
    'fields' => array(
      'field_fancy_rel_contents' => 'header',
      'field_fancy_rel_contents_1' => 'left',
      'field_fancy_rel_contents_2' => 'middle',
      'field_fancy_rel_contents_3' => 'right',
      'field_fancy_rel_contents_4' => 'footer',
    ),
    'classes' => array(),
    'wrappers' => array(
      'header' => 'div',
      'left' => 'div',
      'middle' => 'div',
      'right' => 'div',
      'footer' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['fancy_section|simple|fancy_ds_3_stack_fluid'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'fancy_section|simple|fancy_ds_3_weighted';
  $ds_layout->entity_type = 'fancy_section';
  $ds_layout->bundle = 'simple';
  $ds_layout->view_mode = 'fancy_ds_3_weighted';
  $ds_layout->layout = 'ds_3col';
  $ds_layout->settings = array(
    'regions' => array(
      'left' => array(
        0 => 'field_fancy_rel_contents',
      ),
      'middle' => array(
        1 => 'field_fancy_rel_contents_1',
      ),
      'right' => array(
        2 => 'field_fancy_rel_contents_2',
      ),
    ),
    'fields' => array(
      'field_fancy_rel_contents' => 'left',
      'field_fancy_rel_contents_1' => 'middle',
      'field_fancy_rel_contents_2' => 'right',
    ),
    'classes' => array(),
    'wrappers' => array(
      'left' => 'div',
      'middle' => 'div',
      'right' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['fancy_section|simple|fancy_ds_3_weighted'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'fancy_section|simple|fancy_ds_4';
  $ds_layout->entity_type = 'fancy_section';
  $ds_layout->bundle = 'simple';
  $ds_layout->view_mode = 'fancy_ds_4';
  $ds_layout->layout = 'ds_4col';
  $ds_layout->settings = array(
    'regions' => array(
      'first' => array(
        0 => 'field_fancy_rel_contents',
      ),
      'second' => array(
        1 => 'field_fancy_rel_contents_1',
      ),
      'third' => array(
        2 => 'field_fancy_rel_contents_2',
      ),
      'fourth' => array(
        3 => 'field_fancy_rel_contents_3',
      ),
    ),
    'fields' => array(
      'field_fancy_rel_contents' => 'first',
      'field_fancy_rel_contents_1' => 'second',
      'field_fancy_rel_contents_2' => 'third',
      'field_fancy_rel_contents_3' => 'fourth',
    ),
    'classes' => array(
      'layout_class' => array(),
    ),
    'wrappers' => array(
      'first' => 'div',
      'second' => 'div',
      'third' => 'div',
      'fourth' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['fancy_section|simple|fancy_ds_4'] = $ds_layout;

  return $export;
}

/**
 * Implements hook_ds_view_modes_info().
 */
function fancy_section_simple_ds_view_modes_info() {
  $export = array();

  $ds_view_mode = new stdClass();
  $ds_view_mode->api_version = 1;
  $ds_view_mode->view_mode = 'fancy_ds_2';
  $ds_view_mode->label = 'Two column';
  $ds_view_mode->entities = array(
    'fancy_section' => 'fancy_section',
  );
  $export['fancy_ds_2'] = $ds_view_mode;

  $ds_view_mode = new stdClass();
  $ds_view_mode->api_version = 1;
  $ds_view_mode->view_mode = 'fancy_ds_2_fluid';
  $ds_view_mode->label = 'Two column - Fluid';
  $ds_view_mode->entities = array(
    'fancy_section' => 'fancy_section',
  );
  $export['fancy_ds_2_fluid'] = $ds_view_mode;

  $ds_view_mode = new stdClass();
  $ds_view_mode->api_version = 1;
  $ds_view_mode->view_mode = 'fancy_ds_2_stack';
  $ds_view_mode->label = 'Two column stacked';
  $ds_view_mode->entities = array(
    'fancy_section' => 'fancy_section',
  );
  $export['fancy_ds_2_stack'] = $ds_view_mode;

  $ds_view_mode = new stdClass();
  $ds_view_mode->api_version = 1;
  $ds_view_mode->view_mode = 'fancy_ds_2_stack_fluid';
  $ds_view_mode->label = 'Two column stacked - Fluid';
  $ds_view_mode->entities = array(
    'fancy_section' => 'fancy_section',
  );
  $export['fancy_ds_2_stack_fluid'] = $ds_view_mode;

  $ds_view_mode = new stdClass();
  $ds_view_mode->api_version = 1;
  $ds_view_mode->view_mode = 'fancy_ds_3';
  $ds_view_mode->label = 'Three column';
  $ds_view_mode->entities = array(
    'fancy_section' => 'fancy_section',
  );
  $export['fancy_ds_3'] = $ds_view_mode;

  $ds_view_mode = new stdClass();
  $ds_view_mode->api_version = 1;
  $ds_view_mode->view_mode = 'fancy_ds_3_stack';
  $ds_view_mode->label = 'Three column stacked - 25/50/25';
  $ds_view_mode->entities = array(
    'fancy_section' => 'fancy_section',
  );
  $export['fancy_ds_3_stack'] = $ds_view_mode;

  $ds_view_mode = new stdClass();
  $ds_view_mode->api_version = 1;
  $ds_view_mode->view_mode = 'fancy_ds_3_stack_fluid';
  $ds_view_mode->label = 'Three column stacked - Fluid';
  $ds_view_mode->entities = array(
    'fancy_section' => 'fancy_section',
  );
  $export['fancy_ds_3_stack_fluid'] = $ds_view_mode;

  $ds_view_mode = new stdClass();
  $ds_view_mode->api_version = 1;
  $ds_view_mode->view_mode = 'fancy_ds_3_weighted';
  $ds_view_mode->label = 'Three column - 25/50/25';
  $ds_view_mode->entities = array(
    'fancy_section' => 'fancy_section',
  );
  $export['fancy_ds_3_weighted'] = $ds_view_mode;

  $ds_view_mode = new stdClass();
  $ds_view_mode->api_version = 1;
  $ds_view_mode->view_mode = 'fancy_ds_4';
  $ds_view_mode->label = 'Four column';
  $ds_view_mode->entities = array(
    'fancy_section' => 'fancy_section',
  );
  $export['fancy_ds_4'] = $ds_view_mode;

  return $export;
}
