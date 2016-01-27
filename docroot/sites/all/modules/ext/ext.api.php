<?php

/**
 * @file
 * Hooks provided by the Ext module.

 * See includes/*.inc and ext_tree/includes/*.inc for examples of most of the hooks.
 * This documentation assumes knowledge of the Ext JS library (documenting some
 * subset of Ext JS is beyond the scope of this document and is better done in
 * the actual Ext JS documentation).

 * All hook implementations, except administration form handling hooks, can
 * optionally be placed in a file named mymodule.ext.inc in a directory named
 * 'includes' in the implementing module (assuming your module is called
 * 'mymodule', eg in sites/all/modules/mymodule/includes/mymodule.ext.inc).
 * The admin form hooks can similarly be placed in a file named
 * mymodule.ext.admin.inc in the same directory,
 * (eg in sites/all/modules/my_module/includes/mymodule.ext.admin.inc)
 */


// ==== MODEL DEFINITION HOOKS ====


/**
 * Modules providing Model definitions must implement this hook to list the
 * "types" of Models they provide. The type name can be anything you like,
 * but should be representative of the type or source of the data.
 * The ext module uses the name to differentiate between different Model
 * definitions that are related, eg multiple Model definitions are based on
 * nodes, so have 'node' as the type. It is suggested that the name start with
 * (or be identical) to the implementing modules name to avoid type name
 * clashes (yes, the ext module blatantly ignores this recommendation).
 * The return value should be an array, keyed by type names, of associative
 * arrays containing settings for the type, including:
 * - user_created: boolean indicating if Model definitions of this type can be
 *     added, edited and deleted by the user. If true then
 *     hook_ext_model_addedit_form should be implemented.
 */
function hook_ext_model_types() {
  $types = array();
  foreach (array('node', 'user', 'view') as $type) {
    $types[$type] = array(
      'user_created' => FALSE,
    );
  }
  return $types;
}


