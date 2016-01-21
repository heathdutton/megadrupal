<?php

/**
 * Define rules compatible actions.
 *
 * This hook is required in order to add a new rules action. It should be
 * placed into the file MODULENAME.rules.inc, which gets automatically included
 * when the hook is invoked.
 *
 * @return
 *   An array of information about the module's provided rules actions.
 *   The array contains a sub-array for each action, with the action name as
 *   the key. Actions names may only contain lowercase alpha-numeric characters
 *   and underscores and should be prefixed with the providing module name.
 *   Possible attributes for each sub-array are:
 *   - label: The label of the action. Start capitalized. Required.
 *   - group: A group for this element, used for grouping the actions in the
 *     interface. Should start with a capital letter and be translated.
 *     Required.
 *   - parameter: (optional) An array describing all parameter of the action
 *     with the parameter's name as key. Each parameter has to be
 *     described by a sub-array with possible attributes as described
 *     afterwards, whereas the name of a parameter needs to be a lowercase,
 *     valid PHP variable name.
 *   - provides: (optional) An array describing the variables the action
 *     provides to the evaluation state with the variable name as key. Each
 *     variable has to be described by a sub-array with possible attributes as
 *     described afterwards, whereas the name of a parameter needs to be a
 *     lowercase, valid PHP variable name.
 *   - 'named parameter': (optional) If set to TRUE, the arguments will be
 *     passed as a single array with the parameter names as keys. This emulates
 *     named parameters in PHP and is in particular useful if the number of
 *     parameters can vary. Defaults to FALSE.
 *   - base: (optional) The base for action implementation callbacks to use
 *     instead of the action's name. Defaults to the action name.
 *   - callbacks: (optional) An array which allows to set specific function
 *     callbacks for the action. The default for each callback is the actions
 *     base appended by '_' and the callback name.
 *   - 'access callback': (optional) A callback which has to return whether the
 *     currently logged in user is allowed to configure this action. See
 *     rules_node_integration_access() for an example callback.
 *  Each 'parameter' array may contain the following properties:
 *   - label: The label of the parameter. Start capitalized. Required.
 *   - type: The rules data type of the parameter, which is to be passed to the
 *     action. All types declared in hook_rules_data_info() may be specified, as
 *     well as an array of possible types. Also lists and lists of a given type
 *     can be specified by using the notating list<integer> as introduced by
 *     the entity metadata module, see hook_entity_property_info(). The special
 *     keyword '*' can be used when all types should be allowed. Required.
 *   - bundles: (optional) An array of bundle names. When the specified type is
 *     set to a single entity type, this may be used to restrict the allowed
 *     bundles.
 *   - description: (optional) If necessary, a further description of the
 *     parameter.
 *   - options list: (optional) A callback that returns an array of possible
 *     values for this parameter. The callback has to return an array as used
 *     by hook_options_list(). For an example implementation see
 *     rules_data_action_type_options().
 *   - save: (optional) If this is set to TRUE, the parameter will be saved by
 *     rules when the rules evaluation ends. This is only supported for savable
 *     data types. If the action returns FALSE, saving is skipped.
 *   - optional: (optional) May be set to TRUE, when the parameter isn't
 *     required.
 *   - 'default value': (optional) The value to pass to the action, in case the
 *     parameter is optional and there is no specified value.
 *   - 'allow null': (optional) Usually Rules will not pass any NULL values as
 *     argument, but abort the evaluation if a NULL value is present. If set to
 *     TRUE, Rules will not abort and pass the NULL value through. Defaults to
 *     FALSE.
 *   - restriction: (optional) Restrict how the argument for this parameter may
 *     be provided. Supported values are 'selector' and 'input'.
 *   - default mode: (optional) Customize the default mode for providing the
 *     argument value for a parameter. Supported values are 'selector' and
 *     'input'. The default depends on the required data type.
 *   - sanitize: (optional) Allows parameters of type 'text' to demand an
 *     already sanitized argument. If enabled, any user specified value won't be
 *     sanitized itself, but replacements applied by input evaluators are as
 *     well as values retrieved from selected data sources.
 *   - translatable: (optional) If set to TRUE, the provided argument value
 *     of the parameter is translatable via i18n String translation. This is
 *     applicable for textual parameters only, i.e. parameters of type 'text',
 *     'token', 'list<text>' and 'list<token>'. Defaults to FALSE.
 *   - ui class: (optional) Allows overriding the UI class, which is used to
 *     generate the configuration UI of a parameter. Defaults to the UI class of
 *     the specified data type.
 *   - cleaning callback: (optional) A callback that input evaluators may use
 *     to clean inserted replacements; e.g. this is used by the token evaluator.
 *   - wrapped: (optional) Set this to TRUE in case the data should be passed
 *     wrapped. This only applies to wrapped data types, e.g. entities.
 *  Each 'provides' array may contain the following properties:
 *   - label: The label of the variable. Start capitalized. Required.
 *   - type: The rules data type of the variable. All types declared in
 *     hook_rules_data_info() may be specified. Types may be parametrized e.g.
 *     the types node<page> or list<integer> are valid.
 *   - save: (optional) If this is set to TRUE, the provided variable is saved
 *     by rules when the rules evaluation ends. Only possible for savable data
 *     types. Defaults to FALSE.
 *
 *  The module has to provide an implementation for each action, being a
 *  function named as specified in the 'base' key or for the execution callback.
 *  All other possible callbacks are optional.
 *  Supported action callbacks by rules are defined and documented in the
 *  RulesPluginImplInterface. However any module may extend the action plugin
 *  based upon a defined interface using hook_rules_plugin_info(). All methods
 *  defined in those interfaces can be overridden by the action implementation.
 *  The callback implementations for those interfaces may reside in any file
 *  specified in hook_rules_file_info().
 *
 *  @see hook_rules_file_info()
 *  @see rules_action_execution_callback()
 *  @see hook_rules_plugin_info()
 *  @see RulesPluginImplInterface
 */
