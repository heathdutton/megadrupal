<?php

/**
 * @file
 * flag_hide.admin.inc
 * Displays, submits and saves the form for the admin configuration page.
 */

/**
 * Menu callback for admin settings.
 */
function flag_hide_admin_config() {
  $form = array();

  $form['flag_hide_users'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Configuration for users'),
    '#description' => t('<em>Muting</em>: privileged users may opt to hide all content and comments posted by authors they pick to be suppressed. Muting is a personal choice: users select for themselves whose posts they do not wish to see. Muting a user does not prevent that user from posting content or comments to the site. <br/>To allow muting of users you need to first <a href="@user_flag">add or edit a User Flag</a> (requires the <a href="@flag_project">Flag</a> module).', array(
      '@user_flag' => url('admin/structure/flags'),
      '@flag_project' => url('http://drupal.org/project/flag'),
    )),
  );
  $roles = user_roles();
  $roles[DRUPAL_ANONYMOUS_RID] .= ' (' . t('can never be muted') . ')';
  // @todo Want to disable the 'anonymous user' checkbox, but don't know how.
  $form['flag_hide_users']['flag_hide_roles_not_muted'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles that <em>cannot</em> be muted'),
    '#options' => $roles,
    // By default all roles except the admin role may be muted.
    '#default_value' => variable_get('flag_hide_roles_not_muted', array_keys(flag_hide_default_roles_not_muted())),
    '#description' => t('You would normally tick any site administrator roles, so that visitors do not miss site-wide announcements or other important content. Users in roles that cannot be muted are still subject to having individual posts hidden by other users in accordance with the <strong>Configuration for nodes</strong> below.'),
  );

  $form['flag_hide_comment'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Configuration for comments'),
    '#description' => t('The comment configuration options apply to <em>muted</em> users, so require a <a href="@user_flag" target="project">User Flag</a> to be configured. All comments by the muted user will be hidden. You cannot hide individual comments.', array(
      '@user_flag' => url('admin/structure/flags'),
    ))
  );
  $form['flag_hide_comment']['flag_hide_comment_placeholder'] = array(
    '#type' => 'textfield',
    '#size' => 128,
    '#title' => t('Text to replace comment when hidden'),
    '#default_value' => variable_get('flag_hide_comment_placeholder', FLAG_HIDE_COMMENT_PLACEHOLDER),
    '#description' => t('<strong>Example</strong>: "%msg" <br/>Some HTML tags are allowed. Blank out this field if you do not wish to show any traces of comments made by muted users.', array(
      '%msg' => FLAG_HIDE_COMMENT_PLACEHOLDER)
    ),
  );

  $form['flag_hide_node'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Configuration for nodes'),
    '#description' => t('These node configuration options are independent of the <strong>Configuration for users</strong>. So, when a role is configured as <em>"cannot be muted"</em>, users can still hide individual nodes authored by that role, or any other role. The options below apply to all roles and contexts that the <a href="@node_flag">Node Flag</a> was configured for (requires the <a href="@flag_project">Flag</a> module).', array(
      '@node_flag' => url('admin/structure/flags'),
      '@flag_project' => url('http://drupal.org/project/flag'),
    ))
  );
  $form['flag_hide_node']['flag_hide_method'] = array(
    '#type' => 'radios',
    '#title' => t('Method used to hide node content'),
    '#default_value' => variable_get('flag_hide_method', FLAG_HIDE_METHOD_DEFAULT),
    '#options' => array(
      FLAG_HIDE_METHOD_DEFAULT => t('default, no files required'),
      FLAG_HIDE_METHOD_TEMPLATE_SUGGESTION => t('via the file <em>node--hide.tpl.php</em>, added to your theme'),
    ),
    '#description' => t("If you select the second method you must place <em>node--hide.tpl.php</em> in the same directory as your theme's node.tpl.php.<br/> <em>node--hide.tpl.php</em> may be empty or may contain some message with or without rendered variables."),
  );
  $form['flag_hide_node']['flag_hide_node_placeholder'] = array(
    '#type' => 'textfield',
    '#size' => 128,
    '#title' => t('Text to replace node content when hidden'),
    '#default_value' => variable_get('flag_hide_node_placeholder', FLAG_HIDE_NODE_PLACEHOLDER),
    '#description' => t('<strong>Example</strong>: "%msg" <br/>Some HTML tags are allowed. Blank out this field and untick the box below to leave no trace of hidden content.', array(
      '%msg' => FLAG_HIDE_NODE_PLACEHOLDER)
    ),
    '#states' => array(
      'invisible' => array(
        'input[name="flag_hide_method"]' => array(
          'value' => FLAG_HIDE_METHOD_TEMPLATE_SUGGESTION,
        ),
      ),
    ),
  );
  $form['flag_hide_node']['flag_hide_node_display_unhide_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('When node content is hidden, display an <em>Unhide</em> or <em>Unmute user</em> link'),
    '#default_value' => variable_get('flag_hide_node_display_unhide_link', FALSE),
    '#description' => t(''),
    '#states' => array(
      'invisible' => array(
        'input[name="flag_hide_method"]' => array(
          'value' => FLAG_HIDE_METHOD_TEMPLATE_SUGGESTION,
        ),
      ),
    ),
  );
  $form['flag_hide_node']['flag_hide_node_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Also hide page title when node content is hidden'),
    '#default_value' => variable_get('flag_hide_node_title', FALSE),
    '#description' => t('This applies to <em>node/*</em> pages.'),
  );

  $form['flag_hide_node']['pages'] = array(
    '#type' => 'markup',
    '#prefix' => '<div class="pages">',
    '#suffix' => '</div>',
  );
  $form['flag_hide_node']['pages']['flag_hide_include_pages'] = array(
    '#type' => 'textarea',
    '#rows' => 3,
    '#title' => t('Pages on which hiding of nodes should be enabled'),
    '#default_value' => variable_get('flag_hide_include_pages', FLAG_HIDE_DEFAULT_PAGE_INCLUSIONS),
    '#description' => t("Enter relative paths, one per line. Do not start with a slash. You may use path aliases. The asterisk <em>*</em> is the wildcard character, i.e. <em>node/*</em> denotes all pages that have a path starting with <em>node/</em>"),
  );
  $form['flag_hide_node']['pages']['flag_hide_exclude_pages'] = array(
    '#type' => 'textarea',
    '#rows' => 3,
    '#title' => t('Exceptions: pages excluded from the collection on the left'),
    '#default_value' => variable_get('flag_hide_exclude_pages', FLAG_HIDE_DEFAULT_PAGE_EXCLUSIONS),
    '#description' => t('As before, one relative path per line.'),
  );

  $form['flag_hide_blocks'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Configuration for blocks'),
    '#description' => t('Does <strong>not</strong> require the Flag module.'),
  );
  $form['flag_hide_blocks']['flag_hide_enable_block_hiding'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable "No results behaviour" for blocks'),
    '#default_value' => variable_get('flag_hide_enable_block_hiding', FALSE),
    '#description' => t('This adds to each block configuration the option to remove title and remaining markup of blocks containing no text or images, after flagging or otherwise. To activate this option, click the vertical tab <strong>No results behaviour</strong> on the block configuration form.'),
  );

  $form['flag_hide_views'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Configuration for Views'),
    '#description' => t('Does <strong>not</strong> require the Flag module.'),
  );
  $form['flag_hide_views']['flag_hide_empty_view_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide title of empty Views'),
    '#default_value' => variable_get('flag_hide_empty_view_title', FALSE),
    '#description' => t('This removes the title from any View output that contains no text or images.'),
  );

  $form['#attached']['css'][] = drupal_get_path('module', 'flag_hide') . '/css/flag_hide.admin.css';

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['#submit'][] = 'flag_hide_admin_config_submit';
  $form['#theme'] = 'system_settings_form';
  return $form;
}

