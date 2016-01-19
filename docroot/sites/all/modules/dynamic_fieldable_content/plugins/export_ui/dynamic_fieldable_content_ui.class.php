<?php

class dynamic_fieldable_content_ui extends ctools_export_ui {

  function edit_form(&$form, &$form_state) {
    // Correct for an error that came in because filter format changed.
    if (is_array($form_state['item']->settings['body'])) {
      $form_state['item']->settings['format'] = $form_state['item']->settings['body']['format'];
      $form_state['item']->settings['body'] = $form_state['item']->settings['body']['value'];
    }
    if (is_array($form_state['item']->settings['repeat_body'])) {
      $form_state['item']->settings['repeat_body'] = $form_state['item']->settings['repeat_body']['value'];
    }
    parent::edit_form($form, $form_state);
    $form['category'] = array(
        '#type' => 'textfield',
        '#title' => t('Category'),
        '#description' => t('What category this content should appear in. If left blank the category will be "Miscellaneous".'),
        '#default_value' => $form_state['item']->category,
    );

    $form['title'] = array(
        '#type' => 'textfield',
        '#default_value' => $form_state['item']->settings['title'],
        '#title' => t('Title'),
    );
    module_load_include('inc', 'dynamic_fieldable_content', 'plugins/content_types/dynamic_content_pane');
    $form = dynamic_fieldable_content_settings_add_more($form, $form_state, $form_state['item']->settings);
    $form['body'] = array(
        '#type' => 'text_format',
        '#title' => t('Body'),
        '#default_value' => $form_state['item']->settings['body'],
        '#format' => $form_state['item']->settings['format'],
    );
    
    $form['repeat_body'] = array(
        '#type' => 'text_format',
        '#title' => t('Repeat Code'),
        '#default_value' => $form_state['item']->settings['repeat_body'],
        '#format' => $form_state['item']->settings['format'],
    );

    $form['substitute'] = array(
        '#type' => 'checkbox',
        '#title' => t('Use context keywords'),
        '#description' => t('If checked, context keywords will be substituted in this content.'),
        '#default_value' => !empty($form_state['item']->settings['substitute']),
    );
  }

  function edit_form_submit(&$form, &$form_state) {
    parent::edit_form_submit($form, $form_state);

    // Since items in our settings are not in the schema, we have to do these manually:
    $form_state['item']->settings['title'] = $form_state['values']['title'];
    $form_state['item']->settings['body'] = $form_state['values']['body']['value'];
    $form_state['item']->settings['repeat_body'] = $form_state['values']['repeat_body']['value'];
    $form_state['item']->settings['format'] = $form_state['values']['body']['format'];
    $form_state['item']->settings['substitute'] = $form_state['values']['substitute'];

    $ctr = 0;
    foreach ($form_state['values']['settings_fieldset']['label_fieldset']['name'] as $key => $value) {
      $form_state['item']->settings['settings_fieldset_name_'.$key] = $form_state['values']['settings_fieldset']['label_fieldset']['name'][$key];
      $ctr++;
    }
    foreach ($form_state['values']['settings_fieldset']['value_fieldset']['value'] as $key => $value) {
      $form_state['item']->settings['settings_fieldset_value_'.$key] = $form_state['values']['settings_fieldset']['value_fieldset']['value'][$key];
    }
    $form_state['item']->settings['num_names'] = $ctr;
    
    $ctr = 0;
    foreach ($form_state['values']['js_fieldset']['js_url'] as $key => $value) {
      $form_state['item']->settings['js_fieldset_js_url_'.$key] = $form_state['values']['js_fieldset']['js_url'][$key];
      $ctr++;
    }
    $form_state['item']->settings['num_js'] = $ctr;
    
    $ctr = 0;
    foreach ($form_state['values']['css_fieldset']['css_url'] as $key => $value) {
      $form_state['item']->settings['css_fieldset_css_url_'.$key] = $form_state['values']['css_fieldset']['css_url'][$key];
      $ctr++;
    }
    $form_state['item']->settings['num_css'] = $ctr;

  }

