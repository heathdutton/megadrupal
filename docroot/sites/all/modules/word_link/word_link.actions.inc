<?php

/**
 * @file
 * Actions for the Word Link module.
 */

/**
 * Action function.
 *
 * Goes through values and uses them to modify the passed words by
 * replacing the existing values.
 */
function word_link_modify_action($word, $context) {
  $modify = array_filter($context['modify']);
  foreach ($modify as $name => $value) {
    $value = $context['properties'][$name];
    $word->$name = $value;
  }
  word_link_save($word);
  if ($context['progress']['current'] >= $context['progress']['total']) {
    cache_clear_all('*', 'cache_filter', TRUE);
    field_cache_clear();
  }
}

/**
 * Action function.
 *
 * Delete words.
 */
function word_link_delete_action($word, $context) {
  word_link_delete($word->id);
  if ($context['progress']['current'] >= $context['progress']['total']) {
    cache_clear_all('*', 'cache_filter', TRUE);
    field_cache_clear();
  }
}

/**
 * Action form function.
 *
 * Displays form elements for properties of word.
 */
function word_link_modify_action_form($context, &$form_state) {
  $properties = array(
    'weight',
    'case_sensitive',
    'url',
    'url_title',
    'class',
    'rel',
    'visibility',
    'except',
    'status',
  );

  $form['#tree'] = TRUE;

  $form['modify'] = array(
    '#type' => 'fieldset',
    '#title' => t('Choose what to change'),
  );

  foreach ($properties as $property) {
    $form['modify'][$property] = array(
      '#type' => 'checkbox',
      '#title' => t(
        '@title',
        array(
          '@title' => drupal_ucfirst(str_replace('_', ' ', $property)),
        )
      ),
    );
  }

  $form['properties'] = array(
    '#type' => 'container',
  );
  $form['properties']['weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Weight'),
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t('Weight of the word. Lighter weights are higher up, heavier weights go down.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="modify[weight]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['properties']['case_sensitive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Case Sensitivity'),
    '#description' => t('By default Word Link are case sensitive. Uncheck this checkbox if you want this particular Word Link to be case insensitive.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="modify[case_sensitive]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['properties']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#size' => 30,
    '#maxlength' => 255,
    '#description' => t('The URL of the page to link to. External links must start with %http or %https and will be open in new window.', array('%http' => 'http://', '%https' => 'https://')),
    '#autocomplete_path' => 'word-link/ajax/urls',
    '#states' => array(
      'invisible' => array(
        ':input[name="modify[url]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['properties']['url_title'] = array(
    '#type' => 'textfield',
    '#title' => t('URL Title'),
    '#size' => 30,
    '#maxlength' => 255,
    '#description' => t('Title for the above URL. It will be embedded in the created link and appear as a tooltip when hovering the mouse over the link.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="modify[url_title]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['properties']['class'] = array(
    '#type' => 'textfield',
    '#title' => t('Class'),
    '#size' => 30,
    '#maxlength' => 255,
    '#description' => t('Use this to add a class for the link. Default value is "word-link".'),
    '#states' => array(
      'invisible' => array(
        ':input[name="modify[class]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['properties']['rel'] = array(
    '#type' => 'textfield',
    '#title' => t('Relationship'),
    '#size' => 30,
    '#maxlength' => 30,
    '#description' => t('Use this to add a rel attribute to the link.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="modify[rel]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['properties']['visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Show links on specific pages'),
    '#options' => array(
      t('All pages except those listed'),
      t('Only the listed pages'),
    ),
    '#default_value' => 0,
    '#states' => array(
      'invisible' => array(
        ':input[name="modify[visibility]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['properties']['except'] = array(
    '#type' => 'textarea',
    '#description' => t('Specify pages by using their paths. Enter one path per line. E.g. node/1.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="modify[except]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['properties']['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#description' => t('If unchecked this word will not be converted.'),
    '#default_value' => 1,
    '#states' => array(
      'invisible' => array(
        ':input[name="modify[status]"]' => array('checked' => FALSE),
      ),
    ),
  );

  return $form;
}

/**
 * Action form submit function.
 *
 * Passes values to word_link_modify().
 */
function word_link_modify_action_submit($form, $form_state) {
  return array(
    'modify' => $form_state['values']['modify'],
    'properties' => $form_state['values']['properties'],
  );
}

/**
 * Action function.
 *
 * Checks if word URL is valid and if not change word status to disabled.
 */
function word_link_disable_invalid_word_action($word, &$context) {
  if (!drupal_valid_path(drupal_get_normal_path($word->url))) {
    $word->status = FALSE;
    word_link_save($word);
  }
  if ($context['progress']['current'] >= $context['progress']['total']) {
    cache_clear_all('*', 'cache_filter', TRUE);
    field_cache_clear();
  }
}
