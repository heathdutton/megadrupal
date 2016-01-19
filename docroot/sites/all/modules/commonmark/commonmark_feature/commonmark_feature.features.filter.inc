<?php
/**
 * @file
 * commonmark_feature.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function commonmark_feature_filter_default_formats() {
  $formats = array();

  // Exported format: CommonMark (Markdown).
  $formats['commonmark'] = array(
    'format' => 'commonmark',
    'name' => 'CommonMark (Markdown)',
    'cache' => 1,
    'status' => 1,
    'weight' => -10,
    'filters' => array(
      'commonmark' => array(
        'weight' => -50,
        'status' => 1,
        'settings' => array(
          'allowed_tags' => '<a> <abbr> <address> <blockquote> <br> <cite> <code> <dd> <del> <div> <dl> <dt> <em> <h1> <h2> <h3> <h4> <h5> <h6> <hr> <img> <kbd> <li> <ol> <p> <pre> <small> <span> <strong> <sub> <sup> <table> <tbody> <td> <tfoot> <th> <thead> <tr> <ul>',
          'show_examples' => 1,
          'use_global_settings' => 1,
        ),
      ),
      'filter_htmlcorrector' => array(
        'weight' => 10,
        'status' => 1,
        'settings' => array(),
      ),
    ),
  );

  return $formats;
}