function redmine_sync_rules_action_info() {
  $return = array(
    'create_time_entry' => array(
      'label' => t('Create Time Entry'),
      'parameter' => array(
        'mail' => array(
          'type'         => 'text', 
          'label'        => t('Redmine user email'),
        ),
        'issue_id' => array(
          'type'         => 'integer', 
          'label'        => t('Redmine Issue ID'),
        ),
        'spent_on' => array(
          'type'         => 'date', 
          'label'        => t('Time Entry date'),
        ),
        'hours' => array(
          'type'         => 'decimal', 
          'label'        => t('Hours'),
        ),
        'comment' => array(
          'type'         => 'text', 
          'label'        => t('Time Entry comment'),
          'description'  => t('Maximum !num symbols!', array('!num' => 255)),
          'optional'     => TRUE,
        ),
        'activity' => array(
          'type'         => 'integer',
          'label'        => t('Activity'),
          'options list' => '_redmine_sync_rules_get_activities',
        ),
      ),
      'group' => t('Redmine Sync'),
      'callbacks' => array(
        'help'     => 'redmine_sync_rules_time_entry_help',
        'validate' => 'redmine_sync_rules_time_entry_validate',
        'execute'  => 'redmine_sync_rules_time_entry_create',
      ),
    ),
    'create_issue' => array(
      'label' => t('Create Issue'),
      'parameter' => array(
        'mail' => array(
          'type'         => 'text', 
          'label'        => t('Redmine user email'),
        ),
        'project_id' => array(
          'type'         => 'integer', 
          'label'        => t('Project ID'),
        ),
        'tracker_id' => array(
          'type'         => 'integer', 
          'label'        => t('Tracker ID'),
        ),
        'subject' => array(
          'type'         => 'text', 
          'label'        => t('Subject'),
        ),
        'description' => array(
          'type'         => 'text', 
          'label'        => t('Description'),
          'optional'     => TRUE,
        ),
        'status_id' => array(
          'type'         => 'integer', 
          'label'        => t('Status ID'),
          'options list' => '_redmine_sync_rules_get_statuses',
        ),
        'priority_id' => array(
          'type'         => 'integer', 
          'label'        => t('Priority ID'),
          'options list' => '_redmine_sync_rules_get_priorities',
        ),
        'assigned_to_id' => array(
          'type'         => 'integer', 
          'label'        => t('Assigned to ID'),
          'optional'     => TRUE,
        ),
        'category_id' => array(
          'type'         => 'integer', 
          'label'        => t('Category ID'),
          'optional'     => TRUE,
        ),
        'fixed_version_id' => array(
          'type'         => 'integer', 
          'label'        => t('Target version ID'),
          'optional'     => TRUE,
        ),
        'start_date' => array(
          'type'         => 'date', 
          'label'        => t('Start date'),
          'optional'     => TRUE,
        ),
        'due_date' => array(
          'type'         => 'date', 
          'label'        => t('Due date'),
          'optional'     => TRUE,
        ),
        'estimated_hours' => array(
          'type'         => 'decimal', 
          'label'        => t('Estimated time'),
          'optional'     => TRUE,
        ),
        'done_ratio' => array(
          'type'         => 'integer',
          'label'        => t('% Done'),
          'options list' => '_redmine_sync_rules_get_done_ratio',
          'optional'     => TRUE,
        ),
      ),
      'group' => t('Redmine Sync'),
      'callbacks' => array(
        'help'     => 'redmine_sync_rules_issue_help',
        'validate' => 'redmine_sync_rules_issue_validate',
        'execute'  => 'redmine_sync_rules_issue_create',
      ),
    ),
    'create_project' => array(
      'label' => t('Create Project'),
      'parameter' => array(
        'mail' => array(
          'type'  => 'text', 
          'label' => t('Redmine user email'),
        ),
        'name' => array(
          'type'  => 'text', 
          'label' => t('Name'),
        ),
        'description' => array(
          'type'  => 'text', 
          'label' => t('Description'),
        ),
        'id' => array(
          'type'  => 'text', 
          'label' => t('Identifier'),
        ),
        'homepage' => array(
          'type'  => 'text', 
          'label' => t('Homepage'),
        ),
        'public' => array(
          'type'  => 'boolean', 
          'label' => t('Public'),
        ),
        'subproject_id' => array(
          'type'         => 'integer', 
          'label'        => t('Subproject of'),
          'options list' => '_redmine_sync_rules_get_projects',
        ),
        'inherit_members' => array(
          'type'  => 'boolean', 
          'label' => t('Inherit members'),
        ),
      ),
      'group' => t('Redmine Sync'),
      'callbacks' => array(
        'help'     => 'redmine_sync_rules_project_help',
        'validate' => 'redmine_sync_rules_project_validate',
        'execute'  => 'redmine_sync_rules_project_create',
      ),
    ),
    'create_project_membership' => array(
      'label' => t('Create Project Membership'),
      'parameter' => array(
        'mail' => array(
          'type'         => 'text', 
          'label'        => t('Redmine user email'),
        ),
        'project_id' => array(
          'type'         => 'integer', 
          'label'        => t('Project'),
          'options list' => '_redmine_sync_rules_get_projects',
        ),
        'user_id' => array(
          'type'         => 'integer', 
          'label'        => t('User'),
          'options list' => '_redmine_sync_rules_get_users',
        ),
      ),
      'group' => t('Redmine Sync'),
      'callbacks' => array(
        'help'     => 'redmine_sync_rules_project_membership_help',
        'validate' => 'redmine_sync_rules_project_membership_validate',
        'execute'  => 'redmine_sync_rules_project_membership_create',
      ),
    ),
  );

  // Adding modules for "create_project".
  foreach ($GLOBALS['REDMINE_SYNC_MODULE_NAMES_LIST'] as $c_name => $c_title) {
    $return['create_project']['parameter']['module_'.$c_name] = array(
      'type' => 'boolean',
      'entity_type' => 'module_name',
      'entity_id' => $c_name,
      'label' => t('Use !name module', array('!name' => $c_title)),
      'optional' => true,
    );
  }

  // Adding trackers for "create_project".
  foreach (redmine_sync_rest_get_trackers(null, REDMINE_SYNC_RET_VAL_TYPE_DEFAULT) as $c_item) {
    $return['create_project']['parameter']['tracker_'.$c_item['id']] = array(
      'type' => 'boolean',
      'entity_type' => 'tracker_id',
      'entity_id' => $c_item['id'],
      'label' => t('Use !name tracker', array('!name' => $c_item['name'])),
      'optional' => true,
    );
  }

  // Adding roles for "create_project_membership".
  foreach (redmine_sync_rest_get_user_roles(null, REDMINE_SYNC_RET_VAL_TYPE_DEFAULT) as $c_item) {
    $return['create_project_membership']['parameter']['role_'.$c_item['id']] = array(
      'type' => 'boolean',
      'entity_type' => 'rid',
      'entity_id' => $c_item['id'],
      'label' => t('Has !name role', array('!name' => $c_item['name'])),
      'optional' => true,
    );
  }

  // Adding the custom fields.
  $rdm_custom_fields = redmine_sync_rest_get_custom_fields(null, REDMINE_SYNC_RET_VAL_TYPE_DEFAULT);
  foreach ($rdm_custom_fields as $c_field) {
    if (in_array($c_field['customized_type'], array('time_entry', 'issue'))) {
      $return['create_' . $c_field['customized_type']]['parameter']['rdm_custom_' . $c_field['id']] = array(
        'label'     => $c_field['name'],
        'optional'  => $c_field['is_required'] == 'true' ? false : true,
        'rdm_id'    => $c_field['id'],
        'is_custom' => true,
      );
      _redmine_sync_rules_custom_field_prepare($c_field, $return, 'create_' . $c_field['customized_type']);
    }
  }

  return $return;
}