  function list_form(&$form, &$form_state) {
    parent::list_form($form, $form_state);

    $options = array('all' => t('- All -'));
    foreach ($this->items as $item) {
      $options[$item->category] = $item->category;
    }

    $form['top row']['category'] = array(
        '#type' => 'select',
        '#title' => t('Category'),
        '#options' => $options,
        '#default_value' => 'all',
        '#weight' => -10,
    );
  }

  function list_filter($form_state, $item) {
    if ($form_state['values']['category'] != 'all' && $form_state['values']['category'] != $item->category) {
      return TRUE;
    }

    return parent::list_filter($form_state, $item);
  }

  function list_sort_options() {
    return array(
        'disabled' => t('Enabled, title'),
        'title' => t('Title'),
        'name' => t('Name'),
        'category' => t('Category'),
        'storage' => t('Storage'),
    );
  }

  function list_build_row($item, &$form_state, $operations) {
    // Set up sorting
    switch ($form_state['values']['order']) {
      case 'disabled':
        $this->sorts[$item->name] = empty($item->disabled) . $item->admin_title;
        break;
      case 'title':
        $this->sorts[$item->name] = $item->admin_title;
        break;
      case 'name':
        $this->sorts[$item->name] = $item->name;
        break;
      case 'category':
        $this->sorts[$item->name] = $item->category;
        break;
      case 'storage':
        $this->sorts[$item->name] = $item->type . $item->admin_title;
        break;
    }

    $ops = theme('links__ctools_dropbutton', array('links' => $operations, 'attributes' => array('class' => array('links', 'inline'))));

    $this->rows[$item->name] = array(
        'data' => array(
            array('data' => check_plain($item->name), 'class' => array('ctools-export-ui-name')),
            array('data' => check_plain($item->admin_title), 'class' => array('ctools-export-ui-title')),
            array('data' => check_plain($item->category), 'class' => array('ctools-export-ui-category')),
            array('data' => $ops, 'class' => array('ctools-export-ui-operations')),
        ),
        'title' => check_plain($item->admin_description),
        'class' => array(!empty($item->disabled) ? 'ctools-export-ui-disabled' : 'ctools-export-ui-enabled'),
    );
  }

  function list_table_header() {
    return array(
        array('data' => t('Name'), 'class' => array('ctools-export-ui-name')),
        array('data' => t('Title'), 'class' => array('ctools-export-ui-title')),
        array('data' => t('Category'), 'class' => array('ctools-export-ui-category')),
        array('data' => t('Operations'), 'class' => array('ctools-export-ui-operations')),
    );
  }

}

/**
 * @defgroup ajax_degradation_example Example: AJAX Graceful Degradation
 * @ingroup examples
 * @{
 * These examples show AJAX with graceful degradation when Javascript is not
 * available.
 *
 * In each of these the key idea is that the form is rebuilt different ways
 * depending on form input. In order to accomplish that, the formbuilder function
 * is in charge of almost all logic.
 */

/**
 * A form with a dropdown whose options are dependent on a
 * choice made in a previous dropdown.
 *
 * On changing the first dropdown, the options in the second
 * are updated. Gracefully degrades if no javascript.
 *
 * A bit of CSS and javascript is required. The CSS hides the "add more" button
 * if javascript is not enabled. The Javascript snippet is really only used
 * to enable us to present the form in degraded mode without forcing the user
 * to turn off Javascript.  Both of these are loaded by using the
 * #attached FAPI property, so it is a good example of how to use that.
 *
 * The extra argument $no_js_use is here only to allow presentation of this
 * form as if Javascript were not enabled. dynamic_fieldable_content_settings_menu() provides two
 * ways to call this form, one normal ($no_js_use = FALSE) and one simulating
 * Javascript disabled ($no_js_use = TRUE).
 */
