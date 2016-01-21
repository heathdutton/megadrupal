<?php
/**
 * Function for altering forms at admin/structure/types/manage/*.
 */
function content_type_extras_node_type_form(&$form) {
// Not sure what this js was for (or if it ever actually existed), but I wanted
// to keep the code here just in case I remember why it was here. :)
//  drupal_add_js(drupal_get_path('module', 'content_type_extras') . '/js/content_type_extras.admin.js');

  // We need to set the weights of the 'Preview' button radios
  $form['submission']['node_preview']['#weight'] = 2;

  $type = $form['type']['#default_value'];

  // We need to check whether the description field should be required or not.
  $form['description']['#required'] = content_type_extras_get_setting('content_type_extras_descriptions_required', $type);

  // Add Preview button text option
  $form['submission']['content_type_extras_preview_button'] = array(
    '#type'          => 'textfield',
    '#title'         => "'Preview' button value",
    '#description'   => t(''),
    '#default_value' => content_type_extras_get_setting('content_type_extras_preview_button', $type),
    '#weight'        => $form['submission']['node_preview']['#weight'] + 1,
    '#states'        => array(
      'invisible' => array(
        'input[name=node_preview]' => array('value' => '0'),
      ),
    ),
  );

  // Add the option to have a "Save and New" button added to the content type
  $form['submission']['content_type_extras_save_and_new'] = array(
    '#type'        => 'radios',
    '#title'       => "'Save and New' button",
    '#description' => t('If enabled, a button will be added to the bottom of node edit forms that will quickly allow the administrator to add a new content of the same type.'),
    '#options'     => array(
      t('Disabled'),
      t('Enabled'),
    ),
    '#default_value' => content_type_extras_get_setting('content_type_extras_save_and_new', $type),
    '#weight'        => $form['submission']['node_preview']['#weight'] + 2,
  );
  $form['submission']['content_type_extras_save_and_new_button'] = array(
    '#type'          => 'textfield',
    '#title'         => "'Save and New' button value",
    '#description'   => t(''),
    '#default_value' => content_type_extras_get_setting('content_type_extras_save_and_new_button', $type),
    '#weight'        => $form['submission']['content_type_extras_save_and_new']['#weight'] + 1,
    '#states'        => array(
      'visible' => array(
        'input[name=content_type_extras_save_and_new]' => array('value' => '1'),
      ),
    ),
  );

  // Add the option to have a "Save and Edit" button added to the content type
  $form['submission']['content_type_extras_save_and_edit'] = array(
    '#type'        => 'radios',
    '#title'       => "'Save and Edit' button",
    '#description' => t('If enabled, a button will be added to the bottom of node edit forms that will quickly allow the administrator to save the current node and continue editing it.'),
    '#options'     => array(
      t('Disabled'),
      t('Enabled'),
    ),
    '#default_value' => content_type_extras_get_setting('content_type_extras_save_and_edit', $type),
    '#weight'        => $form['submission']['node_preview']['#weight'] + 4,
  );
  $form['submission']['content_type_extras_save_and_edit_button'] = array(
    '#type'          => 'textfield',
    '#title'         => "'Save and Edit' button value",
    '#description'   => t(''),
    '#default_value' => content_type_extras_get_setting('content_type_extras_save_and_edit_button', $type),
    '#weight'        => $form['submission']['content_type_extras_save_and_edit']['#weight'] + 1,
    '#states'        => array(
      'visible' => array(
        'input[name=content_type_extras_save_and_edit]' => array('value' => '1'),
      ),
    ),
  );

  // Add the option to have a "Cancel" button added to the content type
  $form['submission']['content_type_extras_cancel'] = array(
    '#type'        => 'radios',
    '#title'       => "Cancel button",
    '#description' => t('If enabled, a button will be added to the bottom of node edit forms that will allow the administrator to go back to the previous page without saving any changes.<br><strong>NOTE:</strong> This feature requires that the admin has javascript enabled.'),
    '#options'     => array(
      t('Disabled'),
      t('Enabled'),
    ),
    '#default_value' => content_type_extras_get_setting('content_type_extras_cancel', $type),
    '#weight'        => $form['submission']['node_preview']['#weight'] + 6,
  );

  // Set weight of help text area
  $form['submission']['help']['#weight'] = $form['submission']['node_preview']['#weight'] + 7;

  // Set 'Title field label'
  $form['submission']['title_label']['#default_value'] = content_type_extras_get_setting('title_label', $type);
  // Set 'Preview' button default
  $form['submission']['node_preview']['#default_value'] = content_type_extras_get_setting('node_preview', $type);
  // Set 'Display author and date information.'
  $form['display']['node_submitted']['#default_value'] = content_type_extras_get_setting('node_submitted', $type);

  // Set 'Publishing options'
  $form['workflow']['node_options']['#default_value'] = content_type_extras_get_setting('node_options', $type);

  if (module_exists('comment')) {
    // A new content type is being added
    if (empty($form['#node_type']->name)) {
      $comment_settings = content_type_extras_get_setting('comment', $type);
      $form['comment']['comment']['#default_value'] = $comment_settings['comment'];
      $form['comment']['comment_default_mode']['#default_value'] = $comment_settings['default_mode'];
      $form['comment']['comment_default_per_page']['#default_value'] = $comment_settings['default_per_page'];
      $form['comment']['comment_anonymous']['#default_value'] = $comment_settings['anonymous'];
      $form['comment']['comment_subject_field']['#default_value'] = $comment_settings['subject_field'];
      $form['comment']['comment_form_location']['#default_value'] = $comment_settings['form_location'];
      $form['comment']['comment_preview']['#default_value'] = $comment_settings['preview'];
    }
  }

  if (module_exists('xmlsitemap')) {
    // XML Sitemap stores its variables a little differently, so we have to adjust for it here.
    $xmlsitemap_settings = content_type_extras_get_setting('xmlsitemap_settings', 'node_' . $type);
    $form['xmlsitemap']['status']['#default_value'] = $xmlsitemap_settings['status'];
    $form['xmlsitemap']['priority']['#default_value'] = $xmlsitemap_settings['priority'];
  }

  if (module_exists('scheduler')) {
    // Scheduler stores its variables a little differently, so we have to adjust for it here.
    $scheduler_settings = content_type_extras_get_setting('scheduler_settings', 'node_' . $type);

    $form['scheduler']['publish']['scheduler_publish_enable']['#default_value'] = $scheduler_settings['publish_enable'];
    $form['scheduler']['publish']['scheduler_publish_touch']['#default_value'] = $scheduler_settings['publish_touch'];
    $form['scheduler']['publish']['scheduler_publish_require']['#default_value'] = $scheduler_settings['publish_require'];
    $form['scheduler']['publish']['scheduler_publish_revision']['#default_value'] = $scheduler_settings['publish_revision'];
    $form['scheduler']['unpublish']['scheduler_unpublish_enable']['#default_value'] = $scheduler_settings['unpublish_enable'];
    $form['scheduler']['unpublish']['scheduler_unpublish_require']['#default_value'] = $scheduler_settings['unpublish_require'];
    $form['scheduler']['unpublish']['scheduler_unpublish_revision']['#default_value'] = $scheduler_settings['unpublish_revision'];
  }

  // Get all available user roles
  $roles = user_roles();
  $admin_role = variable_get('user_admin_role', 0);
  if ($admin_role != 0) {
    $roles[$admin_role] .= t(' <em>(administrator role)</em>');
  }

  $create_roles = array();
  $edit_roles = array();
  $delete_roles = array();
  $edit_own_roles = array();
  $delete_own_roles = array();

  // If we are on an existing content type form
  if (!empty($form['name']['#default_value'])) {
    $create_roles = user_roles(FALSE, 'create ' . $form['#node_type']->type . ' content');
    $edit_roles   = user_roles(FALSE, 'edit any ' . $form['#node_type']->type . ' content');
    $delete_roles = user_roles(FALSE, 'delete any ' . $form['#node_type']->type . ' content');
    $delete_own_roles = user_roles(FALSE, 'delete own ' . $form['#node_type']->type . ' content');
    $edit_own_roles = user_roles(FALSE, 'edit own ' . $form['#node_type']->type . ' content');
  }
  // We are creating a new content type
  else {
    $user_permissions = content_type_extras_get_setting('user_permissions', '');

    $selected_perms = content_type_extras_get_selected_roles($user_permissions);

    $create_roles = $selected_perms['create_roles'];
    $edit_roles   = $selected_perms['edit_roles'];
    $edit_own_roles   = $selected_perms['edit_own_roles'];
    $delete_roles = $selected_perms['delete_roles'];
    $delete_own_roles = $selected_perms['delete_own_roles'];
  }

  $permission_select = content_type_extras_get_default('content_type_extras_user_permissions_select');
  if ($permission_select == 'cte') {
    // We need to remove FPA's implementation
    unset($form['fpa_fieldset']);

    $form['user_permissions'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('User permissions'),
      '#description' => t('The below permissions duplicate the permissions set on the <a href="/admin/people/permissions">Permissions</a> page. It is provided here for convenience.'),
      '#group'       => 'additional_settings',
      '#tree'        => TRUE,
      'create_roles' => array(
        '#type'          => 'checkboxes',
        '#title'         => t('Roles that can CREATE content of this type'),
        '#options'       => $roles,
        '#default_value' => array_keys($create_roles),
      ),
      'edit_roles' => array(
        '#type'          => 'checkboxes',
        '#title'         => t('Roles that can EDIT any content of this type'),
        '#options'       => $roles,
        '#default_value' => array_keys($edit_roles),
      ),
      'edit_own_roles' => array(
        '#type'          => 'checkboxes',
        '#title'         => t('Roles that can EDIT own content of this type'),
        '#options'       => $roles,
        '#default_value' => array_keys($edit_own_roles),
      ),
      'delete_roles' => array(
        '#type'          => 'checkboxes',
        '#title'         => t('Roles that can DELETE any content of this type'),
        '#options'       => $roles,
        '#default_value' => array_keys($delete_roles),
      ),
        'delete_own_roles' => array(
        '#type'          => 'checkboxes',
        '#title'         => t('Roles that can DELETE own content of this type'),
        '#options'       => $roles,
        '#default_value' => array_keys($delete_own_roles),
      ),
    );
  }

  $form['extras'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Extra settings'),
    '#group' => 'additional_settings',
    'content_type_extras_title_hide' => array(
      '#type'          => 'checkbox',
      '#title'         => t('Hide node titles from displaying'),
      '#description'   => t('If checked, node titles for this content type will be hidden by default. Users with the <a href="/admin/people/permissions#module-content_type_extras">appropriate permission</a> can override this on a per-node basis.'),
      '#default_value' => content_type_extras_get_setting('content_type_extras_title_hide', $type),
      '#weight'        => 0,
    ),
  );

  if (!empty($form['#node_type']->is_new)) {
    // I decided to make this option only available when creating content types because:
    // 1. Once the content type is created, the admin can manage the body field just like
    //    any other field under "Manage fields"
    // 2. We would have to provide some kind of data checking on existing content types
    //    to see if there was already data entered in the body field for that content type
    //    and, if so, how the admin wanted to handle that...that just doesn't make sense!
    $form['extras']['content_type_extras_remove_body'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Remove body field from this content type'),
      '#default_value' => content_type_extras_get_setting('content_type_extras_remove_body', $type),
      '#weight'        => 10,
    );
  }

  if (module_exists('pathauto')) {
    $form['extras']['pathauto_node'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Path alias'),
      '#description'   => t('This is a shortcut method to using the normal !link.', array('!link' => l(t('pathauto settings'), 'admin/config/search/path/patterns'))) . '<br>' . t('If this field is left blank the default node pattern of') . ' <strong>' . variable_get('pathauto_node_pattern') . '</strong> ' . t('will be used.'),
      '#default_value' => content_type_extras_get_setting('pathauto_node', $type . '_pattern'),
      '#weight'        => 20,
    );

    $form['extras']['token_help'] = array(
      '#title'       => t('Replacement patterns'),
      '#type'        => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
      'help' => array(
        '#theme'       => 'token_tree',
        '#token_types' => array('node'),
      ),
      '#weight' => 30,
    );
  }

  // Custom form submission handler
  // See why this is done in the notes for the redirect function in
  // content_type_extras.module
  $form['#submit'][] = 'content_type_extras_node_type_form_submit_redirect';
}

