<?php
/**
 * @file
 * Template functions for theme
 */

/**
 * Implements hook_html_head_alter().
 */
function bootstrapblue_html_head_alter(&$head_elements) {
  // Removes Drupal version meta tag from head
  if (theme_get_setting('meta_generator') == 1) {
    unset($head_elements['system_meta_generator']);
  }
}

/**
 * Implements hook_form_comment_form_alter().
 */
function bootstrapblue_form_comment_form_alter(&$form, &$form_state, $form_id) {
  // Alter the comment form if enabled in theme settings.
  if (theme_get_setting('enable_comment_override') == 1) {
    $form['author']['name']['#attributes']['placeholder'] = t('Name');
    $form['author']['name']['#title_display'] = 'invisible';
    $form['author']['_author']['#title_display'] = 'invisible';
    $form['author']['mail']['#attributes']['placeholder'] = t('E-mail');
    $form['author']['mail']['#title_display'] = 'invisible';
    $form['author']['mail']['#description'] = FALSE;
    $form['author']['homepage']['#access'] = FALSE;
    $form['subject']['#access'] = FALSE;
    $form['comment_body']['#attributes']['placeholder'] = t('E-mail');
    $form['comment_body'][LANGUAGE_NONE]['0']['#title_display'] = 'invisible';
    $form['comment_body'][LANGUAGE_NONE]['0']['#attributes']['placeholder'] = t('Comment');
  }

  // Remove format tips if enabled in theme settings.
  if (theme_get_setting('enable_tips_override') == 1) {
    $form['comment_body']['#after_build'][] = 'bootstrapblue_customize_comment_form';
  }
}

/**
 * Function for modifying comment form after build.
 */
function bootstrapblue_customize_comment_form(&$form) {
  // Hide text format tips.
  $form[LANGUAGE_NONE][0]['format']['guidelines']['#access'] = FALSE;
  $form[LANGUAGE_NONE][0]['format']['help']['#access'] = FALSE;
  return $form;
}

/**
 * Implements theme_textarea().
 */
function bootstrapblue_textarea($variables) {
  // Modify textarea to remove resize handle.
  $element = $variables['element'];
  $element['#attributes']['name'] = $element['#name'];
  $element['#attributes']['id'] = $element['#id'];
  $element['#attributes']['cols'] = $element['#cols'];
  $element['#attributes']['rows'] = $element['#rows'];
  _form_set_class($element, array('form-textarea'));
  $wrapper_attributes = array(
    'class' => array('form-textarea-wrapper'),
  );
  if (!empty($element['#resizable'])) {
    if (theme_get_setting('disable_drupal_resize') == 0) {
      drupal_add_library('system', 'drupal.textarea');
    }
    $wrapper_attributes['class'][] = 'resizable';
  }
  $output = '<div' . drupal_attributes($wrapper_attributes) . '>';
  $output .= '<textarea' . drupal_attributes($element['#attributes']) . '>' . check_plain($element['#value']) . '</textarea>';
  $output .= '</div>';
  return $output;
}

/**
 * Implements hook_preprocess_node().
 */
function bootstrapblue_preprocess_node(&$vars, $hook) {
  // Modify submitted by information.
  if (theme_get_setting('submittedby') == 1) {
    $date = theme_get_setting('submittedby_custom_date');
    if ($date == '') {
      $date = "M jS, Y";
    }
    $vars['submitted'] = t('By !username on !date', array(
      '!username' => $vars['name'],
      '!date' => date($date, $vars['created']),
    ));
  }
}

/**
 * Implements hook_node_view_alter().
 */
function bootstrapblue_node_view_alter(&$build) {
  // Remove "add comment" link from node teaser mode display.
  unset($build['links']['comment']['#links']['comment-add']);

  // And if logged out this will cause another list item to appear.
  unset($build['links']['comment']['#links']['comment_forbidden']);
}

/**
 * Implements hook_field__taxonomy_term_reference().
 */
function bootstrapblue_field__taxonomy_term_reference($variables) {
  $output = '';
  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<span class="field-label">' . $variables['label'] . ': </span>';
  }

  // Render the items.
  $output .= ($variables['element']['#label_display'] == 'inline') ? '<div class="links inline">' : '<div class="links">';
  $align = '';
  if ($variables['element']['#field_name'] == 'field_tags') {
    $align = 'pull-left ';
    foreach ($variables['items'] as $delta => $item) {
      $output .= '<span class="taxonomy-term-reference-' . $delta . ' label label-info tag"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</span>';
    }
  }
  elseif ($variables['element']['#field_name'] == 'field_category') {
    foreach ($variables['items'] as $delta => $item) {
      $output .= '<span class="taxonomy-term-reference-' . $delta . ' label label-info"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</span>';
    }
  }
  else {
    foreach ($variables['items'] as $delta => $item) {
      $output .= '<span class="taxonomy-term-reference-' . $delta . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</span>';
    }
  }
  $output .= '</div>';

  // Render the top-level DIV.
  $output = '<div class="' . $align . $variables['classes'] . (!in_array('clearfix', $variables['classes_array']) ? ' clearfix' : '') . '"' . $variables['attributes'] . '>' . $output . '</div>';

  return $output;
}
/**
 * Preprocess variables for region.tpl.php.
 *
 * @see region.tpl.php
 */
function bootstrapblue_preprocess_region(&$variables, $hook) {
  // Remove "well" class added in bootstrap base theme.
  if ($variables['region'] == 'sidebar_first') {
    if (($key = array_search('well', $variables['classes_array'])) !== FALSE) {
      unset($variables['classes_array'][$key]);
    }
  }
}
