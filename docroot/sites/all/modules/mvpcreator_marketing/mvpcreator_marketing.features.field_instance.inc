<?php
/**
 * @file
 * mvpcreator_marketing.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function mvpcreator_marketing_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'fieldable_panels_pane-mvpcreator_marketing_cta-field_mvpcreator_marketing_btn'
  $field_instances['fieldable_panels_pane-mvpcreator_marketing_cta-field_mvpcreator_marketing_btn'] = array(
    'bundle' => 'mvpcreator_marketing_cta',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'fieldable_panels_pane',
    'field_name' => 'field_mvpcreator_marketing_btn',
    'label' => 'Button',
    'required' => 1,
    'settings' => array(
      'absolute_url' => 1,
      'attributes' => array(
        'class' => 'mvpcreator-marketing-cta-btn',
        'configurable_class' => 0,
        'configurable_title' => 0,
        'rel' => '',
        'target' => 'default',
        'title' => '',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => 1,
      'linkit' => array(
        'button_text' => 'Search',
        'enable' => 0,
        'profile' => '',
      ),
      'rel_remove' => 'default',
      'title' => 'required',
      'title_label_use_field_label' => 0,
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
      'user_register_form' => FALSE,
      'validate_url' => 1,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'link',
      'settings' => array(),
      'type' => 'link_field',
      'weight' => -3,
    ),
  );

  // Exported field_instance: 'fieldable_panels_pane-mvpcreator_marketing_cta-field_mvpcreator_marketing_link'
  $field_instances['fieldable_panels_pane-mvpcreator_marketing_cta-field_mvpcreator_marketing_link'] = array(
    'bundle' => 'mvpcreator_marketing_cta',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_default',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'fieldable_panels_pane',
    'field_name' => 'field_mvpcreator_marketing_link',
    'label' => 'Alternate link',
    'required' => 0,
    'settings' => array(
      'absolute_url' => 1,
      'attributes' => array(
        'class' => 'mvpcreator-marketing-cta-alt',
        'configurable_class' => 0,
        'configurable_title' => 0,
        'rel' => '',
        'target' => 'default',
        'title' => '',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => 1,
      'linkit' => array(
        'button_text' => 'Search',
        'enable' => 0,
        'profile' => '',
      ),
      'rel_remove' => 'default',
      'title' => 'required',
      'title_label_use_field_label' => 0,
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
      'user_register_form' => FALSE,
      'validate_url' => 1,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'link',
      'settings' => array(),
      'type' => 'link_field',
      'weight' => -2,
    ),
  );

  // Exported field_instance: 'fieldable_panels_pane-mvpcreator_marketing_cta-field_mvpcreator_marketing_text'
  $field_instances['fieldable_panels_pane-mvpcreator_marketing_cta-field_mvpcreator_marketing_text'] = array(
    'bundle' => 'mvpcreator_marketing_cta',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'fieldable_panels_pane',
    'field_name' => 'field_mvpcreator_marketing_text',
    'label' => 'Description',
    'required' => 0,
    'settings' => array(
      'linkit' => array(
        'button_text' => 'Search',
        'enable' => 0,
        'profile' => '',
      ),
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => -4,
    ),
  );

  // Exported field_instance: 'fieldable_panels_pane-mvpcreator_marketing_testimonial-field_mvpcreator_marketing_image'
  $field_instances['fieldable_panels_pane-mvpcreator_marketing_testimonial-field_mvpcreator_marketing_image'] = array(
    'bundle' => 'mvpcreator_marketing_testimonial',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'image',
        'settings' => array(
          'image_link' => '',
          'image_style' => 'panopoly_image_thumbnail',
        ),
        'type' => 'image',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'fieldable_panels_pane',
    'field_name' => 'field_mvpcreator_marketing_image',
    'label' => 'Customer image',
    'required' => 0,
    'settings' => array(
      'alt_field' => 0,
      'default_image' => 0,
      'file_directory' => '',
      'file_extensions' => 'png gif jpg jpeg',
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '',
      'title_field' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'image',
      'settings' => array(
        'manualcrop_crop_info' => 1,
        'manualcrop_default_crop_area' => 1,
        'manualcrop_enable' => 1,
        'manualcrop_inline_crop' => 0,
        'manualcrop_instant_crop' => 0,
        'manualcrop_instant_preview' => 1,
        'manualcrop_keyboard' => 1,
        'manualcrop_maximize_default_crop_area' => 0,
        'manualcrop_require_cropping' => array(),
        'manualcrop_styles_list' => array(
          'panopoly_image_full' => 'panopoly_image_full',
        ),
        'manualcrop_styles_mode' => 'include',
        'manualcrop_thumblist' => 0,
        'preview_image_style' => 'panopoly_image_thumbnail',
        'progress_indicator' => 'throbber',
      ),
      'type' => 'image_image',
      'weight' => 0,
    ),
  );

  // Exported field_instance: 'fieldable_panels_pane-mvpcreator_marketing_testimonial-field_mvpcreator_marketing_name'
  $field_instances['fieldable_panels_pane-mvpcreator_marketing_testimonial-field_mvpcreator_marketing_name'] = array(
    'bundle' => 'mvpcreator_marketing_testimonial',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'fieldable_panels_pane',
    'field_name' => 'field_mvpcreator_marketing_name',
    'label' => 'Customer name',
    'required' => 1,
    'settings' => array(
      'linkit' => array(
        'button_text' => 'Search',
        'enable' => 0,
        'profile' => '',
      ),
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => -2,
    ),
  );

  // Exported field_instance: 'fieldable_panels_pane-mvpcreator_marketing_testimonial-field_mvpcreator_marketing_text'
  $field_instances['fieldable_panels_pane-mvpcreator_marketing_testimonial-field_mvpcreator_marketing_text'] = array(
    'bundle' => 'mvpcreator_marketing_testimonial',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'fieldable_panels_pane',
    'field_name' => 'field_mvpcreator_marketing_text',
    'label' => 'Quote',
    'required' => 1,
    'settings' => array(
      'linkit' => array(
        'button_text' => 'Search',
        'enable' => 0,
        'profile' => '',
      ),
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => -3,
    ),
  );

  // Exported field_instance: 'fieldable_panels_pane-mvpcreator_marketing_testimonial-field_mvpcreator_marketing_title'
  $field_instances['fieldable_panels_pane-mvpcreator_marketing_testimonial-field_mvpcreator_marketing_title'] = array(
    'bundle' => 'mvpcreator_marketing_testimonial',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Who or what the customer represents. This can be a job title (ex. "CEO of Acme Corp"), life role (ex. "Mother of four") or even a location (ex. "Milwaukee, WI").',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 4,
      ),
    ),
    'entity_type' => 'fieldable_panels_pane',
    'field_name' => 'field_mvpcreator_marketing_title',
    'label' => 'Customer title',
    'required' => 0,
    'settings' => array(
      'linkit' => array(
        'button_text' => 'Search',
        'enable' => 0,
        'profile' => '',
      ),
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => -1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Alternate link');
  t('Button');
  t('Customer image');
  t('Customer name');
  t('Customer title');
  t('Description');
  t('Quote');
  t('Who or what the customer represents. This can be a job title (ex. "CEO of Acme Corp"), life role (ex. "Mother of four") or even a location (ex. "Milwaukee, WI").');

  return $field_instances;
}
