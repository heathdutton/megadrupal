<?php
/**
 * @file
 * Virtual Site - administration forms.
 */

/**
 * Form constructor for the virtual site list form.
 *
 * Path: admin/structure/virtual-site
 *
 * @see virtual_site_menu()
 * @see theme_virtual_site_list_form()
 * @see virtual_site_list_form_submit()
 *
 * @ingroup forms
 */
function virtual_site_list_form($form, &$form_state) {
  foreach (_virtual_site_load(NULL, TRUE) as $site) {
    $form['sites'][$site->sid] = array(
      '#tree' => TRUE,
      'name' => array(
        '#type' => 'item',
        '#markup' => $site->name,
      ),
      'status' => array(
        '#type' => 'checkbox',
        '#default_value' => $site->status,
      ),
      'weight' => array(
        '#type' => 'weight',
        '#default_value' => $site->weight,
        '#delta' => 50,
      ),
      'edit' => array(
        '#type' => 'link',
        '#title' => t('edit'),
        '#href' => 'admin/structure/virtual-site/'. $site->sid .'/edit',
      ),
      'delete' => array(
        '#type' => 'link',
        '#title' => t('delete'),
        '#href' => 'admin/structure/virtual-site/'. $site->sid .'/delete',
      ),
    );
  }

  $form['add'] = array(
    '#type' => 'link',
    '#title' => t('Add new virtual site'),
    '#href' => 'admin/structure/virtual-site/add',
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );

  $form['#theme'] = 'virtual_site_list_form';
  $form['#submit'][] = 'virtual_site_list_form_submit';
  return system_settings_form($form);
}

/**
 * Returns HTML for the conditions list form.
 * Turn the form into a table with tablesorting on.
 *
 * @ingroup themeable
 */
function theme_virtual_site_list_form($variables) {
  $form = &$variables['form'];

  $rows = array();
  if (isset($form['sites'])) {
    foreach (element_children($form['sites']) as $sid) {
      $element = &$form['sites'][$sid];
      $element['weight']['#attributes']['class'][] = 'weight';
      $row = array();
      $row[] = render($element['name']);
      $row[] = array('data' => render($element['status']), 'class' => 'checkbox');
      $row[] = render($element['weight']);
      $row[] = render($element['edit']);
      $row[] = render($element['delete']);
      $rows[] = array(
        'data' => $row,
        'class' => array('draggable'),
        'id' => 'virtual-sites-list-row-'. $sid,
        );
    }
  }

  $header = array(
    t('Site'),
    array('data' => t('Enabled'), 'class' => 'checkbox'),
    t('Weight'),
    array('data' => t('Operations'), 'colspan' => '2'),
  );

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No virtual sites are defined.'), 'colspan' => '6'));
  }

  drupal_add_tabledrag('order-virtual-sites', 'order', 'sibling', 'weight');
  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(
      'id' => 'order-virtual-sites',
      'style' => 'width: 100%',
      ),
    ));
  $output .= drupal_render_children($form);
  $output .= theme('pager');
  return $output;
}

/**
 * Form submission handler for virtual_site_list_form().
 */
function virtual_site_list_form_submit($form, &$form_state) {
  foreach (element_children($form['sites']) as $sid) {
    db_update('virtual_sites')
      ->fields(array(
        'status' => $form_state['values'][$sid]['status'],
        'weight' => $form_state['values'][$sid]['weight'],
        ))
      ->condition('sid', $sid)
      ->execute();
  }
}

/**
 * Form constructor for the virtual site edit form.
 *
 * Path: admin/structure/virtual-site/add
 * Path: admin/structure/virtual-site/%/edit
 *
 * @param $sid
 *   (Optional) The virtual site to edit.
 *
 * @see virtual_site_menu()
 * @see virtual_site_edit_form_submit()
 *
 * @ingroup forms
 */
function virtual_site_edit_form($form, &$form_state, $sid = NULL) {
  if (isset($sid)) {
    $form['sid'] = array(
      '#type' => 'hidden',
      '#value' => $sid,
    );
    $site = _virtual_site_load($sid);
    drupal_set_title(t('Edit virtual site %name', array('%name' => check_plain($site->name))), PASS_THROUGH);
  }
  else {
    drupal_set_title(t('Add virtual site'));
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Name to identify the virtual site with.'),
    '#required' => TRUE,
    '#maxlength' => 255,
    '#default_value' => isset($site->name) ? $site->name : '',
  );

  $context = isset($site->conditions) ? $site->conditions : array();
  $form = array_merge($form, module_invoke('virtual_site_condition', 'selection_form', $context));

  $features = _virtual_site_features();
  if (count($features)) {
    foreach ($features as $feature_id => $feature) {
      $form['features'][$feature_id] = array(
        '#type' => 'fieldset',
        '#title' => check_plain($feature['name']),
        '#description' => check_plain($feature['description']),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $context = array('sid' => $sid);
      if (isset($site->features[$feature_id])) {
        $context = array_merge($context, (array) $site->features[$feature_id]);
      }
      $form['features'][$feature_id] = array_merge($form['features'][$feature_id], $feature['form']($context));
      $form['#validate'][] = $feature['validate'];
    }
  }
  else {
    $form['add'] = array(
      '#type' => 'link',
      '#title' => t('Add a feature module'),
      '#href' => 'admin/modules',
      '#prefix' => '<p>',
      '#suffix' => '</p>',
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  $form['#submit'][] = 'virtual_site_edit_form_submit';
  return $form;
}

/**
 * Form submission handler for virtual_site_edit_form().
 */
function virtual_site_edit_form_submit($form, &$form_state) {
  $site = isset($form['sid']['#value']) ? _virtual_site_load($form['sid']['#value']) : new stdClass();
  $site->name = $form_state['values']['name'];
  $site->conditions = module_invoke('virtual_site_condition', 'selection_submit', $form, $form_state);
  $site->features = array();
  foreach (_virtual_site_features() as $feature_id => $feature) {
    $site->features[$feature_id] = $form_state['values'][$feature_id];
  }
  _virtual_site_save($site);
  drupal_set_message(t('The virtual site %name has been saved.', array('%name' => $site->name)));
  $form_state['redirect'] = 'admin/structure/virtual-site';
}

/**
 * Form constructor for the virtual site delete form.
 *
 * Path: admin/structure/virtual-site/%/delete
 *
 * @param $sid
 *   (Optional) The virtual site to delete.
 *
 * @see virtual_site_menu()
 * @see virtual_site_delete_form_submit()
 *
 * @ingroup forms
 */
function virtual_site_delete_form($form, &$form_state, $sid) {
  $form['sid'] = array(
    '#type' => 'hidden',
    '#value' => $sid,
  );

  $site = _virtual_site_load($sid);
  return confirm_form($form,
    t('Are you sure you wan to delete the virtual site "!name"?', array('!name' => $site->name)),
    'admin/structure/virtual-site',
    NULL,
    t('Delete')
    );
}

/**
 * Form submission handler for virtual_site_delete_form().
 */
function virtual_site_delete_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    virtual_site_delete($form_state['values']['sid']);
    drupal_set_message(t('The virtual site has been deleted.'));
  }
  $form_state['redirect'] = 'admin/structure/virtual-site';
}