/**
 * Allows modules to provide definitions of new Ext Models.
 * A module providing Model definitions must also implement
 * hook_ext_load_model_data() and hook_ext_save_model_data().
 * The return value should be an array, keyed by Model names, of arrays of model
 * definitions that follow the same format as
 * http://docs.sencha.com/ext-js/4-0/#!/api/Ext.data.Model , with the following
 * caveats:<pre>
 *   - A Model definition is an associative array.
 *
 *   - Model names should use a CamelCase format (eg Node or NodePage).
 *
 *   - In Ext JS the Models defined via this module are defined in
 *     Ext.drupal.[ModelName]
 *
 *   - For the sake of simplicity it is suggested that Model field names be
 *     identical to those used server-side (underscores and all).
 *
 *   - A Model definition should only specify a subset of the configuration
 *     options specified by the Ext Model API, at the moment these are:
 *     'associations', 'fields', 'idProperty', 'idgen', 'listeners',
 *     'persistenceProperty', 'validations'. While other properties such as
 *     'hasMany' will be included in the generated JS Model definition code,
 *     they will not be detected by the ext module or typically other modules
 *     (eg the ext_tree module will not detect 'hasMany' as an association and
 *     so will not allow selecting it to be used for the child/parent
 *     relationship).
 *
 *   - Model class names will be prefixed with a standard name space of the
 *     format: "<drupalext name space>.model", where <drupalext name space>
 *     is the name space given by ext_get_namespace() (and which may be set
 *     by an admin in the Basic settings form). Thus the typical full class
 *     name for the Node Model is "DrupalExt.model.Node".
 *
 *   - Association definitions:
 *     - Only the 'association' option should be used, not the 'hasMany' or
 *       'belongsTo' definition options.
 *
 *     - The 'model' property of an association definition should not be
 *       prefixed with any name space, it will be prefixed with the standard
 *       Model namespace as outlined in the previous paragraph.
 *
 *     - The 'foreignKey' and 'primaryKey' properties should always be
 *       specified.
 *
 *     - If the 'associationKey' or function name properties (either 'name' or
 *       'setterName' and 'getterName') of an association definition are not
 *       provided then defaults will be provided. The foreignKey (the name of
 *       the field on the Model defining the association) and the associated
 *       Model name are concatenated to produce a name. If the foreignKey starts
 *       with 'field_' then this is stripped from the generated name. The name
 *       is converted to CamelCase format. If the association type is 'hasMany'
 *       then 'List' is appended to the name. The associationKey is then set to
 *       the name but with a lower case first letter. If the association type
 *       is 'belongsTo' then the get and set functions are set to the generic
 *       name prefixed with 'get' and 'set' respectively. If the association
 *       type is 'hasMany' then the function name (there's only one for
 *       'hasMany' associations) is set to the generic name with a lowercase
 *       first letter. Note that if a custom associationKey is specified then
 *       this will be used when loading nested data from associations via
 *       ext_load_model_data().
 *
 *     - By default reverse associations are automatically generated. Eg for
 *       a 'belongsTo' association a 'hasMany' association will automatically
 *       be created on the associated Model. If you wish to suppress this
 *       behaviour set the '#no_reverse' property to TRUE in the association
 *       definition. This can be useful if a custom associationKey or function
 *       name is required. A '#reverse_of' property will be set to the
 *       associationKey of the reverse association for both the original and
 *       generated associations.
 *
 *     - '#load_nested_max_depth' can be set in an association definition to
 *       allow loading associated Model data in a nested format. This property
 *       should be an integer. When Model data is loaded this property is used
 *       to determine if associated Model data should be included: the initial
 *       call to ext_load_model_data has a depth value of 0, associated Model
 *       data is loaded by recursive calls to ext_load_model_data, increasing
 *       the depth for every call. When this depth exceeds the value set in
 *       #load_nested_max_depth for an association the associated data won't
 *       be loaded. For example imagine we have a Model definition with an
 *       association forming a linked-list data structure. If we set
 *       #load_nested_max_depth to 2 then when we load one instance of this
 *       Model then two associated instances will be loaded as well (nested
 *       into the returned instance data via the associationKey).
 *       Caution should probably be exercised to try and prevent loading large
 *       amounts of associated Model data unnecessarily.
 *       Note: Both ext_load_model_data() and ext_load_store_data(), as well as
 *       the loading Store data REST interface, allow overriding this property.
 *
 *      - It is recommended that modules providing hasMany associations for Models,
 *       either directly or indirectly via an automatically generated 'reverse'
 *       association, provide a Store for that Model that is able to filter results
 *       by the foreignKey for the hasMany association. This allows the ext module
 *       to automatically include nested Model data from hasMany associations when
 *       loading a Model instance (this behaviour can be controlled via the
 *       association definition and when calling ext_load_model_data()).
 *      
 *      - It is generally assumed by the ext module and the Ext JS library that the 
 *       associationKey is unique for a Model definition. 
 *
 *   - If the 'proxy' config option is not specified then a proxy definition
 *     will be added that uses the RESTful API and the
 *     hook_ext_[load|save|delete]_model_data hook provided by this module.
 *     This is the recommended method.
 *
 *   - A '#proxy_reader_root' config option can be provided to specify the
 *     'root' configuration option for the reader.
 *
 *   - If the 'alternateClassName' config option is not specified then a
 *     default alternateClassName of the form 'model.<model name>' will be
 *     provided, where <model name> is the name of the model. This is provided
 *     as the root of the namespace is modifiable by the user.
 *
 *   - By default Models will extend the DrupalModel class (which extends
 *     Ext.data.Model). However the 'extend' config option can be specified
 *     which will override this. See /js/ext/model.DrupalModel.js for more
 *     info about the config options provided by this class.
 *
 *   - Arbitrary non-Model API config options can be included by prefixing the
 *     key with a '#' (see below for examples).
 *
 *   - The definition should include a '#type' member that matches one of the
 *     types given by hook_ext_model_types().
 *
 *   - A Model may extend another Model by specifying an '#extends' member, eg
 *     a Model for a specific content type can extend a generic node Model.
 *     This module provides Model definitions for all content types and these
 *     definitions include all CCK fields defined for the type. Thus a module
 *     implementing a custom node type containing CCK fields can extend this
 *     definition and does not have to include the CCK fields (or other
 *     standard node fields) in its definition (and the definition provided
 *     by this module can be altered via hook_ext_models_alter() if necessary).
 *     NOTE: It is assumed that a module extending an existing Model definition
 *     will handle all processing for the save, load, validate and delete
 *     operations.
 *     Generally this can be handled by invoking the relevant hook on the module
 *     defining the base Model being extended, for example a module extending
 *     the definition for some content type (as defined by this module) could
 *     invoke ext_ext_[op]_model_data() to perform the basic processing.
 *
 *   - NOT IMPLEMENTED YET, MAY NOT BE FOR SOME TIME, IF EVER.
 *     A Model may specify that it requires server-side validation by setting
 *     the member '#server_validate' to TRUE (otherwise it may be left out or
 *     set to FALSE). If this is set then the module defining the Model should
 *     implement hook_ext_validate_model_data(). If any Model in the
 *     inheritance chain of a Model requires server-side validation then it
 *     will automatically be set for the sub-Model.
 *
 *   - Javascript code may be inserted for some values by wrapping it in
 *     <jscode> tags (inside a string value for the model definition), eg:
 *     "<jscode>Drupal.settings.myModule.myExtModel.serverValidate</jscode>".
 *     Note: the tags must be right at the start and end of the string, with no
 *     spaces or other characters before the opening tag or after the closing
 *     tag (this is due to the way drupal_to_js() works).
 * </pre>
 */