/**
 * Form submit handler for flag_hide_admin_config().
 *
 * @param array $form
 * @param array $form_state
 */
function flag_hide_admin_config_submit($form, &$form_state) {

  // Clear out the form with stuff we do not wish to save, like buttons.
  form_state_values_clean($form_state);

  $form_state['values']['flag_hide_comment_placeholder'] = filter_xss($form_state['values']['flag_hide_comment_placeholder'], flag_hide_allowed_html_tags());
  $form_state['values']['flag_hide_node_placeholder'] = filter_xss($form_state['values']['flag_hide_node_placeholder'], flag_hide_allowed_html_tags());
  $form_state['values']['flag_hide_roles_not_muted'][DRUPAL_ANONYMOUS_RID] = DRUPAL_ANONYMOUS_RID;

  foreach ($form_state['values'] as $key => $value) {
    variable_set($key, $value);
  }
  drupal_theme_rebuild();
  drupal_set_message(t('The Flag, Hide & Mute configuration was saved.'));

  flag_hide_unflag_unmuted();
}

/**
 * Unflag content by posters whose roles cannot be muted.
 *
 * If 'flag_hide_roles_not_muted' was changed to add a role currently muted
 * to no longer being mutable, then remove ANY flaggings of specific
 * nodes posted by users in this role. Also unflag (unmute) all users in this
 * role.
 */
function flag_hide_unflag_unmuted() {

  if (!module_exists('flag')) {
    return;
  }
  if ($user_flag = variable_get('flag_hide_flag_name_user')) {
    foreach (flag_get_flag_flagging_data($user_flag) as $flagging_record) {
      $poster_uid = $flagging_record->entity_id;
      $posters = entity_load('user', array($poster_uid));
      if (!empty($posters[$poster_uid]) && !flag_hide_may_be_muted(array_keys($posters[$poster_uid]->roles))) {
        // Unflag without permission check.
        $account = new stdClass();
        $account->uid = $flagging_record->uid;
        if (flag_get_flag($user_flag)->flag('unflag', $poster_uid, $account, TRUE)) {
          drupal_set_message(t("One or more users had previously muted poster %user (id @uid). Because this poster's role may no longer be muted, %user's posts were <em>unhidden</em> from all users, except for nodes that were hidden individually.", array(
            '%user' => $posters[$poster_uid]->name,
            '@uid' => $poster_uid)), 'status', FALSE
          );
        }
      }
    }
  }
/**
  if ($node_flag = variable_get('flag_hide_flag_name')) {
    foreach (flag_get_flag_flagging_data($node_flag) as $flagging_record) {
      // $flagging_record->entity_type will always be node.
      $nid = $flagging_record->entity_id;
      $entities = entity_load($flagging_record->entity_type, array($nid));
      if (!empty($entities)) {
        // For this node, find the author id and their role(s) and check if any
        // roles must not be muted. If so, unhide the node.
        $uid = $entities[$nid]->uid;
        $posters = entity_load('user', array($uid));
        if (!empty($posters[$uid]) && !flag_hide_may_be_muted(array_keys($posters[$uid]->roles))) {
          // Unflag without permission check.
          $account = new stdClass();
          $account->uid = $flagging_record->uid;
          if (flag_get_flag($node_flag)->flag('unflag', $nid, $account, TRUE)) {
            drupal_set_message(t('One or more users had previously hidden the @type "%title". Because the author role(s) of this content may no longer be muted, this @type was <em>unhidden</em> from all users.', array(
              '@type' => $entities[$nid]->type,
              '%title' => $entities[$nid]->title)), 'status', FALSE
            );
          }
        }
      }
    }
  }
 */
}
