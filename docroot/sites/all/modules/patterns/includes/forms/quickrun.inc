<?php
/**
 * @file
 * Functions, forms related to the Patterns editor.
 */

/**
 * Form constructor for the Quick Run form.
 *
 * @see patterns_quickrun_submit()
 * @ingroup forms
 */
function patterns_quickrun($form, &$form_state) {

  $form = array();
  if (!patterns_engine_is_on()) {
    drupal_set_message(t('Patterns engine is off. You can change the state of the engine from the settings page if you want to execute the pattern. This attempt has been logged.'), 'warning');
    watchdog('patterns', t('Attempt to quickrun a pattern with engine off.'));
    drupal_goto('admin/patterns');
    return $form;
  }

  if (!patterns_parser_ready()) {
    $messages = t('No available patterns parser was found.</br>');
    $messages .= t(' Go to the !modules page to enable more Patterns parsers.', array('!modules' => l(t('modules'), 'admin/modules')));
    drupal_set_message($messages, 'warning');
    return $form;
  }

  $content = '';

  $text = t('The quick run mode should be used only in development environment, for testing, or if you really know what you are doing.');
  patterns_forms_add_page_header($form, 'Important!', $text);

  $options_mode = array(
    'extend' => 'Extend',
    'runover' => 'Run-Over',
  );

  // Add the execution options
  $form = patterns_forms_get_execution_options($form);

  $form['validation_result'] = array(
    '#markup' => 'This pattern has not been validated yet.',
    '#prefix' => '<div id="validation_result">',
    '#suffix' => '</div>',
  );

  $form = patterns_forms_get_formats_selector($form);
  $form = patterns_forms_get_validation_level_selector($form, PATTERNS_VALIDATE_SYNTAX);

  //Set the field as required for the quickrun case
  $form['format']['#required'] = TRUE;

  $form['content'] = array(
    '#type' => 'textarea',
    '#title' => t('Pattern\'s code'),
    '#rows' => 25,
    '#default_value' => $content,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Run'),
  );

  $form['validate'] = array(
    '#prefix' => '<span style=\'margin-right:10px\'>',
    '#markup' => "<a href='#' id='validate_pattern'>" . t('Validate') . "</a>",
    '#suffix' => '</span> ',
  );

  $form['cancel'] = array(
    '#markup' => l(t('Back'), 'admin/patterns'),
  );

  return $form;
}

/**
 * Form submission handler for patterns_quickrun().
 *
 * @see patterns_quickrun()
 */

function patterns_quickrun_submit($form, $form_state) {

  // Parse the content of the textarea
  $pattern['pattern'] = patterns_parser_parse($form_state['values']['content'], $form_state['values']['format']);

  $pattern = patterns_get_pattern_obj($pattern);

  $params['run-subpatterns'] = 'always'; // TODO
  $params['quickrun'] = TRUE;

  return patterns_start_engine($pattern, $params, $form_state['values']['mode']);
}
