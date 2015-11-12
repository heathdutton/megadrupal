<?php

/**
 * @file googleanalytics_perpage.admin.inc
 * Administrative callbacks & functionality for googleanalytics_perpage module.
 */
 
/**
 * Implements hook_form().
 * Form function for the perpage analytics settings.
 */
function googleanalytics_perpage_form($form, &$form_state) {
  if (empty($form_state['values']['snippets'])) {
    $snippets = googleanalytics_perpage_get_snippets();
  }
  else {
    $snippets = $form_state['values']['snippets'];
  }
  if (!isset($form_state['snippet_count'])) {
    if (!count($snippets)) {
      $form_state['snippet_count'] = 1;
    }
    else {
      $form_state['snippet_count'] = count($snippets);
    }
  }
  
  $form['wrapper'] = array(
    '#type'   => 'fieldset',
    '#title'  => t('URL Specific Extra Tracking Code Snippets'),
  );
  $form['wrapper']['snippets'] = array(
    '#type'   => 'markup',
    '#prefix' => '<div id="googleanalytics-perpage-ajax-wrapper">',
    '#suffix' => '</div>',
    '#tree'   => TRUE,
    '#theme'  => 'googleanalytics_perpage_snippet_table',
  );

  for ($key = 0; $key < $form_state['snippet_count']; $key++) {
    $values = (isset($snippets[$key])) ? $snippets[$key] : array();
    $form['wrapper']['snippets'][$key] = googleanalytics_perpage_build_snippet($key, $values);
  }

  $form['wrapper']['add_snippet'] = array(
    '#type'   => 'submit',
    '#value'  => t('Add Another'),
    '#submit' => array('googleanalytics_perpage_more_submit'),
    '#ajax'   => array(
      'callback' => 'googleanalytics_perpage_ajax',
      'wrapper'  => 'googleanalytics-perpage-ajax-wrapper',
      'method'   => 'replace',
      'effect'    => 'fade',
      'progress'  => array(
        'type'    => 'throbber',
        'message' => '',
      ),
    ),
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}


/**
 * Implements hook_form_validate().
 * Ensure any snippet table row has either none of the fields, or all of the fields filled.
 */
function googleanalytics_perpage_form_validate($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Save') && isset($form_state['values']['snippets'])) {
    foreach($form_state['values']['snippets'] as $key => $snippet) {
      if (
           ($snippet['title'] && !$snippet['snippet'] || !$snippet['paths']) ||
           ($snippet['snippet'] && !$snippet['title'] || !$snippet['paths']) ||
           ($snippet['paths'] && !$snippet['snippet'] || !$snippet['title'])
         ) {
        form_set_error('snippets]['. $key .'][title', t('Please provide data for every field on row !rownum', array('!rownum' => ($key+1))));
        form_set_error('snippets]['. $key .'][snippet', t('Please provide data for every field on row !rownum', array('!rownum' => ($key+1))));
        form_set_error('snippets]['. $key .'][paths', t('Please provide data for every field on row !rownum', array('!rownum' => ($key+1))));
        // unset two of the three messages, so fields highlight but message only displays once.
        array_pop($_SESSION['messages']['error']);
        array_pop($_SESSION['messages']['error']);
      }
    }
  }
}


/**
 * Implements hook_form_submit().
 */
function googleanalytics_perpage_form_submit($form, &$form_state) {
  if (isset($form_state['values']['snippets'])) {
    $snippets = $form_state['values']['snippets'];

    foreach ($snippets as $snippet) {
      unset($snippet['remove']);

      // Ensure we're not saving any blank rows.
      if ($snippet['title']) {
        if ($snippet['gappid']) {
          // Update if it's a previously existing entry
          db_merge('googleanalytics_perpage')
            ->key(array('gappid' => $snippet['gappid']))
            ->fields($snippet)
            ->execute();
        }
        else {
          // Insert if it's a new entry
          db_insert('googleanalytics_perpage')
            ->fields($snippet)
            ->execute();
        }
      }
    }
    drupal_set_message(t('Your URL specific extra tracking code snippets have been saved.'));
  }
}


