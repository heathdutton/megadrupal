<?php

/**
 * @file
 * Theme functions for lingwo_pron
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_lingwo_pron_form($variables) {
  $form = $variables['form'];
  drupal_add_tabledrag('lingwo-pron-form-table', 'order', 'sibling', 'pron-weight');

  $elements = element_children($form);
  if (empty($elements)) {
    return '';
  }

  $header = array(
    '',
    array(
      'data' => t('Pronunciation'),
      'width' => '100%',
    ),
    t('Weight'),
    t('Remove'),
  );

  $rows = array();
  foreach (element_children($form) as $key) {
    if (!is_int($key)) {
      continue;
    }

    $element = &$form[$key];
    $element['weight']['#attributes']['class'][] = 'pron-weight';
    unset($element['weight']['#title']);
    unset($element['remove']['#title']);

    $weight = drupal_render($element['weight']);
    $remove = drupal_render($element['remove']);

    $row = array();
    $row[] = '';
    $row[] = drupal_render($element);
    $row[] = $weight;
    $row[] = $remove;

    $row = array('data' => $row);
    $row['class'][] = 'draggable';
    $rows[] = $row;
  }

  drupal_add_css(drupal_get_path('module', 'lingwo_pron') . '/lingwo_pron.css');

  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'lingwo-pron-form-table'))) . drupal_render_children($form);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_lingwo_pron_audio_widget($variables) {
  $file = $variables['file'];
  if ($file->filemime == 'audio/mpeg' && module_exists('swftools')) {
    return swf(url($file->filepath));
  }

  drupal_add_js(drupal_get_path('module', 'lingwo_pron') . '/lingwo_pron.player.js');
  return l(t('Listen'), file_create_url($file->uri), array('attributes' => array('target' => '_blank', 'class' => array('lingwo-pron-audio'))));
}

