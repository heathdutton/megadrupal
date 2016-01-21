<?php

/**
 * @file smackdown.content.inc
 *
 * This is our content export which includes the two nodereference fields that
 * we need for the module to operate upon.
 */
function smackdown_content_export() {
  $output = "\$content['type']  = array (
    'name' => 'Smackdown',
    'type' => 'smackdown',
    'description' => 'A smackdown pairs two nodes of your choice and lets users vote on them according to the criteria given.',
    'title_label' => 'Question',
    'body_label' => '',
    'min_word_count' => '0',
    'help' => '',
    'node_options' =>
    array (
      'status' => true,
      'promote' => false,
      'sticky' => false,
      'revision' => false,
    ),
    'old_type' => 'smackdown',
    'orig_type' => '',
    'module' => 'node',
    'custom' => '1',
    'modified' => '1',
    'locked' => '0',
    'comment' => '0',
    'comment_default_mode' => '4',
    'comment_default_order' => '1',
    'comment_default_per_page' => '50',
    'comment_controls' => '3',
    'comment_anonymous' => 0,
    'comment_subject_field' => '1',
    'comment_preview' => '1',
    'comment_form_location' => '0',
    'fivestar' => 0,
    'fivestar_stars' => 5,
    'fivestar_labels_enable' => 1,
    'fivestar_label_0' => 'Cancel rating',
    'fivestar_label_1' => 'Poor',
    'fivestar_label_2' => 'Okay',
    'fivestar_label_3' => 'Good',
    'fivestar_label_4' => 'Great',
    'fivestar_label_5' => 'Awesome',
    'fivestar_label_6' => 'Give it @star/@count',
    'fivestar_label_7' => 'Give it @star/@count',
    'fivestar_label_8' => 'Give it @star/@count',
    'fivestar_label_9' => 'Give it @star/@count',
    'fivestar_label_10' => 'Give it @star/@count',
    'fivestar_style' => 'average',
    'fivestar_text' => 'dual',
    'fivestar_title' => 1,
    'fivestar_feedback' => 1,
    'fivestar_unvote' => 0,
    'fivestar_position_teaser' => 'hidden',
    'fivestar_position' => 'below',
  );
  \$content['fields']  = array (
    0 =>
    array (
      'label' => 'First Contestant',
      'field_name' => 'field_ref1',
      'type' => 'nodereference',
      'widget_type' => 'nodereference_autocomplete',
      'change' => 'Change basic information',
      'weight' => '-4',
      'autocomplete_match' => 'contains',
      'description' => '',
      'default_value' =>
      array (
        0 =>
        array (
          'nid' => NULL,
          '_error_element' => 'default_value_widget][field_ref1][0][nid][nid',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => NULL,
      'required' => 1,
      'multiple' => '0',
      'referenceable_types' =>
      array (
        'story' => 'story',
        'blog' => 0,
        'deadwood_category' => 0,
        'deadwood_item' => 0,
        'page' => 0,
        'poll' => 0,
        'smackdown' => 0,
        'classified' => false,
        'discussion' => false,
        'event' => false,
        'file' => false,
        'list' => false,
        'photo' => false,
        'question' => false,
        'recipe' => false,
        'review' => false,
      ),
      'show_add_link' => true,
      'op' => 'Save field settings',
      'module' => 'nodereference',
      'widget_module' => 'nodereference',
      'columns' =>
      array (
        'nid' =>
        array (
          'type' => 'int',
          'unsigned' => true,
          'not null' => false,
        ),
      ),
      'display_settings' =>
      array (
        'label' =>
        array (
          'format' => 'above',
          'exclude' => 0,
        ),
        'teaser' =>
        array (
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' =>
        array (
          'format' => 'default',
          'exclude' => 0,
        ),
        4 =>
        array (
          'format' => 'default',
          'exclude' => 0,
        ),
        2 =>
        array (
          'format' => 'default',
          'exclude' => 0,
        ),
        3 =>
        array (
          'format' => 'default',
          'exclude' => 0,
        ),
        'token' =>
        array (
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    1 =>
    array (
      'label' => 'First Contestant Caption',
      'field_name' => 'field_ref1_caption',
      'type' => 'text',
      'widget_type' => 'text_textfield',
      'change' => 'Change basic information',
      'weight' => '-3',
      'rows' => 5,
      'size' => '60',
      'description' => '',
      'default_value' =>
      array (
        0 =>
        array (
          'value' => '',
          '_error_element' => 'default_value_widget][field_ref1_caption][0][value',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' =>
      array (
        'field_ref1_caption' =>
        array (
          0 =>
          array (
            'value' => '',
            '_error_element' => 'default_value_widget][field_ref1_caption][0][value',
          ),
        ),
      ),
      'required' => 0,
      'multiple' => '0',
      'text_processing' => '0',
      'max_length' => '',
      'allowed_values' => '',
      'allowed_values_php' => '',
      'op' => 'Save field settings',
      'module' => 'text',
      'widget_module' => 'text',
      'columns' =>
      array (
        'value' =>
        array (
          'type' => 'text',
          'size' => 'big',
          'not null' => false,
          'sortable' => true,
          'views' => true,
        ),
      ),
      'display_settings' =>
      array (
        'label' =>
        array (
          'format' => 'above',
          'exclude' => 0,
        ),
        'teaser' =>
        array (
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' =>
        array (
          'format' => 'default',
          'exclude' => 0,
        ),
        4 =>
        array (
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    2 =>
    array (
      'label' => 'Second Contestant',
      'field_name' => 'field_ref2',
      'type' => 'nodereference',
      'widget_type' => 'nodereference_autocomplete',
      'change' => 'Change basic information',
      'weight' => '-2',
      'autocomplete_match' => 'contains',
      'description' => '',
      'default_value' =>
      array (
        0 =>
        array (
          'nid' => NULL,
          '_error_element' => 'default_value_widget][field_ref2][0][nid][nid',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' =>
      array (
        'field_ref2' =>
        array (
          0 =>
          array (
            'nid' =>
            array (
              'nid' => '',
              '_error_element' => 'default_value_widget][field_ref2][0][nid][nid',
            ),
            '_error_element' => 'default_value_widget][field_ref2][0][nid][nid',
          ),
        ),
      ),
      'required' => 1,
      'multiple' => '0',
      'referenceable_types' =>
      array (
        'story' => 'story',
        'blog' => 0,
        'deadwood_category' => 0,
        'deadwood_item' => 0,
        'page' => 0,
        'poll' => 0,
        'smackdown' => 0,
        'classified' => false,
        'discussion' => false,
        'event' => false,
        'file' => false,
        'list' => false,
        'photo' => false,
        'question' => false,
        'recipe' => false,
        'review' => false,
      ),
      'show_add_link' => true,
      'op' => 'Save field settings',
      'module' => 'nodereference',
      'widget_module' => 'nodereference',
      'columns' =>
      array (
        'nid' =>
        array (
          'type' => 'int',
          'unsigned' => true,
          'not null' => false,
        ),
      ),
      'display_settings' =>
      array (
        'label' =>
        array (
          'format' => 'above',
          'exclude' => 0,
        ),
        'teaser' =>
        array (
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' =>
        array (
          'format' => 'default',
          'exclude' => 0,
        ),
        4 =>
        array (
          'format' => 'default',
          'exclude' => 0,
        ),
        2 =>
        array (
          'format' => 'default',
          'exclude' => 0,
        ),
        3 =>
        array (
          'format' => 'default',
          'exclude' => 0,
        ),
        'token' =>
        array (
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    3 =>
    array (
      'label' => 'Second Contestant Caption',
      'field_name' => 'field_ref2_caption',
      'type' => 'text',
      'widget_type' => 'text_textfield',
      'change' => 'Change basic information',
      'weight' => '-1',
      'rows' => 5,
      'size' => '60',
      'description' => '',
      'default_value' =>
      array (
        0 =>
        array (
          'value' => '',
          '_error_element' => 'default_value_widget][field_ref2_caption][0][value',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' =>
      array (
        'field_ref2_caption' =>
        array (
          0 =>
          array (
            'value' => '',
            '_error_element' => 'default_value_widget][field_ref2_caption][0][value',
          ),
        ),
      ),
      'required' => 0,
      'multiple' => '0',
      'text_processing' => '0',
      'max_length' => '',
      'allowed_values' => '',
      'allowed_values_php' => '',
      'op' => 'Save field settings',
      'module' => 'text',
      'widget_module' => 'text',
      'columns' =>
      array (
        'value' =>
        array (
          'type' => 'text',
          'size' => 'big',
          'not null' => false,
          'sortable' => true,
          'views' => true,
        ),
      ),
      'display_settings' =>
      array (
        'label' =>
        array (
          'format' => 'above',
          'exclude' => 0,
        ),
        'teaser' =>
        array (
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' =>
        array (
          'format' => 'default',
          'exclude' => 0,
        ),
        4 =>
        array (
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
  );
  \$content['extra']  = array (
    'title' => '-5',
    'menu' => '0',
  );";
  return $output;
}
