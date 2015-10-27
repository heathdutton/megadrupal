<?php
/**
 * @file
 * cms_portfolio.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function cms_portfolio_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-portfolio-body'
  $field_instances['node-portfolio-body'] = array(
    'bundle' => 'portfolio',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_formatter_class' => 'lead',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'mdp_full_popup' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'mdp_inline_popup' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_formatter_class' => '',
          'trim_length' => 300,
        ),
        'type' => 'text_summary_or_trimmed',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'fences_wrapper' => 'div',
    'field_name' => 'body',
    'label' => 'Details',
    'required' => 0,
    'settings' => array(
      'display_summary' => 1,
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
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-portfolio-field_cms_portfolio_client'
  $field_instances['node-portfolio-field_cms_portfolio_client'] = array(
    'bundle' => 'portfolio',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_formatter_class' => 'text-muted',
        ),
        'type' => 'text_default',
        'weight' => 18,
      ),
      'mdp_full_popup' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'mdp_inline_popup' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'fences_wrapper' => 'div',
    'field_name' => 'field_cms_portfolio_client',
    'label' => 'Client',
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
      'weight' => 15,
    ),
  );

  // Exported field_instance: 'node-portfolio-field_cms_portfolio_links'
  $field_instances['node-portfolio-field_cms_portfolio_links'] = array(
    'bundle' => 'portfolio',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'link',
        'settings' => array(
          'field_formatter_class' => '',
        ),
        'type' => 'link_default',
        'weight' => 19,
      ),
      'mdp_full_popup' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'mdp_inline_popup' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'fences_wrapper' => 'div',
    'field_name' => 'field_cms_portfolio_links',
    'label' => 'Project Links',
    'required' => 0,
    'settings' => array(
      'absolute_url' => 1,
      'attributes' => array(
        'class' => 'btn btn-md',
        'configurable_class' => 1,
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
      'title' => 'optional',
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
      'weight' => 16,
    ),
  );

  // Exported field_instance: 'node-portfolio-field_cms_portoflio_custom'
  $field_instances['node-portfolio-field_cms_portoflio_custom'] = array(
    'bundle' => 'portfolio',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_formatter_class' => 'text-muted',
        ),
        'type' => 'text_default',
        'weight' => 17,
      ),
      'mdp_full_popup' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'mdp_inline_popup' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'fences_wrapper' => 'div',
    'field_name' => 'field_cms_portoflio_custom',
    'label' => 'Custom Field',
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
      'weight' => 14,
    ),
  );

  // Exported field_instance: 'node-portfolio-field_page_attachments'
  $field_instances['node-portfolio-field_page_attachments'] = array(
    'bundle' => 'portfolio',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
      'mdp_full_popup' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'mdp_inline_popup' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'fences_wrapper' => 'div',
    'field_name' => 'field_page_attachments',
    'label' => 'Attachments',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => '',
      'file_extensions' => 'jpg jpeg gif png txt doc docx xls xlsx pdf ppt pptx pps ppsx odt ods odp mp3 mov mp4 m4a m4v mpeg avi ogg oga ogv weba webp webm',
      'max_filesize' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'media',
      'settings' => array(
        'allowed_schemes' => array(
          'public' => 'public',
        ),
        'allowed_types' => array(
          'audio' => 0,
          'document' => 0,
          'image' => 0,
          'video' => 0,
        ),
        'browser_plugins' => array(
          'media_default--media_browser_1' => 0,
          'media_default--media_browser_my_files' => 0,
          'media_internet' => 0,
          'upload' => 0,
        ),
      ),
      'type' => 'media_generic',
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'node-portfolio-field_portfolio_images'
  $field_instances['node-portfolio-field_portfolio_images'] = array(
    'bundle' => 'portfolio',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'image_field_caption',
        'settings' => array(
          'caption' => '',
          'field_formatter_class' => '',
          'image_link' => '',
          'image_style' => 'bootstrap3_col12',
          'text_format' => 'wysiwyg_simple',
        ),
        'type' => 'image_field_caption',
        'weight' => 1,
      ),
      'mdp_full_popup' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'mdp_inline_popup' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'node',
    'fences_wrapper' => 'div',
    'field_name' => 'field_portfolio_images',
    'label' => 'Portfolio images',
    'required' => 1,
    'settings' => array(
      'alt_field' => 1,
      'default_image' => 0,
      'file_directory' => 'portfolio',
      'file_extensions' => 'png gif jpg jpeg',
      'image_field_caption' => 1,
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '',
      'title_field' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'media',
      'settings' => array(
        'allowed_schemes' => array(
          0 => 'public',
          1 => 'private',
        ),
        'allowed_types' => array(
          0 => 'image',
        ),
        'browser_plugins' => array(),
        'filefield_sources' => array(
          'filefield_sources' => array(
            'attach' => 0,
            'clipboard' => 0,
            'plupload' => 'plupload',
            'reference' => 'reference',
            'remote' => 'remote',
            'upload' => 0,
          ),
          'source_attach' => array(
            'absolute' => 0,
            'attach_mode' => 'move',
            'path' => 'file_attach',
          ),
          'source_imce' => array(
            'imce_mode' => 0,
          ),
          'source_reference' => array(
            'autocomplete' => 1,
          ),
        ),
        'preview_image_style' => 'thumbnail',
        'progress_indicator' => 'throbber',
      ),
      'type' => 'media_generic',
      'weight' => 3,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Attachments');
  t('Client');
  t('Custom Field');
  t('Details');
  t('Portfolio images');
  t('Project Links');

  return $field_instances;
}
