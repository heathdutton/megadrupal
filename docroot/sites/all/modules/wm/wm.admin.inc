<?php

/**
 * System settings form for webmaster toolbar.
 *
 * @param $form
 * @param $form_state
 * @return array
 */
function wm_settings_form($form, &$form_state) {

  $form['placeholder'] = array(
    '#markup' => '<p>Here there will be wm settings.</p>',
  );

  $form = system_settings_form($form);

  return $form;
}

/**
 * @param $form
 * @param $form_state
 */
function wm_toolbar_permissions($form, &$form_state) {
  // Retrieve role names for columns.
  $role_names = user_roles();

  // remove anonymous and generic authenticated
  unset($role_names[1]);
  unset($role_names[2]);

  // Fetch permissions for selected role.
  $role_permissions = _wm_role_permissions($role_names);

  // Store $role_names for use when saving the data.
  $form['role_names'] = array(
    '#type' => 'value',
    '#value' => $role_names,
  );

  $options = array();
  $blocks = module_invoke_all('wm_item');
  //kpr($blocks);

  foreach ($blocks as $key => $block) {
    if (count($block['#items']) > 0) {
      $form['permission'][] = array(
        '#markup' => $block['#title'],
        '#id' => $key,
      );

      foreach ($block['#items'] as $key2 => $item) {
        $options[$key2] = '';
        $form['permission'][$key2] = array(
          '#type' => 'item',
          '#markup' => $item['title'],
          '#description' => $item['help'],
        );
        foreach ($role_names as $rid => $name) {
          // Builds arrays for checked boxes for each role
          if (isset($role_permissions[$rid][$key2])) {
            $status[$rid][] = $key2;
          }
        }
      }

    }
  }

  // Have to build checkboxes here after checkbox arrays are built
  foreach ($role_names as $rid => $name) {
    $form['checkboxes'][$rid] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => isset($status[$rid]) ? $status[$rid] : array(),
      '#attributes' => array('class' => array('rid-' . $rid)),
    );
    $form['role_names'][$rid] = array('#markup' => check_plain($name), '#tree' => TRUE);
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save permissions'));

  return $form;
}

/**
 * @param $form
 * @param $form_state
 */
function wm_toolbar_permissions_submit($form, &$form_state) {
  foreach ($form_state['values']['role_names'] as $rid => $name) {
    wm_role_change_permissions($rid, $form_state['values'][$rid]);
  }

  drupal_set_message(t('The changes have been saved.'));

  // Clear the cached pages and blocks.
  cache_clear_all();
}

/**
 * @param $variables
 * @return string
 */
function theme_wm_toolbar_permissions($variables) {
  $form = $variables['form'];

  $roles = user_roles();
  foreach (element_children($form['permission']) as $key) {
    $row = array();
    // Module name
    if (is_numeric($key)) {
      $row[] = array(
        'data' => drupal_render($form['permission'][$key]),
        'class' => array('module'),
        'id' => 'module-' . $form['permission'][$key]['#id'],
        'colspan' => count($form['role_names']['#value']) + 1
      );
    }
    else {
      // Permission row.
      $row[] = array(
        'data' => drupal_render($form['permission'][$key]),
        'class' => array('permission'),
      );
      foreach (element_children($form['checkboxes']) as $rid) {
        $form['checkboxes'][$rid][$key]['#title'] = $roles[$rid] . ': ' . $form['permission'][$key]['#markup'];
        $form['checkboxes'][$rid][$key]['#title_display'] = 'invisible';
        $row[] = array('data' => drupal_render($form['checkboxes'][$rid][$key]), 'class' => array('checkbox'));
      }
    }
    $rows[] = $row;
  }
  $header[] = (t('Permission'));
  foreach (element_children($form['role_names']) as $rid) {
    $header[] = array('data' => drupal_render($form['role_names'][$rid]), 'class' => array('checkbox'));
  }
  $output = theme('system_compact_link');
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'permissions')));
  $output .= drupal_render_children($form);
  return $output;
}