function dynamic_fieldable_content_settings_dependent_dropdown_degrades($form, &$form_state, $no_js_use = FALSE) {
  // Get the list of options to populate the first dropdown.
  $options_first = _dynamic_fieldable_content_settings_get_first_dropdown_options();

  // If we have a value for the first dropdown from $form_state['values'] we use
  // this both as the default value for the first dropdown and also as a
  // parameter to pass to the function that retrieves the options for the
  // second dropdown.
  $selected = isset($form_state['values']['dropdown_first']) ? $form_state['values']['dropdown_first'] : key($options_first);

  // Attach the CSS and JS we need to show this with and without javascript.
  // Without javascript we need an extra "Choose" button, and this is
  // hidden when we have javascript enabled.
  $form['#attached']['css'] = array(
      drupal_get_path('module', 'dynamic_fieldable_content') . '/css/dynamic_fieldable_content_settings.css',
  );
  $form['#attached']['js'] = array(
      drupal_get_path('module', 'dynamic_fieldable_content') . '/js/dynamic_fieldable_content_settings.js',
  );

  $form['dropdown_first_fieldset'] = array(
      '#type' => 'fieldset',
  );
  $form['dropdown_first_fieldset']['dropdown_first'] = array(
      '#type' => 'select',
      '#title' => 'Instrument Type',
      '#options' => $options_first,
      '#attributes' => array('class' => array('enabled-for-ajax')),

      // The '#ajax' property allows us to bind a callback to the server whenever this
      // form element changes. See dynamic_fieldable_content_settings_autocheckboxes and
      // dynamic_fieldable_content_settings_dependent_dropdown in dynamic_fieldable_content_settings.module for more details.
      '#ajax' => array(
          'callback' => 'dynamic_fieldable_content_settings_dependent_dropdown_degrades_first_callback',
          'wrapper' => 'dropdown-second-replace',
      ),
  );

  // This simply allows us to demonstrate no-javascript use without
  // actually turning off javascript in the browser. Removing the #ajax
  // element turns off AJAX behaviors on that element and as a result
  // ajax.js doesn't get loaded. This is for demonstration purposes only.
  if ($no_js_use) {
    unset($form['dropdown_first_fieldset']['dropdown_first']['#ajax']);
  }

  // Since we don't know if the user has js or not, we always need to output
  // this element, then hide it with with css if javascript is enabled.
  $form['dropdown_first_fieldset']['continue_to_second'] = array(
      '#type' => 'submit',
      '#value' => t('Choose'),
      '#attributes' => array('class' => array('next-button')),
  );

  $form['dropdown_second_fieldset'] = array(
      '#type' => 'fieldset',
  );
  $form['dropdown_second_fieldset']['dropdown_second'] = array(
      '#type' => 'select',
      '#title' => $options_first[$selected] . ' ' . t('Instruments'),
      '#prefix' => '<div id="dropdown-second-replace">',
      '#suffix' => '</div>',
      '#attributes' => array('class' => array('enabled-for-ajax')),
      // When the form is rebuilt during processing (either AJAX or multistep),
      // the $selected variable will now have the new value and so the options
      // will change.
      '#options' => _dynamic_fieldable_content_settings_get_second_dropdown_options($selected),
  );
  $form['dropdown_second_fieldset']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('OK'),
      // This class allows attached js file to override the disabled attribute,
      // since it's not necessary in ajax-enabled form.
      '#attributes' => array('class' => array('enabled-for-ajax')),
  );

  // Disable dropdown_second if a selection has not been made on dropdown_first.
  if (empty($form_state['values']['dropdown_first'])) {
    $form['dropdown_second_fieldset']['dropdown_second']['#disabled'] = TRUE;
    $form['dropdown_second_fieldset']['dropdown_second']['#description'] = t('You must make your choice on the first dropdown before changing this second one.');
    $form['dropdown_second_fieldset']['submit']['#disabled'] = TRUE;
  }

  return $form;
}

/**
 * Submit function for dynamic_fieldable_content_settings_dependent_dropdown_degrades().
 */
function dynamic_fieldable_content_settings_dependent_dropdown_degrades_submit($form, &$form_state) {

  // Now handle the case of the next, previous, and submit buttons.
  // only submit will result in actual submission, all others rebuild.
  switch ($form_state['triggering_element']['#value']) {
    case t('OK'): // Submit: We're done.
      drupal_set_message(t('Your values have been submitted. dropdown_first=@first, dropdown_second=@second', array('@first' => $form_state['values']['dropdown_first'], '@second' => $form_state['values']['dropdown_second'])));
      return;
  }
  // 'Choose' or anything else will cause rebuild of the form and present
  // it again.
  $form_state['rebuild'] = TRUE;
}

