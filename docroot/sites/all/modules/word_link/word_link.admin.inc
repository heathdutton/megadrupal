<?php

/**
 * @file
 * Administrative pages for the Word Link module.
 */

/**
 * Form builder for a settings page.
 */
function word_link_admin_settings() {
  $form['word_link_add_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add default CSS'),
    '#description' => t('If turned on default CSS from module will be added.'),
    '#default_value' => variable_get('word_link_add_css', 1),
  );

  return system_settings_form($form);
}

/**
 * Form builder for add or edit page.
 */
function word_link_add_form($form, &$form_state, $word = NULL) {
  $form['word'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add word'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );
  $form['word']['text'] = array(
    '#type' => 'textfield',
    '#title' => t('Word/Phrase'),
    '#size' => 30,
    '#maxlength' => 255,
    '#description' => t('The word or phrase you wish to convert to a link. This field is case sensitive.'),
    '#required' => TRUE,
    '#autocomplete_path' => 'word-link/ajax/words',
    '#default_value' => !empty($word->text) ? $word->text : '',
  );
  $form['word']['weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Weight'),
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t('Weight of the word. Lighter weights are higher up, heavier weights go down.'),
    '#required' => TRUE,
    '#default_value' => !empty($word->weight) ? $word->weight : 0,
  );
  $form['word']['case_sensitive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Case Sensitive'),
    '#description' => t('By default Word Link are case sensitive. Uncheck the checkbox if you want this particular Word Link to be case insensitive.'),
    '#default_value' => isset($word->case_sensitive) ? (int) $word->case_sensitive : 1,
  );
  $form['word']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#size' => 30,
    '#maxlength' => 255,
    '#description' => t('The URL of the page to link to. External links must start with %http or %https and will be open in new window.', array('%http' => 'http://', '%https' => 'https://')),
    '#required' => TRUE,
    '#autocomplete_path' => 'word-link/ajax/urls',
    '#default_value' => isset($word->url) ? $word->url : '',
  );
  $form['word']['url_title'] = array(
    '#type' => 'textfield',
    '#title' => t('URL Title'),
    '#size' => 30,
    '#maxlength' => 255,
    '#description' => t('Title for the above URL. It will be embedded in the created link and appear as a tooltip when hovering the mouse over the link.'),
    '#default_value' => isset($word->url_title) ? $word->url_title : '',
  );
  $form['word']['class'] = array(
    '#type' => 'textfield',
    '#title' => t('Class'),
    '#size' => 30,
    '#maxlength' => 255,
    '#description' => t('Use this to add a class for the link. Default value is "word-link".'),
    '#default_value' => isset($word->class) ? $word->class : 'word-link',
  );
  $form['word']['rel'] = array(
    '#type' => 'textfield',
    '#title' => t('Relationship'),
    '#size' => 30,
    '#maxlength' => 30,
    '#description' => t('Use this to add a rel attribute to the link.'),
    '#default_value' => isset($word->rel) ? $word->rel : '',
  );
  $form['word']['visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Show links on specific pages'),
    '#options' => array(
      t('All pages except those listed'),
      t('Only the listed pages'),
    ),
    '#default_value' => isset($word->visibility) ? $word->visibility : 0,
  );
  $form['word']['except'] = array(
    '#type' => 'textarea',
    '#description' => t('Specify pages by using their paths. Enter one path per line. E.g. node/1.'),
    '#default_value' => isset($word->except) ? $word->except : '',
  );
  $form['word']['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#description' => t('If unchecked this word will not be converted.'),
    '#default_value' => isset($word->status) ? (int) $word->status : 1,
  );
  $form['word']['id'] = array(
    '#type' => 'hidden',
    '#value' => isset($word->id) ? $word->id : NULL,
  );
  $form['actions'] = array(
    '#type' => 'container',
  );
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 0,
    '#submit' => array('word_link_add_form_save_submit'),
  );
  if (!empty($word)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 1,
      '#submit' => array('word_link_add_form_delete_submit'),
    );
  }

  return $form;
}

/**
 * Validate form for add or edit page.
 */
function word_link_add_form_validate($form, &$form_state) {
  $values = $form_state['values']['word'];
  if ($form_state['values']['op'] == t('Save')) {
    $normal_path = drupal_get_normal_path($values['url']);
    if (!drupal_valid_path($normal_path)) {
      form_set_error('url', t("The path '@url' is either invalid or you do not have access to it.", array('@url' => $values['url'])));
    }
    if (!empty($values['class']) && !preg_match('/^[_a-zA-Z]+[_a-zA-Z0-9-\s]*$/', trim($values['class']))) {
      form_set_error('class', t('Class is not valid.'));
    }
  }
}

/**
 * Submit for save action.
 */
function word_link_add_form_save_submit($form, &$form_state) {
  $word = $form_state['values']['word'];

  $action = 'created';
  if (isset($word['id'])) {
    $action = 'updated';
  }
  word_link_save($word);
  drupal_set_message(
    t(
      'Word %text has been @action.',
      array(
        '%text' => $word->text,
        '@action' => $action,
      )
    )
  );

  cache_clear_all('*', 'cache_filter', TRUE);
  field_cache_clear();
  $form_state['redirect'] = 'admin/config/content/word-link/list';
}

/**
 * Submit for delete action.
 */
function word_link_add_form_delete_submit($form, &$form_state) {
  $form_state['redirect'] = array(
    'admin/config/content/word-link/delete/' . $form_state['values']['word']['id'],
  );
}

/**
 * Form builder for delete confirm page.
 */
function word_link_delete_form($form, &$form_state, $word) {
  $form['#word'] = $word;
  $form['id'] = array('#type' => 'value', '#value' => $word->id);
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $word->text)),
    'admin/config/content/word-link/list',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit form for delete confirm page.
 */
function word_link_delete_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    word_link_delete($form_state['values']['id']);
    drupal_set_message(
      t(
        'Word %text has been deleted.',
        array(
          '%text' => $form['#word']->text,
        )
      )
    );
    cache_clear_all('*', 'cache_filter', TRUE);
    field_cache_clear();
  }
  $form_state['redirect'] = 'admin/config/content/word-link/list';
}
