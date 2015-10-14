<?php

/**
 * @file
 * Hansel test page
 */

/**
 * Menu callback to generate the Hansel test form.
 *
 * @param array $form_state
 * @return array
 */
function hansel_ui_test_form($form, $form_state) {
  $form = array();

  $path = empty($form_state['values']['path']) ? base64_decode(arg(5)) : $form_state['values']['path'];

  $form['input'] = array(
    '#type' => 'fieldset',
    '#title' => t('Test input'),
  );

  $form['input']['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#default_value' => $path,
  );

  $form['input']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Test'),
    '#ajax' => array(
      'callback' => 'hansel_ui_test_form_ajax',
      'wrapper' => 'hansel-output',
      'effect' => 'fade',
    ),
  );

  $form['output'] = array(
    '#prefix' => '<div id="hansel-output">',
    '#suffix' => '</div>',
  );

  if (!isset($form_state['values']) && arg(5) == '') {
    $form['output']['info'] = array(
      // It is important to have any content in the wrapper, otherwise the
      // wrapper will not be displayed resulting in ahah malfunction
      '#value' => ' '
    );
  }
  else {
    _hansel_set_test_path($path);
    $breadcrumbs = hansel_get_breadcrumbs(TRUE);
    global $_hansel_test_messages;
    $breadcrumbs = is_array($breadcrumbs['breadcrumb']) ? implode(' &raquo; ', $breadcrumbs['breadcrumb']) : t('Restore old breadcrumbs');
    $form['output']['breadcrumbs'] = array(
      '#type' => 'fieldset',
      '#title' => t('Breadcrumbs'),
    );
    $form['output']['breadcrumbs']['output'] = array(
      '#markup' => $breadcrumbs
    );
    $form['output']['trace'] = array(
      '#type' => 'fieldset',
      '#title' => t('Trace'),
    );
    $form['output']['trace']['messages'] = array(
      '#markup' => '<ul><li>' . implode('</li><li>', $_hansel_test_messages) . '</li></ul>'
    );
  }

  return $form;
}

/**
 * Ajax form handler
 */
function hansel_ui_test_form_ajax($form, &$form_state) {
  return $form['output'];
}

/**
 * Form submit handler.
 */
function hansel_ui_test_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/build/hansel/test/' . base64_encode($form_state['values']['path']);
  $form_state['rebuild'] = TRUE;
}