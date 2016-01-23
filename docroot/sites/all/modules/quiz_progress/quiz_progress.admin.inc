<?php
/**
 * @file
 * Quiz_progress admin.inc file.
 *
 * This file includes the configuration form for quiz progress module
 *   - Using this form we can set either question title or number to be displayed in the quiz progress block
 *   - One can set text & background colors for questions in the block depending on their status answered, skipped or queued
 */
/**
 * Quiz Progress configuration form
 */
function quiz_progress_config_form() {
  $form['quiz_progress_output_style'] = array(
    '#title' => t('Output style'),
    '#type' => 'radios',
    '#options' => array(
      'number' => t('Question numbers'),
      'title' => t('Questions title')
    ),
    '#default_value' => variable_get('quiz_progress_output_style', 'number'),
  );
  $form['quiz_progress_number_style_col_size'] = array(
    '#title' => t('Column size for "<em>question numbers</em>" style output'),
    '#description' => t('Counts of question numbers to be displayed per row in questions numbers stype output.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('quiz_progress_number_style_col_size', 4),
    '#maxlength' => 3,
    '#size' => 4,
  );
  $form['quiz_progress_title_style_length'] = array(
    '#title' => t('Maximum length of title for "<em>question numbers</em>" style output'),
    '#description' => t('Counts of question numbers to be displayed per row in questions numbers stype output.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('quiz_progress_title_style_length', 32),
    '#maxlength' => 3,
    '#size' => 4,
  );
  $form['quiz_progress_output_color'] = array(
    '#type' => 'fieldset',
    '#title' => t('Color'),
    '#description' => t('Hexa color code to hightlight upcoming, skipped and answered questions. Eg #FFFFFF for white. Leave this field empty to use default color defined in your theme.'),
  );
  // Answered questions
  $form['quiz_progress_output_color']['quiz_progress_output_bg_color_answered'] = array(
    '#title' => t('Answered questions background color'),
    '#type' => 'textfield',
    '#default_value' => variable_get('quiz_progress_output_bg_color_answered', '#009900'),
    '#maxlength' => 7,
    '#size' => 10,
  );
  $form['quiz_progress_output_color']['quiz_progress_output_text_color_answered'] = array(
    '#title' => t('Answered questions text color'),
    '#type' => 'textfield',
    '#default_value' => variable_get('quiz_progress_output_text_color_answered', '#001F00'),
    '#maxlength' => 7,
    '#size' => 10,
  );
  // skipped questions
  $form['quiz_progress_output_color']['quiz_progress_output_bg_color_skipped'] = array(
    '#title' => t('Skipped questions background color'),
    '#type' => 'textfield',
    '#default_value' => variable_get('quiz_progress_output_bg_color_skipped', '#FF0000'),
    '#maxlength' => 7,
    '#size' => 10,
  );
  $form['quiz_progress_output_color']['quiz_progress_output_text_color_skipped'] = array(
    '#title' => t('Skipped questions text color'),
    '#type' => 'textfield',
    '#default_value' => variable_get('quiz_progress_output_text_color_skipped', '#800000'),
    '#maxlength' => 7,
    '#size' => 10,
  );
  // upcoming questions
  $form['quiz_progress_output_color']['quiz_progress_output_bg_color_queued'] = array(
    '#title' => t('Upcoming questions Background color'),
    '#type' => 'textfield',
    '#default_value' => variable_get('quiz_progress_output_bg_color_queued', '#FFA500'),
    '#maxlength' => 7,
    '#size' => 10,
  );
  $form['quiz_progress_output_color']['quiz_progress_output_text_color_queued'] = array(
    '#title' => t('Upcoming questions text color'),
    '#type' => 'textfield',
    '#default_value' => variable_get('quiz_progress_output_text_color_queued', '#804C00'),
    '#maxlength' => 7,
    '#size' => 10,
  );
  return system_settings_form($form);
}