/**
 * Selects just the second dropdown to be returned for re-rendering.
 *
 * @return
 *   Renderable array (the second dropdown).
 */
function dynamic_fieldable_content_settings_dependent_dropdown_degrades_first_callback($form, $form_state) {
  return $form['dropdown_second_fieldset']['dropdown_second'];
}


/**
 * Example of a form with portions dynamically enabled or disabled, but
 * with graceful degradation in the case of no javascript.
 *
 * The idea here is that certain parts of the form don't need to be displayed
 * unless a given option is selected, but then they should be displayed and
 * configured.
 *
 * The third $no_js_use argument is strictly for demonstrating operation
 * without javascript, without making the user/developer turn off javascript.
 */
function dynamic_fieldable_content_settings_dynamic_sections($form, &$form_state, $no_js_use = FALSE) {

  // Attach the CSS and JS we need to show this with and without javascript.
  // Without javascript we need an extra "Choose" button, and this is
  // hidden when we have javascript enabled.
  $form['#attached']['css'] = array(
      drupal_get_path('module', 'dynamic_fieldable_content') . '/css/dynamic_fieldable_content_settings.css',
  );
  $form['#attached']['js'] = array(
      drupal_get_path('module', 'dynamic_fieldable_content') . '/js/dynamic_fieldable_content_settings.js',
  );
  $form['description'] = array(
      '#type' => 'markup',
      '#markup' => '<div>' . t('This example demonstrates a form which dynamically creates various sections based on the configuration in the form.
          It deliberately allows graceful degradation to a non-javascript environment.
          In a non-javascript environment, the "Choose" button next to the select control
          is displayed; in a javascript environment it is hidden by the module CSS.
          <br/><br/>The basic idea here is that the form is built up based on
          the selection in the question_type_select field, and it is built the same
          whether we are in a javascript/AJAX environment or not.
          <br/><br/>
          Try the <a href="!ajax_link">AJAX version</a> and the <a href="!non_ajax_link">simulated-non-AJAX version</a>.
          ', array('!ajax_link' => url('examples/dynamic_fieldable_content_settings/dynamic_sections'), '!non_ajax_link' => url('examples/dynamic_fieldable_content_settings/dynamic_sections_no_js') )) . '</div>',
  );
  $form['question_type_select'] = array(
      '#type' => 'select',
      '#title' => t('Question style'),
      '#options' => drupal_map_assoc(array(t('Choose question style'), t('Multiple Choice'), t('True/False'), t('Fill-in-the-blanks'))),
      '#ajax' => array(
          'wrapper' => 'questions-fieldset-wrapper',
          'callback' => 'dynamic_fieldable_content_settings_dynamic_sections_select_callback',
      ),
  );
  // The CSS for this module hides this next button if JS is enabled.
  $form['question_type_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Choose'),
      '#attributes' => array('class' => array('next-button')),
      '#limit_validation_errors' => array(),  // No need to validate when submitting this.
      '#validate' => array(),
  );

  // This simply allows us to demonstrate no-javascript use without
  // actually turning off javascript in the browser. Removing the #ajax
  // element turns off AJAX behaviors on that element and as a result
  // ajax.js doesn't get loaded.
  if ($no_js_use) {
    // Remove the #ajax from the above, so ajax.js won't be loaded.
    unset($form['question_type_select']['#ajax']);
  }

  // This fieldset just serves as a container for the part of the form
  // that gets rebuilt.
  $form['questions_fieldset'] = array(
      '#type' => 'fieldset',
      // These provide the wrapper referred to in #ajax['wrapper'] above.
      '#prefix' => '<div id="questions-fieldset-wrapper">',
      '#suffix' => '</div>',
  );
  if (!empty($form_state['values']['question_type_select'])) {

    $form['questions_fieldset']['question'] = array(
        '#markup' => t('Who was the first president of the U.S.?'),
    );
    $question_type = $form_state['values']['question_type_select'];

    switch ($question_type) {
      case t('Multiple Choice'):
        $form['questions_fieldset']['question'] = array(
        '#type' => 'radios',
        '#title' => t('Who was the first president of the United States'),
        '#options' => drupal_map_assoc(array(t('George Bush'), t('Adam McGuire'), t('Abraham Lincoln'), t('George Washington'))),
        );
        break;

      case t('True/False'):
        $form['questions_fieldset']['question'] = array(
        '#type' => 'radios',
        '#title' => t('Was George Washington the first president of the United States?'),
        '#options' => array(t('George Washington') => t("True"), 0 => t("False")),
        '#description' => t('Click "True" if you think George Washington was the first president of the United States.'),
        );
        break;

      case t('Fill-in-the-blanks'):
        $form['questions_fieldset']['question'] = array(
        '#type' => 'textfield',
        '#title' => t('Who was the first president of the United States'),
        '#description' => t('Please type the correct answer to the question.'),
        );
        break;
    }

    $form['questions_fieldset']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit your answer'),
    );
  }
  return $form;
}

