<?php
/**
 * @file
 * cms_core_content.features.uuid_bean.inc
 */

/**
 * Implements hook_uuid_features_default_beans().
 */
function cms_core_content_uuid_features_default_beans() {
  $beans = array();

  $beans[] = array(
    'label' => 'Contact Form Text',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'contact-form-text',
    'view_mode' => 'default',
    'created' => 1449411776,
    'log' => '',
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '766642d5-aa48-45c3-b2af-952f22210b7c',
    'uuid' => '990b775f-d1d1-4715-b25e-8c764152a779',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<h2 data-fontsize="18" data-lineheight="27">We’d Love to Hear From You, Lets Get In Touch!</h2><p>Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa.</p><p>Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim.</p>',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
    'user_uuid' => 'c155b92b-6053-4816-8abf-401a4fcd0619',
  );
  return $beans;
}
