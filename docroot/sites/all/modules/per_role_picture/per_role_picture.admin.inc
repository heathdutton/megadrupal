<?php

/**
 * @file
 * Admin page callback file for the per_role_picture module.
 */

/**
 * Form builder; Return form for upload user role picture.
 */
function per_role_picture_form($form, &$form_state, $role) {
  if ($role->rid == DRUPAL_ANONYMOUS_RID) {
    drupal_not_found();
    drupal_exit();
  }
  $picture = per_role_picture_load($role->rid);
  $form['rid'] = array(
    '#type' => 'value',
    '#value' => $role->rid,
  );

  $form['user_role_picture'] = array(
    '#title' => t('Picture'),
    '#type' => 'managed_file',
    '#progress_message' => t('Please wait...'),
    '#upload_validators' => array(
      'file_validate_extensions' => array('png jpg gif'),
      'file_validate_size' => array(
        variable_get('user_picture_file_size', '30') * 1024,
      ),
    ),
    '#description' => t('The uploaded image will be display on user profile page based on user role.'),
    '#default_value' => $picture ? $picture->fid : '',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Submit handler for upload picture form.
 */
function per_role_picture_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $fid = $values['user_role_picture'];
  $rid = $values['rid'];

  // If picture support is enabled, check whether the picture directory exists.
  $picture_path = file_default_scheme() . '://' . variable_get('user_picture_path', 'pictures');
  $role_picture_path = $picture_path . '/role';
  if (!file_prepare_directory($role_picture_path, FILE_CREATE_DIRECTORY)) {
    form_set_error('user_role_picture', t('The directory %directory does not exist or is not writable.', array('%directory' => $role_picture_path)));
    watchdog('file system', 'The directory %directory does not exist or is not writable.', array('%directory' => $role_picture_path), WATCHDOG_ERROR);
  }

  $picture = per_role_picture_load($rid);
  if (empty($fid) && !empty($picture)) {
    $uploaded_file_obj = file_load($picture->fid);
    per_role_picture_role_delete($rid);
    file_usage_delete($uploaded_file_obj, 'per_role_picture', 'user', $rid);
  }
  if (!empty($fid)) {
    $file = file_load($fid);

    // Move the temporary file into the final location.
    if ($file = file_move($file, $role_picture_path)) {
      $file->status = FILE_STATUS_PERMANENT;
      $file_saved = file_save($file);
      file_usage_add($file, 'per_role_picture', 'user', $rid);
      $record = array(
        'fid' => $file_saved->fid,
      );
      if ($file_saved->status === FILE_STATUS_PERMANENT) {
        db_merge('per_role_picture')->key(array('rid' => $rid))->fields($record)->execute();
      }
    }
  }

  drupal_set_message(t('The configuration options have been saved.'));
  $form_state['redirect'] = 'admin/people/permissions/roles';
}

/**
 * Return form to set the weight for a role to display picture in priority.
 */
function per_role_picture_role_order_settings($form, $form_state) {
  $roles = user_roles();
  $form['roles'] = array(
    '#tree' => TRUE,
  );
  $sorted_roles = array();
  $role_order = variable_get('per_role_picture_role_order', NULL);
  if (isset($role_order)) {
    foreach ($role_order as $key => $value) {
      $sorted_roles[$key] = $roles[$key];
    }
    $roles = $sorted_roles;
  }

  $order = 0;
  foreach ($roles as $rid => $name) {
    if ($rid == DRUPAL_ANONYMOUS_RID) {
      continue;
    }
    $form['roles'][$rid]['#role'] = (object) array(
      'rid' => $rid,
      'name' => $name,
      'weight' => $order,
    );
    $form['roles'][$rid]['#weight'] = $order;
    $form['roles'][$rid]['weight'] = array(
      '#type' => 'textfield',
      '#title' => t('Weight for @title', array('@title' => $name)),
      '#title_display' => 'invisible',
      '#size' => 4,
      '#default_value' => $order,
      '#attributes' => array('class' => array('role-weight')),
    );
    $order++;
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save order'),
  );

  return $form;
}

/**
 * Submit handler for set role weight.
 */
function per_role_picture_role_order_settings_submit($form, &$form_state) {
  $role_weight = array();
  $values = $form_state['values'];
  foreach ($values['roles'] as $rid => $weight) {
    $role_weight[$rid] = $weight['weight'];
  }
  asort($role_weight);
  variable_set('per_role_picture_role_order', $role_weight);
  drupal_set_message(t('The role configuration have been saved.'));
}

/**
 * Returns HTML for the role picture order.
 */
function theme_per_role_picture_role_order_settings($variables) {
  $form = $variables['form'];

  $header = array(t('Name'), t('Weight'), t('Picture'));
  foreach (element_children($form['roles']) as $rid) {
    $picture = t('Not uploaded');
    $name = $form['roles'][$rid]['#role']->name;
    // Get picture for a role by role id.
    $picture_obj = per_role_picture_load($rid);
    // Display picture for a role if exist.
    if ($picture_obj) {
      $file = file_load($picture_obj->fid);
      $alt = t("@role's picture", array('@role' => $name));

      if (module_exists('image') && file_valid_uri($file->uri)) {
        $picture = theme('image_style', array(
          'style_name' => 'thumbnail',
          'path' => $file->uri,
          'title' => $alt,
          )
        );
      }
      else {
        $picture = theme('image', array(
          'path' => $file->uri,
          'title' => $alt,
          )
        );
      }
    }
    $row = array();

    $row[] = check_plain($name);
    $row[] = drupal_render($form['roles'][$rid]['weight']);
    $row[] = $picture;

    $rows[] = array('data' => $row, 'class' => array('draggable'));
  }

  drupal_add_tabledrag('per-role-picture', 'order', 'sibling', 'role-weight');

  $output = theme('table',
    array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array(
        'id' => 'per-role-picture',
      ),
    )
  );
  $output .= drupal_render_children($form);

  return $output;
}
