<?php
/**
 * @file
 * Admin forms.
 */

/**
 * Form constructor for the silverpop webform management form.
 *
 * @param array $form
 * @param array $form_state
 * @param object $webform_node
 *   The webform node silverpop information is attached to.
 *
 * @see silverpop_webform_components_form_submit()
 * @see silverpop_webform_components_form_delete_submit()
 *
 * @ingroup forms
 */
function silverpop_webform_components_form($form, $form_state, $webform_node) {
  $form = array();

  $form['#tree'] = TRUE;
  $form['#node'] = $webform_node;
  $nid = $webform_node->nid;
  $record = silverpop_webform_load($nid);

  $form['#record'] = $record;

  if (!isset($record->is_active)) {
    $record = new stdClass();
    $record->is_active = "";
    $record->add_to_contact_list = TRUE;
    $record->database_id = "";
    $record->list_id = "";
  }
  $form['details'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
  );
  $form['details']['is_active'] = array(
    '#title' => 'Is active',
    '#type' => 'checkbox',
    '#default_value' => $record->is_active,
  );
  $form['details']['database_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Silverpop Database ID'),
    '#default_value' => $record->database_id,
    '#size' => 60,
    '#maxlength' => 255,
    '#description' => 'Click "Show Additional Details" when you are viewing the
      database summary page to locate the Database ID.',
    '#required' => TRUE,
  );
  $form['details']['add_to_contact_list'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add to contact list'),
    '#default_value' => $record->add_to_contact_list,
    '#description' => t('Additionally adds the recipient to a contact list.'),
  );
  $form['details']['list_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Silverpop Contact List ID'),
    '#default_value' => !empty($record->list_id) ? $record->list_id : '',
    '#size' => 60,
    '#maxlength' => 255,
    '#description' => 'Click "Show Additional List Details" when you are viewing
      the contact list summary page to locate the List ID.',
    '#states' => array(
      'visible' => array(
        '#edit-details-add-to-contact-list' => array('checked' => TRUE),
      ),
    ),
  );
  $form['components'] = array(
    '#tree' => TRUE,
  );
  foreach ($webform_node->webform['components'] as $k => $component) {
    $form['components'][$k] = array(
      '#component' => $component,
      'key' => array(
        '#title' => 'Field name',
        '#type' => 'textfield',
        '#size' => 25,
        '#default_value' => isset($record->data[$component['form_key']]['key']) ? $record->data[$component['form_key']]['key'] : '',
      ),
    );
  }
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('silverpop_webform_components_form_delete_submit'),
  );

  return $form;
}

/**
 * Theme callback for silverpop_webform_components_form().
 *
 * @param array $form
 */
function theme_silverpop_webform_components_form($form) {
  $form = $form['form'];
  $rows = array();
  $output = '';

  $header = array(t('Name'), t('Type'), t('Silverpop key'));

  foreach (element_children($form['components']) as $k) {
    $row = array();
    // Name
    $row[] = $form['#node']->webform['components'][$k]['name'];
    // Type
    $row[] = $form['#node']->webform['components'][$k]['type'];
    // Silverpop key
    unset($form['components'][$k]['key']['#title']);
    $row[] = drupal_render($form['components'][$k]['key']);
    $rows[] = $row;
  }

  $output .= drupal_render($form['details']);
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Form validator for the silverpop webform management form.
 *
 * @param $form
 * @param $form_state
 */
function silverpop_webform_components_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  if (!empty($values['details']['add_to_contact_list']) && empty($values['details']['list_id'])) {
    form_error($form['details']['list_id'], t('Silverpop Contact List ID is required.'));
  }
}

/**
 * Form submission handler for silverpop_webform_components_form().
 *
 * Saves the silverpop webform information to the database.
 *
 * @param array $form
 * @param array $form_state
 */
function silverpop_webform_components_form_submit($form, $form_state) {
  $node = $form['#node'];

  $record = $form['#record'];
  if ($record) {
    $update = array('nid');
  }
  else {
    $record = new stdClass();
    $update = array();
  }

  $data = array();
  foreach (element_children($form['components']) as $k) {
    $component = $form['components'][$k]['#component'];
    $data[$component['form_key']]['key'] = $form_state['values']['components'][$k]['key'];
  }
  $record->nid = $node->nid;
  $record->database_id = $form_state['values']['details']['database_id'];
  $record->add_to_contact_list = $form_state['values']['details']['add_to_contact_list'];
  $record->list_id = $form_state['values']['details']['list_id'];
  $record->is_active = (bool) $form_state['values']['details']['is_active'];
  $record->data = $data;
  drupal_write_record('silverpop_webform', $record, $update);

  drupal_set_message(t('Silverpop webform configuration has been saved.'));
}

/**
 * Form submission handler for silverpop_webform_components_form().
 *
 * Deletes the silverpop webform settings from the database for the node.
 *
 * @param array $form
 * @param array $form_state
 */
function silverpop_webform_components_form_delete_submit($form, &$form_state) {
  $node = $form['#node'];
  db_delete('silverpop_webform')
    ->condition('nid', $node->nid)
    ->execute();

  drupal_set_message(t('Silverpop settings for this webform deleted.'), 'status');
}