function hook_ext_models() {
  $models = array(
    'NodeMyType' => array (
      '#extends' => 'Node',
      '#type' => 'node',
      '#server_validate' => "<jscode>Drupal.settings.myModule.myExtModel.serverValidate</jscode>",
      'idProperty' => 'nid',
      'fields' => array (
        array (
          'name' => 'field_mynoderef', // This is a CCK nodereference field.
        ),
        //...
        array (
          'name' => 'my_user_ref', // A custom defined reference to a user.
        ),
      ),
      'associations' => array (
        array (
          'type' => 'hasMany',    // The CCK field allows multiple values,
          'model' => 'NodePage',  // so hasMany makes the most sense.
          'primaryKey' => 'field_mynoderef',
          'foreignKey' => 'nid',
        ),
        //...
        array (
          'type' => 'belongsTo', // Only a single value is allowed.
          'model' => 'User',
          'primaryKey' => 'my_user_ref',
          'foreignKey' => 'uid',
        ),
      ),
      'validations' => array (
        array (
          'field' => 'field_mynoderef',
          'type' => 'presence',
        ),
        //...
        array (
          'field' => 'my_user_ref',
          'type' => 'inclusion',
          'list' => array(1, 2 ,3),
        ),
      ),
    ),
  );
  return $models;
}


/**
 * Allows modules to alter any aspect of existing Model definitions.
 * The $models parameter contains an array in the same format as that returned
 * by hook_ext_models().
 *
 * @see hook_ext_models()
 *
 * @param $models An array containing all existing Model definitions.
 */
function hook_ext_models_alter(&$models) {
  $models['NodeMyType']['idProperty'] = 'myIdField';
}



// ==== MODEL CRUD HOOKS ====


/**
 * Modules providing Model definitions must implement this hook to allow
 * the system to determine if the given user has permission to
 * perform the specified operation on the specified model data.
 * The return value should be TRUE or FALSE, to allow or deny permission
 * respectively.
 *
 * @param $model_instance_id The unique ID for the model
 *   instance, for example for a Node Model this would be the node ID (nid).
 *   The ID will be null for the 'create' operation.
 * @param $model_name The name of the Model to check access for.
 * @param $op Specifies the operation to perform. This can be one
 *   'create', 'view', 'update' or 'delete'.
 * @param $account The user account in question.
 */
function hook_ext_access_model_data($model_instance_id, $model_name, $op, $account) {
  $model = ext_get_models($model_name);
  if ($model['#type'] == 'node') {
    if ($op == 'create') {
      $node_type = substr(_ext_uncamelcase($model_name), 5);
      return node_access($op, $node_type, $account);
    }
    $node = node_load($model_instance_id);
    return node_access($op, $node, $account);
  }
  elseif ($model['#type'] == 'user') {
    if ($op == 'create')
      return user_access('administer users');
    $account = user_load($model_instance_id);
    if ($op == 'update' || $op == 'delete')
      return user_edit_access($account);
    if ($op == 'view')
      return user_view_access($account);
  }
  return FALSE;
}


/**
 * Modules providing Model definitions must implement this hook to allow
 * loading the data for an instance of any Model it defines. Other modules may
 * alter the data via hook_ext_load_model_data_alter. This data will then be sent to
 * the Ext JS client.
 * The return value should be an associative array containing keys matching
 * those defined in the Model $model_name. The returned array may also contain
 * other data, which will be ignored. FALSE should be returned if the load
 * failed.
 *
 * @param $model_instance_id An array of unique IDs for the model
 *   instance(s), for example for a Node Model this would be the node ID (nid).
 * @param $model_name The name of the Model that the data should be an
 *   instance of.
 * @param $options An array of options that may be used by some
 *   implementations of hook_ext_load_model_data. Two options defined (and
 *   reserved) by the ext module are: 'load_nested_max_depth',
 *   'load_nested_depth', 'load_nested_parent_id' and 'load_nested_parent',
 *   see ext_load_model_data() for more info on these. If you are going to
 *   define your own options, it's a very good idea to prefix them with the
 *   name of your module.
 * @return an array of model data arrays keyed by ID.
 */
