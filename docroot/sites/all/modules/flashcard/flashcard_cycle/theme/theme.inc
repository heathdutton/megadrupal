<?php

/**
 * @file
 * Preprocess functions for Flashcard Cycle.
 */

/**
 * Theme preprocess function for flashcard.tpl.php.
 */
function template_preprocess_flashcard_cycle(&$variables) {
  $element = $variables['element'];
  $node = $element['#object'];

  $flip = (isset($_GET['flip']));
  if (!isset($_GET['card'])) {
    $_GET['card'] = 1;
  }
  $card = $_GET['card'];

  $variables['items'] = array();
  foreach ($element['#items'] as $delta => $item) {
    if (!empty($element[$delta])) {
      if ($flip) {
        $element[$delta]['#attributes']['class'] = array('flipped', 'flashcard-' . $delta+1);
      }
      if (isset($card) && $card != $delta+1) {
        $element[$delta]['#attributes']['class'][] = 'hidden';
      }
      $variables['items'][$delta] = $element[$delta];
    }
  }

  if ($restart = theme('flashcard_cycle_restart', array('element' => $element))) {
    $variables['restart'] = $restart;
  }

  if ($pager = theme('flashcard_cycle_pager', array('element' => $element))) {
    $variables['pager'] = $pager;
  }

  if ($mark = theme('flashcard_cycle_mark', array('element' => $element))) {
    $variables['mark'] = $mark;
  }

  if ($mode = theme('flashcard_cycle_mode', array('element' => $element))) {
    $variables['mode'] = $mode;
  }

  if (node_access('update', $node)) {
    $variables['edit'] = array(
      '#theme' => 'link',
      '#path' => $_GET['q'] . '/edit',
      '#text' => t('Edit'),
      '#options' => array(
        'html' => FALSE,
        'external' => FALSE,
        'attributes' => array(
          'class' => array('edit'),
        ),
      ),
    );
  }

  $speed = variable_get('flashcard_cycle_speed_setting', 'slow');
  if ($speed == 'custom') {
    $speed = variable_get('flashcard_cycle_custom_speed', '1000');
  }

  $settings = array(
    'speed' => $speed,
    'flip' => variable_get('flashcard_cycle_flip', 'flip'),
    'keyboard' => variable_get('flashcard_cycle_keyboard', 1),
    'button' => variable_get('flashcard_cycle_jquery_ui_theme', 1),
    'fx' => variable_get('flashcard_cycle_effect', 'fade'),
  );

  drupal_add_library('system', 'ui.button');
  drupal_add_js(array('flashcardCycle' => $settings), 'setting');
  drupal_add_js(libraries_get_path('jquery.cycle') . '/jquery.cycle.all.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'flashcard_cycle') . '/js/flashcard_cycle.js', 'file');
  drupal_add_css(drupal_get_path('module', 'flashcard_cycle') . '/css/flashcard_cycle.theme.css', 'file');
}

/**
 * Returns a renderable array for a restart link.
 */
function theme_flashcard_cycle_restart($variables) {
  return array(
    '#theme' => 'link',
    '#path' => '',
    '#text' => t('Restart'),
    '#options' => array(
      'html' => FALSE,
      'external' => TRUE,
      'attributes' => array(
        'class' => array('restart'),
      ),
      'query' => array('card' => 1),
    ),
  );
}

/**
 * Set default text for theme function.
 */
function flashcard_cycle_preprocess_flashcard_cycle_mark(&$variables) {
  $variables['title'] = t('Mark');
}

/**
 * Returns a renderable array for a checkbox.
 */
function theme_flashcard_cycle_mark($variables) {
  return array(
    '#type' => 'checkbox',
    '#title' => $variables['title'],
    '#id' => drupal_html_id('flashcard-cycle-mark'),
    '#attributes' => array(
      'class' => array('mark'),
    ),
  );
}

/**
 * Set default text for theme function.
 */
function flashcard_cycle_preprocess_flashcard_cycle_mode(&$variables) {
  $variables['title'] = t('See');
  $variables['options'] = array(
    'all' => t('All cards'),
    'unmarked' => t('Unmarked cards'),
    'marked' => t('Marked cards'),
  );
}

/**
 * Returns a renderable array for the deck mode.
 */
function theme_flashcard_cycle_mode($variables) {
  $class = 'flashcard-cycle-mode';

  $output = '<label for="' . $class . '">' . $variables['title'] . ":</label>\n";
  foreach ($variables['options'] as $key => $option) {
    $id = $class . '-' . $key;
    $output .= '<input type="radio" id="' . $id . '" name="' . $class . '" value="' . $key . '"';
    if ($key == 'all') {
      $output .= ' checked="checked"';
    }
    $output .= ">\n";
    $output .= '<label class="option" for="' . $id . '">' . $option . "</label>\n";
  }

  return $output;
}

/**
 * Returns a renderable array for a pager.
 */
function theme_flashcard_cycle_pager($variables) {
  $count = count(element_children($variables['element']));
  if (isset($_GET['card'])) {
    $card = $_GET['card'];
    $card_prev = ($card == 1) ? $count : $card-1;
    $card_next = ($card == $count) ? 1 : $card+1;
  }
  else {
    $card_prev = $count;
    $card_next = ($count < 2) ? $count : 2;
  }

  $defaults = array(
    '#theme' => 'link',
    '#path' => '',
  );
  $options = array(
    'html' => FALSE,
    'external' => TRUE,
  );

  $next = $defaults + array(
    '#text' => t('Next ›'),
    '#options' => $options + array(
      'attributes' => array(
        'class' => array('next'),
      ),
      'query' => array('card' => $card_next),
    ),
  );

  $arg = $_GET;
  foreach (array('page', 'q', 'flip') as $key) {
    if (isset($arg[$key])) {
      unset($arg[$key]);
    }
  }
  if (!isset($_GET['flip'])) {
    $arg['flip'] = '1';
  }
  elseif (isset($arg['card']) && variable_get('flashcard_cycle_flip', 'flip') == 'next') {
    $arg['card'] = ($arg['card'] == $count) ? 1 : $arg['card']+1;
  }

  $flip = $defaults + array(
    '#text' => t('Flip'),
    '#options' => $options + array(
      'attributes' => array(
        'class' => array('flip'),
      ),
      'query' => $arg,
    ),
  );

  $prev = $defaults + array(
    '#text' => t('‹ Previous'),
    '#options' => $options + array(
      'attributes' => array(
        'class' => array('prev'),
      ),
      'query' => array('card' => $card_prev),
    ),
  );

  return array($prev, $flip, $next);
}