/**
 * Validation function for dynamic_fieldable_content_settings_dynamic_sections().
 */
function dynamic_fieldable_content_settings_dynamic_sections_validate($form, &$form_state) {
  $answer = $form_state['values']['question'];
  if ($answer !== t('George Washington')) {
    form_set_error('question', t('Wrong answer. Try again. (Hint: The right answer is "George Washington".)'));
  }
}

/**
 * Submit function for dynamic_fieldable_content_settings_dynamic_sections().
 */
function dynamic_fieldable_content_settings_dynamic_sections_submit($form, &$form_state) {
  // This is only executed when a button is pressed, not when the AJAXified
  // select is changed.
  // Now handle the case of the next, previous, and submit buttons.
  // Only submit will result in actual submission, all others rebuild.
  switch ($form_state['triggering_element']['#value']) {
    case t('Submit your answer'): // Submit: We're done.
      $form_state['rebuild'] = FALSE;
      $answer = $form_state['values']['question'];

      // Special handling for the checkbox.
      if ($answer == 1 && $form['questions_fieldset']['question']['#type'] == 'checkbox') {
        $answer = $form['questions_fieldset']['question']['#title'];
      }
      if ($answer === t('George Washington')) {
        drupal_set_message(t('You got the right answer: @answer', array('@answer' => $answer)));
      }
      else {
        drupal_set_message(t('Sorry, your answer (@answer) is wrong', array('@answer' => $answer)));
      }
      $form_state['rebuild'] = FALSE;
      return;
      // Any other form element will cause rebuild of the form and present
      // it again.
    case t('Choose'):
      $form_state['values']['question_type_select'] =
      $form_state['input']['question_type_select'];
      // Fall through.
    default:
      $form_state['rebuild'] = TRUE;
  }
}

/**
 * Callback for the select element.
 *
 * This just selects and returns the questions_fieldset.
 */
function dynamic_fieldable_content_settings_dynamic_sections_select_callback($form, $form_state) {
  return $form['questions_fieldset'];
}


/**
 * This example is a classic wizard, where a different and sequential form
 * is presented on each step of the form.
 *
 * In the AJAX version, the form is replaced for each wizard section. In the
 * multistep version, it causes a new page load.
 *
 * @param $form
 * @param $form_state
 * @param $no_js_use
 *   Used for this demonstration only. If true means that the form should be
 *   built using a simulated no-javascript approach (ajax.js will not be
 *   loaded.)
 */