function hook_ext_load_model_data($model_instance_ids, $model_name, $options) {
  $model = ext_get_models($model_name);
  if (isset($model['#entity'])) {
    $data = entity_load($model['#entity'], $model_instance_ids);
    foreach ($data as $id => &$entity) {
      $entity = (array) $entity;
      _ext_convert_from_cck_fields($model, $entity);
    }
    return $data;
  }
  return array();
}


/**
 * Allows any module to alter the data for a Model instance after it is loaded
 * and before it is sent to the Ext JS client.
 *
 * @param $data The data to alter.
 * @param $model_name The name of the Model that the data is an instance of.
 * @param $options An array of options that may be used by some
 *   implementations of hook_ext_load_model_data. Two options defined (and
 *   reserved) by the ext module are: 'load_nested_max_depth' and
 *   'load_nested_depth', see ext_load_model_data() for more info on these.
 */
function hook_ext_load_model_data_alter(&$data, $model_name, $options) {
  if ($model_name = 'NodeMyType') {
    $data['myField'] = 'abc';
  }
}


/**
 * Modules providing Model definitions must implement this hook to allow
 * saving the data for an instance of any Model it defines. Other modules may
 * alter the data before it is saved via hook_ext_save_model_data_alter.
 *
 * @param $data An associative array containing keys matching fields defined in
 *   the Model $model_name. All data sent from the Ext client is decoded as arrays
 *   (associative to represent Javascript objects). All fields in $data
 *   should be updated with current values (eg calculated values, The ID for new
 *   Model instances, etc).
 * @param $model_name The name of the Model that the data is an instance of.
 * @param $new If set to TRUE then a new instance should be created,
 *   EVEN IF $data contains a primary ID, otherwise the existing instance
 *   represented by $data will contain the primary ID and should be updated
 *   accordingly.
 * @param $options An array of options that may be used by some
 *   implementations of hook_ext_save_model_data. See hook_ext_load_model_data for 
 *   a list of reserved key values. Options are typically passed via the REST
 *   create/put request.
 * @return Should evaluate to FALSE if the save failed, otherwise TRUE.
 */
function hook_ext_save_model_data(&$data, $model_name, $new, $options) {
  $model = ext_get_models($model_name);
  if ($model['#type'] == 'view') {
    // NOTE: this is not implemented yet, see below TODO.
    return FALSE;
  }

  if ($model['#type'] == 'node') {
    if ($new) {
      unset($data['nid']);
      unset($data['vid']);
    }
    _ext_convert_to_drupal_fields($data);

    // node_save expects an object.
    $data = (object) $data;

    $changed_orig = isset($data->changed) ? $data->changed : NULL;
    node_save($data);
    // If the node is new but didn't save.
    if (!$data->nid) {
      return FALSE;
    }
    $changed_new = db_result(db_query("SELECT changed FROM {node} WHERE nid=%d", $data->nid));
    // If the node wasn't new and didn't save.
    if ($changed_orig == $changed_new) {
      return FALSE;
    }
    return TRUE;
  }
  elseif ($model['#type'] == 'user') {
    if ($new) {
      unset($data['uid']);
    }
    // user_save expects an object.
    $data = (object) $data;
    return user_save($data);
  }
}


/**
 * Allows any module to alter the data for a Model instance before it is saved.
 *
 * @param $data An associative array containing keys matching those defined in
 *   the Model $model_name. All data sent from the Ext client is decoded as arrays
 *   (associative to represent Javascript objects).
 * @param $model_name The name of the Model that the data is an instance of.
 */
function hook_ext_save_model_data_alter(&$data, $model_name) {
  if ($model_name = 'NodeMyType') {
    $data['myField'] = 'abc';
  }
}


/**
 * Modules providing Model definitions should implement this hook if any of
 * their Model definitions have the 'server_validate' member set to TRUE. Other
 * modules may also implement this hook to provide validation (particularly if
 * they implement hook_ext_[load|save]_model_data_alter().
 *
 * @param $data An associative array containing keys matching those defined in
 *   the Model $model_name. All data sent from the Ext client is decoded as arrays
 *   (associative to represent Javascript objects).
 * @param $model_name The name of the Model that the data is an instance of.
 * @return Should be an array keyed by field names containing translated error
 *   messages (if there are no errors an empty array should be returned).
 */
function hook_ext_validate_model_data($data, $model_name) {
  $model = ext_get_models($model_name);
  if ($model['#type'] == 'node') {
    // node_validate expects an object.
    $data = (object) $data;
    _ext_convert_from_drupal_fields($data);
    node_validate($data);
    $errors = array();
    // Error messages are set using form_set_error in node_validate,
    // retrieve them with form_get_errors().
    foreach (form_get_errors() as $key => $error) {
      $field = array_pop(explode('][', $key));
      $errors[$field] = $error; // Should already be translated.
    }
    form_set_error(NULL, NULL, TRUE); // Clear form errors.
    drupal_get_messages(); // Clear message queue.
    return $errors;
  }
  return array();
}


