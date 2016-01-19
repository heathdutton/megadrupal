<?php
/**
 * @file
 * Administration pages and code for the Google Groups module.
 */

/**
 * Lists groups entered in into the site.
 *
 * @return
 *  Page HTML.
 */
function google_groups_list_page() {
  $header = array(t('Group name'), t('Account form'), array('data' => t('Operations'), 'colspan' => '2'));
  $rows = array();
  $settings = variable_get('google_groups_settings', array());

  foreach ($settings['groups'] as $delta => $group) {
    if ($group['register']) {
      // @todo wrap this in theme('item_list');
      $reg = '<li>' . t('List on form: Yes') . '</li>';
      $reg .= '<li>' . t('Default: ') . ($group['register_default'] ? t('On') : t('Off')) . '</li>';
      $reg .= '<li>' . t('Forced: ') . ($group['register_default_forced'] ? t('Yes') : t('No')) . '</li>';
    } 
    else {
      $reg = '<li>' . t('List on form: No') . '</li>';
    }
    $reg = '<ul>' . $reg . '</ul>';
    $row = array(check_plain($group['name']), $reg);

    // Set the edit column.
    $row[] = array('data' => l(t('edit'), 'admin/config/services/googlegroups/edit/' . $delta));
    // Set the delete column.
    $row[] = array('data' => l(t('delete'), 'admin/config/services/googlegroups/delete/' . $delta));

    $rows[] = $row;
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No Google Groups have been created.'), 'colspan' => '4', 'class' => 'message'));
  }

  $renderable = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  return $renderable;
}

/**
 * Administration form.
 *
 * @return
 *  FAPI array.
 */
function google_groups_add_form() {
  $delta = (arg(4) === 'edit' && arg(5)) ? arg(5) : NULL;
  if ($delta) {
    $settings = variable_get('google_groups_settings', array());
    $group    = $settings['groups'][$delta];
  }

  $form['group_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Group Name'),
    '#default_value' => $delta ? $group['name'] : '',
    '#required' => TRUE,
    '#size' => 40,
    '#maxlength' => 100,
    '#description' => t("Your Google Group's human readable name."),
  );
  $form['group_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Group ID'),
    '#default_value' => ($delta ? $group['id'] : ''),
    '#required' => TRUE,
    '#size' => 40,
    '#maxlength' => 64,
    '#description' => t("Your Group ID is usually what comes before the @ symbol in your group email address.  For example, if your group email address is mygroup@googlegroups.com your group ID will be 'mygroup'"),
  );
  $form['register'] = array(
    '#type' => 'fieldset',
    '#title' => t('Registration Form'),
    '#description' => t('Options for subscribing via the registration form.'),
  );
  $form['register']['register'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow subscription during registration'),
    '#default_value' => ($delta ? $group['register'] : ''),
    '#description' => t('This will add a subscription checkbox to the registration form.'),
  );
  $form['register']['default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Checked by default'),
    '#default_value' => ($delta ? $group['register_default'] : ''),
    '#description' => t('If checked, the subscription checkbox on the registration form will be checked by default.'),
  );
  $form['register']['forced'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force default'),
    '#default_value' => ($delta ? $group['register_default_forced'] : ''),
    '#description' => t('If checked, the subscription checkbox on the registration form will be disabled. User will not be able to change it from its default setting. Useful to force user subscription.'),
  );
  $form['delta'] = array(
    '#type' => 'value',
    '#value' => $delta,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save group'),
  );

  $form['#submit'][] = 'google_groups_add_form_submit';

  return $form;
}

function google_groups_add_form_submit($form, &$form_state) {
  $settings = variable_get('google_groups_settings', array());
  $groups   = &$settings['groups'];
  $values   = &$form_state['values'];
  $delta    = check_plain($values['delta']);
  $group    = array();

  $group['name'] = check_plain($values['group_name']);
  $group['id'] = check_plain($values['group_id']);
  $group['domain'] = check_plain($values['group_domain']);
  $group['register'] = (int)$values['register'];
  $group['register_default'] = (int)$values['default'];
  $group['register_default_forced'] = (int)$values['forced'];

  if ($delta) {
    $settings['groups'][$delta] = array_merge($settings['groups'][$delta], $group);
  }
  else {
    $delta = &$settings['next_delta'];
    $settings['groups']['gg' . $delta] = $group;
    $delta++;
  }

  variable_set('google_groups_settings', $settings);
  drupal_set_message(t('Google Group saved.'), 'status');
  $form_state['redirect'] = 'admin/config/services/googlegroups';
  return;
}

/**
 * Menu callback; delete a single Google Group.
 */
function google_groups_delete_confirm(&$form_state) {
  $settings = variable_get('google_groups_settings', array());
  $delta = check_plain(arg(4));
  $form['delta'] = array(
    '#type' => 'value',
    '#value' => $delta,
  );

  $message = t('Are you sure you want to delete the Google Group @group?', array('@group' => $settings['groups'][$delta]['name']));
  $caption = '<p>'. t('This action cannot be undone.') .'</p>';

  return confirm_form($form, $message, 'admin/config/services/googlegroups', $caption, t('Delete'));
}

/**
 * Process Google Group delete confirm submissions.
 */
function google_groups_delete_confirm_submit($form, &$form_state) {
  $settings = variable_get('google_groups_settings', array());

  drupal_set_message(t('The Google Group %name has been deleted.', array('%name' => $settings['groups'][$form_state['delta']]['name'])));
  unset($settings['groups'][$form_state['values']['delta']]);
  variable_set('google_groups_settings', $settings);

  $form_state['redirect'] = 'admin/config/services/googlegroups';
  return;
}
