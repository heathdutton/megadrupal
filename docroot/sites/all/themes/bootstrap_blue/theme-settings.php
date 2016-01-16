<?php
/**
 * @file
 * Settings for theme
 */

/**
 * Implements hook_form_system_theme_settings_alter().
 */
function bootstrapblue_form_system_theme_settings_alter(&$form, $form_state, $form_id = NULL) {
  $form['comment'] = array(
    '#type' => 'fieldset',
    '#title' => t('Comment Settings'),
  );
  $form['comment']['enable_comment_override'] = array(
    '#type' => 'checkbox',
    '#title' => t('Override the default Drupal comment form'),
    '#default_value' => theme_get_setting('enable_comment_override'),
    '#description' => t('This will alter the default comment form removing text format, subject field and more for a slimmed down appearance'),
  );
  $form['comment']['enable_tips_override'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide format tips'),
    '#default_value' => theme_get_setting('enable_tips_override'),
    '#description' => t('This will hide the format tips for all users'),
  );
  $form['comment']['disable_drupal_resize'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable Drupals default textarea resize handle'),
    '#default_value' => theme_get_setting('disable_drupal_resize'),
    '#description' => t('This will remove Drupals default handling of comment textareas, allowing for the fallback HTML/CSS method'),
  );
  $form['node'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node settings'),
  );
  $form['node']['submittedby'] = array(
    '#type' => 'checkbox',
    '#title' => t('Override the default node byline'),
    '#default_value' => theme_get_setting('submittedby'),
    '#description' => t('Override the standard byline or "Submitted by" information with this themes settings'),
  );
  $form['node']['submittedby_custom_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom date value'),
    '#default_value' => theme_get_setting('submittedby_custom_date'),
    '#description' => t('Override the theme date output using the PHP date format <a href="http://php.net/manual/en/function.date.php">http://php.net/manual/en/function.date.php</a>. If none is provided the themes default will be used'),
    '#states' => array(
      'invisible' => array(
        ':input[name="submittedby"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['head'] = array(
    '#type' => 'fieldset',
    '#title' => t('HTML head settings'),
  );
  $form['head']['meta_generator'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove Drupal version meta tag'),
    '#default_value' => theme_get_setting('meta_generator'),
    '#description' => t('Drupal automatically prints the CMS name and version in the HTML output of the head, check this to remove it'),
  );
}