/**
 * ############################################################################################################
 * ############################################ Add new Time Entry ############################################
 * ############################################################################################################
 */

/**
 * Add new Time Entry: custom help callback.
 */
function redmine_sync_rules_time_entry_help(RulesAction $action) {
}

/**
 * Add new Time Entry: custom validation callback for the data set action.
 */
function redmine_sync_rules_time_entry_validate(RulesAbstractPlugin $plugin) {
  // if (empty($plugin->settings['hours'])) {
  //   throw new RulesIntegrityException(t('Error message.'), array($plugin, 'parameter', 'hours'));
  // }
}

/**
 * Add new Time Entry: custom callback for execute action.
 */
function redmine_sync_rules_time_entry_create() {
  $args = func_get_args();

  // Prepare inputed RulesAction and RulesState.
  $rules_act = null;
  $rules_ste = null;
  foreach ($args as $c_arg) {
    if ($rules_act == null && $c_arg instanceof RulesAction) $rules_act = $c_arg;
    if ($rules_ste == null && $c_arg instanceof RulesState)  $rules_ste = $c_arg;
  }

  // Prepare fields.
  $fields = array();
  if ($rules_act) {
    $rules_act_info = $rules_act->info();
    $fields = $rules_act_info['parameter'];
    foreach ($fields as $c_name => $c_field) {
      $c_value = array_shift($args);
      // Prepare date.
      if ($c_field['type'] == 'date') {
        $c_value = date('Y-m-d', $c_value);
      }
      $fields[$c_name]['value'] = $c_value;
    }
  }

  // Note: if this action started on cron run then redmine_sync_init_rest() return REDMINE_SYNC_REST_API_INIT_NO_USER_KEY
  if (count($fields) && redmine_sync_init_rest() == REDMINE_SYNC_REST_API_INIT_OK) {
    $rdm_user_info = redmine_sync_rest_get_user_by_mail($fields['mail']['value']);
    if ($rdm_user_info) {

      // Create Time entry post array.
      // @todo: check if the user can access to project with selected issue.
      $time_entry_post = array(
        'time_entry' => array(
          'issue_id'    => $fields['issue_id']['value'],
          'spent_on'    => $fields['spent_on']['value'],
          'hours'       => $fields['hours']['value'],
          'comments'    => substr($fields['comment']['value'], 0, 255),
          'activity_id' => $fields['activity']['value'],
        )
      );

      // Add custom fields values.
      $custom_fields_data = array();
      foreach ($fields as $c_field) {
        if (isset($c_field['is_custom']) &&
                  $c_field['is_custom']) {
          $c_buf = array();
          $c_buf['key'] = 'custom_field';
          $c_buf['attributes']['id'] = $c_field['rdm_id'];
          if (is_array($c_field['value'])) {
            $c_buf['attributes']['multiple'] = 'true';
            $c_buf['value'][0] = array('key' => 'value', 'attributes' => array('type' => 'array'));
            foreach ($c_field['value'] as $c_val) {
              $c_buf['value'][0]['value'][] = array('key' => 'value', 'value' => $c_val);
            }
          } else {
            $c_buf['value'] = array('value' => $c_field['value']);
          }            
          $custom_fields_data[]= $c_buf;
        }
      }
      if (count($custom_fields_data)) {
        $time_entry_post['time_entry'][] = array(
          'key'        => 'custom_fields',
          'attributes' => array('type' => 'array'),
          'value'      => $custom_fields_data,
        );
      }

      // Post request.
      $request = redmine_sync_rest_request('time_entries', 'POST', $time_entry_post, $rdm_user_info['login']);

      // Show result message.
      if ($request->code == 201) {
        drupal_set_message(t('New entry %name with id = %id was created.',       array('%name' => 'Time entries', '%id' => (int)$request->data_prepared->id)));
        watchdog('redmine_sync', 'New entry %name with id = %id was created.',   array('%name' => 'Time entries', '%id' => (int)$request->data_prepared->id));
      } else {
        drupal_set_message(t('Can\'t create new entry %name! Error: %error',     array('%name' => 'Time entries', '%error' => $request->error . (isset($request->data_prepared->error) ? ' - '.$request->data_prepared->error : ''))), 'error');
        watchdog('redmine_sync', 'Can\'t create new entry %name! Error: %error', array('%name' => 'Time entries', '%error' => $request->error . (isset($request->data_prepared->error) ? ' - '.$request->data_prepared->error : '')), WATCHDOG_ERROR);
      }
    }
  }
}


