<?php
/**
 * @file
 * Admin page callbacks for the welcome module.
 */

/**
 * Admin form to configurable welcome message.
 */
function welcome_admin_settings($form) {

  $token_help = t('Available variables are: [site:name], [site:url], [user:name], [user:mail], [site:login-url], [site:url-brief], [user:edit-url], [user:one-time-login-url], [user:cancel-url].');

  $form['#tree'] = TRUE;

  // Get saved messages.
  $query = db_select('welcome_message', 'wm')
    ->fields('wm', array('rid', 'message', 'weight'))
    ->orderBy('weight');
  $result = $query->execute();

  $existing = array();
  foreach ($result as $record) {
    $existing[$record->rid] = array('message' => $record->message, 'weight' => $record->weight);
  }

  // Add fields for any roles.
  $roles = user_roles(TRUE);

  // Sort the roles to match the weight, if form has been saved already.
  if (count($existing) > 0) {
    foreach($existing as $rid => $data ) {
      $sorted_roles[$rid] = $roles[$rid];
    }
  }
  else {
    $sorted_roles = $roles;
  }

  foreach ($sorted_roles as $rid => $role) {
    $form['welcome_message'][$rid]['msg'] = array(
      '#type' => 'textarea',
      '#title' => t('Welcome message for @role', array('@role' => $role)),
      '#rows' => 5,
      '#default_value' => count($existing) > 0 ? $existing[$rid]['message'] : '',
    );

    $form['welcome_message'][$rid]['weight'] = array(
      '#type' => 'weight',
      '#delta' => 10,
      '#default_value' => count($existing) > 0  ? $existing[$rid]['weight'] : 10,
    );

    if (count($existing) > 0) {
      if ($existing[$rid] != FALSE) {
        $form['welcome_message'][$rid]['current'] = array(
          '#type' => 'hidden',
          '#value' => $rid,
        );
      }
    }
  }

  // Submit button.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Changes'),
  );

  return $form;
}

/**
 * Save message, weight and settings for Welcome module.
 */
function welcome_admin_settings_submit($form, &$form_state) {
  foreach ($form_state['values']['welcome_message'] as $id => $data) {
    $table = 'welcome_message';
    $record = array(
      'rid' => $id,
      'message' => $data['msg'],
      'weight' => $data['weight'],
    );
    if (empty($data['current']) || !isset($data['current'])) {
      drupal_write_record($table, $record);
    }
    else {
      drupal_write_record($table, $record, array('rid'));
    }
    drupal_set_message(t('Saved welcome messages'));
  }
}

/**
 * Returns HTML for the welcome admin form.
 */
function theme_welcome_admin_settings($variables) {
  $form = $variables['form'];
  $rows = array();
  foreach (element_children($form['welcome_message']) as $id) {
    $form['welcome_message'][$id]['weight']['#attributes']['class'] = array('text-welcome-order-weight');
    $rows[] = array(
      'data' => array(
        drupal_render($form['welcome_message'][$id]['msg']),
        drupal_render($form['welcome_message'][$id]['weight']),
      ),
      'class' => array('draggable'),
    );
  }
  $header = array(t('Message'), t('Weight'));
  $output = theme('table',
    array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array('id' => 'text-welcome-order'),
    )
  );
  $output .= drupal_render_children($form);

  drupal_add_tabledrag('text-welcome-order', 'order', 'sibling', 'text-welcome-order-weight');

  return $output;
}
