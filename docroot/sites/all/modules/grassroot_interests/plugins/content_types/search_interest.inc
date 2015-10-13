<?php
/**
 * @file
 * grassroot_interests
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */

$plugin = array(
  'title' => t('GI - Your Interests'),
  'description' => t('This will grassroot results matching exact keywords.'),
  'render callback' => 'grassroot_interests_content_type_render',
  'edit form' => 'grassroot_interests_content_type_edit_form',
  'icon' => 'icon_example.png',
  'category' => array(t('Search Interests'), -9),
);

/**
 * Run-time rendering of the body of the block.
 */
function grassroot_interests_content_type_render($subtype, $conf, $args, $context) {
  // Including the required file.
  module_load_include('inc', 'grassroot_interests', '/includes/grassroot_interests');
  // The arguments to be fed.
  $block = new stdClass();
  $block->title   = '';
  $dispno = array();
  $disptext = array();
  $dispno = $conf['item1'];
  $disptext = $conf['item2'];
  $keyword = '';
  if (isset($args[0])) {
    $keyword = strip_tags($args[0]);
  }
  $block->content = grassroot_interests_search_interest_process($keyword, $disptext, $dispno);
  return $block;
}

/**
 * Implements hook_form().
 */
function grassroot_interests_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['item1'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of results to display'),
    '#size' => 50,
    // Nine results are to be displayed by default.
    '#description' => t('Number of results to display (by default it is set to Show All)'),
    '#default_value' => !empty($conf['item1']) ? $conf['item1'] : 0,
    '#prefix' => '<div class="clear-block no-float">',
    '#suffix' => '</div>',
  );
  $form['item2'] = array(
    '#type' => 'textfield',
    '#title' => t('display text before url title'),
    '#size' => 50,
    '#description' => t('Something like "You might be interested in - (title)"'),
    '#default_value' => !empty($conf['item2']) ? $conf['item2'] : 'You might be interested in ',
    '#prefix' => '<div class="clear-block no-float">',
    '#suffix' => '</div>',
  );
  return $form;
}

/**
 * Implements hook_form_submit().
 */
function grassroot_interests_content_type_edit_form_submit(&$form, &$form_state) {
  $form_state['conf']['item1'] = $form_state['values']['item1'];
  $form_state['conf']['item2'] = $form_state['values']['item2'];
}
