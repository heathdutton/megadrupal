<?php
/**
 * @file
 * fancy_section_responsive.ds.inc
 */

/**
 * Implements hook_ds_layout_settings_info().
 */
function fancy_section_responsive_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'fancy_section|responsive|default';
  $ds_layout->entity_type = 'fancy_section';
  $ds_layout->bundle = 'responsive';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'fancy_grid_one';
  $ds_layout->settings = array(
    'regions' => array(
      'one' => array(
        0 => 'field_fancy_rel_contents',
      ),
    ),
    'fields' => array(
      'field_fancy_rel_contents' => 'one',
    ),
    'classes' => array(),
    'wrappers' => array(
      'one' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
    'fancy_grid' => array(
      'enabled' => 1,
      'wrapper' => array(
        'additional_classes' => '',
        'no_alignment' => 0,
        'is_container' => 0,
        'is_wrapper' => 1,
        'extra_wrapper' => 0,
        'layouts' => array(
          'custom.user.mobile' => array(
            'position' => 'none',
            'linebreak' => 0,
            'hidden' => 0,
          ),
          'custom.user.tablet' => array(
            'position' => 'none',
            'linebreak' => 0,
            'hidden' => 0,
          ),
          'custom.user.desktop' => array(
            'position' => 'none',
            'linebreak' => 0,
            'hidden' => 0,
          ),
        ),
      ),
      'regions' => array(
        'one' => array(
          'additional_classes' => '',
          'no_alignment' => 0,
          'is_container' => 0,
          'is_wrapper' => 0,
          'extra_wrapper' => 0,
          'layouts' => array(
            'custom.user.mobile' => array(
              'position' => '1|12',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.tablet' => array(
              'position' => '1|12',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.desktop' => array(
              'position' => '1|12',
              'linebreak' => 0,
              'hidden' => 0,
            ),
          ),
        ),
      ),
    ),
  );
  $export['fancy_section|responsive|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'fancy_section|responsive|fancy_grid_four_column';
  $ds_layout->entity_type = 'fancy_section';
  $ds_layout->bundle = 'responsive';
  $ds_layout->view_mode = 'fancy_grid_four_column';
  $ds_layout->layout = 'fancy_grid_four';
  $ds_layout->settings = array(
    'regions' => array(
      'one' => array(
        0 => 'field_fancy_rel_contents',
      ),
      'two' => array(
        1 => 'field_fancy_rel_contents_1',
      ),
      'three' => array(
        2 => 'field_fancy_rel_contents_2',
      ),
      'four' => array(
        3 => 'field_fancy_rel_contents_3',
      ),
    ),
    'fields' => array(
      'field_fancy_rel_contents' => 'one',
      'field_fancy_rel_contents_1' => 'two',
      'field_fancy_rel_contents_2' => 'three',
      'field_fancy_rel_contents_3' => 'four',
    ),
    'classes' => array(),
    'wrappers' => array(
      'one' => 'div',
      'two' => 'div',
      'three' => 'div',
      'four' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
    'fancy_grid' => array(
      'enabled' => 1,
      'wrapper' => array(
        'additional_classes' => '',
        'no_alignment' => 0,
        'is_container' => 0,
        'is_wrapper' => 1,
        'extra_wrapper' => 0,
        'layouts' => array(
          'custom.user.mobile' => array(
            'position' => 'none',
            'linebreak' => 0,
            'hidden' => 0,
          ),
          'custom.user.tablet' => array(
            'position' => 'none',
            'linebreak' => 0,
            'hidden' => 0,
          ),
          'custom.user.desktop' => array(
            'position' => 'none',
            'linebreak' => 0,
            'hidden' => 0,
          ),
        ),
      ),
      'regions' => array(
        'one' => array(
          'additional_classes' => '',
          'no_alignment' => 0,
          'is_container' => 0,
          'is_wrapper' => 0,
          'extra_wrapper' => 0,
          'layouts' => array(
            'custom.user.mobile' => array(
              'position' => '1|6',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.tablet' => array(
              'position' => '1|3',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.desktop' => array(
              'position' => '1|3',
              'linebreak' => 0,
              'hidden' => 0,
            ),
          ),
        ),
        'two' => array(
          'additional_classes' => '',
          'no_alignment' => 0,
          'is_container' => 0,
          'is_wrapper' => 0,
          'extra_wrapper' => 0,
          'layouts' => array(
            'custom.user.mobile' => array(
              'position' => '7|6',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.tablet' => array(
              'position' => '4|3',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.desktop' => array(
              'position' => '4|3',
              'linebreak' => 0,
              'hidden' => 0,
            ),
          ),
        ),
        'three' => array(
          'additional_classes' => '',
          'no_alignment' => 0,
          'is_container' => 0,
          'is_wrapper' => 0,
          'extra_wrapper' => 0,
          'layouts' => array(
            'custom.user.mobile' => array(
              'position' => '1|6',
              'linebreak' => 1,
              'hidden' => 0,
            ),
            'custom.user.tablet' => array(
              'position' => '7|3',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.desktop' => array(
              'position' => '7|3',
              'linebreak' => 0,
              'hidden' => 0,
            ),
          ),
        ),
        'four' => array(
          'additional_classes' => '',
          'no_alignment' => 0,
          'is_container' => 0,
          'is_wrapper' => 0,
          'extra_wrapper' => 0,
          'layouts' => array(
            'custom.user.mobile' => array(
              'position' => '7|6',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.tablet' => array(
              'position' => '10|3',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.desktop' => array(
              'position' => '10|3',
              'linebreak' => 0,
              'hidden' => 0,
            ),
          ),
        ),
      ),
    ),
  );
  $export['fancy_section|responsive|fancy_grid_four_column'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'fancy_section|responsive|fancy_grid_three_column';
  $ds_layout->entity_type = 'fancy_section';
  $ds_layout->bundle = 'responsive';
  $ds_layout->view_mode = 'fancy_grid_three_column';
  $ds_layout->layout = 'fancy_grid_three';
  $ds_layout->settings = array(
    'regions' => array(
      'one' => array(
        0 => 'field_fancy_rel_contents',
      ),
      'two' => array(
        1 => 'field_fancy_rel_contents_1',
      ),
      'three' => array(
        2 => 'field_fancy_rel_contents_2',
      ),
    ),
    'fields' => array(
      'field_fancy_rel_contents' => 'one',
      'field_fancy_rel_contents_1' => 'two',
      'field_fancy_rel_contents_2' => 'three',
    ),
    'classes' => array(),
    'wrappers' => array(
      'one' => 'div',
      'two' => 'div',
      'three' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
    'fancy_grid' => array(
      'enabled' => 1,
      'wrapper' => array(
        'additional_classes' => '',
        'no_alignment' => 0,
        'is_container' => 0,
        'is_wrapper' => 1,
        'extra_wrapper' => 0,
        'layouts' => array(
          'custom.user.mobile' => array(
            'position' => 'none',
            'linebreak' => 0,
            'hidden' => 0,
          ),
          'custom.user.tablet' => array(
            'position' => 'none',
            'linebreak' => 0,
            'hidden' => 0,
          ),
          'custom.user.desktop' => array(
            'position' => 'none',
            'linebreak' => 0,
            'hidden' => 0,
          ),
        ),
      ),
      'regions' => array(
        'one' => array(
          'additional_classes' => '',
          'no_alignment' => 0,
          'is_container' => 0,
          'is_wrapper' => 0,
          'extra_wrapper' => 0,
          'layouts' => array(
            'custom.user.mobile' => array(
              'position' => '1|12',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.tablet' => array(
              'position' => '1|4',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.desktop' => array(
              'position' => '1|4',
              'linebreak' => 0,
              'hidden' => 0,
            ),
          ),
        ),
        'two' => array(
          'additional_classes' => '',
          'no_alignment' => 0,
          'is_container' => 0,
          'is_wrapper' => 0,
          'extra_wrapper' => 0,
          'layouts' => array(
            'custom.user.mobile' => array(
              'position' => '1|12',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.tablet' => array(
              'position' => '5|4',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.desktop' => array(
              'position' => '5|4',
              'linebreak' => 0,
              'hidden' => 0,
            ),
          ),
        ),
        'three' => array(
          'additional_classes' => '',
          'no_alignment' => 0,
          'is_container' => 0,
          'is_wrapper' => 0,
          'extra_wrapper' => 0,
          'layouts' => array(
            'custom.user.mobile' => array(
              'position' => '1|12',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.tablet' => array(
              'position' => '9|4',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.desktop' => array(
              'position' => '9|4',
              'linebreak' => 0,
              'hidden' => 0,
            ),
          ),
        ),
      ),
    ),
  );
  $export['fancy_section|responsive|fancy_grid_three_column'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'fancy_section|responsive|fancy_grid_two_column';
  $ds_layout->entity_type = 'fancy_section';
  $ds_layout->bundle = 'responsive';
  $ds_layout->view_mode = 'fancy_grid_two_column';
  $ds_layout->layout = 'fancy_grid_two';
  $ds_layout->settings = array(
    'regions' => array(
      'one' => array(
        0 => 'field_fancy_rel_contents',
      ),
      'two' => array(
        1 => 'field_fancy_rel_contents_1',
      ),
    ),
    'fields' => array(
      'field_fancy_rel_contents' => 'one',
      'field_fancy_rel_contents_1' => 'two',
    ),
    'classes' => array(),
    'wrappers' => array(
      'one' => 'div',
      'two' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
    'fancy_grid' => array(
      'enabled' => 1,
      'wrapper' => array(
        'additional_classes' => '',
        'no_alignment' => 0,
        'is_container' => 0,
        'is_wrapper' => 1,
        'extra_wrapper' => 0,
        'layouts' => array(
          'custom.user.mobile' => array(
            'position' => 'none',
            'linebreak' => 0,
            'hidden' => 0,
          ),
          'custom.user.tablet' => array(
            'position' => 'none',
            'linebreak' => 0,
            'hidden' => 0,
          ),
          'custom.user.desktop' => array(
            'position' => 'none',
            'linebreak' => 0,
            'hidden' => 0,
          ),
        ),
      ),
      'regions' => array(
        'one' => array(
          'additional_classes' => '',
          'no_alignment' => 0,
          'is_container' => 0,
          'is_wrapper' => 0,
          'extra_wrapper' => 0,
          'layouts' => array(
            'custom.user.mobile' => array(
              'position' => '1|12',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.tablet' => array(
              'position' => '1|6',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.desktop' => array(
              'position' => '1|6',
              'linebreak' => 0,
              'hidden' => 0,
            ),
          ),
        ),
        'two' => array(
          'additional_classes' => '',
          'no_alignment' => 0,
          'is_container' => 0,
          'is_wrapper' => 0,
          'extra_wrapper' => 0,
          'layouts' => array(
            'custom.user.mobile' => array(
              'position' => '1|12',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.tablet' => array(
              'position' => '7|6',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.desktop' => array(
              'position' => '7|6',
              'linebreak' => 0,
              'hidden' => 0,
            ),
          ),
        ),
      ),
    ),
  );
  $export['fancy_section|responsive|fancy_grid_two_column'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'fancy_section|responsive|fancy_grid_two_column_stacked';
  $ds_layout->entity_type = 'fancy_section';
  $ds_layout->bundle = 'responsive';
  $ds_layout->view_mode = 'fancy_grid_two_column_stacked';
  $ds_layout->layout = 'fancy_grid_two';
  $ds_layout->settings = array(
    'regions' => array(
      'one' => array(
        0 => 'field_fancy_rel_contents',
        1 => 'field_fancy_rel_contents_1',
      ),
      'two' => array(
        2 => 'field_fancy_rel_contents_2',
        3 => 'field_fancy_rel_contents_3',
      ),
    ),
    'fields' => array(
      'field_fancy_rel_contents' => 'one',
      'field_fancy_rel_contents_1' => 'one',
      'field_fancy_rel_contents_2' => 'two',
      'field_fancy_rel_contents_3' => 'two',
    ),
    'classes' => array(),
    'wrappers' => array(
      'one' => 'div',
      'two' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
    'fancy_grid' => array(
      'enabled' => 1,
      'wrapper' => array(
        'additional_classes' => '',
        'no_alignment' => 0,
        'is_container' => 0,
        'is_wrapper' => 1,
        'extra_wrapper' => 0,
        'layouts' => array(
          'custom.user.mobile' => array(
            'position' => '1|12',
            'linebreak' => 0,
            'hidden' => 0,
          ),
          'custom.user.tablet' => array(
            'position' => '1|12',
            'linebreak' => 0,
            'hidden' => 0,
          ),
          'custom.user.desktop' => array(
            'position' => '1|12',
            'linebreak' => 0,
            'hidden' => 0,
          ),
        ),
      ),
      'regions' => array(
        'one' => array(
          'additional_classes' => '',
          'no_alignment' => 0,
          'is_container' => 0,
          'is_wrapper' => 0,
          'extra_wrapper' => 0,
          'layouts' => array(
            'custom.user.mobile' => array(
              'position' => '1|12',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.tablet' => array(
              'position' => '1|6',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.desktop' => array(
              'position' => '1|6',
              'linebreak' => 0,
              'hidden' => 0,
            ),
          ),
        ),
        'two' => array(
          'additional_classes' => '',
          'no_alignment' => 0,
          'is_container' => 0,
          'is_wrapper' => 0,
          'extra_wrapper' => 0,
          'layouts' => array(
            'custom.user.mobile' => array(
              'position' => '1|12',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.tablet' => array(
              'position' => '7|6',
              'linebreak' => 0,
              'hidden' => 0,
            ),
            'custom.user.desktop' => array(
              'position' => '7|6',
              'linebreak' => 0,
              'hidden' => 0,
            ),
          ),
        ),
      ),
    ),
  );
  $export['fancy_section|responsive|fancy_grid_two_column_stacked'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'fancy_section|responsive|inline_entity_form_preview';
  $ds_layout->entity_type = 'fancy_section';
  $ds_layout->bundle = 'responsive';
  $ds_layout->view_mode = 'inline_entity_form_preview';
  $ds_layout->layout = 'fancy_grid_one';
  $ds_layout->settings = array(
    'regions' => array(
      'one' => array(
        0 => 'field_fancy_rel_contents',
      ),
    ),
    'fields' => array(
      'field_fancy_rel_contents' => 'one',
    ),
    'classes' => array(),
    'wrappers' => array(),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => TRUE,
    'layout_link_attribute' => FALSE,
    'layout_link_custom' => '',
  );
  $export['fancy_section|responsive|inline_entity_form_preview'] = $ds_layout;

  return $export;
}

/**
 * Implements hook_ds_view_modes_info().
 */
function fancy_section_responsive_ds_view_modes_info() {
  $export = array();

  $ds_view_mode = new stdClass();
  $ds_view_mode->api_version = 1;
  $ds_view_mode->view_mode = 'fancy_grid_four_column';
  $ds_view_mode->label = 'Four column';
  $ds_view_mode->entities = array(
    'fancy_section' => 'fancy_section',
  );
  $export['fancy_grid_four_column'] = $ds_view_mode;

  $ds_view_mode = new stdClass();
  $ds_view_mode->api_version = 1;
  $ds_view_mode->view_mode = 'fancy_grid_three_column';
  $ds_view_mode->label = 'Three column';
  $ds_view_mode->entities = array(
    'fancy_section' => 'fancy_section',
  );
  $export['fancy_grid_three_column'] = $ds_view_mode;

  $ds_view_mode = new stdClass();
  $ds_view_mode->api_version = 1;
  $ds_view_mode->view_mode = 'fancy_grid_two_column';
  $ds_view_mode->label = 'Two column';
  $ds_view_mode->entities = array(
    'fancy_section' => 'fancy_section',
  );
  $export['fancy_grid_two_column'] = $ds_view_mode;

  $ds_view_mode = new stdClass();
  $ds_view_mode->api_version = 1;
  $ds_view_mode->view_mode = 'fancy_grid_two_column_stacked';
  $ds_view_mode->label = 'Two column stacked';
  $ds_view_mode->entities = array(
    'fancy_section' => 'fancy_section',
  );
  $export['fancy_grid_two_column_stacked'] = $ds_view_mode;

  return $export;
}
