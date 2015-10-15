<?php
/**
 * @file
 * fancy_section_simple.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function fancy_section_simple_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'fancy_section-simple-field_fancy_rel_contents'
  $field_instances['fancy_section-simple-field_fancy_rel_contents'] = array(
    'bundle' => 'simple',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'view_mode' => 'default',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 0,
      ),
      'fancy_ds_2' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'view_mode' => 'default',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 0,
      ),
      'fancy_ds_2_fluid' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'view_mode' => 'default',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 0,
      ),
      'fancy_ds_2_stack' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'view_mode' => 'default',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 0,
      ),
      'fancy_ds_2_stack_fluid' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'view_mode' => 'default',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 0,
      ),
      'fancy_ds_2col_fluid' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'view_mode' => 'default',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 0,
      ),
      'fancy_ds_2col_stacked' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'view_mode' => 'default',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 0,
      ),
      'fancy_ds_2col_stacked_fluid' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'view_mode' => 'default',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 0,
      ),
      'fancy_ds_3' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'view_mode' => 'default',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 0,
      ),
      'fancy_ds_3_stack' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'view_mode' => 'default',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 0,
      ),
      'fancy_ds_3_stack_fluid' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'view_mode' => 'default',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 0,
      ),
      'fancy_ds_3_weighted' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'view_mode' => 'default',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 0,
      ),
      'fancy_ds_3col' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'view_mode' => 'default',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 0,
      ),
      'fancy_ds_3col_stacked' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'view_mode' => 'default',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 0,
      ),
      'fancy_ds_3col_stacked_equal' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'view_mode' => 'default',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 0,
      ),
      'fancy_ds_3col_stacked_fluid' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'view_mode' => 'default',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 0,
      ),
      'fancy_ds_4' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'view_mode' => 'default',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 0,
      ),
      'fancy_ds_4col' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'view_mode' => 'default',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 0,
      ),
      'inline_entity_form_preview' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'view_mode' => 'inline_entity_form_preview',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 0,
      ),
      'view_mode_selector' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'fancy_section',
    'field_name' => 'field_fancy_rel_contents',
    'label' => 'Content',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'inline_entity_form',
      'settings' => array(
        'fields' => array(),
        'preview' => array(
          'enable' => 0,
          'fallback_view_mode' => 'default',
          'view_mode' => 'inline_entity_form_preview',
        ),
        'type_settings' => array(
          'allow_existing' => 1,
          'delete_references' => 0,
          'label_plural' => 'entities',
          'label_singular' => 'entity',
          'match_operator' => 'CONTAINS',
          'override_labels' => 0,
        ),
      ),
      'type' => 'inline_entity_form',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'fancy_section-simple-field_fancy_view_mode_selector'
  $field_instances['fancy_section-simple-field_fancy_view_mode_selector'] = array(
    'bundle' => 'simple',
    'default_value' => array(
      0 => array(
        'value' => 'default',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'fancy_ds_2' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'fancy_ds_2_fluid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'fancy_ds_2_stack' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'fancy_ds_2_stack_fluid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'fancy_ds_3' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'fancy_ds_3_stack' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'fancy_ds_3_stack_fluid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'fancy_ds_3_weighted' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'fancy_ds_4' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'inline_entity_form_preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'view_mode_selector' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'fancy_section',
    'field_name' => 'field_fancy_view_mode_selector',
    'label' => 'View mode',
    'required' => 1,
    'settings' => array(
      'number_placeholders' => FALSE,
      'user_register_form' => FALSE,
      'view_modes' => array(
        'default' => array(
          'enable' => 1,
          'hide_title' => 0,
          'icon' => 0,
        ),
        'fancy_ds_2' => array(
          'enable' => 1,
          'hide_title' => 0,
          'icon' => 0,
        ),
        'fancy_ds_2_fluid' => array(
          'enable' => 0,
          'hide_title' => 0,
          'icon' => 0,
        ),
        'fancy_ds_2_stack' => array(
          'enable' => 1,
          'hide_title' => 0,
          'icon' => 0,
        ),
        'fancy_ds_2_stack_fluid' => array(
          'enable' => 0,
          'hide_title' => 0,
          'icon' => 0,
        ),
        'fancy_ds_3' => array(
          'enable' => 1,
          'hide_title' => 0,
          'icon' => 0,
        ),
        'fancy_ds_3_stack' => array(
          'enable' => 1,
          'hide_title' => 0,
          'icon' => 0,
        ),
        'fancy_ds_3_stack_fluid' => array(
          'enable' => 0,
          'hide_title' => 0,
          'icon' => 0,
        ),
        'fancy_ds_3_weighted' => array(
          'enable' => 1,
          'hide_title' => 0,
          'icon' => 0,
        ),
        'fancy_ds_4' => array(
          'enable' => 1,
          'hide_title' => 0,
          'icon' => 0,
        ),
        'inline_entity_form_preview' => array(
          'enable' => 0,
          'hide_title' => 0,
          'icon' => 0,
        ),
        'teaser' => array(
          'enable' => 0,
          'hide_title' => 0,
          'icon' => 0,
        ),
        'token' => array(
          'enable' => 0,
          'hide_title' => 0,
          'icon' => 0,
        ),
      ),
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'view_mode_selector',
      'settings' => array(),
      'type' => 'view_mode_selector_radios',
      'weight' => 0,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Content');
  t('View mode');

  return $field_instances;
}
