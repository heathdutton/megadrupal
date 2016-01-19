<?php
/**
 * @file
 * Form and page callbacks for Anki administration pages.
 */

/**
 * Form constructor for the main Anki settings page.
 *
 * @see anki_settings_form_validate()
 *
 * @ingroup forms
 */
function anki_settings_form($form, &$form_state) {
  $form['anki_server_base_url'] = array(
    '#title' => t('AnkiServer REST API url'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => anki_server_base_url(),
    '#description' => t('The base URL for the <a href="!ankiserver_url">AnkiServer</a> REST API. For example: <em>http://127.0.0.1:27701/</em>', array('!ankiserver_url' => 'https://github.com/dsnopek/anki-sync-server')),
  );
  $form = system_settings_form($form);
  $form['actions']['test'] = array(
    '#type' => 'submit',
    '#value' => t('Test connection'),
  );
  return $form;
}

/**
 * Form validation handler for anki_settings_form().
 *
 * @see anki_settings_form()
 */
function anki_settings_form_validate($form, &$form_state) {
  if (!valid_url($form_state['values']['anki_server_base_url'], TRUE)) {
    form_set_error('anki_server_base_url', t('Must be a valid URL!'));
  }
  if ($form_state['clicked_button']['#value'] == t('Test connection')) {
    if (anki_server_connection($form_state['values']['anki_server_base_url'])->check()) {
      drupal_set_message(t('Successfully tested connection to AnkiServer!'));
    }
    else {
      form_set_error('anki_server_base_url', t('Failed to connect to AnkiServer at this URL!'));
    }
  }
}

