<?php

/**
 * Pieces callback form: Block
 */
function pieces_callback_form_block(&$form, &$form_state) {
  $args = $form_state['piece']['callback_arguments'];
  $blocks = _pieces_get_blocks();

  $form['block'] = array(
    '#title' => t('Block'),
    '#type' => 'select',
    '#options' => $blocks,
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'pieces_form_ajax_callback',
      'wrapper' => 'pieces-callback-wrapper',
    ),
    '#default_value' => !empty($args['module']) ? $args['module'] . ':' . $args['delta'] : NULL
  );

  if (!empty($form_state['values']['block']) && $form_state['triggering_element']['#name'] == 'block') {
    list($module, $delta) = explode(':', $form_state['values']['block']);

    $form_state['values']['callback_arguments'] = array(
      'module' => $module,
      'delta' => $delta,
    );
  }
}

/**
 * Helper function: returns a list of blocks.
 *
 * @return array
 */
function _pieces_get_blocks() {
  $blocks_list = &drupal_static(__FUNCTION__, array());

  if (empty($blocks_list)) {
    $blocks = _block_rehash();
    $blocks_list = array();

    foreach ($blocks as $block) {
      $key = $block['module'] . ':' . $block['delta'];
      $blocks_list[$key] = $block['info'] . ' (' . $block['module'] . ':' . $block['delta'] . ')';
    }
  }
  return $blocks_list;
}

/**
 * Pieces callback form: Entity
 */
function pieces_callback_form_entity(&$form, &$form_state) {
  $args = $form_state['piece']['callback_arguments'];

  $entity_types = array();
  foreach (entity_get_info() as $type => $info) {
    $entity_types[$type] = $info['label'];
  }

  $form['entity_type'] = array(
    '#title' => t('Entity type'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => $entity_types,
    '#ajax' => array(
      'callback' => 'pieces_form_ajax_callback',
      'wrapper' => 'pieces-callback-wrapper',
    ),
  );

  $form['entity_id'] = array(
    '#title' => t('Entity'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'pieces_form_ajax_callback',
      'wrapper' => 'pieces-callback-wrapper',
    ),
    '#default_value' => !empty($args['entity_id']) ? $args['entity_id'] : NULL,
    '#description' => t('Please enter the entity ID of the entity to be displayed.'),
  );

  if (!empty($form_state['values']['entity_type'])) {
    $entity_info = entity_get_info($form_state['values']['entity_type']);
    $view_modes = array();
    foreach ($entity_info['view modes'] as $view_mode_name => $view_mode) {
      $view_modes[$view_mode_name] = $view_mode['label'];
    }

    $form['entity_view_mode'] = array(
      '#type' => 'select',
      '#title' => t('View mode'),
      '#options' => $view_modes,
      '#ajax' => array(
        'callback' => 'pieces_form_ajax_callback',
        'wrapper' => 'pieces-callback-wrapper',
      ),
      '#default_value' => !empty($args['view_mode']) ? $args['view_mode'] : NULL,
    );
  }

  if (!empty($form_state['values']['entity_id']) && in_array($form_state['triggering_element']['#name'], array(
      'entity_type',
      'entity_id',
      'entity_view_mode',
    ))
  ) {
    $form_state['values']['callback_arguments'] = array(
      'entity_type' => !empty($form_state['values']['entity_type']) ? $form_state['values']['entity_type'] : NULL,
      'entity_id' => !empty($form_state['values']['entity_id']) ? $form_state['values']['entity_id'] : NULL,
      'view_mode' => !empty($form_state['values']['entity_view_mode']) ? $form_state['values']['entity_view_mode'] : NULL,
    );
  }
}

/**
 * Pieces callback form: Views
 */
function pieces_callback_form_views(&$form, &$form_state) {
  $args = $form_state['piece']['callback_arguments'];
  $views = _pieces_get_views();

  $form['views'] = array(
    '#title' => t('Views'),
    '#type' => 'select',
    '#options' => $views,
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'pieces_form_ajax_callback',
      'wrapper' => 'pieces-callback-wrapper',
    ),
    '#default_value' => !empty($args['name']) ? $args['name'] . ':' . $args['display'] : NULL,
  );


  if (!empty($form_state['values']['views']) && $form_state['triggering_element']['#name'] == 'views') {
    list($name, $display) = explode(':', $form_state['values']['views']);

    $form_state['values']['callback_arguments'] = array(
      'name' => $name,
      'display' => $display,
    );
  }
}

/**
 * Helper function: returns a list of views.
 *
 * @return array
 */
function _pieces_get_views() {
  $views_list = &drupal_static(__FUNCTION__, array());

  if (empty($views_list)) {
    $views = views_get_all_views();
    $views_list = array();

    foreach ($views as $view_name => $view) {
      foreach ($view->display as $display) {
        $views_list[$view_name][$view_name . ':' . $display->id] = $view->human_name . ' (' . $display->display_title . ')';
      }
    }
  }

  ksort($views_list);

  return $views_list;
}

/**
 * Pieces callback form: Function
 */
function pieces_callback_form_function(&$form, &$form_state) {
  // leave empty.
}
