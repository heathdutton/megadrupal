<?php

/**
 * @file
 * Crud operations for Smart Chart.
 */

/**
 * Show available items for this Chart.
 *
 * @param $js
 *   Nojs or ajax.
 * @param $chart_nid
 *   Chart nid.
 * @param $item_nid
 *   Current item nid.
 */
function smart_chart_item_select_callback($js = NULL, $chart_nid = NULL, $item_nid = NULL) {
  if ($js) {
    // Required includes for ctools to work.
    ctools_include('modal');
    ctools_include('ajax');
    ctools_add_js('ajax-responder');
    ctools_modal_add_js();

    // Load available items.
    $content = views_embed_view('_smart_suite_chart_content', 'page', $chart_nid, $item_nid);

    ctools_modal_render(t('Select item'), $content);
  }
}

/**
 * New Smart Chart item.
 *
 * @param $js
 *   Nojs or ajax.
 * @param $chart_nid
 *   Chart nid.
 * @param $parent_nid
 *   Current item nid(parent).
 */
function smart_chart_item_new_callback($js = NULL, $chart_nid = NULL, $parent_nid = NULL) {
  if ($js) {
    // Required includes for ctools to work.
    ctools_include('modal');
    ctools_include('ajax');
    ctools_add_js('ajax-responder');
    ctools_modal_add_js();

    $form = drupal_get_form('smart_chart_item_new_form');
    // Add values.
    $form['container_new']['chart_nid']['#value'] = $chart_nid;
    $form['container_new']['parent_nid']['#value'] = $parent_nid;
    $content = drupal_render($form);

    ctools_modal_render(t('Create item'), $content);
  }
}

/**
 * New item form.
 */
function smart_chart_item_new_form($form, &$form_state) {

  // Get content types that don't and do have a connection
  // entity reference field.
  module_load_include('inc', 'smart_chart', 'includes/smart_chart.functions');
  $types_selected = sc_get_chart_item_types();

  // Create form for new item.
  $form = array();
  $form['#attributes']['id'] = 'chart_item_form_new';
  $form['#attributes']['class'][] = 'chart-item-form-new';
  // Container.
  $form['container_new'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('clearfix', 'container-new'),
    ),
  );
  // Textbox.
  $form['container_new']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  // Select content type.
  if (count($types_selected) > 1) {
    $form['container_new']['type'] = array(
      '#type' => 'select',
      '#title' => t('Type'),
      '#required' => TRUE,
      '#options' => $types_selected,
    );
  }
  else {
    $form['container_new']['type'] = array(
      '#type' => 'hidden',
      '#required' => TRUE,
      '#value' => key($types_selected),
    );
  }
  // Chart nid.
  $form['container_new']['chart_nid'] = array(
    '#type' => 'hidden',
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  // Parent nid.
  $form['container_new']['parent_nid'] = array(
    '#type' => 'hidden',
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  // Submit.
  $form['container_new']['actions'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('actions'),
    ),
  );
  $form['container_new']['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#ajax' => array(
      'callback' => '_smart_chart_item_new_form_submit',
      'wrapper' => 'chart_item_form_new',
    ),
    '#attributes' => array(
      'class' => array('button'),
    ),
  );

  return $form;
}

/**
 * New item form submit.
 */
function _smart_chart_item_new_form_submit($form, $form_state) {

  // Validate the form.
  drupal_validate_form('smart_chart_item_new_form', $form, $form_state);
  // If there are errors, return the form to display the error messages.
  if (form_get_errors()) {
    $form_state['rebuild'] = TRUE;
    return $form;
  }

  // Get values.
  $title = $form_state['values']['title'];
  $type = $form_state['values']['type'];
  $chart_nid = $form_state['values']['chart_nid'];
  $parent_nid = $form_state['values']['parent_nid'];

  // Create a item and a new connection.
  module_load_include('inc', 'smart_chart', 'includes/smart_chart.functions');
  $chart = entity_metadata_wrapper('node', $chart_nid);
  $child = sc_item_new($title, $chart->author->getIdentifier(), $type);
  sc_connection_new($chart_nid, $parent_nid, $child->getIdentifier());

  $close_selector = '.popups-close';
  $chart_selector = '#chart_container_' . $parent_nid . ' .chart-actions .refresh.refresh_' . $parent_nid;
  $method = 'click';
  $result = array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_invoke($chart_selector, $method),
      ajax_command_invoke($close_selector, $method),
    ),
  );

  return $result;
}

/**
 * Confirmation dialog.
 *
 * @param $js
 *   Nojs or ajax.
 * @param $action
 *   Break, delete etc.
 * @param $chart_nid
 *   Main chart nid.
 * @param $item_nid
 *   Current item nid.
 */