/**
 * AJAX Submit callback to add an additional snippet table row.
 */
function googleanalytics_perpage_more_submit($form, &$form_state) {
  if ($form_state['values']['snippets']) {
    $form_state['snippet_count'] = count($form_state['values']['snippets']) + 1;
  }
  else {
    $form_state['snippet_count'] = 1;
  }
  $form_state['rebuild'] = TRUE;
}


/**
 * AJAX Submit callback to remove a snippet table row.
 */
function googleanalytics_perpage_remove_snippet_submit($form, &$form_state) {
  $button = explode('_', $form_state['clicked_button']['#name']);
  $delta = isset($button[2]) ? $button[2] : NULL;
  if (!is_null($delta)) {
    unset($form_state['input']['snippets']);
    // If this was an existing entry, delete it from the DB.
    if ($form_state['values']['snippets'][$delta]['gappid']) {
      db_delete('googleanalytics_perpage')
        ->condition('gappid', $form_state['values']['snippets'][$delta]['gappid'])
        ->execute();
    }
    unset($form_state['values']['snippets'][$delta]);
    // Renumber child entries:
    $snippets = $form_state['values']['snippets'];
    unset($form_state['values']['snippets']);
    $i = 0;
    foreach ($snippets as $snippet) {
      $form_state['values']['snippets'][$i] = $snippet;
      $i++;
    }
    $form_state['snippet_count'] = count($form_state['values']['snippets']);
    $form_state['rebuild'] = TRUE;
  }
}


/**
 * AJAX Callback to re-render the form.
 */
function googleanalytics_perpage_ajax($form, &$form_state) {
  return $form['wrapper']['snippets'];
}


/**
 * Helper function to build a group of fields for a single snippet
 */
function googleanalytics_perpage_build_snippet($key = 0, $values = array()) {
  $form['gappid'] = array(
    '#type'    => 'hidden',
    '#value'   => isset($values['gappid']) ? $values['gappid'] : 0,
    '#parents' => array('snippets', $key, 'gappid'),
  );
  $form['title'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Name'),
    '#length'        => 60,
    '#maxlength'     => 60,
    '#default_value' => isset($values['title']) ? $values['title'] : '',
    '#parents'       => array('snippets', $key, 'title'),
  );
  $form['active'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Active'),
    '#default_value' => isset($values['active']) ? $values['active'] : 1,
    '#parents'       => array('snippets', $key, 'active'),
  );
  $form['snippet'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Snippet'),
    '#default_value' => isset($values['snippet']) ? $values['snippet'] : '',
    '#parents'       => array('snippets', $key, 'snippet'),
  );
  $form['paths'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Path(s)'),
    '#default_value' => isset($values['paths']) ? $values['paths'] : '',
    '#parents'       => array('snippets', $key, 'paths'),
  );
  $form['remove'] = array(
    '#type'       => 'submit',
    '#name'       => 'remove_snippet_'. $key,
    '#value'      => t('Remove'),
    '#submit'     => array('googleanalytics_perpage_remove_snippet_submit'),
    '#ajax'       => array(
      'callback'  => 'googleanalytics_perpage_ajax',
      'wrapper'   => 'googleanalytics-perpage-ajax-wrapper',
      'method'    => 'replace',
      'effect'    => 'fade',
      'progress'  => array(
        'type'    => 'throbber',
        'message' => '',
      ),
    ),
    '#parents'    => array('snippets', $key, 'remove'),
  );

  return $form;
}


/**
 * Helper function to load all snippets from the DB.
 */
function googleanalytics_perpage_get_snippets() {
  $result = db_query('SELECT * FROM {googleanalytics_perpage}');

  $snippets = array();
  foreach($result as $record) {
    $snippets[] = array(
      'gappid'  => $record->gappid,
      'title'   => $record->title,
      'snippet' => $record->snippet,
      'paths'   => $record->paths,
      'active'  => $record->active,
    );
  }

  return $snippets;
}