function dynamic_fieldable_content_settings_wizard($form, &$form_state, $no_js_use = FALSE) {

  // Provide a wrapper around the entire form, since we'll replace the whole
  // thing with each submit.
  $form['#prefix'] = '<div id="wizard-form-wrapper">';
  $form['#suffix'] = '</div>';
  $form['#tree'] = TRUE; // We want to deal with hierarchical form values.

  $form['description'] = array(
      '#markup' => '<div>' . t('This example is a step-by-step wizard. The <a href="!ajax">AJAX version</a> does it without page reloads; the <a href="!multistep">multistep version</a> is the same code but simulates a non-javascript environment, showing it with page reloads.',
          array('!ajax' => url('examples/dynamic_fieldable_content_settings/wizard'), '!multistep' => url('examples/dynamic_fieldable_content_settings/wizard_no_js')))
      . '</div>',
  );

  // $form_state['storage'] has no specific drupal meaning, but it is
  // traditional to keep variables for multistep forms there.
  $step = empty($form_state['storage']['step']) ? 1 : $form_state['storage']['step'];
  $form_state['storage']['step'] = $step;

  switch ($step) {
    case 1:
      $form['step1'] = array(
      '#type' => 'fieldset',
      '#title' => t('Step 1: Personal details'),
      );
      $form['step1']['name'] = array(
          '#type' => 'textfield',
          '#title' => t('Your name'),
          '#default_value' => empty($form_state['values']['step1']['name']) ? '' : $form_state['values']['step1']['name'],
          '#required' => TRUE,
      );
      break;

    case 2:
      $form['step2'] = array(
      '#type' => 'fieldset',
      '#title' => t('Step 2: Street address info'),
      );
      $form['step2']['address'] = array(
          '#type' => 'textfield',
          '#title' => t('Your street address'),
          '#default_value' => empty($form_state['values']['step2']['address']) ? '' : $form_state['values']['step2']['address'],
          '#required' => TRUE,
      );
      break;

    case 3:
      $form['step3'] = array(
      '#type' => 'fieldset',
      '#title' => t('Step 3: City info'),
      );
      $form['step3']['city'] = array(
          '#type' => 'textfield',
          '#title' => t('Your city'),
          '#default_value' => empty($form_state['values']['step3']['city']) ? '' : $form_state['values']['step3']['city'],
          '#required' => TRUE,
      );
      break;
  }
  if ($step == 3) {
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t("Submit your information"),
    );
  }
  if ($step < 3) {
    $form['next'] = array(
        '#type' => 'submit',
        '#value' => t('Next step'),
        '#ajax' => array(
            'wrapper' => 'wizard-form-wrapper',
            'callback' => 'dynamic_fieldable_content_settings_wizard_callback',
        ),
    );
  }
  if ($step > 1) {
    $form['prev'] = array(
        '#type' => 'submit',
        '#value' => t("Previous step"),

        // Since all info will be discarded, don't validate on 'prev'.
        '#limit_validation_errors' => array(),
        // #submit is required to use #limit_validation_errors
        '#submit' => array('dynamic_fieldable_content_settings_wizard_submit'),
        '#ajax' => array(
            'wrapper' => 'wizard-form-wrapper',
            'callback' => 'dynamic_fieldable_content_settings_wizard_callback',
        ),
    );
  }

  // This simply allows us to demonstrate no-javascript use without
  // actually turning off javascript in the browser. Removing the #ajax
  // element turns off AJAX behaviors on that element and as a result
  // ajax.js doesn't get loaded.
  // For demonstration only! You don't need this.
  if ($no_js_use) {
    // Remove the #ajax from the above, so ajax.js won't be loaded.
    // For demonstration only.
    unset($form['next']['#ajax']);
    unset($form['prev']['#ajax']);
  }

  return $form;
}

function dynamic_fieldable_content_settings_wizard_callback($form, $form_state) {
  return $form;
}

/**
 * Submit function for dynamic_fieldable_content_settings_wizard.
 *
 * In AJAX this is only submitted when the final submit button is clicked,
 * but in the non-javascript situation, it is submitted with every
 * button click.
 */
