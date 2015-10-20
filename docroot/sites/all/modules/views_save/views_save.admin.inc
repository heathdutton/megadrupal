<?php

/**
 * @file
 * Admin pages for the Views Save module.
 */

/**
 * Page callback: Lists all available bundles for saved views.
 *
 * @see views_save_menu()
 */
function views_save_overview() {
  $rows = array();
  $base = 'admin/structure/views_save/';
  foreach (views_save_get_bundles() as $id => $name) {
    $path = $base . $id;
    $rows[] = array(
      check_plain($name) . ' <small>' . t('(Machine name: @type)', array('@type' => $id)) . '</small>',
      l(t('edit'), $path),
      l(t('manage fields'), $path . '/fields'),
      l(t('manage display'), $path . '/display'),
      l(t('delete'), $path . '/delete'),
    );
  }

  return array(
    '#theme' => 'table',
    '#header' => array(
      t('Label'),
      array('data' => t('Operations'), 'colspan' => 4),
    ),
    '#rows' => $rows,
    '#empty' => t('No bundles found. Add the "Global: Save" area to some views to create bundles for those views.'),
  );
}

/**
 * Form constructor for creating or editing a saved view type.
 *
 * @param string|null $id
 *   A bundle ID, or NULL to create a new one.
 *
 * @see views_save_bundle_edit_validate()
 * @see views_save_bundle_edit_submit()
 *
 * @ingroup forms
 */
function views_save_bundle_edit(array $form, array &$form_state, $id = NULL) {
  $form_state['create_new'] = !$id;
  $label = $id ? views_save_bundle_get_name($id) : '';

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#description' => t("The type's human-readable name."),
    '#default_value' => $label,
    '#required' => TRUE,
  );
  $form['id'] = array(
    '#type' => 'machine_name',
    '#maxlength' => 30,
    '#machine_name' => array(
      'exists' => 'views_save_bundle_get_name',
      'source' => array('label'),
    ),
    '#default_value' => $id,
    '#required' => TRUE,
    '#disabled' => (bool) $id,
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  if ($id) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete type'),
      '#submit' => array('views_save_bundle_edit_submit_delete'),
    );
  }

  return $form;
}

/**
 * Form submission handler for views_save_bundle_edit().
 */
function views_save_bundle_edit_submit(array $form, array &$form_state) {
  $fields = array('id' => 1, 'label' => 1);
  $bundle = (object) array_intersect_key($form_state['values'], $fields);
  $new = !empty($form_state['create_new']);

  views_save_bundle_save($bundle, $new);

  $form_state['redirect'] = 'admin/structure/views_save';
  if ($new) {
    $form_state['redirect'] .= "/{$bundle->id}/fields";
  }

  drupal_set_message(t('The type was successfully saved.'));
}

/**
 * Form submission handler for views_save_bundle_edit().
 *
 * Handles the "Delete type" button.
 */
function views_save_bundle_edit_submit_delete(array $form, array &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $form_state['redirect'] = array('admin/structure/views_save/' . $form_state['values']['id'] . '/delete', array('query' => $destination));
}

/**
* Form constructor for deleting a saved view type.
*
* @see views_save_bundle_delete_confirm_submit()
*
* @ingroup forms
*/
function views_save_bundle_delete_confirm($form, &$form_state, $id) {
  $form_state['bundle'] = $id;
  $variables['%name'] = views_save_bundle_get_name($id);
  $question = t('Do you really want to delete the %name type?', $variables);

  $description = '<p>';
  $sql = 'SELECT COUNT(id) FROM {views_save} WHERE bundle = :bundle';
  $count = db_query($sql, array(':bundle' => $id))->fetchField();
  if ($count) {
    $description .= format_plural($count, '1 saved view will be deleted.', '@count saved views will be deleted.') . ' ';
  }
  $description .= t('Any views still offering saving views with this type will become faulty.') . '</p>';
  $description .= '<p>' . t('This action cannot be undone.') . '</p>';

  return confirm_form($form, $question, 'admin/structure/views_save', $description);
}

/**
* Form submission handler for views_save_bundle_delete_confirm().
*/
function views_save_bundle_delete_confirm_submit(array $form, array &$form_state) {
  views_save_bundle_delete($form_state['bundle']);
  $form_state['redirect'] = 'admin/structure/views_save';
}
