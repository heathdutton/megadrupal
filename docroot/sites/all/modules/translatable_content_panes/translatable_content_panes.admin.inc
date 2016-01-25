<?php

/**
 * Form builder callback for the edit form.
 */
function translatable_content_panes_edit_form($form, &$form_state, $translatable_content_pane) {
  // Can use the base form builder, but without the admin title.
  $form = translatable_content_panes_form_builder($translatable_content_pane);
  $form['admin_title']['#access'] = FALSE;
  $form['code_content']['#access'] = FALSE;

  // We'll also need a submit button.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update content'),
  );

  // Store the pane id in the form state.
  $form_state['pane_id']  = $translatable_content_pane['id'];

  // Set the title to a better one.
  drupal_set_title(t('Editing translatable content pane %admin_title:', array('%admin_title' => $translatable_content_pane['admin_title'])), PASS_THROUGH);

  return $form;
}

/**
 * Submit handler for the above edit form.
 */
function translatable_content_panes_edit_form_submit($form, &$form_state) {
  // We can just use save content helper.
  translatable_content_panes_save_pane_content($form_state['pane_id'], $form_state['values']);
  $form_state['redirect'] = 'admin/content/translatable-content-panes';
  drupal_set_message(t('Translatable content pane %admin_title updated.', array('%admin_title' => $form_state['values']['admin_title'])));
}

/**
 * Form builder helper function for the add / edit content pane forms.
 */
function translatable_content_panes_form_builder($default_values) {
  // Base defaults.
  $default_values += array(
    'id' => '',
    'admin_title' => '',
    'translations' => array(),
    'code_content' => FALSE,
  ) + _translatable_content_panes_get_content_defaults();

  // Build the up form now.
  $form = array(
    '#tree' => TRUE,
  );

  $form['admin_title'] = array(
    '#title' =>  t('Administrative title'),
    '#type' => 'textfield',
    '#default_value' => $default_values['admin_title'],
    '#required' => TRUE,
    '#description' => t('Administrative title for this content.'),
  );

  $form['code_content'] = array(
    '#title' => t('Manage content in code?'),
    '#type' => 'checkbox',
    '#default_value' => $default_values['code_content'],
    '#description' => t('When this is ticked, the content will be managed via code. It will not be editable via the admin interface (it will be editable via the Panels UI.'),
  );

  // The default content - used as a fallback if the langauge version doesn't exist.
  $form += _translatable_content_panes_content_fieldset_builder(LANGUAGE_NONE, t('Default content'), $default_values);

  // If the locale module is enabled, add a fieldset per language.
  if (module_exists('locale')) {
    // The language specific bits
    $languages = locale_language_list();
    foreach ($languages as $langcode => $name) {
      // Add the content fields for the language to the form.
      $form += _translatable_content_panes_content_fieldset_builder($langcode, t('@language content', array('@language' => $name)), $default_values);
    }
  }

  return $form;
}

/**
 * Helper to return the default values for the content bits.
 */
function _translatable_content_panes_get_content_defaults() {
  return array(
    'title' => '',
    'content' => '',
    'format' => NULL,
  );
}

/**
 * Fieldset builder for the content form builder above.
 */