/**
 * Form submission for $form_id node_type_form
 */
function content_type_extras_node_type_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];
  $user_permissions = $values['user_permissions'];

  $selected_perms = content_type_extras_get_selected_roles($user_permissions);

  foreach ($user_permissions as $action => $group) {
    list($type, $trash) = explode('_', $action);
    $set_perms = array();
    if ($type == 'create') {
      $set_perms = array(
        'create ' . $values['type'] . ' content',
      );
    }
    elseif ($type == 'edit') {
      if($trash == 'own') {
        $set_perms = array(
          $type . ' own ' . $values['type'] . ' content',
        );
      } else {
        $set_perms = array(
          $type . ' any ' . $values['type'] . ' content',
        );
      }
    }
    elseif ($type == 'delete') {
      if($trash == 'own') {
        $set_perms = array(
          $type . ' own ' . $values['type'] . ' content',
        );
      } else {
        $set_perms = array(
          $type . ' any ' . $values['type'] . ' content',
        );
      }
    }

    foreach ($group as $rid => $setting) {
      if ($setting) {
        user_role_grant_permissions($rid, $set_perms);
      }
      else {
        user_role_revoke_permissions($rid, $set_perms);
      }
    }
  }

  if (!empty($values['content_type_extras_remove_body'])) {
    // I'm not sure of a better way to not have a body field, other than to
    // delete it after it has been created by Core.
    $instance = field_read_instance('node', 'body', $values['type']);
    field_delete_instance($instance);
  }
  // Remove variable that is automatically created, it's not needed
  variable_del('content_type_extras_remove_body_' . $values['type']);

  // We have to rename the pathauto variable. Drupal by default creates one, but
  // it's not named properly for pathauto to recognize it.
  variable_set('pathauto_node_' . $values['type'] . '_pattern', variable_get('pathauto_node_' . $values['type']));
  variable_del('pathauto_node_' . $values['type']);
}


function content_type_extras_get_selected_roles($permissions, $perm_type = NULL) {
  $selected_perms = array();
  foreach ($permissions as $type => $group) {
    foreach ($group as $rid => $value) {
      $selected_perms[$type] = array();
      if ($value != 0) {
        $selected_perms[$type][$rid] = $rid;
      }
    }
  }

  if (!empty($perm_type)) {
    return $selected_perms[$perm_type];
  }

  return $selected_perms;
}