/**
 * Modules providing Model definitions must implement this hook to allow
 * deleting the data for an instance of any Model it defines.
 *
 * @param $model_instance_id The unique ID for the model
 *   instance, for example for a Node Model this would be the node ID (nid).
 * @param $model_name The name of the Model of the instance to delete.
 */
function hook_ext_delete_model_data($model_instance_id, $model_name) {
  $model = ext_get_models($model_name);
  if ($model['#type'] == 'node') {
    node_delete($model_instance_id);
    return db_result(db_query("SELECT COUNT(*) FROM {node} WHERE nid=%d", $model_instance_id)) == 0;
  }
  elseif ($model['#type'] == 'user') {
    user_delete(array(), $model_instance_id);
    return db_result(db_query("SELECT COUNT(*) FROM {users} WHERE uid=%d", $model_instance_id)) == 0;
  }
}


// ==== MODEL USER CREATED HOOKS ====


/**
 * For modules that define Model types, allows specifying additional form
 * elements on the Model adding and editing form. By default these forms
 * contain only an 'enable' checkbox (and the add form contains a Model name
 * field. The below validate and submit hooks may be used to handle custom
 * validation and submission.
 *
 * @param $form_state The form state array.
 * @param $model_type The name of the the type of the Model being created. @see hook_ext_model_types().
 * @param $model_name The name of the Model as entered by the user. If an add form
 *   is being displayed this will be NULL, if an edit form is being displayed this
 *   will be the name the user entered.
 */
function hook_ext_model_addedit_form(&$form_state, $model_type, $model_name=NULL) {
  $form = array();
  $form['settings'] = array(
    '#tree' => TRUE,
  );

  // If a new tree node Model is being added, choose the Model to base it on.
  if ($model_name == NULL) {
    $options = array();
    $models = ext_get_models();
    foreach ($models as $base_model_name => $base_model) {
      if ($base_model['#type'] != 'treenode' && !empty($base_model['associations'])) {
        // To be used as a Tree node Model the Model must be able to reference
        // itself in order to form a hierarchy.
        $self_reference = FALSE;
        foreach ($base_model['associations'] as $assoc) {
          if ($assoc['model'] == $base_model_name) {
            $self_reference = TRUE;
            break;
          }
        }
        if ($self_reference) {
          $options[$base_model_name] = $base_model_name;
        }
      }
    }
    $form['settings']['base_model'] = array(
      '#type' => 'select',
      '#title' => t("Base Model"),
      '#description' => t("A Tree node Model is based on an existing Model. Once you have selected a Model to base it on you can configure the Tree node Model settings. The list of Models that can be used contains only existing non-Tree node Models, and Models that contain an association referencing the same Model (ie the base Model must have a field which can reference another instance of the base Model in order to construct a tree hierarchy)."),
      '#options' => $options,
    );
  }
  // Otherwise edit the existing tree node Model.
  else {
    $settings = ext_tree_get_model_settings($model_name);
    $base_model = ext_get_models($settings['base_model']);

    $form['settings']['base_model'] = array(
      '#type' => 'value',
      '#value' => $settings['base_model'],
    );

    $associations = array();
    foreach ($base_model['associations'] as $assoc) {
      // If self-referential association.
      if ($assoc['type'] == 'belongsTo' && $assoc['model'] == $settings['base_model']) {
        $associations[$assoc['associationKey']] = $assoc['associationKey'];
      }
    }
    $form['settings']['association'] = array(
      '#type' => 'select',
      '#title' => t("Parent association"),
      '#description' => t("Select the association that is used to specify the parent of a tree node. This association will be used to construct the tree hierarchy. Only associations that reference the base Model type and which are of the type 'belongsTo' (indicating a 'parent' type relationship) are shown."),
      '#options' => $associations,
      '#default_value' => ($settings && isset($settings['association']) ? $settings['association'] : NULL),
    );

    // Collect all fields from the base Model.
    $field_names = _ext_extract_values_from_sub_arrays_by_key('name', $base_model['fields'], 'value');
    array_unshift($field_names, '<'. t('None'). '>');

    // Get configurable Ext tree node options.
    $options = ext_tree_node_options();
    $form['settings']['field_options'] = array(
      '#type' => 'fieldset',
      '#title' => t("Field to tree node option mapping"),
      '#description' => t("You can select which fields from the base Model to use for various properties of the tree node. When an instance of the tree node Model is loaded the mapped values from the base Model will be used to populate the tree node fields, and when an instance of the tree node Model is saved the (possibly modified) values from the tree node Model will be copied back into the mapped base Model fields. It is recommended that at least the 'text' option be specified. Refer to the Ext documentation for details for the tree node fields: !link", array('!link' => l('http://docs.sencha.com/ext-js/4-0/#!/api/Ext.data.NodeInterface', 'http://docs.sencha.com/ext-js/4-0/#!/api/Ext.data.NodeInterface'))),
    );
    foreach ($options as $option) {
      $form['settings']['field_options'][$option] = array(
        '#type' => 'select',
        '#title' => $option, // Don't localise/translate as this is the config property for an Ext class.
        '#options' => $field_names,
        '#default_value' => ($settings && isset($settings['field_options'][$option])) ? $settings['field_options'][$option] : NULL,
        // Don't allow mapping tree node fields that are already defined in
        // the base Model.
        '#enabled' => !isset($field_names[$option]),
      );
    }
  }

  return $form;
}