function _translatable_content_panes_content_fieldset_builder($langcode, $fieldset_label, $values) {
  // Work out the content values.
  if ($langcode != LANGUAGE_NONE) {
    $content_values = isset($values['translations'][$langcode]) ? $values['translations'][$langcode] : _translatable_content_panes_get_content_defaults();
  }
  else {
    $content_values = $values;
  }

  // Build up the form.
  $form = array(
    $langcode => array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => !($langcode == LANGUAGE_NONE || isset($values['translations'][$langcode])),
      '#title' => $fieldset_label,
      'title' => array(
        '#type' => 'textfield',
        '#default_value' => $content_values['title'],
        '#title' => t('Title'),
      ),
      'content' => array(
        '#type' => 'text_format',
        '#default_value' => $content_values['content'],
        '#format' => $content_values['format'],
        '#title' => t('Content'),
        // Remove when https://drupal.org/node/997826 is fixed.
        '#process' => array(
          'filter_process_format',
          'translatable_content_panes_filter_process_format',
        ),
      ),
    ),
  );

  // If it's a proper language, we'll need either Remove or Enable checkboxes.
  if ($langcode != LANGUAGE_NONE) {
    // If there is an array, we'll need the Remove button and a fake "Enable" button.
    if (isset($values['translations'][$langcode])) {
      // Used to remove a translation.
      $form[$langcode]['remove'] = array(
        '#type' => 'checkbox',
        '#default_value' => FALSE,
        '#title' => t('Remove translation'),
        '#weight' => -99,
      );

      // Fake "enable" button.
      $form[$langcode]['enable'] = array(
        '#type' => 'value',
        '#value' => TRUE,
      );

      // State element info.
      $state_element = 'remove';
      $state_status = 'unchecked';
    }
    else {
      // An enable checkbox to enable the translation.
      $form[$langcode]['enable'] = array(
        '#type' => 'checkbox',
        '#default_value' => FALSE,
        '#title' => t('Enable translation'),
        '#weight' => -99,
      );

      // State element info.
      $state_element = 'enable';
      $state_status = 'checked';
    }

    // Add in the #states info to make the content fields only show when they'll be saved.
    foreach (array('title', 'content') as $field) {
      $form[$langcode][$field]['#states'] = array(
        'visible' => array(
          ':input[name="' . $langcode . '[' . $state_element .']"]' => array($state_status => TRUE),
        ),
      );
    }
  }

  return $form;
}

/**
 * Helper function to handle submitting a form built with the form builder.
 */
function translatable_content_panes_save_pane_content($pane_id, $values) {
  // Start a transaction.
  $transaction = db_transaction();

  // Build up an array of language keys.
  if (module_exists('locale')) {
    $languages = locale_language_list() + array(LANGUAGE_NONE => 'Default language');
  }
  else {
    $languages = array(LANGUAGE_NONE => 'Default language');
  }

  try {
    // Ensure the admin title / code content is updated.
    db_update('translatable_content_pane')
      ->fields(array('admin_title' => $values['admin_title'], 'code_content' => $values['code_content']))
      ->condition('id', $pane_id)
      ->execute();

    // Loop through the values looking for the languages.
    foreach (element_children($values) as $language) {
      // Ignore any non-languages.
      if (!array_key_exists($language, $languages)) {
        continue;
      }

      // Easier variable.
      $value = $values[$language];

      // If the 'remove' checkbox was checked, we want to remove the data (we'll leave revisions in-tact).
      if (!empty($value['remove'])) {
        db_delete('translatable_content_pane_data')
          ->condition('id', $pane_id)
          ->condition('language', $language)
          ->execute();
      }
      // Only save / update if the "enable" value is non-empty, or it's the default language.
      else if (!empty($value['enable']) || $language == LANGUAGE_NONE) {
        // We only make a revision if it is not a code content pane.
        if (empty($values['code_content'])) {
          // Build up an array for the revision record.
          $revision_record = array(
            'id' => $pane_id,
            'language' => $language,
            'title' => $value['title'],
            'content' => $value['content']['value'],
            'format' => $value['content']['format'],
            'uid' => $GLOBALS['user']->uid,
            'timestamp' => time(),
          );

          // Save the revision so that we get the revision id.
          drupal_write_record('translatable_content_pane_revision_data', $revision_record);
        }
        else {
          // Simulate the revision record array for usage below.
          $revision_record = array('revision_id' => NULL);
        }

        // Update the normal data table now we've got a revision id.
        db_merge('translatable_content_pane_data')
          ->key(array('id' => $pane_id, 'language' => $language))
          ->fields(array(
            'id' => $pane_id,
            'revision_id' => $revision_record['revision_id'],
            'language' => $language,
            'title' => $value['title'],
            'content' => $value['content']['value'],
            'format' => $value['content']['format'],
          ))
          ->execute();
      }
    }
  }
  catch (Exception $e) {
    $transaction->rollback();
    watchdog_exception('translatable_content_panes', $e);
    throw $e;
  }
}