function dynamic_fieldable_content_settings_wizard_submit($form, &$form_state) {

  // Save away the current information.
  $current_step = 'step' . $form_state['storage']['step'];
  if (!empty($form_state['values'][$current_step])) {
    $form_state['storage']['values'][$current_step] = $form_state['values'][$current_step];
  }

  // Increment or decrement the step as needed. Recover values if they exist.
  if ($form_state['triggering_element']['#value'] == t('Next step')) {
    $form_state['storage']['step']++;
    // If values have already been entered for this step, recover them from
    // $form_state['storage'] to pre-populate them.
    $step_name = 'step' . $form_state['storage']['step'];
    if (!empty($form_state['storage']['values'][$step_name])) {
      $form_state['values'][$step_name] = $form_state['storage']['values'][$step_name];
    }
  }
  if ($form_state['triggering_element']['#value'] == t('Previous step')) {
    $form_state['storage']['step']--;
    // Recover our values from $form_state['storage'] to pre-populate them.
    $step_name = 'step' . $form_state['storage']['step'];
    $form_state['values'][$step_name] = $form_state['storage']['values'][$step_name];
  }

  // If they're done, submit.
  if ($form_state['triggering_element']['#value'] == t('Submit your information')) {
    $value_message = t('Your information has been submitted:') . ' ';
    foreach ($form_state['storage']['values'] as $step => $values) {
      $value_message .= "$step: ";
      foreach ($values as $key => $value) {
        $value_message .= "$key=$value, ";
      }
    }
    drupal_set_message($value_message);
    $form_state['rebuild'] = FALSE;
    return;
  }

  // Otherwise, we still have work to do.
  $form_state['rebuild'] = TRUE;
}


/**
 * This example shows a button to "add more" - add another textfield, and
 * the corresponding "remove" button.
 *
 * It works equivalently with javascript or not, and does the same basic steps
 * either way.
 *
 * The basic idea is that we build the form based on the setting of
 * $form_state['num_names']. The custom submit functions for the "add-one"
 * and "remove-one" buttons increment and decrement $form_state['num_names']
 * and then force a rebuild of the form.
 *
 * The $no_js_use argument is simply for demonstration: When set, it prevents
 * '#ajax' from being set, thus making the example behave as if javascript
 * were disabled in the browser.
 */
