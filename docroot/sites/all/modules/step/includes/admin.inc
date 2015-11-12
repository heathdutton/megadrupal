<?php

/**
 * @file
 * Multi-Step Registration administrative functionality.
 */

/**
 * Reorder registration steps based on weight.
 *
 * @param array $steps
 *   The list of registration steps to be saved.
 */
function step_order(array &$steps) {
  // Save the 'register' step to be always prepended.
  $register = $steps[STEP_REGISTER];
  unset($steps[STEP_REGISTER]);

  // Order remaining steps by weight.
  uasort($steps, 'drupal_sort_weight');

  $steps = array_keys($steps);

  // Rebuild the whole list with 'register' first.
  array_unshift($steps, STEP_REGISTER);
}

/**
 * Form API builder function for list of registration steps.
 */
function step_list_form($form, $form_state) {
  $form['step_steps'] = array(
    '#tree' => TRUE,
    '#weight' => 0,
  );

  $weight = 0;
  foreach (variable_get('step_steps', step_default()) as $delta => $step) {
    $weight = max($delta, $weight);

    $form['step_steps'][$step] = array(
      'title' => array(
        '#markup' => check_plain(variable_get("step_title_$step", step_default_title($step))),
      ),
      'id' => array(
        '#markup' => check_plain($step),
      ),
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $delta,
        '#delta' => 10,
        '#title_display' => 'invisible',
        '#attributes' => array(
          'class' => array('step-weight'),
        ),
      ),
    );

    $actions = array();
    $actions[] = array(
      'title' => t('Edit'),
      'href' => 'admin/config/people/step/' . $step . '/edit',
    );

    if ($step != STEP_REGISTER) {
      $actions[] = array(
        'title' => t('Delete'),
        'href' => 'admin/config/people/step/' . $step . '/delete',
      );
    }

    $form['step_steps'][$step]['actions']['#markup'] = theme(
      'links__ctools_dropbutton',
      array(
        'links' => $actions,
        'attributes' => array('class' => array('links', 'inline')),
      )
    );
  }
  $weight++;

  // New step.
  $form['step_steps']['__new'] = array(
    '#tree' => FALSE,
  );
  $form['step_steps']['__new']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Register step title'),
    '#title_display' => 'invisible',
    '#maxlength' => 255,
    '#size' => 20,
  );
  $form['step_steps']['__new']['id'] = array(
    '#type' => 'machine_name',
    '#title' => t('Step machine-name'),
    '#title_display' => 'invisible',
    '#maxlength' => 64,
    '#machine_name' => array(
      'exists' => 'step_exists',
      'source' => array('step_steps', '__new', 'title'),
    ),
    '#required' => FALSE,
  );
  $form['step_steps']['__new']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight for new step'),
    '#title_display' => 'invisible',
    '#default_value' => $weight,
  );
  $form['step_steps']['__new']['actions'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#validate' => array('step_list_form_add_step_validate'),
    '#submit' => array('step_list_form_submit', 'step_list_form_add_step_submit'),
  );

  $form['step_show_trail'] = array(
    '#type' => 'checkbox',
    '#title' => t('Wizard trail'),
    '#description' => t('Displays the wizard trail. The trail can be customized by theming <code>ctools_wizard_trail__step</code>.'),
    '#default_value' => variable_get('step_show_trail', TRUE),
    '#weight' => 10,
  );

  $form['step_show_back'] = array(
    '#type' => 'checkbox',
    '#title' => t('Back button'),
    '#description' => t('Adds a "Back" button.'),
    '#default_value' => variable_get('step_show_back', FALSE),
    '#weight' => 20,
  );

  $form['step_return_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect on Finish'),
    '#description' => t('The system path where the user will be redirected after finishing the wizard. Defaults to frontpage <code><front></code>.'),
    '#default_value' => variable_get('step_return_path', '<front>'),
    '#weight' => 30,
  );

  $form['step_show_description'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the registration description on top of each step form.'),
    '#description' => t('The output can be themed by implementing <code>step_description</code> theme.'),
    '#default_value' => variable_get('step_show_description', FALSE),
    '#weight' => 40,
  );

  if (STEP_PROFILE2) {
    $form['step_profile2_type_fieldset'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show each Profile2 type form in a fieldset.'),
      '#description' => t('By default, each profile form is enclosed into a "fieldset" container.'),
      '#default_value' => variable_get('step_profile2_type_fieldset', TRUE),
      '#weight' => 50,
    );
  }

  $form['buttons'] = array(
    '#type' => 'fieldset',
    '#title' => t('Button labels'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 60,
  );

  $form['buttons']['step_button_next'] = array(
    '#type' => 'textfield',
    '#title' =>  t('Next button'),
    '#default_value' => variable_get('step_button_next', 'Continue'),
  );

  $form['buttons']['step_button_back'] = array(
    '#type' => 'textfield',
    '#title' =>  t('Back button'),
    '#default_value' => variable_get('step_button_back', 'Back'),
  );

  $form['buttons']['step_button_finish'] = array(
    '#type' => 'textfield',
    '#title' =>  t('Finish button'),
    '#default_value' => variable_get('step_button_finish', 'Finish'),
  );

  $form['#theme'] = 'step_list_form';
  $form['#submit'][] = 'step_list_form_submit';

  return system_settings_form($form);
}

