<?php

/**
 * Admin form for the redirects.
 */
function doccheck_admin_redirect_form($form, &$form_state) {
  $form['#prefix'] = '<div id="doccheck-redirect-form-wrapper">';
  $form['#suffix'] = '</div>';

  $form['list'] = array(
    '#type' => 'container',
    '#theme_wrappers' => array(
      'doccheck_form_table',
    ),
  );

  $form['list']['header'] = array(
    '#type' => 'container',
    '#theme_wrappers' => array(
      'doccheck_form_tr',
    ),
  );

  $form['list']['header'][] = array(
    '#markup' => t('Pattern'),
    '#theme_wrappers' => array(
      'doccheck_form_th',
    ),
  );

  $form['list']['header'][] = array(
    '#markup' => t('Redirect'),
    '#theme_wrappers' => array(
      'doccheck_form_th',
    ),
  );

  $form['list']['header'][] = array(
    '#markup' => '&nbsp;',
    '#theme_wrappers' => array(
      'doccheck_form_th',
    ),
  );

  if (!isset($form_state['redirects'])) {
    $form_state['redirects'] = doccheck_get_redirects();
  }

  foreach ($form_state['redirects'] as $pattern => $redirect) {
    $form['list'][$pattern] = array(
      '#type' => 'container',
      '#theme_wrappers' => array(
        'doccheck_form_tr',
      ),
      'pattern' => array(
        '#markup' => check_plain($pattern),
        '#theme_wrappers' => array(
          'doccheck_form_td',
        ),
      ),
      'redirect' => array(
        '#markup' => check_plain($redirect),
        '#theme_wrappers' => array(
          'doccheck_form_td',
        ),
      ),
      'remove' => array(
        '#type' => 'submit',
        '#value' => t('Remove'),
        '#name' => drupal_html_class("{$pattern}_{$redirect}_remove"),
        '#pattern' => $pattern,
        '#theme_wrappers' => array(
          'button',
          'doccheck_form_td',
        ),
        '#submit' => array(
          '_doccheck_redirect_remove',
        ),
        '#ajax' => array(
          'callback' => '_doccheck_mapping_ajax',
          'wrapper' => 'doccheck-redirect-form-wrapper',
        ),
      ),
    );
  }

  $form['addwrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add'),
  );

  $form['addwrapper']['pattern'] = array(
    '#title' => t('Pattern'),
    '#type' => 'textfield',
  );

  $form['addwrapper']['redirect'] = array(
    '#title' => t('Redirect'),
    '#type' => 'textfield',
  );

  $form['addwrapper']['add'] = array(
    '#type' => 'submit',
    '#submit' => array('_doccheck_redirect_add'),
    '#value' => t('Add'),
    '#ajax' => array(
      'callback' => '_doccheck_mapping_ajax',
      'wrapper' => 'doccheck-redirect-form-wrapper',
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Saves redirects.
 */
function doccheck_admin_redirect_form_submit($form, &$form_state) {
  variable_set('doccheck_redirects', $form_state['redirects']);
}

/**
 * Removes a redirect.
 */
function _doccheck_redirect_remove($form, &$form_state) {
  $form_state['rebuild'] = TRUE;

  $pattern = $form_state['triggering_element']['#pattern'];
  unset($form_state['redirects'][$pattern]);
}

/**
 * Adds a redirects.
 */
function _doccheck_redirect_add($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $pattern = $form_state['values']['pattern'];
  $redirect = $form_state['values']['redirect'];
  $form_state['redirects'][$pattern] = $redirect;
}