function smart_chart_item_confirm($js = NULL, $action = NULL, $chart_nid = NULL, $item_nid = NULL) {
  if ($js) {
    // Required includes for ctools to work.
    ctools_include('modal');
    ctools_include('ajax');
    ctools_modal_add_js();

    // Create form and containers.
    $form = array();
    $form['confirm'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('confirm-dialog'),
      ),
    );
    $form['confirm']['description'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('description'),
      ),
    );
    $form['confirm']['actions'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('actions'),
      ),
    );

    // Text.
    $form['confirm']['description']['text'] = array(
      '#type' => 'item',
      '#markup' => $content = t('This action cannot be undone. Continue?'),
    );

    // Yes button.
    $href = 'chart/' . $action . '/nojs/' . $chart_nid . '/' . $item_nid;
    $yes = ctools_modal_text_button(t('Yes'), $href, t('Yes'), 'ctools-modal-common-modal-style button');
    $form['confirm']['actions']['yes'] = array(
      '#type' => 'item',
      '#markup' => $yes,
    );

    // No button.
    $form['confirm']['actions']['no'] = array(
      '#type' => 'item',
      '#markup' => l(t('No'), 'javascript:void(0)',
        array(
          'fragment' => '',
          'attributes' => array(
            'class' => array(
              'button',
            ),
            'onclick' => 'Drupal.CTools.Modal.dismiss();',
          ),
          'html' => TRUE,
          'external' => TRUE,
        )
      ),
    );

    ctools_modal_render(t('Please confirm'), drupal_render($form));
  }
}

/**
 * Add an item callback.
 *
 * @param $js
 *   Nojs or ajax.
 * @param $chart_nid
 *   Chart nid.
 * @param $parent_nid
 *   parent nid.
 * @param $child_nid
 *   New item nid to add to parent.
 */
function smart_chart_item_add_callback($js = NULL, $chart_nid = NULL, $parent_nid = NULL, $child_nid = NULL) {

  if ($js) {
    // Required includes for ctools to work.
    ctools_include('modal');
    ctools_include('ajax');
    ctools_modal_add_js();
  }

  module_load_include('inc', 'smart_chart', 'includes/smart_chart.functions');
  // Add item to Chart.
  sc_item_add($chart_nid, $parent_nid, $child_nid);

  // Close modal and refresh.
  _smart_chart_close_modal_refresh($parent_nid);
}

/**
 * Break an item callback.
 *
 * @param $js
 *   Nojs or ajax.
 * @param $chart_nid
 *   Chart nid.
 * @param $item_nid
 *   New item nid to add to parent.
 */
function smart_chart_item_break_callback($js = NULL, $chart_nid = NULL, $item_nid = NULL) {

  if ($js) {
    // Required includes for ctools to work.
    ctools_include('modal');
    ctools_include('ajax');
    ctools_modal_add_js();
  }

  // Get parent nid to refresh later.
  module_load_include('inc', 'smart_chart', 'includes/smart_chart.functions');
  $parent_nid = sc_item_parent($item_nid);

  // Break item from a chart.
  sc_item_break($chart_nid, $item_nid);

  // Close modal and refresh.
  if ($parent_nid != FALSE) {
    _smart_chart_close_modal_refresh($parent_nid);
  }
}

/**
 * Delete an item callback.
 *
 * @param $js
 *   Nojs or ajax.
 * @param $chart_nid
 *   Chart nid.
 * @param $item_nid
 *   New item nid to add to parent.
 */
function smart_chart_item_delete_callback($js = NULL, $chart_nid = NULL, $item_nid = NULL) {

  if ($js) {
    // Required includes for ctools to work.
    ctools_include('modal');
    ctools_include('ajax');
    ctools_modal_add_js();
  }

  // Get parent nid to refresh later.
  module_load_include('inc', 'smart_chart', 'includes/smart_chart.functions');
  $parent_nid = sc_item_parent($item_nid);

  // Delete item.
  sc_item_delete($chart_nid, $item_nid);

  // Close modal and refresh.
  if ($parent_nid != FALSE) {
    _smart_chart_close_modal_refresh($parent_nid);
  }
}

/**
 * Close modal and refresh spesific item.
 *
 * @param $item_nid
 *   Item to refresh.
 */
function _smart_chart_close_modal_refresh($item_nid) {
  $commands = array();
  $chart_selector = '#chart_container_' . $item_nid . ' .chart-actions .refresh.refresh_' . $item_nid;
  $close_selector = '.popups-close';
  $method = 'click';
  $commands[] = ajax_command_invoke($chart_selector, $method);
  $commands[] = ajax_command_invoke($close_selector, $method);
  $page = array('#type' => 'ajax', '#commands' => $commands);
  ajax_deliver($page);
}
