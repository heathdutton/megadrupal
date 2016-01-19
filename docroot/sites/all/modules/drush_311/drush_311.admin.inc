<?php

/**
 * @file
 * Administrative functionality for drush_311 module.
 */

/**
 * Build a (sortable) form containing exisiting drush 311 entries.
 */
function drush_311_form($form, &$form_state) {
  $form['#header'] = array(
    array('data' => t('Name'), 'field' => 'name', 'sort' => 'asc'),
    array('data' => t('keywords'), 'field' => 'keywords'),
    array('data' => t('Information'), 'field' => 'information'),
    t('Delete'),
  );

  $query = db_select('drush_311', 'd')->extend('PagerDefault')->extend('TableSort');
  $query->fields('d')
    ->limit(50);
  $result = $query->execute();

  $form['drush_311'] = array('#tree' => TRUE);
  $form['drush_311']['existing'] = array('#tree' => TRUE);
  foreach ($result as $entry) {
    $form['drush_311']['existing'][$entry->id] = array(
      '#tree' => TRUE,
    );
    $form['drush_311']['existing'][$entry->id]['name'] = array(
      '#type' => 'textfield',
      '#default_value' => check_plain($entry->name),
    );
    $form['drush_311']['existing'][$entry->id]['keywords'] = array(
      '#type' => 'textfield',
      '#default_value' => check_plain($entry->keywords),
    );
    $form['drush_311']['existing'][$entry->id]['information'] = array(
      '#type' => 'textfield',
      '#default_value' => check_plain($entry->information),
    );
    $form['drush_311']['existing'][$entry->id]['delete'] = array('#type' => 'link', '#title' => t('delete'), '#href' => "admin/config/user-interface/drush_311/$entry->id/delete");
  }

  // Add extra rows for new entries.
  $form['drush_311']['new'] = array(
    '#tree' => TRUE,
  );
  for ($i=0; $i < 5; $i++) {
  	$form['drush_311']['new'][$i] = array(
      '#tree' => TRUE,
  	);
  	$form['drush_311']['new'][$i]['name'] = array(
  	  '#type' => 'textfield',
  	);
  	$form['drush_311']['new'][$i]['keywords'] = array(
  	  '#type' => 'textfield',
  	);
  	$form['drush_311']['new'][$i]['information'] = array(
  	  '#type' => 'textfield',
  	);
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );
  return $form;
}

/**
 * Output a sortable table containing all drush 311 entries.
 */
function theme_drush_311_form($variables) {
  $form = $variables['form'];
  $output = '';
  $rows = array();

  $items = $form['drush_311'];
  foreach (element_children($form['drush_311']['existing']) as $key) {
    $rows[] = array(
      drupal_render($form['drush_311']['existing'][$key]['name']),
      drupal_render($form['drush_311']['existing'][$key]['keywords']),
      drupal_render($form['drush_311']['existing'][$key]['information']),
      drupal_render($form['drush_311']['existing'][$key]['delete']),
    );
  }

  foreach (element_children($form['drush_311']['new']) as $key) {
    $rows[] = array(
      drupal_render($form['drush_311']['new'][$key]['name']),
      drupal_render($form['drush_311']['new'][$key]['keywords']),
      drupal_render($form['drush_311']['new'][$key]['information']),
      '',
    );
  }

  $item['entries'] = array(
    '#markup' => theme('table', array('header' => $form['#header'], 'rows' => $rows)),
    '#suffix' => theme('pager'),
    '#weight' => -1,
  );
  $output .= drupal_render($item);
  // Render internal FAPI and potential extra form elements.
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Form submit callback.
 */
function drush_311_form_submit($form, &$form_state) {
  // Determine drush_311 entries to update.
  if (isset($form_state['values']['drush_311']['existing'])) {
  	foreach ($form_state['values']['drush_311']['existing'] as $id => $values) {
  		$values['id'] = $id;
  	  $entry = drush_311_load($id);
  	  // Only update changed entries.
  	  if ($entry != $values) {
  	  	// If all fields were emptied out (except id), delete the entry.
  	  	if (count(array_filter($values)) == 1) {
  	  		drush_311_delete($id);
  	  	}
  	  	// Otherwise save the changes.
  	    drush_311_save($values);
  	  }
    }
  }
  // Add new drush_311 entries.
  foreach ($form_state['values']['drush_311']['new'] as $id => $values) {
    if (array_filter($values)) {
    	drush_311_save($values);
    }
  }
}

/**
 * Form constructor for the drush 311 delete confirmation form.
 *
 * @param $entry
 *   The drush 311 entry to delete.
 *
 * @see drush_311_delete_confirm_submit()
 * @ingroup forms
 */
function drush_311_delete_confirm($form, &$form_state, $entry) {
  $form['id'] = array('#type' => 'value', '#value' => $entry['id']);
  $output = confirm_form($form,
    t('Are you sure you want to delete the drush 311 entry?'),
    'admin/config/user-interface/drush_311',
    NULL,
    t('Delete'));
  return $output;
}

/**
 * Process drush_311_delete_confirm() form submissions.
 */
function drush_311_delete_confirm_submit($form, &$form_state) {
  drush_311_delete($form_state['values']['id']);
  drupal_set_message(t('The drush 311 entry was deleted.'));

  $form_state['redirect'] = 'admin/config/user-interface/drush_311';
}
