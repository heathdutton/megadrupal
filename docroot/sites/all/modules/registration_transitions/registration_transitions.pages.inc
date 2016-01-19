<?php

/**
 * Page/form callback for registration/%registration/transition/%registration_transition.
 */
function registration_transitions_confirm_form($form, &$form_state, $location, Registration $registration, $meta) {
  $form_state['location'] = $location;
  $form_state['registration'] = $registration;
  $form_state['transition'] = $meta;

  if ($location == 'page') {
    if (!empty($meta['confirmation_page']['text']['value'])) {
      $text = $meta['confirmation_page']['text'];
      $form['intro'] = array(
        '#markup' => check_markup(t($text['value']), $text['format']),
        '#prefix' => '<div class="registration-transitions-confirmation-page-text">',
        '#suffix' => '</div>',
      );
    }

    $form['actions'] = array('#type' => 'actions');
  }

  $action = t($meta['label']);
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => $location == 'page' ? t('Confirm: @action', array('@action' => $action)) : $action,
    '#submit' => array('registration_transitions_confirm_form_submit'),
    '#transition' => $meta['id'],
  );

  return $form;
}

/**
 * Page/form callback for admin/structure/registration/registration_transitions.
 */
function registration_transitions_form($form, &$form_state) {
  $states = registration_states();
  $states_options = array_reduce($states, function($options, $state) {
    return $options + array($state->name => $state->label);
  }, array());

  $transitions = variable_get('registration_transitions', array());
  $form_state['transitions'] = $transitions;

  $form['transitions'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Transitions (!num)', array('!num' => count($transitions))),
  );
  foreach ($transitions as $id => $meta) {
    list($from, $to) = explode('__', $id);
    $form['transitions'][$id] = array(
      '#type' => 'fieldset',
      '#title' => t('%from to %to', array('%from' => $states_options[$from], '%to' => $states_options[$to])),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['transitions'][$id]['delete'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete transition'),
      '#prefix' => '<div style="float: right">',
      '#suffix' => '</div>',
    );
    $form['transitions'][$id]['label'] = array(
      '#type' => 'textfield',
      '#title' => t('Label'),
      '#default_value' => @$meta['label'] ?: '',
      '#required' => TRUE,
      '#description' => t('Will be interface-translated.'),
    );
    $form['transitions'][$id]['vbo'] = array(
      '#type' => 'checkbox',
      '#title' => t('Expose as VBO operation?'),
      '#default_value' => (bool) @$meta['vbo'],
    );
    $form['transitions'][$id]['display'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show button on view/display?'),
      '#default_value' => (bool) @$meta['display'],
      '#description' => t('Every button will have its own extra field.'),
    );
    $form['transitions'][$id]['form'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show button on form?'),
      '#default_value' => (bool) @$meta['form'],
      '#description' => t('Every button will have its own extra field.'),
    );
    $form['transitions'][$id]['message'] = array(
      '#type' => 'textfield',
      '#title' => t('Confirmation status message'),
      '#default_value' => @$meta['message'] ?: '',
      '#description' => t('Will be interface-translated.'),
    );
    $form['transitions'][$id]['confirm'] = array(
      '#type' => 'checkbox',
      '#title' => t('Confirmation (page) required?'),
      '#default_value' => (bool) @$meta['confirm'],
    );
    $form['transitions'][$id]['confirmation_page'] = array(
      '#type' => 'fieldset',
      '#title' => t('Confirmation page'),
      '#collapsible' => FALSE,
      '#states' => array(
        'visible' => array(
          '#edit-transitions-' . $from . '-' . $to . '-confirm' => array('checked' => TRUE),
        ),
      ),
    );
    $form['transitions'][$id]['confirmation_page']['text'] = array(
      '#type' => 'text_format',
      '#title' => t('Confirmation page text'),
      '#default_value' => @$meta['confirmation_page']['value'],
      '#format' => @$meta['confirmation_page']['format'] ?: 'plain_text',
      '#states' => array(
        'required' => array(
          '#edit-transitions-' . $from . '-' . $to . '-confirm' => array('checked' => TRUE),
        ),
      ),
    );
  }

  $form['new'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('New transition'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['new']['from'] = array(
    '#type' => 'select',
    '#title' => t('From'),
    '#options' => $states_options,
    '#required' => TRUE,
  );
  $form['new']['to'] = array(
    '#type' => 'select',
    '#title' => t('To'),
    '#options' => $states_options,
    '#required' => TRUE,
  );
  $form['new']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add transition'),
    '#validate' => array('registration_transitions_form_validate_new'),
    '#submit' => array(
      'registration_transitions_form_submit_new',
      '_registration_transitions_form_submit_all',
    ),
    '#limit_validation_errors' => array(array('new')),
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array(
      'registration_transitions_form_submit',
      '_registration_transitions_form_submit_all',
    ),
    '#limit_validation_errors' => array(array('transitions')),
  );

  return $form;
}

/**
 *
 */
function registration_transitions_form_submit($form, &$form_state) {
  $values = $form_state['values']['transitions'];

  foreach ($values as $id => &$meta) {
    if (!empty($meta['delete'])) {
      unset($values[$id]);
      continue;
    }

    unset($meta['delete']);

    unset($meta);
  }

  variable_set('registration_transitions', $values);
}

/**
 *
 */
function registration_transitions_form_validate_new($form, &$form_state) {
  if (form_get_errors()) return;

  $values = $form_state['values']['new'];

  if ($values['from'] == $values['to']) {
    return form_error($form['new'], t("<em>From</em> and <em>To</em> states must be different."));
  }

  $id = $values['from'] . '__' . $values['to'];
  if (isset($form_state['transitions'][$id])) {
    return form_error($form['new'], t("This transition already exists."));
  }
}

/**
 *
 */
function registration_transitions_form_submit_new($form, &$form_state) {
  $values = $form_state['values']['new'];

  $id = $values['from'] . '__' . $values['to'];
  $form_state['transitions'][$id] = array();

  variable_set('registration_transitions', $form_state['transitions']);
}

/**
 *
 */
function _registration_transitions_form_submit_all($form, &$form_state) {
  cache_clear_all('field_info:bundle_extra:registration:', 'cache_field', TRUE);
}
