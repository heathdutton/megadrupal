<?php

/**
 * Ajax menu callback to add a frame to a Panels Frame Stack.
 */
function panels_frame_stack_frame_ajax_delegate($op, $cache_key, $name = NULL) {
  ctools_include('ajax');
  ctools_include('cache');
  ctools_include('modal');

  // Fail early if we don't have the data we need.
  if (empty($op) || empty($cache_key)) {
    ctools_ajax_render_error();
  }

  // Load stored object from cache.
  if (!($item = panels_frame_cache_get('stack', $cache_key))) {
    ctools_ajax_render_error(t('Invalid object name.'));
  }

  $form_state = array(
    'title' => t('!op Frame @name', array('!op' => ucwords($op), '@name' => $name)),
    'cache_key' => $cache_key,
    'item' => $item,
    'frame' => $name,
    'ajax' => TRUE,
    'op' => $op,
  );

  $commands = ctools_modal_form_wrapper("panels_frame_stack_frame_$op", $form_state);

  if ($form_state['executed']) {
    $commands = array();

    // Rebuild stack edit form with new settings.
    $form = drupal_build_form('panels_frame_stack_edit_form', $form_state);
    $commands[] = ajax_command_replace('#panels-frame-stack-edit-form', drupal_render($form));
    $commands[] = ctools_modal_command_dismiss();
  }

  print ajax_render($commands);
  exit;
}

/**
 * Helper for building the frame form.
 */
function panels_frame_stack_frame_form(&$form_state) {
  $data = $form_state['item']->data;
  $name = $form_state['frame'];

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#required' => TRUE,
    '#size' => 40,
    '#default_value' => isset($data[$name]['label']) ? $data[$name]['label'] : '',
  );

  $form['identifier'] = array(
    '#type' => 'machine_name',
    '#title' => t('Identifier'),
    '#size' => 40,
    '#machine_name' => array(
      'exists' => 'panels_frame_frame_identifier_exists',
      'source' => array('label'),
      'label' => 'identifier',
    ),
    '#default_value' => isset($data[$name]['identifier']) ? $data[$name]['identifier'] : '',
  );

  $form['weight'] = array(
    '#type' => 'value',
    '#default_value' => isset($data[$name]['weight']) ? $data[$name]['weight'] : 0,
  );

  $form['layout'] = array(
    '#title' => 'layout',
    '#type' => 'panels_frame_layout',
    '#required' => TRUE,
    '#no_builders' => TRUE,
    '#no_frames' => TRUE,
    '#default_value' => isset($data[$name]['layout']) ? $data[$name]['layout'] : '',
  );

  return $form;
}

/**
 * Form builder for "add"-ing a frame.
 */
function panels_frame_stack_frame_add($form, &$form_state) {
  $form = panels_frame_stack_frame_form($form_state);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#validate' => array('panels_frame_stack_frame_layout_validate'),
    '#submit' => array('panels_frame_stack_frame_common_submit'),
  );

  return $form;
}

/**
 * Form builder for "edit"-ing a frame.
 */
function panels_frame_stack_frame_edit($form, &$form_state) {
  $form = panels_frame_stack_frame_form($form_state);
  $form['identifier']['#disabled'] = TRUE;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#validate' => array('panels_frame_stack_frame_layout_validate'),
    '#submit' => array('panels_frame_stack_frame_common_submit'),
  );

  return $form;
}

/**
 * Form builder for "clone"-ing a frame.
 */
function panels_frame_stack_frame_clone($form, &$form_state) {
  $form = panels_frame_stack_frame_form($form_state);
  $form['label']['#default_value'] .= ' ' . t('copy');
  unset($form['identifier']['#default_value']);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Clone'),
    '#validate' => array('panels_frame_stack_frame_layout_validate'),
    '#submit' => array('panels_frame_stack_frame_common_submit'),
  );

  return $form;
}

/**
 * Form builder for "delete"-ing a frame.
 * @todo create a small, custom JS theme for ctools modal.
 */
function panels_frame_stack_frame_delete($form, &$form_state) {
  $question = t('Are you sure you want to remove the frame @name?', array('@name' => $form_state['frame']));
  $form = confirm_form($form, $question, '');
  $form['description']['#markup'] = $question . ' ' . $form['description']['#markup'];
  unset($form['actions']['cancel']);

  return $form;
}

/**
 * Submission handler for "delete"-ing a frame.
 */
function panels_frame_stack_frame_delete_submit(&$form, &$form_state) {
  $cache_key = $form_state['cache_key'];
  $name = $form_state['frame'];

  $item = $form_state['item'];
  unset($item->data[$name]);
  panels_frame_cache_set('stack', $cache_key, $item);
}

/**
 * Validation handler to prevent nested layouts.
 * @fixme Need a proper helper that builds layout name (ex: panels_frame:stack:name).
 */
function panels_frame_stack_frame_layout_validate(&$form, &$form_state) {
  $name = $form_state['item']->name;
  $frame = $form_state['values']['identifier'];
  $layout = $form_state['values']['layout'];
  if ($layout == 'panels_frame_stack:' . $name) {
    form_set_error('layout', t('You cannot use %layout as a child layout as this will cause an infinite loop!', array('%layout' => $name)));
  }
}

/**
 * Common submission handler for add/edit/clone operations.
 */
function panels_frame_stack_frame_common_submit(&$form, &$form_state) {
  $cache_key = $form_state['cache_key'];
  $item = $form_state['item'];

  foreach (array('label', 'identifier', 'weight', 'layout') as $key) {
    $frame[$key] = $form_state['values'][$key];
  }

  $item->data[$frame['identifier']] = $frame;
  panels_frame_cache_set('stack', $cache_key, $item);
}

