<?php
/**
 * @file
 * feature_wysiwyg_editor.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function feature_wysiwyg_editor_filter_default_formats() {
  $formats = array();

  // Exported format: CKEditor WYSIWYG.
  $formats['ckeditor_wysiwyg'] = array(
    'format' => 'ckeditor_wysiwyg',
    'name' => 'CKEditor WYSIWYG',
    'cache' => 1,
    'status' => 1,
    'weight' => -9,
    'filters' => array(
      'feature_wysiwyg_editor_align' => array(
        'weight' => -50,
        'status' => 1,
        'settings' => array(),
      ),
      'feature_wysiwyg_editor_video' => array(
        'weight' => -49,
        'status' => 1,
        'settings' => array(),
      ),
      'image_resize_filter' => array(
        'weight' => -48,
        'status' => 1,
        'settings' => array(
          'link' => 0,
          'link_class' => '',
          'link_rel' => '',
          'image_locations' => array(
            'local' => 'local',
            'remote' => 0,
          ),
        ),
      ),
      'filter_html' => array(
        'weight' => -47,
        'status' => 1,
        'settings' => array(
          'allowed_html' => '<a> <em> <strong> <ul> <ol> <li> <p> <br> <img> <h2> <h3> <h4> <pre> <blockquote> <acronym> <tr><td><th><table> <div> <button> <iframe> <span>',
          'filter_html_help' => 1,
          'filter_html_nofollow' => 0,
        ),
      ),
    ),
  );

  // Exported format: TinyMCE WYSIWYG.
  $formats['tinymce_wysiwyg'] = array(
    'format' => 'tinymce_wysiwyg',
    'name' => 'TinyMCE WYSIWYG',
    'cache' => 1,
    'status' => 1,
    'weight' => -10,
    'filters' => array(
      'feature_wysiwyg_editor_align' => array(
        'weight' => -50,
        'status' => 1,
        'settings' => array(),
      ),
      'feature_wysiwyg_editor_video' => array(
        'weight' => -49,
        'status' => 1,
        'settings' => array(),
      ),
      'image_resize_filter' => array(
        'weight' => -48,
        'status' => 1,
        'settings' => array(
          'link' => 0,
          'link_class' => '',
          'link_rel' => '',
          'image_locations' => array(
            'local' => 'local',
            'remote' => 0,
          ),
        ),
      ),
      'filter_html' => array(
        'weight' => -47,
        'status' => 1,
        'settings' => array(
          'allowed_html' => '<a> <em> <strong> <ul> <ol> <li> <p> <br> <img> <h2> <h3> <h4> <pre> <blockquote> <acronym> <tr><td><th><table>',
          'filter_html_help' => 1,
          'filter_html_nofollow' => 0,
        ),
      ),
    ),
  );

  return $formats;
}