/**
 * For modules that define Model types, allows validating the form data from
 * Model adding and editing forms.
 *
 * @see hook_ext_model_addedit_form()
 *
 * @param $form The form definition array.
 * @param $form_state The form state array.
 * @param $model_type The name of the the type of the Model being created. @see hook_ext_model_types().
 * @param $model_name The name of the Model.
 */
function hook_ext_model_addedit_form_validate($form, &$form_state, $model_type, $model_name) {
  if ($form_state['values']['settings']['myField'] != 'theCorrectValue') {
    form_set_error('settings][myField', t("Please enter a correcct value."));
  }
}

/**
 * For modules that define Model types, allows custom submit handling of the form
 * data from Model adding and editing forms.
 *
 * @see hook_ext_model_addedit_form()
 *
 * @param $form The form definition array.
 * @param $form_state The form state array.
 * @param $model_type The name of the Model type being created. @see hook_ext_model_types().
 * @param $model_name The name of the Model.
 */
function hook_ext_model_addedit_form_submit($form, &$form_state, $model_type, $model_name) {
  // If this is a new tree node Model redirect to the edit page so the user
  // can configure it.
  if (!isset($tree_models[$model_name])) {
    $form_state['redirect'] = "admin/build/ext/models/edit/treenode/$model_name";
  }

  $tree_models = ext_tree_get_model_settings();
  $tree_models[$model_name] = $form_state['values']['settings'];
  variable_set('ext_tree_models', $tree_models);
}


/**
 * Modules that define user creatable Model types should implement this hook to
 * remove any data stored for a user created Model that has been deleted.
 *
 * @param $model_type The name of the type of the Model being deleted.
 *   @see hook_ext_model_types().
 * @param $model_name The name of the Model to delete.
 */
function hook_ext_model_delete($model_type, $model_name) {
  $tree_models = ext_tree_get_model_settings();
  unset($tree_models[$model_name]);
  variable_set('ext_tree_models', $tree_models);
}


// ==== STORE DEFINITION HOOKS ====


/**
 * Modules providing Store definitions must implement this hook to list the
 * "types" of Stores they provide. The type name can be anything you like,
 * but should be representative of the type or source of the data.
 * The ext module uses the name to differentiate between different Store
 * definitions that are related, eg multiple Store definitions are based on
 * Views, so have 'view' as the type. It is suggested that the name start with
 * (or be identical) to the implementing modules name to avoid type name
 * clashes (yes, the ext module blatantly ignores this recommendation).
 * The return value should be an array, keyed by type names, of associative
 * arrays containing settings for the type, including:
 * - user_created: boolean indicating if Store definitions of this type can be
 *     added, edited and deleted by the user. If TRUE then
 *     hook_ext_store_addedit_form should be implemented.
 */
function hook_ext_store_types() {
  $types = array();
  $types['tree'] = array(
    'user_created' => TRUE,
  );
  return $types;
}


