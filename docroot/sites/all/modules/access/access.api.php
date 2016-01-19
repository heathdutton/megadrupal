<?php
/**
 * @file
 * Hooks provided by the access control kit module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Declares information about access realm types.
 *
 * Modules can implement this hook to make various types of Drupal objects and
 * values (such as taxonomy terms, lists, etc.) available as access realms.
 *
 * @return
 *   An associative array defining one or more access realm types. The keys are
 *   the machine names for the realm types, which must be unique among all
 *   modules implementing this hook; module authors are advised to prefix these
 *   machine names with the name of their module (e.g. "modulename_realmtype").
 *   The array values are associative arrays describing the new realm type, and
 *   should contain the following key=>value pairs:
 *
 *   - 'label': the human-readable name for the realm type (e.g. "a taxonomy
 *     term"). This will be displayed in the realm type field when creating an
 *     access scheme.
 *
 *   - 'field_type': the type of access field to be used to store realm values.
 *     This must be one of the following, based on the data type required by the
 *     new realm type: 'access_text', 'access_float', 'access_integer', or
 *     'access_boolean'. For example, for realms based on taxonomy terms, the
 *     field type would be 'access_integer' because terms are identified by tid.
 *
 *   - 'arguments': (optional) an associative array where the keys are
 *     configuration variables needed by the realm type, and the values are the
 *     default values for those variables. For example, taxonomy realms would
 *     need to set this equal to array('vocabulary' => '') in order to specify
 *     which vocabulary is to be used for access control. Modules making use of
 *     this option should also implement hook_access_realm_settings() in order
 *     to provide the form elements needed to set these argument values.
 */
function hook_access_realm_info() {
  $info['taxonomy_term'] = array(
    'label' => t('a taxonomy term'),
    'field_type' => 'access_integer',
    'arguments' => array('vocabulary' => ''),
  );
  return $info;
}

/**
 * Provides the form elements needed to configure a realm type for a scheme.
 *
 * Modules that implement hook_access_realm_info() and use the 'arguments' key
 * for one or more of their defined realm types should implement this hook to
 * provide the form elements needed to set those argument values when creating
 * an access scheme based on those realm types.
 *
 * @param $realm_type
 *   The access realm type machine name, as defined in hook_access_realm_info().
 * @param $has_data
 *   A boolean indicating whether access grants already exist for the scheme.
 * @param $values
 *   The current values of the realm type arguments. When creating a new access
 *   scheme, these will be the defaults set in hook_access_realm_info(). When
 *   editing an existing access scheme, these will be the values previously set
 *   by the scheme administrator.
 *
 * @return
 *   Field elements to populate the access scheme's realm settings form. The
 *   field keys should correspond to the 'arguments' array keys set for the
 *   $realm_type in hook_access_realm_info().
 */
function hook_access_realm_settings($realm_type, $has_data, $values = array()) {
  $form = array();
  switch ($realm_type) {
    case 'taxonomy_term':
      $vocabularies = taxonomy_get_vocabularies();
      $options = array();
      foreach ($vocabularies as $vocabulary) {
        $options[$vocabulary->machine_name] = $vocabulary->name;
      }
      $form['vocabulary'] = array(
        '#type' => 'select',
        '#title' => t('Vocabulary'),
        '#default_value' => $values['vocabulary'],
        '#options' => $options,
        '#required' => TRUE,
        '#disabled' => $has_data,
      );
      break;
  }
  return $form;
}

/**
 * Returns the list of realms for an access scheme.
 *
 * Modules that implement hook_access_realm_info() must also implement this hook
 * to provide the list of realms when one of the module's defined realm types is
 * used in an access scheme. The return value of this hook will be used as the
 * allowed values list for the access field when creating new access grants.
 *
 * @param $realm_type
 *   The access realm type machine name, as defined in hook_access_realm_info().
 * @param $arguments
 *   (optional) An array of configuration values set for this realm type on a
 *   scheme. See hook_access_realm_info() and hook_access_realm_settings() for
 *   information on how modules can define and set the configuration values
 *   needed for their realm type implementations.
 *
 * @return
 *   The array of available realms. Array keys are the values to be stored, and
 *   should match the data type declared for the realm type's 'field_type' in
 *   hook_access_realm_info(). Array values are the labels to display within
 *   the access field's widget. These labels should NOT be sanitized;
 *   options.module will handle that in the widget.
 */
function hook_access_realms($realm_type, $arguments = array()) {
  switch ($realm_type) {
    case 'taxonomy_term':
      // Re-use the allowed values function for term reference fields.
      $field = array();
      $field['settings']['allowed_values'][] = array('vocabulary' => $arguments['vocabulary'], 'parent' => 0);
      return taxonomy_allowed_values($field);
  }
}

/**
 * Act on access schemes when inserted.
 *
 * Modules implementing this hook can act on the scheme object after it has been
 * saved to the database.
 *
 * @param $scheme
 *   An access scheme object.
 */
function hook_access_scheme_insert($scheme) {
  if ($scheme->realm_type == 'example') {
    variable_set('access_scheme_example', TRUE);
  }
}

/**
 * Act on access schemes when updated.
 *
 * Modules implementing this hook can act on the scheme object after it has been
 * updated in the database.
 *
 * @param $scheme
 *   An access scheme object.
 */
function hook_access_scheme_update($scheme) {
  $status = ($scheme->realm_type == 'example') ? TRUE : FALSE;
  variable_set('access_scheme_example', $status);
}

/**
 * Respond to the deletion of access schemes.
 *
 * Modules implementing this hook can respond to the deletion of access schemes
 * from the database.
 *
 * @param $scheme
 *   An access scheme object.
 */
function hook_access_scheme_delete($scheme) {
  if ($scheme->realm_type == 'example') {
    variable_del('access_scheme_example');
  }
}

/**
 * Act on an access grant before it is saved.
 *
 * Modules implementing this hook can act on the access grant object before it
 * is inserted or updated.
 *
 * @param $grant
 *   An access grant object.
 */
function hook_access_grant_presave($grant) {
  $grant->foo = 'bar';
}

/**
 * Act on access grants when inserted.
 *
 * Modules implementing this hook can act on the grant object after it has been
 * saved to the database.
 *
 * @param $grant
 *   An access grant object.
 */
function hook_access_grant_insert($grant) {
  if ($grant->uid == 1) {
    drupal_set_message('An access grant has been created for the site administrator.');
  }
}

/**
 * Act on access grants when updated.
 *
 * Modules implementing this hook can act on the grant object after it has been
 * updated in the database.
 *
 * @param $grant
 *   An access grant object.
 */
function hook_access_grant_update($grant) {
  if ($grant->uid == 1) {
    drupal_set_message('Access changed for the site administrator.');
  }
}

/**
 * Respond to the deletion of access grants.
 *
 * Modules implementing this hook can respond to the deletion of access grants
 * from the database.
 *
 * @param $grant
 *   An access grant object.
 */
function hook_access_grant_delete($grant) {
  if ($grant->uid == 1) {
    drupal_set_message('An access grant was revoked for the site administrator.');
  }
}

/**
 * @} End of "addtogroup hooks".
 */
