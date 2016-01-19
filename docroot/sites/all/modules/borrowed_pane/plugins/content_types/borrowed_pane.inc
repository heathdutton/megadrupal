<?php

/**
 * @file
 * Borrowed pane CTools content type.
 */

$plugin = array(
  'title' => t('Borrowed pane'),
  'description' => t('Populate pane with result of jQuery selector\'s contents.'),
  'category' => array(t('Miscellaneous')),
  'render callback' => 'borrowed_pane_pane_content_type_render',
  'edit form' => 'borrowed_pane_pane_content_type_edit_form',
  'admin info' => 'borrowed_pane_pane_admin_info',
  'defaults' => array(
    'selector' => '',
    'refresh' => false,
    'events' => false,
    'flag' => '',
   ),
);

/**
 * Supply admin information for the pane in the admin GUI.
 */
function borrowed_pane_pane_admin_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass;
    $block->title = t('jQuery selector: %selector', array('%selector' => $conf['selector']));
    $block->content = theme('item_list', array('items' => array(
      t('Refresh on Ajax: %refresh', array('%refresh' => $conf['refresh'] ? t('Yes') : t('No'))),
      t('Copy data and events from borrowed content: %events', array('%events' => $conf['events'] ? t('Yes') : t('No'))),
      t('Flag closest parent selector: %flag', array('%flag' => $conf['flag'])),
    )));
    return $block;
  }
}

/**
 * Render function.
 */
function borrowed_pane_pane_content_type_render($subtype, $conf, $context = NULL) {
  $block = new stdClass();
  $block->content = array(
    '#type' => 'container',
    '#attributes' => array(
      'data-selector' => array($conf['selector']),
      'data-refresh' => array($conf['refresh']),
      'data-events' => array($conf['events']),
      'data-flag' => array($conf['flag']),
      'class' => array('borrowed-pane-container'),
    ),
  );
  return $block;
}

/**
 * Edit form.
 */
function borrowed_pane_pane_content_type_edit_form($form, &$form_state) {
  $form['selector'] = array(
    '#title' => t('Borrow selector'),
    '#description' => t('The <a href="http://api.jquery.com/jQuery/" target="_blank">jQuery selector</a> of the element to copy the contents of.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#size' => 50,
    '#default_value' => $form_state['conf']['selector'],
  );
  $form['refresh'] = array(
    '#title' => t('Refresh on Ajax'),
    '#description' => t('Refresh the borrowed content from the borrow selector upon any Ajax interaction. Leave disabled if you will not be updating the source content with Ajax for performance.'),
    '#type' => 'checkbox',
    '#default_value' => $form_state['conf']['refresh'],
  );
  $form['events'] = array(
    '#title' => t('Copy data and events from borrowed content'),
    '#description' => t('Leave disabled unless you have Javascript acting on the source content that needs to continue running in the borrowed instance.'),
    '#type' => 'checkbox',
    '#default_value' => $form_state['conf']['events'],
  );
  $form['flag'] = array(
    '#title' => t('Flag closest parent'),
    '#description' => t('The borrow selector will be flagged with a class of <em>borrowed-pane-lender</em>. If you desire to have a parent item flagged instead of the borrow selector itself, you can supply the <a href="http://api.jquery.com/closest/" target="_blank">closest selector</a> to apply the flag to.'),
    '#type' => 'textfield',
    '#size' => 50,
    '#default_value' => $form_state['conf']['flag'],
  );
  return $form;
}

/**
 * Submit handler.
 */
function borrowed_pane_pane_content_type_edit_form_submit($form, &$form_state) {
  $form_state['conf']['selector'] = trim($form_state['values']['selector']);
  $form_state['conf']['refresh'] = (bool) $form_state['values']['refresh'];
  $form_state['conf']['events'] = (bool) $form_state['values']['events'];
  if (strlen(trim($form_state['values']['flag'])) === 0) {
    $form_state['conf']['flag'] = FALSE;
  }
  else {
    $form_state['conf']['flag'] = trim($form_state['values']['flag']);
  }
}