/**
 * Allows modules to provide definitions of new Ext Stores.
 *
 * It is recommended that modules providing hasMany associations for Models,
 * either directly or indirectly via an automatically generated 'reverse'
 * association, provide a Store for that Model that is able to filter results
 * by the foreignKey for the hasMany association. This allows the ext module
 * to automatically include nested Model data from hasMany associations when
 * loading a Model instance (this behaviour can be controlled via the
 * association definition and when calling ext_load_model_data()).
 *
 * The return value should be an array, keyed by Store names/IDs, of arrays of
 * Store definitions that follow the same format as
 * http://docs.sencha.com/ext-js/4-0/#!/api/Ext.data.Store , with the following
 * caveats:<pre>It is recommended
 * - A Store definition is an associative array.
 *   - Arbitrary non-Model API config options can be included by prefixing the
 *     key with a '#' (see below for examples).
 *   - The definition should include a '#type' member that matches one of the
 *     types given by hook_ext_store_types().
 *   - The Store definition should not specify a 'storeId', this is set
 *     automatically by the ext module to be the Store name/ID.
 *   - If the 'proxy' config option is not specified then a proxy definition
 *     will be added that uses the RESTful API and hook_ext_load_store_data
 *     hook provided by this module. This is the recommended method.
 *   - A '#proxy_reader_root' config option can be provided to specify the
 *     'root' configuration option for the reader.
 *   - The following meta properties can be declared:
 *     - #class Optionally specify the class of the Store. Default is
 *       Ext.data.Store.
 *     - #generic indicates that the Store will return all Model instances
 *       (unless filters are specified).
 *     - #filterable_fields is an array of field names that the Store can
 *       filter on.
 *     - #sortable_fields is an array of field names that the Store can sort
 *       on.
 * </pre>
 */
function hook_ext_stores() {
  $stores = array();
  $tree_stores = ext_tree_get_store_settings();
  foreach ($tree_stores as $store_name => $settings) {
    $store = array(
      '#type' => 'tree',
      '#class' => 'Ext.data.TreeStore',
      'model' => $settings['model'],
      'autoLoad' => TRUE,
    );
    $stores[$store_name] = $store;
  }
  return $stores;
}


/**
 * Allows modules to alter any aspect of existing Store definitions.
 * The $models parameter contains an array in the same format as that returned
 * by hook_ext_stores().
 * @param $stores An array containing all existing Store definitions. @see hook_ext_stores().
 */
function hook_ext_stores_alter(&$stores) {
  $stores['MyStore']['autoLoad'] = FALSE;
}


// ==== STORE CRUD HOOKS ====


/**
 * Modules providing Store definitions must implement this hook to allow
 * the system to determine if the current or given user has permission to
 * load/view data from the specified Store.
 * The return value should be TRUE or FALSE, to allow or deny permission
 * respectively.
 *
 * @param $store_name The name of the Store to check access for.
 * @param $options An array of options as provided to hook_ext_load_store_data().
 * @param $account The user account in question.
 */
function hook_ext_access_store_data($store_name, $options, $account) {
  list($model, $view_name, $display_name) = _ext_unpack_view_model($store_name);
  if ($view_name) {
    $view = views_get_view($view_name);
    if ($view) {
      return $view->access($display_name, $account);
    }
  }
  return FALSE;
}


/**
 * Modules providing Store definitions must implement this hook to allow
 * loading the data for a Store it defines. Other modules may alter the data
 * via hook_ext_load_store_data_alter. This data will then be sent to
 * the Ext JS client.
 * The return value may be an array of Model instance IDs (these will then
 * be passed to ext_load_model_data to retrieve the actual data), or an array of
 * arrays containing Model instance data. 
 * NOTE: the returned array must have sequential numeric keys starting from 0 (this
 * can easily be achieved by passing the array to return through array_values()).  
 *
 * NOTE: Access checks for individual Model instances returned must be
 * performed by this function. This could be achieved by calling
 * ext_access_model_data() for each potential Model instance, however
 * it's often more efficient to include the access check as part of the
 * query that selects the Model instances.
 *
 * FALSE should be returned if the load failed.
 * The $options parameter is an associative array containing information that
 * the defining module needs to determine the result set (eg View arguments).
 *
 * A predefined custom option that may be present in $options is
 * 'load_nested_max_depth', which specifies the depth of nested model data to
 * include from associations. If your implementation of hook_ext_load_store_data
 * returns Model instance IDs then this can be ignored (ext_load_store_data()
 * will handle it). Otherwise you should probably honour it. See
 * ext_load_store_data() for more information.
 *
 * @param $store_name The name of the Store to load data for.
 * @param $options An array of options used to query the store, such as 'limit',
 *   'start' and 'page' or filters and sorters. If you are going to define your
 *   own options, it's a very good idea to prefix them with the name of your module.
 */
function hook_ext_load_store_data($store_name, $options) {
  // Determine object type of the view.
  $store = ext_get_stores($store_name);
  $model = ext_get_models($store['model']);
  $data = mymodule_get_my_store_data($store, $options);

  // If a View based Model is being used (when display row style is 'fields').
  if ($model['#type'] == 'view') {
    // Just use raw result data from the View.
    $return = $data;
  }
  else {
    // Get the ID of the base object/table type the View.
    $return = array();
    foreach ($data as $row) {
      $return[] = $row[$model['idProperty']];
    }
  }
  return $return;
}


