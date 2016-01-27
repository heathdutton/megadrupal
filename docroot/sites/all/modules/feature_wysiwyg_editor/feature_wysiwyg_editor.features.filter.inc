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
    'name' => 'WYSIWYG',
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
          'allowed_html' => '<a> <em> <strong> <ul> <ol> <li> <p> <br> <img> <h2> <h3> <h4> <pre> <blockquote> <acronym> <table> <thead> <tbody> <tr> <td> <th> <div> <button> <iframe> <span> <hr> <sup> <sub>',
          'filter_html_help' => 1,
          'filter_html_nofollow' => 0,
        ),
      ),
    ),
  );

  // Exported format: CKEditor WYSIWYG Minimal.
  $formats['ckeditor_wysiwyg_minimal'] = array(
    'format' => 'ckeditor_wysiwyg_minimal',
    'name' => 'WYSIWYG Minimal',
    'cache' => 1,
    'status' => 1,
    'weight' => 0,
    'filters' => array(
      'feature_wysiwyg_editor_align' => array(
        'weight' => -12,
        'status' => 1,
        'settings' => array(),
      ),
      'feature_wysiwyg_editor_lists' => array(
        'weight' => -12,
        'status' => 1,
        'settings' => array(),
      ),
      'feature_wysiwyg_editor_video' => array(
        'weight' => -11,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_html' => array(
        'weight' => -10,
        'status' => 1,
        'settings' => array(
          'allowed_html' => '<a> <em> <strong> <cite> <blockquote> <code> <ul> <ol> <li> <dl> <dt> <dd>',
          'filter_html_help' => 1,
          'filter_html_nofollow' => 0,
        ),
      ),
      'image_resize_filter' => array(
        'weight' => 0,
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
    ),
  );


  return $formats;
}