/**
 * ############################################################################################################
 * ############################################## Add new Issue ###############################################
 * ############################################################################################################
 */

/**
 * Add new Issue: custom help callback.
 */
function redmine_sync_rules_issue_help(RulesAction $action) {
}

/**
 * Add new Issue: custom validation callback for the data set action.
 */
function redmine_sync_rules_issue_validate(RulesAbstractPlugin $plugin) {
}

/**
 * Add new Issue: custom callback for execute action.
 */
function redmine_sync_rules_issue_create() {
  $args = func_get_args();

  // Prepare inputed RulesAction and RulesState.
  $rules_act = null;
  $rules_ste = null;
  foreach ($args as $c_arg) {
    if ($rules_act == null && $c_arg instanceof RulesAction) $rules_act = $c_arg;
    if ($rules_ste == null && $c_arg instanceof RulesState)  $rules_ste = $c_arg;
  }

  // Prepare fields.
  $fields = array();
  if ($rules_act) {
    $rules_act_info = $rules_act->info();
    $fields = $rules_act_info['parameter'];
    foreach ($fields as $c_name => $c_field) {
      $c_value = array_shift($args);
      // Prepare date.
      if ($c_field['type'] == 'date') {
        $c_value = date('Y-m-d', $c_value);
      }
      $fields[$c_name]['value'] = $c_value;
    }
  }

  // Note: if this action started on cron run then redmine_sync_init_rest() return REDMINE_SYNC_REST_API_INIT_NO_USER_KEY
  if (count($fields) && redmine_sync_init_rest() == REDMINE_SYNC_REST_API_INIT_OK) {
    $rdm_user_info = redmine_sync_rest_get_user_by_mail($fields['mail']['value']);
    if ($rdm_user_info) {

      // Create Issue post array.
      $issue_post = array(
        'issue' => array(
          'project_id'       => $fields['project_id']['value'],
          'tracker_id'       => $fields['tracker_id']['value'],
          'subject'          => $fields['subject']['value'],
          'description'      => $fields['description']['value'],
          'status_id'        => $fields['status_id']['value'],
          'priority_id'      => $fields['priority_id']['value'],
          'assigned_to_id'   => $fields['assigned_to_id']['value'],
          'category_id'      => $fields['category_id']['value'],
          'fixed_version_id' => $fields['fixed_version_id']['value'],
          'start_date'       => $fields['start_date']['value'],
          'due_date'         => $fields['due_date']['value'],
          'estimated_hours'  => $fields['estimated_hours']['value'],
          'done_ratio'       => $fields['done_ratio']['value'],
        )
      );

      // Add custom fields values.
      $custom_fields_data = array();
      foreach ($fields as $c_field) {
        if (isset($c_field['is_custom']) &&
                  $c_field['is_custom']) {
          $c_buf = array();
          $c_buf['key'] = 'custom_field';
          $c_buf['attributes']['id'] = $c_field['rdm_id'];
          if (is_array($c_field['value'])) {
            $c_buf['attributes']['multiple'] = 'true';
            $c_buf['value'][0] = array('key' => 'value', 'attributes' => array('type' => 'array'));
            foreach ($c_field['value'] as $c_val) {
              $c_buf['value'][0]['value'][] = array('key' => 'value', 'value' => $c_val);
            }
          } else {
            $c_buf['value'] = array('value' => $c_field['value']);
          }            
          $custom_fields_data[]= $c_buf;
        }
      }
      if (count($custom_fields_data)) {
        $issue_post['issue'][] = array(
          'key'        => 'custom_fields',
          'attributes' => array('type' => 'array'),
          'value'      => $custom_fields_data,
        );
      }

      // Post request.
      $request = redmine_sync_rest_request('issues', 'POST', $issue_post, $rdm_user_info['login']);

      // Show result message.
      if ($request->code == 201) {
        drupal_set_message(t('New entry %name with id = %id was created.',       array('%name' => 'Issue', '%id' => (int)$request->data_prepared->id)));
        watchdog('redmine_sync', 'New entry %name with id = %id was created.',   array('%name' => 'Issue', '%id' => (int)$request->data_prepared->id));
      } else {
        drupal_set_message(t('Can\'t create new entry %name! Error: %error',     array('%name' => 'Issue', '%error' => $request->error . (isset($request->data_prepared->error) ? ' - '.$request->data_prepared->error : ''))), 'error');
        watchdog('redmine_sync', 'Can\'t create new entry %name! Error: %error', array('%name' => 'Issue', '%error' => $request->error . (isset($request->data_prepared->error) ? ' - '.$request->data_prepared->error : '')), WATCHDOG_ERROR);
      }

    }
  }

}