// ==== STORE USER CREATED HOOKS ====


/**
 * For modules that define Store types, allows specifying additional form
 * elements on the Store adding and editing form. By default these forms
 * contain only an 'enable' checkbox (and the add form contains a Store name
 * field. The below validate and submit hooks may be used to handle custom
 * validation and submission.
 *
 * @param $form_state The form state array.
 * @param $store_type The name of the the type of the Store being created. @see hook_ext_store_types().
 * @param $store_name The name of the Store as entered by the user. If an add form
 *   is being displayed this will be NULL, if an edit form is being displayed this
 *   will be the name the user entered.
 */
function hook_ext_store_addedit_form(&$form_state, $store_type, $store_name=NULL) {

}


/**
 * For modules that define Store types, allows validating the form data from
 * Store adding and editing forms.
 *
 * @see hook_ext_model_addedit_form()
 *
 * @param $form The form definition array.
 * @param $form_state The form state array.
 * @param $store_type The name of the the type of the Store being created. @see hook_ext_store_types().
 * @param $store_name The name of the Model.
 */
function hook_ext_store_addedit_form_validate($form, $form_state, $store_type, $store_name) {
}


/**
 * For modules that define Store types, allows validating the form data from
 * Store adding and editing forms.
 *
 * @see hook_ext_model_addedit_form()
 *
 * @param $form The form definition array.
 * @param $form_state The form state array.
 * @param $store_type The name of the the type of the Store being created. @see hook_ext_store_types().
 * @param $store_name The name of the Store.
 */
function hook_ext_store_addedit_form_submit($form, $form_state, $store_type, $store_name) {
}


/**
 * Modules that define user creatable Store types should implement this hook to
 * remove any data stored for a user created Store that has been deleted.
 *
 * @param $store_type The name of the type of the Store being deleted.
 *   @see hook_ext_store_types().
 * @param $store_name The name of the Store to delete.
 */
function hook_ext_store_delete($store_type, $store_name) {
  $tree_stores = ext_tree_get_store_settings();
  unset($tree_stores[$store_name]);
  variable_set('ext_tree_stores', $tree_stores);
}


// ==== MISCELLANEOUS HOOKS ====


/**
 * Allows directly altering the generated JS code files and folder structure
 * before they are saved into the <default files directory>/ext directory.
 * The $js parameter is a recursive associative array in which the keys are a
 * file or directory name and, respectively, the values are either the file
 * contents (as a string) or an array of files and folders in the
 * (sub-)directory.
 * By default the folder structure and file names follow the best-practice
 * conventions outlined in the Ext JS documentation.
 * @param $js The array containing all JavaScript code to be saved.
 */
function hook_ext_generated_js_alter(&$js) {
  if (!isset($js['myFolder'])) {
    $js['myFolder'] = array();
  }
  $js['myFolder']['myjs.js'] = "alert('My JS!');";
}


/**
 * Allows adding properties to the configuration object passed to
 * Ext.application(). This is useful if you want to define, for example,
 * your own launch() function.
 *
 * @return An associative array where keys are the names of properties to set
 *   and values are the corresponding values to assign to the property.
 */
function hook_ext_application_properties() {
  return array(
    'controllers' => array('Files', 'Debug'),
    'autoCreateViewport' => TRUE,
    'launch' => "<jscode>function() { CO.app = this; }</jscode>",
  );
}


/**
 * Allows adding arbitrary JS files to the Ext application. This is how JS files
 * for Controllers and Views are usually added.
 * The return value of implementations of this hook should be the path to a
 * directory containing the JS files to add. The files in this directory
 * are included in the Ext application directory (typically
 * sites/default/files/ext/app). Only files with a 'js' extensions are included.
 * To specify that a file should be included in a sub-directory its file name
 * should be prefixed with the name(s) of the sub-directories seperated by
 * periods, for example to include a file called bar.js in the sub-directories
 * view/foo it should be named 'view.foo.bar.js' (it will then typically be
 * copied to sites/default/files/ext/app/view/foo/bar.js).
 *
 * @return The path to the directory containing the JS file to add.
 */
function hook_ext_additional_js_files() {
  return drupal_get_path('module', 'mymodule'). "/js/ext";
}


/**
 * Called when ext_load() is called. Allows loading additional JS or CSS, or
 * performing other additional tasks, when Ext is being loaded.
 */
function hook_ext_load() {

}