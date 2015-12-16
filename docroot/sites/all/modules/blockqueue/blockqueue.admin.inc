<?php

/**
 * @file
 * Block Queue Admin UI File.
 */

/**
 * Define the preset add/edit form.
 */
function blockqueue_export_ui_form(&$form, &$form_state) {

  module_load_include('inc', 'views_ui', 'includes/admin');

  $form['info']['#type'] = 'fieldset';

  $blockqueue = $form_state['item'];
  $op         = $form_state['op'];
  $form_type  = $form_state['form type'];

  // Add Blockqueue's configuration interface.
  $form['info']['size'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Queue size'),
    '#description'   => t('The maximum number of blocks will appear in the queue. Enter 0 for no limit.'),
    '#default_value' => (int) $blockqueue->size,
    '#required'      => TRUE,
    '#maxlength'     => 3,
    '#size'          => 3,
  );

  if ($op == 'edit' || $form_type == 'import') {
    // Avoid form field to be flattened.
    $form['blocks'] = array(
      '#tree'   => TRUE,
      '#theme'  => array('blockqueue_export_ui_form_table'),
      '#prefix' => '<div class="form-item">',
      '#suffix' => '<div class="description">' . t('Drag items up or down in the list to reorder.') . '</div></div>',
    );

    // Load blocks.
    $blocks = _block_rehash();
    // Sort blocks by label.
    uasort($blocks, 'blockqueue_sort_info');
    // Index block list by token.
    foreach ($blocks as $block) {
      $blocks[$block['module'] . '|' . $block['delta']] = $block;
    }

    $weight = 0;

    foreach ($blockqueue->blocks as $token) {
      $block = $blocks[$token];

      $row = array(
        '#tree'  => TRUE,
        'name'   => array('#markup' => $block['info']),
        'module' => array('#markup' => $block['module']),
        'delta'  => array('#markup' => $block['delta']),
        'remove' => array(
          '#markup' => l(
            t('Remove from queue'),
            "admin/structure/blockqueue/list/{$blockqueue->name}/remove/{$token}"
          ),
        ),
        'weight' => array(
          '#type'          => 'weight',
          '#title'         => t('Weight'),
          '#default_value' => $weight,
          '#delta'         => 10,
          '#title_display' => 'invisible',
        ),
      );

      $form['blocks'][$token] = $row;

      $weight++;
    }

    $modules = array();
    foreach (system_list('module_enabled') as $name => $module) {
      $modules[$name] = $module->info['name'];
    }

    if ($op == 'edit') {
      $options = array('' => t('- None -'));
      foreach ($blocks as $block) {
        if ($block['module'] != 'blockqueue') {
          $token = $block['module'] . '|' . $block['delta'];
          if (!in_array($token, $blockqueue->blocks)) {
            $options[$modules[$block['module']]][$token] = $block['info'];
          }
        }
      }

      // Sort by module name.
      ksort($options);

      $form['new'] = array(
        '#type'  => 'fieldset',
        '#title' => t('New block'),
      );

      $form['new']['new_block'] = array(
        '#type'        => 'select',
        '#description' => t(
          'Select a block in the list and click on %button button.',
          array('%button' => t('Add block'))
        ),
        '#options'     => $options,
      );

      $form['add'] = array(
        '#type'  => 'submit',
        '#value' => t('Add block'),
      );
    }
  }
}

/**
 * Implements hook_form_FORM_validate().
 */
function blockqueue_export_ui_form_validate($form, &$form_state) {
  $blockqueue = &$form_state['item'];

  // Add new block.
  if (isset($form_state['values']['add']) && $form_state['values']['op'] == $form_state['values']['add']) {
    if (empty($form_state['values']['new_block'])) {
      form_error($form['new']['new_block'], t('You must specify a block to add.'));
    }
    elseif ($form_state['values']['size'] <= count($blockqueue->blocks) && !empty($form_state['values']['size'])) {
      form_error($form['new']['new_block'], t('Too many elements in the queue. Increase queue size to add new block.'));
    }
  }

  // Create or Update Blockqueue.
  if ($form_state['values']['size'] != abs((int) $form_state['values']['size'])) {
    form_error($form['info']['size'], t('Invalid queue size value.'));
  }
  elseif ($form_state['values']['size'] < count($blockqueue->blocks) && !empty($form_state['values']['size'])) {
    form_error($form['info']['size'], t('Too many elements in the queue. Removes blocks or increase queue size.'));
  }

  $errors = form_get_errors();

  // Sort blocks according to weight.
  if (empty($errors) && isset($form_state['values']['blocks'])) {
    // Blocks with weight value.
    $blocks_sorted = $form_state['values']['blocks'];
    // Blocks sorted.
    uasort($blocks_sorted, 'drupal_sort_weight');
    $form_state['values']['blocks'] = array_keys($blocks_sorted);
  }
}

/**
 * Implements hook_form_FORM_submit().
 */
function blockqueue_export_ui_form_submit(&$form, $form_state) {
  $blockqueue = &$form_state['item'];

  $blockqueue->admin_title = $form_state['values']['admin_title'];
  $blockqueue->size        = (int) $form_state['values']['size'];
  $blockqueue->blocks      = isset($form_state['values']['blocks']) ? $form_state['values']['blocks'] : array();

  // Add new block to the list.
  if (isset($form_state['values']['add']) && $form_state['values']['op'] == $form_state['values']['add']) {
    $token = $form_state['values']['new_block'];

    $blockqueue->blocks[] = $token;

    if (!blockqueue_save($blockqueue)) {
      drupal_set_message(t('An error occurs while adding a block.'), 'error');
    }

    drupal_goto('admin/structure/blockqueue/list/' . $blockqueue->name . '/edit');
  }
  else {
    $is_new = empty($blockqueue->name);

    $blockqueue->name = $form_state['values']['name'];

    if (!blockqueue_save($blockqueue)) {
      drupal_set_message(t('An error occurs while saving block queue.'), 'error');
    }

    if ($is_new) {
      drupal_goto('admin/structure/blockqueue/list/' . $blockqueue->name . '/edit');
    }
    else {
      drupal_goto('admin/structure/blockqueue');
    }
  }
}

/**
 * Implements hook_FORM_ID_form().
 */
function blockqueue_block_remove_form($form, &$form_state, $blockqueue, $token) {
  list($module, $delta) = explode('|', $token, 2);

  foreach (_block_rehash() as $block) {
    if ($block['module'] == $module && $block['delta'] == $delta) {
      break;
    }
  }

  return confirm_form(
    array(),
    t(
      'Are you sure you want to remove %block block from %blockqueue?',
      array(
        '%block'      => $block['info'],
        '%blockqueue' => $blockqueue->admin_title,
      )
    ),
    'admin/structure/blockqueue/' . $blockqueue->name . '/edit',
    t('This action cannot be undone.'),
    t('Remove'),
    t('Cancel')
  );
}

/**
 * Implements hook_FORM_ID_form_submit().
 */
function blockqueue_block_remove_form_submit(&$form, $form_state) {
  $blockqueue = $form_state['build_info']['args'][0];
  $token      = $form_state['build_info']['args'][1];

  $blockqueue->blocks = array_values(array_diff($blockqueue->blocks, array($token)));

  blockqueue_save($blockqueue);

  drupal_goto('admin/structure/blockqueue/list/' . $blockqueue->name . '/edit');
}
