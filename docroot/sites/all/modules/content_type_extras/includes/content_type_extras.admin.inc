<?php

function content_type_extras_settings() {
  $defaults = content_type_extras_get_default();
  // Get all available user roles
  $roles = user_roles();
  $admin_role = variable_get('user_admin_role', 0);

  if ($admin_role != 0) {
    $roles[$admin_role] .= t(' <em>(administrator role)</em>');
  }
  if(!isset($defaults['user_permissions']['edit_own_roles'])) {
    $defaults['user_permissions']['edit_own_roles'] = array();
  }
  if(!isset($defaults['user_permissions']['delete_own_roles'])) {
    $defaults['user_permissions']['delete_own_roles'] = array();
  }

  $form = array(
    'intro' => array(
      '#markup' => t('Set the default settings for all new content types below. These settings can be overridden on the content type page.'),
    ),
    'content_type_extras' => array(
      '#type' => 'vertical_tabs',
      'submission' => array(
        '#type'     => 'fieldset',
        '#title'    => t('Submission form settings'),
        '#attached' => array(
          'js' => array(
            'vertical-tabs' => drupal_get_path('module', 'content_type_extras') . '/js/content_type_extras.vertical_tabs.js',
          ),
        ),
        '#weight' => 0,
        'title_label' => array(
          '#type'          => 'textfield',
          '#title'         => t('Title field label'),
          '#default_value' => $defaults['title_label'],
          '#required'      => 1,
        ),
        'node_preview' => array(
          '#type'    => 'radios',
          '#title'   => t('Preview before submitting'),
          '#options' => array(
            t('Disabled'),
            t('Optional'),
            t('Required'),
          ),
          '#default_value' => $defaults['node_preview'],
        ),
        'content_type_extras_preview_button' => array(
          '#type'          => 'textfield',
          '#title'         => t('Preview button value'),
          '#description'   => t('The name of the Preview button that will display on the content edit form if enabled'),
          '#default_value' => $defaults['content_type_extras_preview_button'],
          '#required'      => 1,
        ),
        'content_type_extras_save_and_new' => array(
          '#type'    => 'radios',
          '#title'   => t('Save and New button'),
          '#options' => array(
            t('Disabled'),
            t('Enabled'),
          ),
          '#default_value' => $defaults['content_type_extras_save_and_new'],
        ),
        'content_type_extras_save_and_new_button' => array(
          '#type'          => 'textfield',
          '#title'         => t('Save and New button value'),
          '#description'   => t('The name of the Save and New button that will display on the content edit form if enabled'),
          '#default_value' => $defaults['content_type_extras_save_and_new_button'],
          '#required'      => 1,
        ),
        'content_type_extras_save_and_edit' => array(
          '#type'    => 'radios',
          '#title'   => t('Save and Edit button'),
          '#options' => array(
            t('Disabled'),
            t('Enabled'),
          ),
          '#default_value' => $defaults['content_type_extras_save_and_edit'],
        ),
        'content_type_extras_save_and_edit_button' => array(
          '#type'          => 'textfield',
          '#title'         => t('Save and Edit button value'),
          '#description'   => t('The name of the Save and Edit button that will display on the content edit form if enabled'),
          '#default_value' => $defaults['content_type_extras_save_and_edit_button'],
          '#required'      => 1,
        ),
        'content_type_extras_cancel' => array(
          '#type'    => 'radios',
          '#title'   => t('Cancel button'),
          '#options' => array(
            t('Disabled'),
            t('Enabled'),
          ),
          '#default_value' => $defaults['content_type_extras_cancel'],
        )
      ),
      'workflow' => array(
        '#type'   => 'fieldset',
        '#title'  => t('Publishing options'),
        '#weight' => 10,
        'node_options' => array(
          '#type'    => 'checkboxes',
          '#title'   => t('Default options'),
          '#options' => array(
            'status'   => t('Published'),
            'promote'  => t('Promoted to front page'),
            'sticky'   => t('Sticky at top of lists'),
            'revision' => t('Create new revision'),
          ),
          '#default_value' => $defaults['node_options'],
        ),
      ),
      'display' => array(
        '#type'   => 'fieldset',
        '#title'  => t('Display settings'),
        '#weight' => 20,
        'node_submitted' => array(
          '#type'          => 'checkbox',
          '#title'         => t('Display author and date information.'),
          '#description'   => t('Author username and publish date will be displayed.'),
          '#default_value' => $defaults['node_submitted'],
        ),
      ),
      'user_permissions' => array(
        '#type'   => 'fieldset',
        '#title'  => t('User permissions'),
        '#tree'   => TRUE,
        '#weight' => 30,
        'intro' => array(
          '#markup' => '<p>' . t('These permissions set the defaults for <em>new</em> content types. They do not change global permissions for all content types.'),
        ),
        'create_roles' => array(
          '#type'          => 'checkboxes',
          '#title'         => t('Roles that can CREATE content'),
          '#options'       => $roles,
          '#default_value' => $defaults['user_permissions']['create_roles'],
        ),
        'edit_roles' => array(
          '#type'          => 'checkboxes',
          '#title'         => t('Roles that can EDIT any content'),
          '#options'       => $roles,
          '#default_value' => $defaults['user_permissions']['edit_roles'],
        ),
        'edit_own_roles' => array(
          '#type'          => 'checkboxes',
          '#title'         => t('Roles that can EDIT own content'),
          '#options'       => $roles,
          '#default_value' => $defaults['user_permissions']['edit_own_roles'],
        ),
        'delete_roles' => array(
          '#type'          => 'checkboxes',
          '#title'         => t('Roles that can DELETE any content'),
          '#options'       => $roles,
          '#default_value' => $defaults['user_permissions']['delete_roles'],
        ),
        'delete_own_roles' => array(
          '#type'          => 'checkboxes',
          '#title'         => t('Roles that can DELETE own content'),
          '#options'       => $roles,
          '#default_value' => $defaults['user_permissions']['delete_own_roles'],
        ),
      ),
      'extras' => array(
        '#type'   => 'fieldset',
        '#title'  => t('Extra settings'),
        '#weight' => 40,
        'content_type_forms' => array(
          '#type'   => 'fieldset',
          '#title'  => t('Content type forms'),
          '#weight' => 0,
          'content_type_extras_descriptions_required' => array(
            '#type'          => 'checkbox',
            '#title'         => t('Make description field on all content types required'),
            '#description'   => t('Drupal by default does not require that the description field be filled in. Enabling this option will make the description field required.'),
            '#default_value' => $defaults['content_type_extras_descriptions_required'],
          ),
          'content_type_extras_remove_body' => array(
            '#type'          => 'checkbox',
            '#description'   => t("Drupal by default automatically creates a body field for each content type. Enabling this option will remove the body field from a newly created content type. This option can be overridden on a per content type basis."),
            '#title'         => t('Remove body field from content types'),
            '#default_value' => $defaults['content_type_extras_remove_body'],
          ),
        ),
        'node_titles' => array(
          '#type'   => 'fieldset',
          '#title'  => t('Node titles'),
          '#weight' => 1,
          'content_type_extras_title_hide' => array(
            '#type'          => 'checkbox',
            '#title'         => t('Hide title on node view'),
            '#description'   => t('If checked, node titles will be hidden by default. Users with the <a href="/admin/people/permissions#module-content_type_extras">appropriate permission</a> can override this on a per-node basis.'),
            '#default_value' => $defaults['content_type_extras_title_hide'],
          ),
          'content_type_extras_title_hide_css' => array(
            '#type'          => 'checkbox',
            '#title'         => t('Hide titles with CSS'),
            '#description'   => t('If checked, any node titles that are selected to be hidden will be hidden with CSS so that they render in HTML for devices like screenreaders.'),
            '#default_value' => $defaults['content_type_extras_title_hide_css'],
          ),
          'content_type_extras_title_hide_front' => array(
            '#type'          => 'checkbox',
            '#title'         => t('Hide node title on front page'),
            '#description'   => t('If checked, the node title on the front page will be hidden, regardless of what content type it is associated with'),
            '#default_value' => $defaults['content_type_extras_title_hide_front'],
          ),
        ),
        'form_buttons' => array(
          '#type'   => 'fieldset',
          '#title'  => t('Form buttons'),
          '#weight' => 2,
          'content_type_extras_top_buttons' => array(
            '#type'        => 'checkboxes',
            '#title'       => t('Show form buttons at top of:'),
            '#description' => t('Select the areas to duplicate form submission buttons on the top of the page.'),
            '#options'     => array(
              'manage_fields' => t('Manage fields form'),
              'node_edit'     => t('Node edit form'),
            ),
            '#default_value' => $defaults['content_type_extras_top_buttons'],
          ),
        ),
      ),
      'configuration' => array(
        '#type'   => 'fieldset',
        '#title'  => t('Configuration settings'),
        '#weight' => 50,
        'content_type_extras_excluded_node_forms' => array(
          '#type'          => 'textarea',
          '#title'         => t('Excluded node forms'),
          '#description'   => t('Enter the node form ids that you would like to exclude from being processed by Content Type: Extras, one per line.<br><strong>Note:</strong> \'subscriptions_ui_node_form\' and \'field_collection_item_form\' will always be added to prevent errors.'),
          '#default_value' => implode("\n", $defaults['content_type_extras_excluded_node_forms']),
        ),
      ),
    ),
  );

  if (module_exists('comment')) {
    $form['content_type_extras']['comment'] = array(
      '#type'   => 'fieldset',
      '#title'  => t('Comment settings'),
      '#tree'   => TRUE,
      '#weight' => 25,
      'comment' => array(
        '#type'          => 'select',
        '#title'         => t('Default comment setting for new content types'),
        '#default_value' => $defaults['comment']['comment'],
        '#options'       => array(
          COMMENT_NODE_OPEN   => t('Open'),
          COMMENT_NODE_CLOSED => t('Closed'),
          COMMENT_NODE_HIDDEN => t('Hidden'),
        ),
      ),
      'default_mode' => array(
        '#type'          => 'checkbox',
        '#title'         => t('Threading'),
        '#default_value' => $defaults['comment']['default_mode'],
        '#description'   => t('Show comment replies in a threaded list.'),
      ),
      'default_per_page' => array(
        '#type'          => 'select',
        '#title'         => t('Comments per page'),
        '#default_value' => $defaults['comment']['default_per_page'],
        '#options'       => _comment_per_page(),
      ),
      'anonymous' => array(
        '#type'          => 'select',
        '#title'         => t('Anonymous commenting'),
        '#default_value' => $defaults['comment']['anonymous'],
        '#options'       => array(
          COMMENT_ANONYMOUS_MAYNOT_CONTACT => t('Anonymous posters may not enter their contact information'),
          COMMENT_ANONYMOUS_MAY_CONTACT    => t('Anonymous posters may leave their contact information'),
          COMMENT_ANONYMOUS_MUST_CONTACT   => t('Anonymous posters must leave their contact information'),
        ),
        '#access' => user_access('post comments', drupal_anonymous_user()),
      ),
      'subject_field' => array(
        '#type'          => 'checkbox',
        '#title'         => t('Allow comment title'),
        '#default_value' => $defaults['comment']['subject_field'],
      ),
      'form_location' => array(
        '#type'          => 'checkbox',
        '#title'         => t('Show reply form on the same page as comments'),
        '#default_value' => $defaults['comment']['form_location'],
      ),
      'preview' => array(
        '#type'          => 'radios',
        '#title'         => t('Preview comment'),
        '#default_value' => $defaults['comment']['preview'],
        '#options'       => array(
          DRUPAL_DISABLED => t('Disabled'),
          DRUPAL_OPTIONAL => t('Optional'),
          DRUPAL_REQUIRED => t('Required'),
        ),
      ),
    );
  }

  if (module_exists('xmlsitemap')) {
    $form['content_type_extras']['xmlsitemap_settings'] = array(
      '#type'   => 'fieldset',
      '#title'  => t('XML Sitemap'),
      '#tree'   => TRUE, // Set to TRUE to match node_type_form for saving settings
      '#weight' => 35,
      'status' => array(
        '#type'    => 'select',
        '#title'   => t('Inclusion'),
        '#options' => array(
          1 => t('Included'),
          0 => t('Excluded'),
        ),
        '#default_value' => $defaults['xmlsitemap_settings']['status'],
      ),
      'priority' => array(
        '#type' => 'select',
        '#title' => t('Default priority'),
        '#options' => array(
          '1.0' => t('1.0 (highest)'),
          '0.9' => '0.9',
          '0.8' => '0.8',
          '0.7' => '0.7',
          '0.6' => '0.6',
          '0.5' => t('0.5 (normal)'),
          '0.4' => '0.4',
          '0.3' => '0.3',
          '0.2' => '0.2',
          '0.1' => '0.1',
          '0.0' => t('0.0 (lowest)'),
        ),
        '#default_value' => $defaults['xmlsitemap_settings']['priority'],
        '#states'        => array(
          'invisible' => array(
            'select[name="xmlsitemap_settings[status]"]' => array('value' => '0'),
          ),
        ),
      ),
    );
  }

  if (module_exists('scheduler')) {
    $form['content_type_extras']['scheduler_settings'] = array(
      '#type'   => 'fieldset',
      '#title'  => t('Scheduler'),
      '#tree'   => TRUE, // Set to TRUE to match node_type_form for saving settings
      '#weight' => 35,
      'publish_enable' => array(
        '#type'          => 'checkbox',
        '#title'         => t('Enable Scheduled publishing for new content types'),
        '#default_value' => $defaults['scheduler_settings']['publish_enable'],
      ),
      'publish_touch' => array(
        '#type'          => 'checkbox',
        '#title'         => t('Change content creation time to match the scheduled publish time for new content types'),
        '#default_value' => $defaults['scheduler_settings']['publish_touch'],
      ),
      'publish_require' => array(
        '#type'          => 'checkbox',
        '#title'         => t('Require scheduled publishing for new content types'),
        '#default_value' => $defaults['scheduler_settings']['publish_require'],
      ),
      'publish_revision' => array(
        '#type'          => 'checkbox',
        '#title'         => t('Create a new revision on publishing for new content types'),
        '#default_value' => $defaults['scheduler_settings']['publish_revision'],
      ),
      'unpublish_enable' => array(
        '#type'          => 'checkbox',
        '#title'         => t('Enable scheduled unpublishing for new content types'),
        '#default_value' => $defaults['scheduler_settings']['unpublish_enable'],
      ),
      'unpublish_require' => array(
        '#type'          => 'checkbox',
        '#title'         => t('Require scheduled unpublishing for new content types'),
        '#default_value' => $defaults['scheduler_settings']['unpublish_require'],
      ),
      'unpublish_revision' => array(
        '#type'          => 'checkbox',
        '#title'         => t('Create a new revision on unpublishing for new content types'),
        '#default_value' => $defaults['scheduler_settings']['unpublish_revision'],
      ),
    );
  }

  if (module_exists('fpa')) {
    $form['content_type_extras']['extras']['content_type_forms']['content_type_extras_user_permissions_select'] = array(
      '#type'    => 'radios',
      '#title'   => t('Select which user interface to use to manage permissions on node type forms'),
      '#options' => array(
        'cte' => t('Content Type: Extras'),
        'fpa' => t('Fast Permissions Administration (FPA)'),
      ),
      '#default_value' => $defaults['content_type_extras_user_permissions_select'],
    );
  }

  $form['actions'] = array(
    'submit'   => array(
      '#type'  => 'submit',
      '#value' => t('Save configuration'),
    ),
  );

  return $form;
}

function content_type_extras_settings_submit(&$form, &$form_state) {
  $values = $form_state['values'];
  // Place the values of content_type_extras_excluded_node_forms into an array
  $values['content_type_extras_excluded_node_forms'] = explode("\n", $values['content_type_extras_excluded_node_forms']);

  unset($values['submit'], $values['form_build_id'], $values['form_token'], $values['form_id'], $values['op']);
  variable_set('content_type_extras_default_settings', $values);

  drupal_set_message(t('Default content type settings saved.'));

  $form_state['redirect'] = 'admin/structure/types';
}