/**
 * ############################################################################################################
 * ######################################## Add new Project Membership ########################################
 * ############################################################################################################
 */

/**
 * Add new Project Membership: custom help callback.
 */
function redmine_sync_rules_project_membership_help(RulesAction $action) {
}

/**
 * Add new Project Membership: custom validation callback for the data set action.
 */
function redmine_sync_rules_project_membership_validate(RulesAbstractPlugin $plugin) {
}

/**
 * Add new Project Membership: custom callback for execute action.
 */
function redmine_sync_rules_project_membership_create() {
  $args = func_get_args();

  // Prepare inputed RulesAction and RulesState.
  $rules_act = null;
  $rules_ste = null;
  foreach ($args as $c_arg) {
    if ($rules_act == null && $c_arg instanceof RulesAction) $rules_act = $c_arg;
    if ($rules_ste == null && $c_arg instanceof RulesState)  $rules_ste = $c_arg;
  }

  // Prepare fields.
  $fields = array();
  if ($rules_act) {
    $rules_act_info = $rules_act->info();
    $fields = $rules_act_info['parameter'];
    foreach ($fields as $c_name => $c_field) {
      $c_value = array_shift($args);
      $fields[$c_name]['value'] = $c_value;
    }
  }

  // Note: if this action started on cron run then redmine_sync_init_rest() return REDMINE_SYNC_REST_API_INIT_NO_USER_KEY
  if (count($fields) && redmine_sync_init_rest() == REDMINE_SYNC_REST_API_INIT_OK) {
    $rdm_user_info = redmine_sync_rest_get_user_by_mail($fields['mail']['value']);
    if ($rdm_user_info) {

      // Create Project Membership post array.
      $membership_post = array(
        'membership' => array(
          'project_id' => $fields['project_id']['value'],
          'user_id'    => $fields['user_id']['value'],
        )
      );

      // Added role_ids.
      $c_buf = array('key' => 'role_ids', 'attributes' => array('type' => 'array'), 'values' => array());
      foreach ($fields as $c_field) {
        if (isset($c_field['entity_type']) && $c_field['entity_type'] == 'rid' && $c_field['value'] == '1') {
          $c_buf['value'][]= array('key' => 'role_id', 'value' => $c_field['entity_id']); 
        }        
      }
      if (count($c_buf)) {
        $membership_post['membership'][0] = $c_buf;
      }

      // Post request.
      $request = redmine_sync_rest_request('projects/'.$fields['project_id']['value'].'/memberships', 'POST', $membership_post, $rdm_user_info['login']);

      // Show result message.
      if ($request->code == 201) {
        drupal_set_message(t('New entry %name with id = %id was created.',       array('%name' => 'Project Membership', '%id' => (int)$request->data_prepared->id)));
        watchdog('redmine_sync', 'New entry %name with id = %id was created.',   array('%name' => 'Project Membership', '%id' => (int)$request->data_prepared->id));
      } else {
        drupal_set_message(t('Can\'t create new entry %name! Error: %error',     array('%name' => 'Project Membership', '%error' => $request->error . (isset($request->data_prepared->error) ? ' - '.$request->data_prepared->error : ''))), 'error');
        watchdog('redmine_sync', 'Can\'t create new entry %name! Error: %error', array('%name' => 'Project Membership', '%error' => $request->error . (isset($request->data_prepared->error) ? ' - '.$request->data_prepared->error : '')), WATCHDOG_ERROR);
      }

    }
  }

}