function dynamic_fieldable_content_settings_add_more_disabled($form, &$form_state, $no_js_use = FALSE) {
  $form['description'] = array(
      '#markup' => '<div>' . t('This example shows an add-more and a remove-last button. The <a href="!ajax">AJAX version</a> does it without page reloads; the <a href="!multistep">non-js version</a> is the same code but simulates a non-javascript environment, showing it with page reloads.',
          array('!ajax' => url('examples/dynamic_fieldable_content_settings/add_more'), '!multistep' => url('examples/dynamic_fieldable_content_settings/add_more_no_js')))
      . '</div>',
  );

  /*
   // Because we have many fields with the same values, we have to set
  // #tree to be able to access them.
  $form['#tree'] = TRUE;
  */
  $form['settings_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('People coming to the picnic'),
      // Set up the wrapper so that AJAX will be able to replace the fieldset.
      '#prefix' => '<div id="settings-fieldset-wrapper">',
      '#suffix' => '</div>',
  );
  $form['settings_fieldset']['#tree'] = TRUE;



  // Build the fieldset with the proper number of names. We'll use
  // $form_state['num_names'] to determine the number of textfields to build.
  if (empty($form_state['num_names'])) {
    $form_state['num_names'] = (isset($form_state['item']->settings['num_names']) && !empty($form_state['item']->settings['num_names']) ? $form_state['item']->settings['num_names'] : 1);
  }


  for ($i = 0; $i < $form_state['num_names']; $i++) {
    $form['settings_fieldset']['name'][$i] = array(
        '#type' => 'textfield',
        '#title' => t(''),
        '#default_value' => (isset($form_state['item']->settings['settings_fieldset_name_'.$i]) && !empty($form_state['item']->settings['settings_fieldset_name_'.$i]) ? $form_state['item']->settings['settings_fieldset_name_'.$i] : "Label".($i+1)),
        '#weight' => $i*0,
        '#attributes' => array(
            'class' => array(
                'class-fieldcollection-label'
            )
        ),
    );
    $form['settings_fieldset']['value'][$i] = array(
        '#type' => 'textfield',
        '#title' => t(''),
        '#default_value' => (isset($form_state['item']->settings['settings_fieldset_value_'.$i]) && !empty($form_state['item']->settings['settings_fieldset_value_'.$i]) ? $form_state['item']->settings['settings_fieldset_value_'.$i] : "Value".($i+1)),
        '#weight' => ($i*0)+1,
        '#attributes' => array(
            'class' => array(
                'class-fieldcollection-type'
            )
        ),
    );
  }
  $form['settings_fieldset']['add_name'] = array(
      '#type' => 'submit',
      '#value' => t('Add one more'),
      '#submit' => array('dynamic_fieldable_content_settings_add_more_add_one'),
      // See the examples in dynamic_fieldable_content_settings.module for more details on the
      // properties of #ajax.
      '#ajax' => array(
          'callback' => 'dynamic_fieldable_content_settings_add_more_callback',
          'wrapper' => 'settings-fieldset-wrapper',
      ),
  );

  /* $form['#attached']['css'] = array(
   drupal_get_path('module', 'dynamic_fieldable_content') . '/css/dynamic_fieldable_content_add_more.css',
  );
  */
  if ($form_state['num_names'] > 1) {
    $form['settings_fieldset']['remove_name'] = array(
        '#type' => 'submit',
        '#value' => t('Remove one'),
        '#submit' => array('dynamic_fieldable_content_settings_add_more_remove_one'),
        '#ajax' => array(
            'callback' => 'dynamic_fieldable_content_settings_add_more_callback',
            'wrapper' => 'settings-fieldset-wrapper',
        ),
    );
  }
  /*  $form['submit'] = array(
   '#type' => 'submit',
      '#value' => t('Submit'),
  );
  */
  // This simply allows us to demonstrate no-javascript use without
  // actually turning off javascript in the browser. Removing the #ajax
  // element turns off AJAX behaviors on that element and as a result
  // ajax.js doesn't get loaded.
  // For demonstration only! You don't need this.
  if ($no_js_use) {
    // Remove the #ajax from the above, so ajax.js won't be loaded.
    if (!empty($form['settings_fieldset']['remove_name']['#ajax'])) {
      unset($form['settings_fieldset']['remove_name']['#ajax']);
    }
    unset($form['settings_fieldset']['add_name']['#ajax']);
  }

  return $form;
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */
function dynamic_fieldable_content_settings_add_more_callback($form, $form_state) {
  return $form['settings_fieldset'];
}

function dynamic_fieldable_content_js_add_more_callback($form, $form_state) {
  return $form['js_fieldset'];
}

function dynamic_fieldable_content_css_add_more_callback($form, $form_state) {
  return $form['css_fieldset'];
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function dynamic_fieldable_content_settings_add_more_add_one($form, &$form_state) {
  $form_state['num_names']++;
  $form_state['rebuild'] = TRUE;
}

function dynamic_fieldable_content_js_add_more_add_one($form, &$form_state) {
  $form_state['num_js']++;
  $form_state['rebuild'] = TRUE;
}

function dynamic_fieldable_content_css_add_more_add_one($form, &$form_state) {
  $form_state['num_css']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "remove one" button.
 *
 * Decrements the max counter and causes a form rebuild.
 */
function dynamic_fieldable_content_settings_add_more_remove_one($form, &$form_state) {
  if ($form_state['num_names'] > 1) {
    $form_state['num_names']--;
  }
  $form_state['rebuild'] = TRUE;
}

function dynamic_fieldable_content_js_add_more_remove_one($form, &$form_state) {
  if ($form_state['num_js'] > 1) {
    $form_state['num_js']--;
  }
  $form_state['rebuild'] = TRUE;
}

function dynamic_fieldable_content_css_add_more_remove_one($form, &$form_state) {
  if ($form_state['num_css'] > 1) {
    $form_state['num_css']--;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Final submit handler.
 *
 * Reports what values were finally set.
 */
function dynamic_fieldable_content_settings_add_more_submit($form, &$form_state) {
  $output = t('These people are coming to the picnic: @names',
      array('@names' => implode(', ', $form_state['values']['settings_fieldset']['name'])) );
  drupal_set_message($output);
}

/**
 * @} End of "defgroup ajax_degradation_example".
 */
