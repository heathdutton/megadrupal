<?php
/**
 * @file
 * cms_contact_content.features.uuid_node.inc
 */

/**
 * Implements hook_uuid_features_default_content().
 */
function cms_contact_content_uuid_features_default_content() {
  $nodes = array();

  $nodes[] = array(
  'title' => 'Contact',
  'log' => '',
  'status' => 1,
  'comment' => 1,
  'promote' => 0,
  'sticky' => 0,
  'type' => 'webform',
  'language' => 'und',
  'created' => 1393769008,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '52ec77c2-0281-4140-b891-1aff2cb69709',
  'field_webform_body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent sodales justo sit amet sapien sollicitudin fringilla. Duis dignissim arcu non dui mattis fringilla. Donec eu sapien et nibh blandit bibendum dapibus nec velit. Aenean euismod id lorem sed posuere.</p>
',
        'summary' => '',
        'format' => 'wysiwyg_full',
      ),
    ),
  ),
  'field_glazed_content_design' => array(),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'webform' => array(
    'nid' => 16,
    'next_serial' => 1,
    'confirmation' => '',
    'confirmation_format' => 'wysiwyg_simple',
    'redirect_url' => '<confirmation>',
    'status' => 1,
    'block' => 0,
    'allow_draft' => 0,
    'auto_save' => 0,
    'submit_notice' => 1,
    'confidential' => 0,
    'submit_text' => '',
    'submit_limit' => -1,
    'submit_interval' => -1,
    'total_submit_limit' => -1,
    'total_submit_interval' => -1,
    'progressbar_bar' => 1,
    'progressbar_page_number' => 0,
    'progressbar_percent' => 0,
    'progressbar_pagebreak_labels' => 1,
    'progressbar_include_confirmation' => 1,
    'progressbar_label_first' => 'Start',
    'progressbar_label_confirmation' => 'Complete',
    'preview' => 0,
    'preview_next_button_label' => '',
    'preview_prev_button_label' => '',
    'preview_title' => '',
    'preview_message' => '',
    'preview_message_format' => 'wysiwyg_simple',
    'preview_excluded_components' => array(),
    'record_exists' => TRUE,
    'roles' => array(
      0 => 1,
      1 => 2,
    ),
    'emails' => array(),
    'components' => array(
      1 => array(
        'nid' => 16,
        'cid' => 1,
        'pid' => 0,
        'form_key' => 'name',
        'name' => 'Name',
        'type' => 'textfield',
        'value' => '',
        'extra' => array(
          'title_display' => 'before',
          'private' => 0,
          'wrapper_classes' => '',
          'css_classes' => '',
          'width' => '',
          'maxlength' => '',
          'field_prefix' => '',
          'field_suffix' => '',
          'disabled' => 0,
          'unique' => 0,
          'description' => '',
          'placeholder' => '',
          'attributes' => array(),
          'analysis' => FALSE,
        ),
        'required' => 1,
        'weight' => 0,
        'page_num' => 1,
      ),
      2 => array(
        'nid' => 16,
        'cid' => 2,
        'pid' => 0,
        'form_key' => 'e_mail',
        'name' => 'E-mail',
        'type' => 'email',
        'value' => '[current-user:mail]',
        'extra' => array(
          'title_display' => 'before',
          'private' => 0,
          'wrapper_classes' => '',
          'css_classes' => '',
          'width' => '',
          'unique' => 0,
          'disabled' => 0,
          'description' => '',
          'placeholder' => '',
          'attributes' => array(),
          'analysis' => FALSE,
          'multiple' => 0,
          'format' => 'short',
        ),
        'required' => 1,
        'weight' => 1,
        'page_num' => 1,
      ),
      3 => array(
        'nid' => 16,
        'cid' => 3,
        'pid' => 0,
        'form_key' => 'subject',
        'name' => 'Subject',
        'type' => 'textfield',
        'value' => '',
        'extra' => array(
          'title_display' => 'before',
          'private' => 0,
          'wrapper_classes' => '',
          'css_classes' => '',
          'width' => '',
          'maxlength' => '',
          'field_prefix' => '',
          'field_suffix' => '',
          'disabled' => 0,
          'unique' => 0,
          'description' => '',
          'placeholder' => '',
          'attributes' => array(),
          'analysis' => FALSE,
        ),
        'required' => 1,
        'weight' => 2,
        'page_num' => 1,
      ),
      4 => array(
        'nid' => 16,
        'cid' => 4,
        'pid' => 0,
        'form_key' => 'message',
        'name' => 'Message',
        'type' => 'textarea',
        'value' => '',
        'extra' => array(
          'title_display' => 'before',
          'private' => 0,
          'wrapper_classes' => '',
          'css_classes' => '',
          'rows' => 10,
          'cols' => '',
          'resizable' => 1,
          'disabled' => 0,
          'description' => '',
          'placeholder' => '',
          'attributes' => array(),
          'analysis' => FALSE,
        ),
        'required' => 1,
        'weight' => 3,
        'page_num' => 1,
      ),
    ),
    'conditionals' => array(),
  ),
  'comment_count' => 0,
  'date' => '2014-03-02 14:03:28 +0000',
  'user_uuid' => '27324dc9-31eb-416e-831a-8de6ede64374',
);
  return $nodes;
}