/**
 * ############################################################################################################
 * ############################################## Add new Project #############################################
 * ############################################################################################################
 */

/**
 * Add new Project: custom help callback.
 */
function redmine_sync_rules_project_help(RulesAction $action) {
}

/**
 * Add new Project: custom validation callback for the data set action.
 */
function redmine_sync_rules_project_validate(RulesAbstractPlugin $plugin) {
}

/**
 * Add new Project: custom callback for execute action.
 */
function redmine_sync_rules_project_create() {
  $args = func_get_args();

  // Prepare inputed RulesAction and RulesState.
  $rules_act = null;
  $rules_ste = null;
  foreach ($args as $c_arg) {
    if ($rules_act == null && $c_arg instanceof RulesAction) $rules_act = $c_arg;
    if ($rules_ste == null && $c_arg instanceof RulesState)  $rules_ste = $c_arg;
  }

  // Prepare fields.
  $fields = array();
  if ($rules_act) {
    $rules_act_info = $rules_act->info();
    $fields = $rules_act_info['parameter'];
    foreach ($fields as $c_name => $c_field) {
      $c_value = array_shift($args);
      $fields[$c_name]['value'] = $c_value;
    }
  }

  // Note: if this action started on cron run then redmine_sync_init_rest() return REDMINE_SYNC_REST_API_INIT_NO_USER_KEY
  if (count($fields) && redmine_sync_init_rest() == REDMINE_SYNC_REST_API_INIT_OK) {
    $rdm_user_info = redmine_sync_rest_get_user_by_mail($fields['mail']['value']);
    if ($rdm_user_info) {

      // Create Project post array.
      $project_post = array(
        'project' => array(
          'name'            => $fields['name']['value'],
          'description'     => $fields['description']['value'],
          'identifier'      => $fields['id']['value'],
          'homepage'        => $fields['homepage']['value'],
          'is_public'       => $fields['public']['value'],
          'parent_id'       => $fields['subproject_id']['value'],
          'inherit_members' => $fields['inherit_members']['value'],
        )
      );

      // Added enabled_module_names.
      foreach ($fields as $c_field) {
        if (isset($c_field['entity_type']) && $c_field['entity_type'] == 'module_name' && $c_field['value'] == '1') {
          $project_post['project'][]= array('key' => 'enabled_module_names', 'value' => $c_field['entity_id']);
        }
      }

      // Added tracker_ids.
      foreach ($fields as $c_field) {
        if (isset($c_field['entity_type']) && $c_field['entity_type'] == 'tracker_id' && $c_field['value'] == '1') {
          $project_post['project'][]= array('key' => 'tracker_ids', 'value' => $c_field['entity_id']);
        }
      }

      // Post request.
      $request = redmine_sync_rest_request('projects', 'POST', $project_post, $rdm_user_info['login']);
      if ($request->code == 201) {
        drupal_set_message(t('New entry %name with id = %id was created.',       array('%name' => 'Project', '%id' => (int)$request->data_prepared->id)));
        watchdog('redmine_sync', 'New entry %name with id = %id was created.',   array('%name' => 'Project', '%id' => (int)$request->data_prepared->id));
      } else {
        drupal_set_message(t('Can\'t create new entry %name! Error: %error',     array('%name' => 'Project', '%error' => $request->error)), 'error');
        watchdog('redmine_sync', 'Can\'t create new entry %name! Error: %error', array('%name' => 'Project', '%error' => $request->error), WATCHDOG_ERROR);
      }

    }
  }

}