/**
 * Build the Stack edit form.
 */
function panels_frame_stack_edit_form($form, &$form_state) {
  ctools_include('plugins', 'panels');
  $item = $form_state['item'];

  // Wrapper to allow for ajax replacement.
  $form['#prefix'] = '<div id="panels-frame-stack-edit-form">';
  $form['#suffix'] = '</div>';

  // Call out the values that will have no UI here. It will be referenced in
  // multiple places.
  $no_ui = array('label', 'identifier', 'layout');
  $cache_key = panels_frame_cache_key($item->name);

  // Built as a table-drag interface later..
  $form['data'] = array(
    '#no_ui' => $no_ui,
    '#element_validate' => array('panels_frame_stack_ui_frames_sort'),
    '#after_build' => array('panels_frame_stack_ui_frames_after_build'),
    '#tree' => TRUE,
  );

  foreach ($item->data as $frame_id => $frame) {
    foreach ($no_ui as $hidden) {
      $form['data'][$frame_id][$hidden] = array(
        '#type' => 'value',
        '#value' => $frame[$hidden],
      );
    }

    // Icon
    $layout = panels_get_layout($frame['layout']);
    $form['data'][$frame_id]['icon'] = array(
      '#markup' => panels_print_layout_icon($layout['name'], $layout),
    );

    // Display Title
    $form['data'][$frame_id]['title']['#markup'] = implode('<br />', array(
      '<strong>' . $frame['label'] . '</strong>',
      '<em>' . $layout['title'] . '</em>',
    ));

    // Weight
    $form['data'][$frame_id]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $frame['weight'],
      '#attributes' => array('class' => array('panels-frame-stack-frame-weight')),
    );

    // Operations
    $operations = array(
      array(
        'title' => t('Edit'),
        'href' => "panels_frame/stack/frame/ajax/edit/$cache_key/$frame_id",
        'attributes' => array('class' => array('use-ajax')),
      ),
      array(
        'title' => t('Clone'),
        'href' => "panels_frame/stack/frame/ajax/clone/$cache_key/$frame_id",
        'attributes' => array('class' => array('use-ajax')),
      ),
      array(
        'title' => t('Delete'),
        'href' => "panels_frame/stack/frame/ajax/delete/$cache_key/$frame_id",
        'attributes' => array('class' => array('use-ajax')),
      ),
    );

    $form['data'][$frame_id]['operations'] = array(
      '#theme' => 'links__ctools_dropbutton',
      '#links' => $operations,
      '#attributes' => array('class' => array('links', 'inline')),
    );
  }

  $form['add'] = array(
    '#type' => 'submit',
    '#attributes' => array('class' => array('ctools-use-modal')),
    '#id' => 'panels-frame-stack-frame-add',
    '#value' => t('Add frame'),
  );

  $form['add-url'] = array(
    '#attributes' => array('class' => array("panels-frame-stack-frame-add-url")),
    '#type' => 'hidden',
    '#value' => url("panels_frame/stack/frame/ajax/add/$cache_key", array('absolute' => TRUE)),
  );

  return $form;
}

/**
 * Element validate callback to sort frames. Doesn't actually validate anything,
 * but is used as an opportunity to sort frames by their "weight".
 */
function panels_frame_stack_ui_frames_sort(&$element, &$form_state) {
  $weight = 0;
  foreach ($form_state['values']['data'] as $delta => $item) {
    if (isset($item['identifier']) && $item['weight'] == 0) {
      $form_state['values']['data'][$delta]['weight'] = $weight++;
    }
  }
  $value = drupal_array_get_nested_value($form_state['values'], $element['#parents']);
  if ($value) {
    uasort($value, 'drupal_sort_weight');
    drupal_array_set_nested_value($form_state['values'], $element['#parents'], $value);
  }
}

/**
 * After build callback for frame data table.
 */
function panels_frame_stack_ui_frames_after_build(&$element, &$form_state) {
  $id = 'panels-frame-stack-ui-table';

  ctools_add_css('panels-frame.ui', 'panels_frame');
  ctools_add_css('panels-frame.ui-stack', 'panels_frame');
  drupal_add_tabledrag($id, 'order', 'sibling', 'panels-frame-stack-frame-weight');

  // Explicitly naming our headers for form elements which DO have a UI.
  $table['header'] = array(
    array('data' => t('Icon'), 'class' => 'icon'),
    array('data' => t('Title'), 'class' => 'title'),
    array('data' => t('Weight'), 'class' => 'weight'),
    array('data' => t('Operations'), 'class' => 'operations'),
  );

  $table['rows'] = array();
  // Loop through each frame.
  foreach (element_children($element) as $frame) {
    $rows = array();
    // Loop through each property.
    foreach (element_children($element[$frame]) as $property) {
      // Filter out the properties that have no UI.
      if (!in_array($property, $element['#no_ui'])) {
        $rows[] = array('data' => render($element[$frame][$property]), 'class' => $property);
      }
    }
    $table['rows'][] = array(
      'class' => array('draggable'),
      'data' => $rows,
    );
  }

  $table['attributes']['id'] = $id;
  $table['attributes']['class'][] = 'panels-frame-ui';
  $table['attributes']['class'][] = $id;
  $table['empty'] = t('No frames to display. Hit the %button_text button to get started.', array('%button_text' => t('Add frame')));

  $element['#children'] = theme('table', $table);
  return $element;
}
