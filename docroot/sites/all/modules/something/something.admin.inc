<?php


function something_type_list() {
  $something = something_entity_info();
  $field_ui = module_exists('field_ui');
  $headers = array(
    array('data' => t('Name'), 'width' => '40%'),
    array('data' => t('Operations'), 'colspan' => $field_ui ? 5 : 3));
  $rows = array();
  foreach ($something['something']['bundles'] as $type => $something_type) {
    $url = 'admin/structure/something/manage/' . $type;
    $row = array(l($something_type['label'], $url));
    $row[] = l(t('edit'), $url . '/edit');
    if ($field_ui) {
      $row[] =  l(t('manage fields'), $url . '/fields');
      $row[] =  l(t('display fields'), $url . '/display');
    }
    $row[] =  l(t('delete'), $url . '/delete');
    $rows[] = $row;
  }
  $output = array(
    '#theme' => 'table',
    '#header' => $headers,
    '#rows' => $rows,
    '#empty' => t('No Something entity types available. <a href="@link">Add Something entity type</a>.', array('@link' => url('admin/structure/something/add'))),
  );

  return $output;
}

/**
 * Something entity type bundle settings form.
 *
 * @param $something_type
 *   Something entity type machine name. If this is not provided, assume that we're
 *   creating a new Something entity type.
 */
function something_type_form($form, &$form_state, $something_type = array(), $op = 'edit') {
  $form['#write_record_keys'] = array();

  if ($something_type) {
    $something_type = (object) $something_type;
    if (empty($something_type->in_code_only)) {
      $form['#write_record_keys'][] = 'type';
    }
  }
  else {
    $something_type = (object) array(
      'type' => '',
      'name' => '',
      'help' => '',
      'description' => '',
    );
  }

  $form['labels'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('something-type-form-table'),
    ),
    '#suffix' => '<div class="clearfix"></div>',
  );
  $form['labels']['name'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Label'),
    '#default_value' => $something_type->name,
    '#size'          => 40,
    '#required'      => TRUE,
  );
  $form['labels']['something_type'] = array(
    '#type'          => 'machine_name',
    '#default_value' => $something_type->type,
    '#maxlength' => 32,
    '#disabled'      => $something_type->type,
    '#machine_name' => array(
      'source' => array('labels', 'name'),
      'exists' => 'something_type_load',
    ),
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => 'Description',
    '#default_value' => $something_type->description,
  );
  $form['help'] = array(
    '#type' => 'textarea',
    '#title' => 'Help text',
    '#default_value' => $something_type->help,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#weight' => 100,
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit data from bundle settings page.
 */
function something_type_form_submit($form, &$form_state) {
  $something_type = $form_state['values']['something_type'];
  $record = array(
    'type'   => $something_type,
    'name' => $form_state['values']['name'],
    'help' => $form_state['values']['help'],
    'description' => $form_state['values']['description'],
  );
  something_type_save($record, $form['#write_record_keys']);
  $form_state['redirect'] = "admin/structure/something";

  drupal_set_message(t('The %something_type Something entity type has been saved.', array('%something_type' => $something_type)));
}

/**
 * Menu callback; deletes a single Something entity type.
 */
function something_type_delete_confirm($form, &$form_state, $something_type) {
  $form['something_type'] = array('#type' => 'value', '#value' => $something_type->type);
  $form['name'] = array('#type' => 'value', '#value' => $something_type->name);

  $message = t('Are you sure you want to delete the %name Something entity type?', array('%name' => $something_type->name));
  $caption = '';

  $query = new EntityFieldQuery;
  $num_somethings = $query
    ->entityCondition('entity_type', 'something')
    ->propertyCondition('type', $something_type->type)
    ->count()
    ->execute();

  if ($num_somethings) {
    $caption .= '<p>' . format_plural($num_somethings,
      'The %name Something entity type is used by 1 Something entity on your site. If you remove this Something entity type, you will not be able to edit  %name Something entity and they may not display correctly.',
      'The %name Something entity type is used by @count Something entity on your site. If you remove %name, you will not be able to edit %name Something entity and they may not display correctly.',
      array('%name' => $something_type->name, '@count' => $num_somethings)) . '</p>';
  }

  $caption .= '<p>' . t('This action cannot be undone.') . '</p>';

  return confirm_form($form, $message, 'admin/structure/something', $caption, t('Delete'));
}

/**
 * Process Something entity type delete confirm submissions.
 */
function something_type_delete_confirm_submit($form, &$form_state) {
  something_type_delete($form_state['values']['something_type']);

  $t_args = array('%name' => $form_state['values']['name']);
  drupal_set_message(t('The %name Something entity type has been deleted.', $t_args));
  watchdog('Something entity', 'Deleted the %name Something entity type.', $t_args, WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/structure/something';
}