/**
 * ############################################################################################################
 * ################################################# helpers ##################################################
 * ############################################################################################################
 */

function _redmine_sync_rules_custom_field_prepare($rdm_field_info, &$return, $create_type) {
  switch ($rdm_field_info['field_format']) {
    case 'bool':
      $return[$create_type]['parameter']['rdm_custom_' . $rdm_field_info['id']]['type'] = 'boolean';
      $return[$create_type]['parameter']['rdm_custom_' . $rdm_field_info['id']]['default value'] = $rdm_field_info['default_value'] instanceof SimpleXMLElement ? null : (bool)(int)$rdm_field_info['default_value'];
      break;
    case 'date':
      $return[$create_type]['parameter']['rdm_custom_' . $rdm_field_info['id']]['type'] = 'date';
      $return[$create_type]['parameter']['rdm_custom_' . $rdm_field_info['id']]['default value'] = (string)$rdm_field_info['default_value'];
      break;
    case 'float':
      $return[$create_type]['parameter']['rdm_custom_' . $rdm_field_info['id']]['type'] = 'decimal';
      $return[$create_type]['parameter']['rdm_custom_' . $rdm_field_info['id']]['default value'] = (float)$rdm_field_info['default_value'];
      break;
    case 'int':
      $return[$create_type]['parameter']['rdm_custom_' . $rdm_field_info['id']]['type'] = 'integer';
      $return[$create_type]['parameter']['rdm_custom_' . $rdm_field_info['id']]['default value'] = (int)$rdm_field_info['default_value'];
      break;
    case 'list':
      $return[$create_type]['parameter']['rdm_custom_' . $rdm_field_info['id']]['type'] = 'text';
      $return[$create_type]['parameter']['rdm_custom_' . $rdm_field_info['id']]['default value'] = (string)$rdm_field_info['default_value'];
      $return[$create_type]['parameter']['rdm_custom_' . $rdm_field_info['id']]['options list'] = '_redmine_sync_rules_custom_field_get_list';
      break;
    case 'link':
      $return[$create_type]['parameter']['rdm_custom_' . $rdm_field_info['id']]['type'] = 'uri';
      $return[$create_type]['parameter']['rdm_custom_' . $rdm_field_info['id']]['default value'] = (string)$rdm_field_info['default_value'];
      break;
    case 'user':
    case 'version':
      $return[$create_type]['parameter']['rdm_custom_' . $rdm_field_info['id']]['type'] = 'integer';
      $return[$create_type]['parameter']['rdm_custom_' . $rdm_field_info['id']]['label'].= ' (ID)';
      break;
    default: // Case for types: text (Redmine "long text"), string (Redmine "text").
      $return[$create_type]['parameter']['rdm_custom_' . $rdm_field_info['id']]['type'] = 'text';
      $return[$create_type]['parameter']['rdm_custom_' . $rdm_field_info['id']]['default value'] = (string)$rdm_field_info['default_value'];
    }
}

