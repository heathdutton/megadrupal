<?php

/**
 * @file
 * Theming functions for the quiz_sentence_drop question type.
 * 
 * The theme file for quiz_sentence_drop.
 */

/**
 * Theme function for the quiz_sentence_drop report.
 *
 * @param array $variables
 *   Array of data to be presented in the report.
 */
function theme_quiz_sentence_drop_response($variables) {
  $output = '';
  if ($variables['data']['is_skipped'] == 1) {
    $output = t('Question skipped.');
  } else {
    if ($variables['data']['score'] > 0) {
      $output = t('Nice, You have dropped correct words on the images.');
    } else {
      $output = t('You have dropped wrong words on images.');
    }
  }
  return $output;
}

/**
 * Theme function for the quiz_sentence_drop report.
 *
 * @param array $variables
 *   Array of data to be presented in the report
 */
function theme_quiz_sentence_drop_answer_form($variables) {
  static $css_added;
  global $base_path;
  if (!$css_added) {
    drupal_add_css(drupal_get_path('module', 'quiz_sentence_drop') . '/quiz_sentence_drop.css');
    $css_added = TRUE;

    drupal_add_library('system', 'ui.draggable');
    drupal_add_library('system', 'ui.droppable');
    drupal_add_js(drupal_get_path('module', 'quiz_sentence_drop') . '/quiz_sentence_drop.js');
  }

  $images = '';

  if (isset($variables['data']['answer_options']) && !empty($variables['data']['answer_options'])) {
    foreach ($variables['data']['answer_options'] as $key => $val) {
      $key = strtolower($key);
      $val = strtolower($val);
      $image_id = "dragged_from_" . str_replace("@", "", $key);
      $images[$key] = array('data' => $val, 'class' => array('qsd_answer_words'), 'id' => array($image_id));
    }
  }

  $top_image_path = $base_path . drupal_get_path('module', 'quiz_drag_drop') . "/images/top-arrow.gif";
  $output = '<div class="clearfix seperator">
                <img src="' . $top_image_path . '" alt="" align="absmiddle"/>'
          . t('Drag and drop words to its correct image.') .
          '</div>';
  $output .= '<div class="image_questions"><div class="ques-mid">';

  $images_data = array(
      'items' => $images,
      'title' => '',
      'type' => 'ul',
      'attributes' => array('class' => 'qsd_image_matches clearfix'),
  );
  $output .= theme('item_list', $images_data);
  $output .= '<div></div></div></div>';

  return $output;
}