/**
 * Form API submit function for list of registration steps.
 */
function step_list_form_submit($form, &$form_state) {
  step_order($form_state['values']['step_steps']);
}

/**
 * Form API validation callback for 'Add' submit button.
 */
function step_list_form_add_step_validate($form, &$form_state) {
  if (empty($form_state['values']['id'])) {
    form_error($form['step_steps']['__new']['id'], t('You need to supply a machine name fo this new step.'));
  }
}

/**
 * Form API submit callback for 'Add' submit button.
 */
function step_list_form_add_step_submit($form, &$form_state) {
  $values =& $form_state['values'];

  $steps = variable_get('step_steps', step_default());
  $steps[] = $values['id'];
  variable_set('step_steps', $steps);
  variable_set('step_title_' .  $values['id'], $values['title']);

  drupal_set_message(t("New step '@title' (@id) has been created.", array('@title' => $values['title'], '@id' => $values['id'])));
  $form_state['redirect'] = 'admin/config/people/step/' . $values['id'] . '/edit';
}

/**
 * Form API form builder. Step form.
 */
function step_form($form, &$form_state, $step) {
  $title = variable_get("step_title_$step", step_default_title($step));
  drupal_set_title(t('Edit @title', array('@title' => $title)));

  $form['id'] = array(
    '#type' => 'item',
    '#title' => t('Step ID'),
    '#markup' => $step,
  );

  $form["step_title_$step"] = array(
    '#type' => 'textfield',
    '#title' => t('Register step title'),
    '#description' => t('Human-readable title of register step'),
    '#default_value' => variable_get("step_title_$step", step_default_title($step)),
    '#maxlength' => 255,
    '#required' => TRUE,
  );
  $description = variable_get("step_description_$step", step_empty_description());
  $form["step_description_$step"] = array(
    '#type' => 'text_format',
    '#title' => t('Register step description'),
    '#description' => t('Full description of this register step'),
    '#default_value' => $description['value'],
    '#format' => $description['format'],
  );

  // Add own submit callback.
  $form['#submit'][] = 'step_form_submit';

  return system_settings_form($form);
}

/**
 * Form API submit callback.
 */
function step_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/people/step';
}

/**
 * Checker for '#machine_name' on step machine name field.
 */
function step_exists($name) {
  // Do not allow 'register' as step machine name.
  if ($name == STEP_REGISTER) {
    return TRUE;
  }
  // Do not allow the reserved '__new' ID.
  if ($name == '__new') {
    return TRUE;
  }

  return step_load($name);
}

/**
 * Form API form builder. Deletes step confirmation form.
 */
function step_delete_form($form, &$form_state, $step) {
  $title = variable_get("step_title_$step", step_default_title($step));

  $form['id'] = array(
    '#type' => 'value',
    '#value' => $step,
  );
  $form['title'] = array(
    '#type' => 'value',
    '#value' => $title,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete the registration step "@title" (@id)?', array('@title' => $title, '@id' => $step)),
    'admin/config/people/step'
  );
}

/**
 * Form API form submit. Deletes a step.
 */
function step_delete_form_submit($form, &$form_state) {
  $step = $form_state['values']['id'];
  if ($step != STEP_REGISTER) {
    $steps = variable_get('step_steps', step_default());
    $delta = array_search($step, $steps);
    if ($delta !== FALSE) {
      unset($steps[$delta]);
      variable_set('step_steps', $steps);
    }
    variable_del('step_title_' . $step);
    variable_del('step_description_' . $step);
    drupal_set_message(t('Registration step "@title" (@id) was deleted.', array('@title' => $form_state['values']['title'], '@id' => $step)));
  }
  $form_state['redirect'] = 'admin/config/people/step';
}