function _redmine_sync_rules_custom_field_get_list(RulesAction $action, $el_name) {
  $return = array();
  $rdm_custom_fields = redmine_sync_rest_get_custom_fields(null, REDMINE_SYNC_RET_VAL_TYPE_DEFAULT);
  foreach ($rdm_custom_fields as $c_field) {
    if ($el_name == 'rdm_custom_' . $c_field['id'] && in_array($c_field['customized_type'], array('time_entry', 'issue'))) {
      foreach ($c_field['possible_values'] as $c_value) {
        $return[(string)$c_value->value]= (string)$c_value->value;
      }
      return $return;
    }
  }
}

function _redmine_sync_rules_get_projects(RulesAction $action, $el_name) {
  return redmine_sync_rest_get_projects();
}

function _redmine_sync_rules_get_users(RulesAction $action, $el_name) {
  return redmine_sync_rest_get_users();
}

function _redmine_sync_rules_get_activities(RulesAction $action, $el_name) {
  return redmine_sync_rest_get_time_entry_activities();
}

function _redmine_sync_rules_get_priorities(RulesAction $action, $el_name) {
  return redmine_sync_rest_get_issue_priorities(null, REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST);
}

function _redmine_sync_rules_get_statuses(RulesAction $action, $el_name) {
  $statuses_list = redmine_sync_rest_get_issue_statuses(null, REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST);
  foreach ($statuses_list as $c_id => $c_name) {
    if ($c_name != 'New') {
      unset($statuses_list[$c_id]);
    }
  }
  return $statuses_list;
}

function _redmine_sync_rules_get_done_ratio(RulesAction $action, $el_name) {
  return drupal_map_assoc(range(0, 100, 10));
}

