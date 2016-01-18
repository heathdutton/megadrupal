<?php
// $Id$

/**
 * @file
 * All administration functions, forms, menu hooks.
 */

function arbitrary_content_administer() {
  $content_items = entity_load('arbitrary_content');

  $header = array(t('Name'), t('Description'), t('Type'), array('data' => t('Operations'), 'colspan' => '4'));
  $rows = array();
  foreach ($content_items as $ci) {
    $field_info = field_info_instances('arbitrary_content', $ci->name);
    switch ($ci->status) {
      case ENTITY_CUSTOM:
        $delete = l(t('Delete'), 'admin/structure/ac/manage/' . $ci->name . '/delete');
        break;
      case ENTITY_OVERRIDDEN:
        $delete = l(t('Revert'), 'admin/structure/ac/manage/' . $ci->name . '/delete');
        break;
      case ENTITY_IN_CODE:
        $delete = 'Default';
        break;
      case ENTITY_FIXED:
        $delete = 'Fixed';
        break;
    }

    if (!empty($field_info)) {
      $field_content = l(t('Edit Field Content'), 'admin/structure/ac/manage/' . $ci->name . '/edit_field_data');
    }
    else {
      $field_content = 'Edit Field Content';
    }
    $rows[] = array(
      $ci->name,
      $ci->description,
      $ci->ac_type,
      l(t('Edit'), 'admin/structure/ac/manage/' . $ci->name . '/edit'),
      l(t('Fields'), 'admin/structure/ac/manage/' . $ci->name . '/fields'),
      $field_content,
      $delete,
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

function arbitrary_content_add_form() {
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Content name'),
    '#description' => t('This is the unique name of the Content. It must contain only alphanumeric or underscores. It is used to identify the content internally, to generate unique template names and URL paths.'),
    '#required' => TRUE,
    '#maxlength' => 32,
  );

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Content description'),
    '#description' => t('This description will appear on the administrative UI.'),
  );

  $form['ac-type'] = array(
    '#type' => 'radios',
    '#title' => t('Content type'),
    '#description' => t('The type is how the content will expose itself. Either as a block or a page.'),
    '#options' => array('Block', 'Page'),
    '#default_value' => 0
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  return $form;
}

function arbitrary_content_add_form_validate($form, &$form_state) {
  $name = $form_state['values']['name'];

  // Content name must be alphanumeric or underscores.
  if (preg_match('/[^a-zA-Z0-9_]/', $name))
    form_error($form['name'], t('The name must only use alphanumeric or underscores.'));

  // Content name must be unique.

  $content = arbitrary_content_load($form_state['values']['name']);

  if ($content)
    form_error($form['name'], t('The name must be unique.'));
}

function arbitrary_content_add_form_submit($form, &$form_state) {
  $content = (object) NULL;

  $content->ac_type = $form_state['values']['ac-type'] ? 'page' : 'block';
  $content->name = $form_state['values']['name'];
  $content->description = $form_state['values']['description'];
  $content->export_type = NULL;
  arbitrary_content_save_content($content);
  drupal_theme_rebuild();

  // TODO: This is probably overkill? :)
  entity_info_cache_clear();
  menu_rebuild();

  drupal_goto('admin/structure/ac/list');
}

function arbitrary_content_edit_form($form, &$form_state, $ac) {
  $form_state['ac'] = $ac;

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Content name'),
    '#required' => TRUE,
    '#attributes' => array('readonly' => 'readonly'),
    '#default_value' => $ac->name,
  );

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Content description'),
    '#description' => t('This description will appear on the administrative UI.'),
    '#default_value' => $ac->description,
  );

  $form['ac-type'] = array(
    '#type' => 'textfield',
    '#title' => t('Content type'),
    '#default_value' => ($ac->ac_type == 'block') ? 'Block' : 'Page',
    '#attributes' => array('readonly' => 'readonly'),
  );

  if ($ac->ac_type == 'page') {
    if (module_exists('path')) {
      $form['page-path'] = array(
        '#type' => 'textfield',
        '#title' => t('Page Path Alias'),
        '#default_value' => $ac->data['path'],
      );
    }

    $form['page-title'] = array(
      '#type' => 'textfield',
      '#title' => t('Page Title'),
      '#default_value' => $ac->data['title'],
    );
  }

  if ($ac->ac_type == 'block') {
    $form['block-title'] = array(
      '#type' => 'textfield',
      '#title' => t('Block Title'),
      '#default_value' => $ac->data['title'],
    );
  }


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  return $form;
}

function arbitrary_content_edit_form_validate($form, &$form_state) {
  $ac = $form_state['ac'];
  if ($ac->ac_type == 'page') {
    $path = $form_state['values']['page-path'];
    if (preg_match('/[^a-zA-Z0-9_\/\-]/', $path)) {
      form_error($form['name'], t('The name must only use alphanumeric, underscores, forward slashes and dashes.'));
    }
  }
}

function arbitrary_content_edit_form_submit($form, &$form_state) {
  $ac = $form_state['ac'];

  $ac->description = $form_state['values']['description'];
  if ($ac->ac_type == 'page') {
    $ac->data = array(
      'path' => $form_state['values']['page-path'],
      'title' => $form_state['values']['page-title']
    );
  }

  if ($ac->ac_type == 'block') {
    $ac->data = array(
      'title' => $form_state['values']['block-title']
    );
  }

  arbitrary_content_save_content($ac);

  if ($ac->ac_type == 'page'){
    if (!empty($ac->data['path']) && module_exists('path')) {
      $path_info = array('source' => 'ac/' . $ac->name, 'alias' => $ac->data['path']);
      path_save($path_info);;
    }
  }

  drupal_set_message(t('Arbitrary Content Settings Saved.'));
  drupal_goto('admin/structure/ac');
}

function arbitrary_content_edit_field_data_form($form, &$form_state, $ac) {
  $form_state['ac'] = $ac;
  $form['#attributes'] = array('enctype' => "multipart/form-data");

  field_attach_form('arbitrary_content', $ac, $form, $form_state);

  $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => t('Save'),
    '#weight' => 1000
  );

  return $form;
}

function arbitrary_content_edit_field_data_form_validate($form, &$form_state) {
  $ac = $form_state['ac'];

  field_attach_form_validate('arbitrary_content', $ac, $form, $form_state);
}

function arbitrary_content_edit_field_data_form_submit($form, &$form_state) {
  $ac = $form_state['ac'];

  field_attach_submit('arbitrary_content', $ac, $form, $form_state);

  drupal_set_message($ac->name . ' Saved.');

  $ac->save();
}

function arbitrary_content_delete_confirm($form, &$form_state, $ac) {
  $form_state['ac'] = $ac;
  $form = array();

  $cancel = 'admin/structure/ac/list';
  if (!empty($_REQUEST['cancel']))
    $cancel = $_REQUEST['cancel'];

  return confirm_form($form,
    t('Are you sure you want to delete the content %name?', array('%name' => $ac->name)),
    $cancel,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel'));
}

function arbitrary_content_delete_confirm_submit($form, &$form_state) {
  $form_state['ac']->delete();
  drupal_set_message(t('The content has been deleted.'));
  $form_state['redirect'] = 'admin/structure/ac/list';
}
