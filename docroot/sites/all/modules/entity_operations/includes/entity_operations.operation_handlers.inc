<?php

/**
 * @file
 * Contains handler classes for basic operations as well as base classes:
 *
 * - EntityOperationsOperationBase: abstract base class for all operations.
 *  - EntityOperationsOperationPage: parent class for operations that provide
 *    a page.
 *    - EntityOperationsOperationEntityView: displays the entity.
 *      - EntityOperationsOperationEntityViewOperations: displays the entity,
 *        with available operations in a fieldset.
 *    - EntityOperationsOperationDevel: devel output for the entity.
 *      - EntityOperationsOperationMetadata: devel output for the entity's
 *        metadata properties.
 *    - EntityOperationsOperationPageViews: output the result of a Views
 *      entity tab display.
 *  - EntityOperationsOperationForm: parent class for form operations.
 *    - EntityOperationsOperationEdit: the edit form for the entity.
 *    - EntityOperationsOperationAction: parent class for actions on the entity.
 *      - EntityOperationsOperationPublish: basic publish action.
 *      - EntityOperationsOperationUnPublish: basic unpublish action.
 *  - EntityOperationsOperationAdd: the add form for the entity.
 */

/**
 * Abstract base class for operation handlers.
 */
abstract class EntityOperationsOperationBase {

  /**
   * The operation for entity_access().
   *
   * Subclasses can set this to NULL to allow the path component to determine
   * the access verb.
   *
   * @see getAccessVerb()
   */
  protected $access_verb = 'view';

  /**
   * The path component this operation is on.
   */
  protected $path;

  /**
   * The type of the entity this operation is for.
   */
  protected $entityType;

  /**
   * Returns basic information about the operation.
   *
   * @return
   *  An array with the following keys:
   *  - 'label': The localized admin label for this operation.
   *    See getOperationAdminString().
   *  - 'description': The localized admin description text for this operation.
   *  - 'uses form': Whether this operation's output is a form. This causes a
   *    different page callback to be used when output as a menu item.
   *  - 'provides views field': Whether this operation should be exposed as a
   *    Views field.
   *  - 'provides vbo': Whether this operation should be exposed to Views Bulk
   *    Operations.
   *  - 'provides services resource': Whether this operation should be exposed
   *    to Services as a targeted action on the entity resource.
   * Note that the 'provides vbo' and 'provides services resource' properties
   * should be used with EntityOperationsOperationAction or a subclass.
   */
  function operationInfo() {
    return array(
      // Admin labels.
      'label' => 'Label',
      'description' => 'Description',
      // Default behaviour is to provide a menu item.
      'uses form' => FALSE,
      'provides vbo' => FALSE,
      'provides views field' => TRUE,
    );
  }

  /**
   * Returns strings for the operation.
   *
   * These should be translated, and the following placeholders may be used
   * without values:
   *  '%entity-type': The human-readable label for the entity type.
   *  '@entity-type': The same, but as plain text.
   *  '%label': The entity label.
   *  '@label': The same, but as plain text.
   * If further substitutions are required, they can be provided by overriding
   * getOperationStringSubstitutions().
   *
   * @return
   *  An array containing the following keys:
   *  - 'tab title': The title to show on the menu tab. Note this should not be
   *    translated, and does not allow substitutions. Also, if a handler
   *    overrides menu_item(), it may be simpler to set the tab's menu item
   *    title there, especially if defining the title would be the only purpose
   *    of overriding operationStrings().
   *  - 'page title': The page title to show when the form is shown as a tab.
   */
  function operationStrings() {
    return array(
      'tab title' => '',
      'page title' => '@label',
    );
  }

  /**
   * Constructor for operation handler objects.
   *
   * @param $entity_type
   *  The entity type.
   * @param $operation_path
   *  The path component for the operation.
   */
  public function __construct($entity_type, $operation_path) {
    $this->entityType = $entity_type;
    $this->path = $operation_path;
  }

  /**
   * Get an admin string for this operation, as defined in operationInfo().
   *
   * We need this rather than grabbing it directly because some handlers need
   * to do wacky things rather than just return the string as defined.
   *
   * @param $string_key
   *  The string to get. One of 'label' or 'description'.
   *
   * @return
   *  The translated string.
   */
  public function getOperationAdminString($string_key) {
    $info = $this->operationInfo();
    return $info[$string_key];
  }

  /**
   * Get an operation string.
   *
   * @param $string_key
   *  The key in the array in operationStrings() for the string to retrieve.
   * @param $entity_type
   *  The entity type.
   * @param $entity
   *  The entity. Pass NULL if there is no entity in context.
   * @param $operation_path
   *  The key for the operation.
   * @param $parameters
   *  (Optional) The parameters array from the form submission values.
   *
   * @return
   *  A translated string, with replacements made. (With the exception of the
   *  'tab title' string, which is returned untranslated.)
   */
  function getOperationString($string_key, $entity_type, $entity, $operation_path, $parameters = array()) {
    $strings = $this->operationStrings();

    // The 'tab title' string is a special case, as it is passed to hook_menu()
    // and therefore can't have substitutions. Just return it.
    // TODO: figure out 'title arguments' in hook_menu() so we can at least
    // pass the entity type and entity; assuming this is ever a needed feature!
    if ($string_key == 'tab title') {
      return $strings['tab title'];
    }

    $substitutions = $this->getOperationStringSubstitutions($entity_type, $entity, $operation_path, $parameters);

    // Run the string through format_string() to replace the placeholders
    // which t() will have ignored.
    return format_string($strings[$string_key], $substitutions);
  }

  /**
   * Return an array of string substitutions for t().
   *
   * This may be overridden to add further substitutions, in particular using
   * the parameters array for the submit confirmation message.
   *
   * @param $entity_type
   *  The entity type.
   * @param $entity
   *  The entity. Pass NULL if there is no entity in parameters.
   * @param $operation_path
   *  The key for the operation.
   * @param $parameters
   *  (optional) The parameters array from the form submission values. When this is not
   *  available, for example when retrieving the button label, this will be an
   * empty array.
   *
   * @return
   *  An array of string substitutions as used by t(). These should match those
   *  used in operationStrings(), and should include at least:
   *  - '%entity-type': The human-readable label for the entity type.
   *  - '@entity-type': The human-readable label for the entity type.
   *  - '%entity-type-lower': The human-readable label for the entity type in
   *    lower case.
   *  - '@entity-type-lower': The human-readable label for the entity type in
   *    lower case.
   *  - '%label': The label of the entity.
   *  - '@label': The label of the entity.
   * We provide both the escaped and placeholder versions (i.e., @ and %)
   * because in some circumstances (such as buttons) the HTML included in the
   * placeholder is unsuitable.
   */
  function getOperationStringSubstitutions($entity_type, $entity, $operation_path, $parameters = array()) {
    $entity_info = entity_get_info($entity_type);

    // Only get the label for the entity if it's a) an object and b) not a new
    // entity, as the generic 'add' handler bends the rules and passes us a
    // bundle name string for this, and a new entity object won't have a label.
    if (isset($entity) && is_object($entity) && empty($entity->is_new)) {
      $entity_label = entity_label($entity_type, $entity);
    }
    else {
      $entity_label = '';
    }
    $substitutions = array(
      '%entity-type' => $entity_info['label'],
      '@entity-type' => $entity_info['label'],
      '%entity-type-lower' => drupal_strtolower($entity_info['label']),
      '@entity-type-lower' => drupal_strtolower($entity_info['label']),
      '%label' => $entity_label,
      '@label' => $entity_label,
    );
    return $substitutions;
  }

  /**
   * Get the access verb for this operation.
   *
   * This is taken either from the property $access_verb, or if that is NULL,
   * the operation path component.
   *
   * TODO: revisit this logic and see if anything better can be devised.
   *
   * @return
   *  The access verb for the operation, e.g. 'view', 'edit', and so on.
   */
  public function getAccessVerb() {
    $verb = isset($this->access_verb) ? $this->access_verb : $this->path;
    return $verb;
  }

  /**
   * Properties for the menu item specific to this handler.
   *
   * @param $operation_path
   *  The path component for the operation, e.g., 'view', 'edit'.
   * @param $operation_definition
   *  The definition array for the operation, from the operations definitions
   *  in hook_entity_info().
   * @param $loader_position
   *  The position of the entity loader in the menu base path.
   *
   * @return
   *  An array of items suitable for hook_menu() for this operation. See
   *  EntityOperationsDefaultUIController::hook_menu() for default values.
   */
  function menu_item($operation_path, $operation_definition, $loader_position) {
    return array();
  }

  /**
   * Determine access to this operation on this entity.
   *
   * This calls operationAccess() and userAccess() to check, respectively, the
   * logical access to the operation, and whether the user may perform it.
   */
  function access($entity_type, $entity, $params = array(), $account = NULL) {
    // First see whether the handler has anything to say about access from a
    // purely logical standpoint. This covers things such as a 'publish'
    // operation being denied on a node that is already published, because it
    // simply doesn't make sense.
    $operation_access = $this->operationAccess($entity_type, $entity, $params);
    // If the operation access control denies access, it has precedence. This is
    // because even if, say, the user has access to publish a node, if the node
    // in question simply *can't* be published, then it really can't!
    if ($operation_access === FALSE) {
      return $operation_access;
    }

    // Now hand over to the user access check.
    $user_access = $this->userAccess($entity_type, $entity, $params, $account);
    return $user_access;
  }

  /**
   * Determine whether the action is valid for the given entity.
   *
   * This should take care of determining whether the operation itself makes
   * sense, irrespective of the user performing it.
   *
   * For example, a 'publish' operation should deny access here if the entity
   * is already published.
   *
   * @return
   *  FALSE is the operation should not be allowed, NULL if it should be allowed
   *  from a logical standpoint and further access control passed on to the
   *  user permissions.
   */
  function operationAccess($entity_type, $entity, $params = array()) {
  }

  /**
   * Determine access to the operation for the user.
   *
   * @param $entity_type
   *  The entity type.
   * @param $entity
   *  The current entity.
   * @param $params
   *  An array of additional parameters.
   * @param $account
   *  (Optional) The account to check access for. Defaults to the current user.
   *
   * @return
   *  FALSE is the operation should not be allowed, TRUE if access should be
   *  granted.
   */
  function userAccess($entity_type, $entity, $params = array(), $account = NULL) {
    // We use entity_access() by default.
    // Note that while entity_access() documents itself as only supporting
    // basic values of $op, in fact it just passes them to the access
    // callback, meaning that as long as your entity's access callback
    // understands it, you can do it here.
    // @see entity_operations_get_entity_permissions().
    $verb = $this->getAccessVerb();
    $access = entity_access($verb, $entity_type, $entity, $account);

    return $access;
  }

  /**
   * Build the operation render array.
   *
   * @param $entity_type
   *  The entity type.
   * @param $entity
   *  The entity. Pass NULL if there is no entity in context.
   * @param $params
   *  (optional) An array of additional parameters. It is up to subclasses that
   *  require this to document what they expect.
   *
   * @return
   *  Output suitable for a page callback.
   */
  function build($entity_type, $entity, $params = array()) {
    return 'This is my operation content.';
  }

  /**
   * Execute the operation.
   *
   * Handlers may throw an exception in the case of a problem. It's up to
   * consumers of the operation to react accordingly.
   *
   * @throws Exception
   */
  function execute($entity_type, $entity) {
  }

}

/**
 * Default handler for a page callback.
 */
class EntityOperationsOperationPage extends EntityOperationsOperationBase {

  /**
   * The operation for entity_access().
   */
  public $access_verb = 'view';

  /**
   * Returns the menu item for this operation.
   */
  function menu_item($operation_path, $operation_definition, $loader_position) {
    $item = array(
      // This takes care of calling our build() method, and is set here as a
      // convenience.
      // Subclasses however may specify any page callback they wish.
      'page callback' => 'entity_operations_operation_page_callback',
      'page arguments' => array(
        $this->entityType,
        // We need to pass $this to get_class() because we want the class of
        // the current object, not the class we are sitting in.
        get_class($this),
        $operation_path,
        $loader_position,
      ),
      'access callback' => 'entity_operations_operation_access_callback',
      'access arguments' => array(
        $this->entityType,
        get_class($this),
        $operation_path,
        $loader_position,
      ),
    );

    // Add in what the parent class has.
    $item += parent::menu_item($operation_path, $operation_definition, $loader_position);
    return $item;
  }

  /**
   * Helper to set the page title.
   *
   * This allows handlers to either override the title, or not have to bother
   * about it.
   *
   * @see build() for the parameters.
   */
  function setTitle($entity_type, $entity, $params = array()) {
    drupal_set_title($entity->label());
  }

}

/**
 * Handler for showing a Views display as a tab.
 *
 * @see entity_operations_plugin_display_operation_tab
 */
class EntityOperationsOperationPageViews extends EntityOperationsOperationPage {

  /**
   * The operation for entity_access().
   *
   * TODO: allow the views display plugin to expose this as a user option?
   */
  public $access_verb = 'view';

  /**
   * Get an admin string for this operation, as defined in operationInfo().
   *
   * We can't return a fixed string from operationInfo; rather we have to
   * generate this based on the View display this handler is outputting.
   *
   * @todo This is not actually relevant until http://drupal.org/node/1922722
   * is fixed.
   */
  public function getOperationAdminString($string_key) {
    // We need the operation definition, as that has details about which view
    // this operation outputs, which we need to produce meaningful labels.
    // This is one of the reasons operation info is cached!
    $operation_info = entity_operations_get_operation_info($this->entityType);
    $operation_info[$this->path]['views info']['title'];
    switch ($string_key) {
      case 'label':
        return $operation_info[$this->path]['views info']['title'];
      case 'description':
        return t('Shows the output of the View "!view-name" display "!display-name".', array(
          '!view-name'    => $operation_info[$this->path]['views info']['name'],
          '!display-name' => $operation_info[$this->path]['views info']['display'],
        ));
    }
  }

  /**
   * Determine access to the operation for the user.
   *
   * Hands over to views_access().
   */
  function userAccess($entity_type, $entity, $params = array(), $account = NULL) {
    // There should be only one array for views_access(), so just pull it out.
    $views_access_param = $params[0][0];
    return views_access($views_access_param);
  }

  /**
   * Returns the menu item for this operation.
   *
   * We process the info from entity_operations_get_views() here so it can
   * be overridden easily.
   */
  function menu_item($operation_path, $operation_definition, $loader_position) {
    $item = array(
      // TODO: Either take the op name as tab title when the view has none, or
      // enforce a title in the Views display plugin.
      'title' => $operation_definition['views info']['title'],
      'page callback' => 'entity_operations_operation_page_callback', // ?
      'page arguments' => array(
        $this->entityType,
        // We need to pass $this to get_class() because we want the class of
        // the current object, not the class we are sitting in.
        get_class($this),
        $operation_path,
        $loader_position,
        // Extra params.
        array(
          'view_name' => $operation_definition['views info']['name'],
          'view_display_name' => $operation_definition['views info']['display'],
        ),
      ),
      'access callback' => 'entity_operations_operation_access_callback',
      'access arguments' => array(
        $this->entityType,
        get_class($this),
        $operation_path,
        $loader_position,
        array(
          $operation_definition['views info']['access'],
        ),
      ),
    );

    // Add in what the parent class has.
    $item += parent::menu_item($operation_path, $operation_definition, $loader_position);
    return $item;
  }

  /**
   * Page callback for outputting a View.
   *
   * Requires extra params in $params:
   *  - 'view_name': The name of the view to output.
   *  - 'view_display_name': The name of the view display to use.
   */
  function build($entity_type, $entity, $params = array()) {
    $view_name = $params['view_name'];
    $view_display_name = $params['view_display_name'];

    if ($view = views_get_view($view_name)) {
      // We don't check access to the display here: it's already been taken
      // care of in userAccess().
      $view->set_display($view_display_name);
      $view->current_entity = $entity;
      $result = $view->preview($view_display_name);
    }

    if (!empty($result)) {
      return $result;
    }
    else {
      // We have to return at least an empty string for the page to display.
      // The Views plugin used with this enforces at least one 'no results' text
      // so we shouldn't come here unless that is empty too.
      return '';
    }
  }

}

/**
 * Base handler for operations that show a form.
 *
 * The form for this operation can be shown in other ways than as a tab, and
 * because not everything that consumes this form needs all the same elements,
 * the building of the form is broken out of the form handlers and split up
 * between several methods:
 *   - standard strings such as the button label and confirmation question are
 *     set in operationStrings().
 *   - the form builder itself, form(), should only include such elements that
 *     are needed to capture values from the user. Thus it should not include
 *     the confirmation question or the submit button. In this base class, the
 *     returned form is merely an empty array, and when shown as an entity tab,
 *     the confirmation question, submit button, and cancel link are added by
 *     the wrapper method getForm(). If your form requires nothing more than
 *     these, you don't need to override form() at all, only operationStrings().
 *     Subclasses that need to capture user values should override form() to add
 *     their form elements, which will be similarly wrapped.
 *     Subclasses that need to change which standard form elements are provided
 *     by getForm() can do so in form() by changing properties in $form_state.
 *     For details of other contexts the form is used in, see
 *     EntityOperationsOperationAction.
 * Each form gets a unique form ID for ease of alteration, of the form:
 *  'entity_operations_operation_form_ENTITYTYPE_OPERATIONNAME'
 * (See entity_operations_forms().)
 * It is also possible to define an operation that provides a form with a
 * subclass of EntityOperationsOperationPage, and overriding menu_item() to
 * return the form. However, this will not have the advantages of the form being
 * available in different contexts.
 *
 * See also EntityOperationsOperationAction, which exposes an operation form in
 * further ways.
 */
class EntityOperationsOperationForm extends EntityOperationsOperationBase {

  public $access_verb = NULL;

  /**
   * Returns basic information about the operation.
   */
  function operationInfo() {
    return array(
      'label' => 'Label',
      'description' => 'Description',
      'uses form' => TRUE,
      'provides vbo' => FALSE,
      'provides views field' => TRUE,
    );
  }

  /**
   * Returns strings for the operations.
   *
   * These should be translated, and the following placeholders may be used
   * without values:
   *  '%entity-type': The human-readable label for the entity type.
   *  '@entity-type': The same, but as plain text.
   *  '%label': The entity label.
   *  '@label': The same, but as plain text.
   * If further substitutions are required, they can be provided by overriding
   * getOperationStringSubstitutions().
   *
   * @return
   *  An array containing the following keys, in addition to the parent
   *  class's items:
   *  - 'button label': the label for the form button. If used as a VBO, this
   *      should not contain the '%label' placeholder, as the button is shown
   *      prior to any entities being loaded.
   *  - 'confirm question': the question to ask the user in the form. This may
   *      be blank if no confirmation question is ever required for the form.
   *  - 'submit message': the message shown to convey to the user that the
   *      action has succeeded. This may be blank if no submit message is
   *      required, or if an overridden formSubmit() does something else.
   */
  function operationStrings() {
    return array(
      'tab title' => 'Form',
      'page title' => '@label',
      'button label' => t('Perform action'),
      'confirm question' => t('Are you sure you want to proceed?'),
      'submit message' => t('Action completed on %entity-type %label.', array(
        // This intentionally has no replacements for the placeholders; these
        // are replaced in getOperationString().
      )),
    );
  }

  /**
   * Properties for the menu item specific to this handler.
   */
  function menu_item($operation_path, $operation_definition, $loader_position) {
    $form_id = $this->getFormID($this->entityType, $operation_path);
    return array(
      'page callback' => 'drupal_get_form',
      'page arguments' => array(
        $form_id,
        $this->entityType,
        get_class($this),
        $operation_path,
        $loader_position, // Provides the entity as a parameter.
      ),
      'access callback' => 'entity_operations_operation_access_callback',
      'access arguments' => array(
        $this->entityType,
        get_class($this),
        $operation_path,
        $loader_position,
      ),
    );
  }

  /**
   * Get the form ID for the operation form.
   *
   * @param $entity_type
   *  The entity type.
   * @param $operation_path
   *  The path component for the operation.
   *
   * @return
   *  A form ID that will be recognized by entity_operations_forms().
   */
  function getFormID($entity_type, $operation_path) {
    // Convert characters in the operation path that are not valid in PHP
    // function names to '_' so that the form ID can form part of a valid
    // function name for alter hooks.
    $operation_path = str_replace(array('-', '/'), '_', $operation_path);
    return implode('_', array('entity_operations_operation_form', $entity_type, $operation_path));
  }

  /**
   * Build the operation render array.
   *
   * @param $entity_type
   *  The entity type.
   * @param $entity
   *  The entity. Pass NULL if there is no entity in context.
   * @param $params
   *  (optional) An array of additional parameters. It is up to subclasses that
   *  require this to document what they expect.
   *
   * @return
   *  Output suitable for a page callback.
   */
  function build($entity_type, $entity, $params = array()) {
    $form_id = $this->getFormID($entity_type, $this->path);
    return drupal_get_form($form_id, $entity_type, get_class($this), $this->path, $entity);
  }

  /**
   * Helper to get the form for this operation.
   *
   * This wraps around form(), but allows various permutations of the form to
   * be returned. Additional elements can be requested by the caller by setting
   * the following array keys to TRUE in an array in
   * $form_state['entity_operation_form_elements']:
   *  - 'page title': Set the page title.
   *  - 'confirmation message': The confirmation question. This is shown at the
   *    top of the form, and is a string defined by the operation, e.g. 'Are you
   *    sure you want to do X?'.
   *  - 'form elements': The body of the form as returned by the form() method.
   *  - 'action button': The submit button.
   *  - 'cancel link': A 'cancel' link to return the user to the entity itself,
   *    whose path is given by entity_uri().
   */
  function getForm($form, &$form_state, $entity_type, $entity, $operation_path) {
    // Case 1: tab: always show everything.
    // Case 2: node view: show body and button (but not cancel link).
    // Case 3: VBO: show only body, if configurable.

    // Always show form elements.
    $form_state['entity_operation_form_elements'] += array(
      'form elements' => TRUE,
    );

    // Even though we don't want to add the form elements to the $form array
    // just yet, call form() so that it may alter the list of requested elements
    // set in $form_state['entity_operation_form_elements'] before we begin.
    $form_elements = $this->form($form, $form_state, $entity_type, $entity, $operation_path);

    // Add confirmation message on tab.
    // TODO: ONLY if the form array is empty?
    // (though presumably if you don't want it, just don't set the message)
    if (!empty($form_state['entity_operation_form_elements']['confirmation message'])) {
      $message = $this->getOperationString('confirm question', $entity_type, $entity, $operation_path);
      if (!empty($message)) {
        $form['#attributes']['class'][] = 'confirmation';
        $form['question'] = array(
          '#prefix' => '<p>',
          '#markup' => $message,
          '#suffix' => '</p>',
        );
      }
    }

    // Now add the form elements.
    if (!empty($form_state['entity_operation_form_elements']['form elements'])) {
      $form += $form_elements;
    }

    if (!empty($form_state['entity_operation_form_elements']['action button'])) {
      $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => $this->getOperationString('button label', $entity_type, $entity, $operation_path),
      );
    }
    if (!empty($form_state['entity_operation_form_elements']['cancel link'])) {
      $uri = entity_uri($entity_type, $entity);
      $form['actions']['cancel'] = array(
        '#type' => 'link',
        '#title' => t('Cancel'),
        '#href' => $uri['path'],
        '#options' => $uri['options'],
      );
    }
    if (!empty($form_state['entity_operation_form_elements']['page title'])) {
      drupal_set_title($this->getOperationString('page title', $entity_type, $entity, $operation_path), PASS_THROUGH);
    }

    // Add the actions element properties if it's needed.
    if (isset($form['actions'])) {
      $form['actions'] += array(
        '#type' => 'actions',
        // This weight needs to be sufficiently large to go below any fields on
        // entity edit and add forms.
        // TODO: node_form gets away with a weight of 10 here. How?!
        '#weight' => 50,
      );
    }

    // Set the form array parents so that validate and submit handlers can use
    // the form even if it's embedded within a larger compound form.
    // It is important that subclasses that override this method take care of
    // doing this too.
    $form['#entity_operation_name'] = $operation_path;
    $form['#after_build'][] = 'entity_operations_operation_form_after_build';

    return $form;
  }

  /**
   * Form builder for this operation.
   *
   * This should return the main part of the form, but without any of:
   *  - a confirmation question (if one is required)
   *  - a submit button
   *  - a cancel link
   * These are added as appropriate by getForm(). Overrides of this method may
   * change the settings in $form_state['entity_operation_form_elements'] to
   * force or remove form elements provided by getForm(). See getForm() for the
   * list of properties.
   *
   * To get different parameters, override menu_item().
   *
   * @param $entity_type
   *  The entity type.
   * @param $entity
   *  The current entity.
   * @param $operation_path
   *  The operation path of the current operation. This is the key of the
   *  definition array in hook_entity_info().
   */
  function form($form, &$form_state, $entity_type, $entity, $operation_path) {
    // Return at least an empty array.
    $form = array();
    return $form;
  }

  /**
   * Form validation handler for this operation.
   *
   * Receives the same parameters as the form builder as a convenience.
   */
  function formValidate($form, &$form_state, $entity_type, $entity, $operation_path) {
  }

  /**
   * Form submit handler for this operation.
   *
   * Receives the same parameters as the form builder as a convenience.
   *
   * Displays the message defined in operationStrings()'s 'submit message'.
   */
  function formSubmit($form, &$form_state, $entity_type, $entity, $operation_path) {
    $message = $this->getOperationString('submit message', $entity_type, $entity, $operation_path);
    if (!empty($message)) {
      drupal_set_message($message);
    }
  }

  /**
   * Set the path to redirect the user to after form submission.
   *
   * This is split off from formSubmit() for easier overriding.
   *
   * @param $entity_type
   *  The entity type.
   * @param $entity
   *  The current entity.
   *
   * @return
   *  A Drupal path to redirect the user to.
   */
  function getFormSubmitRedirect($entity_type, $entity) {
    // By default, we redirect the user to the entity URI.
    $uri = entity_uri($entity_type, $entity);
    return $uri['path'];
  }

}

/**
 * Base handler for form operations that require a batch to be run.
 *
 * The form submission process takes care of setting a batch, with a single
 * Batch API operation which calls the operation handler's batchOperation()
 * method. The batch finished callback calls the handler's batchFinished().
 *
 * Subclasses should override:
 *  - operationStrings()
 *  - formSubmitGetParameters()
 *  - batchOperation()
 *  - batchFinished()
 */
class EntityOperationsOperationFormBatch extends EntityOperationsOperationForm {

  /**
   * Returns strings for the operations.
   *
   * These should be translated, and the following placeholders may be used
   * without values:
   *  '%entity-type': The human-readable label for the entity type.
   *  '@entity-type': The same, but as plain text.
   *  '%label': The entity label.
   *  '@label': The same, but as plain text.
   * If further substitutions are required, they can be provided by overriding
   * getOperationStringSubstitutions().
   *
   * @return
   *  An array containing the following keys, in addition to the parent
   *  class's items:
   *  - 'batch title': The title for the batch progress page.
   *  - 'confirm question': the question to ask the user in the form. This may
   *      be blank if no confirmation question is ever required for the form.
   */
  function operationStrings() {
    return array(
      'batch title' => 'Processing operation',
    ) + parent::operationStrings();
  }

  /**
   * Form submit handler for this operation.
   */
  function formSubmit($form, &$form_state, $entity_type, $entity, $operation_path) {
    // Set the operation parameters on the handler so that subclasses that *do*
    // override this can get to it.
    $this->parameters = $this->formSubmitGetParameters($form, $form_state, $entity_type, $entity, $operation_path);

    $batch = array(
      'title' => $this->getOperationString('batch title', $entity_type, $entity, $operation_path),
      'operations' => array(
        array('entity_operations_operation_batch_operation', array($entity_type, $entity, $operation_path, $this->parameters)),
      ),
      'finished' => 'entity_operations_operation_batch_finished',
    );
    batch_set($batch);

    if (empty($form_state['redirect'])) {
      $form_state['redirect'] = $this->getFormSubmitRedirect($this->entityType, $entity);
    }
  }

  /**
   * Build a parameters array from the form values.
   *
   * @return
   *  An array of additional parameters for the operation's batchOperation()
   *  method.
   */
  function formSubmitGetParameters($form, &$form_state, $entity_type, $entity, $operation_path) {
    $parameters = array(
      // This is just an example.
      //  'param' => TRUE,
    );

    return $parameters;
  }

  /**
   * Batch operation callback.
   *
   * @param $entity_type
   *  The entity type.
   * @param $entity
   *  The entity. Pass NULL if there is no entity in context.
   * @param $parameters
   *  The parameters array from formSubmitGetParameters().
   * @param $context
   *  The batch context array. See callback_batch_operation() for details on how
   *  this should be used. The property $context['results']['entity_operations']
   *  is reserved for use by Entity Operations.
   */
  function batchOperation($entity_type, $entity, $parameters, &$context) {
    // Subclasses should override this.
  }

  /**
   * Batch finished callback.
   *
   * Same parameters as callback_batch_finished(). (Note that $operations is
   * the array of batch operations, and not entity operations!)
   */
  function batchFinished($success, $results, $operations) {
    // Subclasses should override this.
  }

}

/**
 * Op handler for viewing an entity.
 */
class EntityOperationsOperationEntityView extends EntityOperationsOperationPage {

  /**
   * Returns basic information about the operation.
   */
  function operationInfo() {
    return array(
      'label' => 'View entity',
      'description' => 'View the rendered entity.',
    ) + parent::operationInfo();
  }

  public $access_verb = 'view';

  /**
   * Returns strings for the operations.
   */
  function operationStrings() {
    return array(
      'tab title' => 'View',
    ) + parent::operationStrings();
  }

  function build($entity_type, $entity, $params = array()) {
    //dsm($entity, 'handler page');
    // Pass in EntityAPI's defaults for the first two parameters so we can
    // set $page to TRUE.
    return $entity->view('full', NULL, TRUE);
  }

}

/**
 * Extended op handler for viewing an entity with operation buttons shown.
 *
 * This adds operation buttons for those with 'entity view' set to TRUE.
 *
 * @see entity_operations_get_entity_operations_fieldset().
 */
class EntityOperationsOperationEntityViewOperations extends EntityOperationsOperationEntityView {

  public $access_verb = 'view';

  /**
   * Returns basic information about the operation.
   */
  function operationInfo() {
    return array(
      'label' => 'View with exposed operations',
      'description' => 'View the rendered entity with the exposed operations form.',
    ) + parent::operationInfo();
  }

  function build($entity_type, $entity, $params = array()) {
    $build = parent::build($entity_type, $entity);

    // Add the automatic entity view operations to the build array.
    $build['operations'] = entity_operations_get_entity_operations_fieldset($entity);
    $build['operations']['#weight'] = 10;

    return $build;
  }

}

/**
 * Op handler for viewing devel output for an entity.
 *
 * Requirements:
 *  - devel module
 */
class EntityOperationsOperationDevel extends EntityOperationsOperationPage {

  public $access_verb = 'view';

  /**
   * Returns basic information about the operation.
   */
  function operationInfo() {
    return array(
      'label' => 'Devel',
      'description' => 'View the devel output for the entity.',
    ) + parent::operationInfo();
  }

  /**
   * Returns strings for the operations.
   */
  function operationStrings() {
    return array(
      'tab title' => 'Devel',
    ) + parent::operationStrings();
  }

  /**
   * Determine whether the action may be performed on the given entity.
   */
  function operationAccess($entity_type, $entity, $params = array()) {
    // This operation depends on devel module.
    return module_exists('devel');
  }

  /**
   * Determine whether the user has access to this operation.
   */
  function userAccess($entity_type, $entity, $params = array(), $account = NULL) {
    return user_access('access devel information');
  }

  /**
   * Page callback.
   */
  function build($entity_type, $entity, $params = array()) {
    $output = kprint_r($entity, TRUE);
    return $output;
  }

}

/**
 * Op handler for viewing tokens for an entity.
 *
 * Requirements:
 *  - token
 *  - devel module
 * You probably also should have entity_token enabled too!
 */
class EntityOperationsOperationToken extends EntityOperationsOperationPage {

  /**
   * Returns basic information about the operation.
   */
  function operationInfo() {
    return array(
      'label' => 'Token',
      'description' => 'View the tokens for the entity.',
    ) + parent::operationInfo();
  }

  /**
   * Returns strings for the operations.
   */
  function operationStrings() {
    return array(
      'tab title' => 'Tokens',
    ) + parent::operationStrings();
  }

  /**
   * Determine whether the action may be performed on the given entity.
   */
  function operationAccess($entity_type, $entity, $params = array()) {
    // This operation depends on devel module.
    return (module_exists('devel') && module_exists('token'));
  }

  /**
   * Determine whether the user has access to this operation.
   */
  function userAccess($entity_type, $entity, $params = array(), $account = NULL) {
    return user_access('access devel information');
  }

  /**
   * Page callback.
   */
  function build($entity_type, $entity, $params = array()) {
    module_load_include('inc', 'token', 'token.pages');
    $build = token_devel_token_object($entity_type, $entity);

    return $build;
  }

}

/**
 * Op handler for viewing devel output for an entity's metadata properties.
 *
 * Requirements:
 *  - entity module
 *  - devel module
 */
class EntityOperationsOperationMetadata extends EntityOperationsOperationDevel {

  /**
   * Returns basic information about the operation.
   */
  function operationInfo() {
    return array(
      'label' => 'Devel metadata',
      'description' => "View the devel output for the entity's metadata.",
    ) + parent::operationInfo();
  }

  /**
   * Returns strings for the operations.
   */
  function operationStrings() {
    return array(
      'tab title' => 'Devel metadata',
    ) + parent::operationStrings();
  }

  /**
   * Page callback.
   */
  function build($entity_type, $entity, $params = array()) {
    $entity_property_info = entity_get_property_info($this->entityType);

    $wrapper = entity_metadata_wrapper($entity_type, $entity);

    $output = array();
    foreach ($entity_property_info['properties'] as $property => $info) {
      $output[$property] = array(
        'raw'   => $wrapper->{$property}->raw(),
        'value' => $wrapper->{$property}->value(),
      );
    }

    if (isset($entity_property_info['bundles'])) {
      list(, , $bundle) = entity_extract_ids($entity_type, $entity);
      foreach ($entity_property_info['bundles'][$bundle]['properties'] as $property => $info) {
        $output[$property] = array(
          'raw'   => $wrapper->{$property}->raw(),
          'value' => $wrapper->{$property}->value(),
        );
      }
    }

    return kprint_r($output, TRUE);
  }

}

/**
 * Op handler for adding an entity: EntityAPI form.
 *
 * Add is a bit of a special case, as it doesn't use an entity loader.
 *
 * This supports entity types with bundles, but to do so, the entity info must
 * have ['admin ui']['path'] set. This is because
 * entity_ui_get_bundle_add_form() tries to use per-bundle forms, and
 * entity_forms() and entity_ui_controller() require this info property in order
 * to return a controller.
 *
 * @TODO: add a common abstract parent class with the Edit op, to avoid
 * repetition in menu_item().
 *
 * See EntityOperationsOperationEdit for requirements of this handler.
 */
class EntityOperationsOperationAdd extends EntityOperationsOperationBase {

  public $access_verb = 'create';
  // Title: "Edit %entity".

  /**
   * Returns basic information about the operation.
   */
  function operationInfo() {
    return array(
      'label' => 'Add',
      'description' => 'Create a new entity, with a bundle selection page if required.',
      'uses form' => TRUE,
      'provides vbo' => FALSE,
      'provides views field' => FALSE,
    );
  }

  /**
   * Properties for the menu item specific to this handler.
   */
  function menu_item($operation_path, $operation_definition, $loader_position) {
    $entity_info = entity_get_info($this->entityType);

    $item = array(
      'title' => 'Add',
      // Special path which doesn't have an entity loader.
      'path' => $entity_info['operations ui']['path'] . '/add',
      // We don't want a tab.
      'type' => MENU_CALLBACK,
      'page callback' => 'entity_operations_operation_page_callback',
      'page arguments' => array(
        $this->entityType,
        get_class($this),
        $operation_path,
        // There is no entity, but there might be a bundle name after the 'add'
        // path component (which is at the loader position).
        $loader_position + 1,
      ),
      'access callback' => 'entity_operations_operation_access_callback',
      'access arguments' => array(
        $this->entityType,
        get_class($this),
        $operation_path,
        NULL,
      ),
    );

    // Use the file setting from entity info 'admin ui', as we expect that to be
    // used for the entity form callback.
    if (isset($entity_info['admin ui']['file'])) {
      $item['file'] = $entity_info['admin ui']['file'];
    }
    if (isset($entity_info['admin ui']['file path'])) {
      $item['file path'] = $entity_info['admin ui']['file path'];
    }
    else {
      $item['file path'] = drupal_get_path('module', $entity_info['module']);
    }

    // Add in what the parent class has.
    $item += parent::menu_item($operation_path, $operation_definition, $loader_position);
    return $item;
  }

  /**
   * Helper to set the page title.
   *
   * @see build() for the parameters.
   */
  function setTitle($entity_type, $entity, $params = array()) {
    if (empty($entity)) {
      module_load_include('inc', 'entity', 'includes/entity.ui');
      drupal_set_title(entity_ui_get_action_title('add', $entity_type));
    }
    else {
      // We're on /add/bundle.
      // build() will call entity_ui_get_bundle_add_form() which takes care of
      // setting the title.
    }
  }

  /**
   * Page callback.
   */
  function build($entity_type, $entity, $params = array()) {
    if (empty($entity)) {
      // We're on just /add.
      $entity_info = entity_get_info($entity_type);
      $values = array();

      // Special case: single-bundled entity.
      if (count($entity_info['bundles']) == 1) {
        // In some instances with one bundle, the name of the bundle may not
        // be the same as the name of the entity so we need to handle this
        // scenario.
        if (!empty($entity_info['entity keys']['bundle'])) {
          $bundle_key = $entity_info['entity keys']['bundle'];
          // There is only one bundle in the bundles array so we can grab the
          // key of the first array element, which is the name of the bundle.
          $values[$bundle_key] = key($entity_info['bundles']);
        }

        // Create our entity - with the additional bundle information if
        // appropriate and then load the form.
        $entity = entity_create($entity_type, $values);
        return entity_ui_get_form($entity_type, $entity, 'add');
      }

      // We can't use entity_ui_bundle_add_page() because that outputs links to
      // paths in the admin UI, so unfortunately we must repeat its work here.
      $items = array();
      foreach ($entity_info['bundles'] as $bundle_name => $bundle_info) {
        // Create an empty entity with just the bundle set to check for access.
        $dummy_entity = entity_create($entity_type, array(
          $entity_info['entity keys']['bundle'] => $bundle_name,
        ));
        // If modules use a uid, they can default to the current-user
        // in their create() method on the storage controller.
        if (entity_access('create', $entity_type, $dummy_entity)) {
          $add_path = $entity_info['operations ui']['path'] . '/add/' . $bundle_name;
          $items[] = l(t('Add @label', array('@label' => $bundle_info['label'])), $add_path);
        }
      }
      return theme('item_list', array('items' => $items));
    }
    else {
      // We're on /add/bundle.
      $bundle_name = $entity;
      return entity_ui_get_bundle_add_form($entity_type, $bundle_name);
    }
  }

}

/**
 * Op handler for adding an entity: EntityAPI form, with prepopulation.
 *
 * This class is a bit messy, as it's a descendant of the form class, but
 * provides a page. TODO: clean up: the only thing we inherit is the menu_item()
 * and that's not really worth this WTF.
 *
 * See EntityOperationsOperationEdit for requirements of this handler.
 */
class EntityOperationsOperationAddPrepopulate extends EntityOperationsOperationAdd {

  /**
   * Properties for the menu item specific to this handler.
   */
  function menu_item($operation_path, $operation_definition, $loader_position) {
    $item = parent::menu_item($operation_path, $operation_definition, $loader_position);

    // Change the page callback so it goes via our form.
    $item['page callback'] = 'entity_operations_operation_page_callback';
    $item['page arguments'] = array(
      $this->entityType,
      get_class($this),
      $operation_path,
      // There is no entity.
      NULL,
    );

    return $item;
  }

  /**
   * Helper to set the page title.
   *
   * We have to override this as the parent implementation will try to get a
   * label out of a nonexistent entity.
   *
   * @see build() for the parameters.
   */
  function setTitle($entity_type, $entity, $params = array()) {
    // entity_ui_get_form() takes care of setting the title.
  }

  /**
   * Determine access to the operation for the user.
   *
   * We have to override this, because otherwise, as is called before build(), it
   * doesn't have the prepopulated entity to work on.
   */
  function userAccess($entity_type, $entity, $params = array(), $account = NULL) {
    // Build the entity first.
    $entity = $this->createEntity();

    $verb = $this->getAccessVerb();
    $access = entity_access($verb, $entity_type, $entity, $account);

    return $access;
  }

  /**
   * Page callback.
   *
   * This is a wrapper around entity_ui_get_form() which builds up a new entity
   * prepopulated with data from the query parameters.
   */
  function build($entity_type, $entity, $params = array()) {
    // This is repeating the same work done by userAccess(). Would it be better
    // to remember the entity in this handler?
    $entity = $this->createEntity();

    return entity_ui_get_form($this->entityType, $entity, 'add');
  }

  /**
   * Helper for userAccess() and build().
   *
   * Creates a new entity based on query parameters.
   *
   * We have to create the entity from parameters during access control, to
   * be able to pass it to entity_access(). This in turn is necessary to allow
   * OG access control for entity creation.
   *
   * @return $entity
   *  An unsaved, newly-created entity.
   */
  function createEntity() {
    // Create an empty entity and get its wrapper.
    $entity = entity_create($this->entityType, array());
    $wrapper = entity_metadata_wrapper($this->entityType, $entity);

    // Set properties from the query parameters on the new entity.
    $query = drupal_get_query_parameters();
    $properties = $wrapper->getPropertyInfo();

    foreach ($query as $property => $value) {
      // Only try to set a property that actually exists. Other things may be
      // present on the query string. This happens in particular within
      // Simpletests.
      if (isset($properties[$property])) {
        $wrapper->$property->set($value);
      }
    }

    return $entity;
  }

}

/**
 * Op handler for editing an entity: EntityAPI form.
 *
 * This handler requires the form for the entity to be defined elsewhere. This
 * should be named 'ENTITY_TYPE_form' (though see entity_ui_get_form() for
 * other options), along with a submit handler for saving the entity.
 *
 * The form builder may be located in an .inc file to be loaded: the properties
 * in the entity type's 'admin ui' are taken into account.
 */
class EntityOperationsOperationEdit extends EntityOperationsOperationForm {

  public $access_verb = 'edit';
  // Title: "Edit %entity".

  /**
   * Returns basic information about the operation.
   */
  function operationInfo() {
    return array(
      'label' => 'Edit',
      'description' => 'Produces a form to edit the entity.',
    ) + parent::operationInfo();
  }

  /**
   * Properties for the menu item specific to this handler.
   *
   * Override the page callback to go straight to entity_ui_get_form().
   */
  function menu_item($operation_path, $operation_definition, $loader_position) {
    $item = array(
      'title' => 'Edit',
      'page callback' => 'entity_ui_get_form',
      'page arguments' => array($this->entityType, $loader_position),
    );

    // Use the file setting from entity info 'admin ui', as we expect that to be
    // used for the entity form callback.
    $entity_info = entity_get_info($this->entityType);
    if (isset($entity_info['admin ui']['file'])) {
      $item['file'] = $entity_info['admin ui']['file'];
    }
    if (isset($entity_info['admin ui']['file path'])) {
      $item['file path'] = $entity_info['admin ui']['file path'];
    }
    else {
      $item['file path'] = drupal_get_path('module', $entity_info['module']);
    }

    // Add in what the parent class has.
    $item += parent::menu_item($operation_path, $operation_definition, $loader_position);
    return $item;
  }

}

/**
 * Op handler for editing an entity: generic form.
 *
 * This can be used as the 'edit' handler for entities that don't require any
 * form elements other than those provided by FieldAPI. Using this handler means
 * that you don't need to provide a form builder for the edit/add form for your
 * entities: the form is contained within this handler.
 *
 * This handler requires:
 *  - The entity type's 'controller class' property to be EntityAPIController
 *    or a subclass.
 *
 * Using this handler (instead of having EntityOperationsOperationEdit and a
 * form builder) has the following limitations:
 *  - The form produced here will not easily be alterable with hook_form_alter()
 *    as it will have the form build ID of our generic operation form builder,
 *    'entity_operations_operation_form'.
 *  - If this handler produces a form with no elements other than those from
 *    FieldAPI, and no hidden bundle element (as a result of the entity
 *    having only one bundle), then a bug in drupal_write_record() will prevent
 *    the entity being saved, because it would have an empty record in the base
 *    table. A simple workaround for this is allowing the entity to use the
 *    'label' element which is automatically provided if the entity info defines
 *    the entity key for it.
 *
 * @todo: use Entity Metadata API to automatically add field elements so that
 * properties such as entity title can be set.
 */
class EntityOperationsOperationEditGeneric extends EntityOperationsOperationForm {

  /**
   * Returns basic information about the operation.
   */
  function operationInfo() {
    return array(
      'label' => 'Edit',
      'description' => 'Produces a generic form to edit the entity.',
    ) + parent::operationInfo();
  }

  /**
   * Returns strings for the operations.
   */
  function operationStrings() {
    return array(
      'tab title' => 'Edit',
      'label' => 'Edit',
      'description' => 'Produces a form to edit the entity.',
      'page title' => t('Edit %label'),
      'button label' => t('Save'),
      // This appears to be sufficient to result in the markup form element
      // not being created.
      'confirm question' => NULL,
      'submit message' => t('The @entity-type %label has been saved.'),
      // These intentionally have no replacements for the placeholders; these
      // are replaced in getOperationString().
    );
  }

  /**
   * Get the form ID for the operation form.
   *
   * @param $entity_type
   *  The entity type.
   * @param $operation_path
   *  The path component for the operation.
   *
   * @return
   *  A form ID that will be recognized by entity_operations_forms().
   */
  function getFormID($entity_type, $operation_path) {
    // Force the form ID, so that child classes get the same one. This means
    // both the add and edit generic operation forms get the same form ID, and
    // thus makes form alteration easier.
    return implode('_', array('entity_operations_operation_form', $entity_type, 'edit'));
  }

  /**
   * Form builder for this operation.
   */
  function form($form, &$form_state, $entity_type, $entity, $operation_path) {
    // Kill the cancel link.
    unset($form_state['entity_operation_form_elements']['cancel link']);

    return $this->entityForm($form, $form_state, $entity_type, $entity, $operation_path, 'edit');
  }

  /**
   * Helper for form(), building just the entity form.
   *
   * Keeps the entity form separate from any wrapping logic and allows clearer
   * reuse by subclasses.
   *
   * Note that overriding this will cause inheritance problems because this is
   * also used by the EntityOperationsOperationAddGeneric handler. If you want
   * to customize the form for your entity, you may want to consider using
   * the EntityOperationsOperationEdit handler instead.
   *
   * @param ...
   *  The same parameters as form().
   * @param $form_op
   *  The 'op' value to set on the entity form state. Either 'edit' or 'add'.
   */
  function entityForm($form, &$form_state, $entity_type, $entity, $operation_path, $form_op) {
    $entity_info = entity_get_info($this->entityType);

    // Ripped from entity_ui_form_defaults().
    $defaults = array(
      'entity_type' => $this->entityType,
    );
    if (isset($entity)) {
      $defaults[$this->entityType] = $entity;
    }
    if (isset($form_op)) {
      $defaults['op'] = $form_op;
    }
    $form_state += $defaults;

    // The bundle needs to be set as a form value for building the pseudo-entity
    // in entity_form_field_validate().
    if (!empty($entity_info['entity keys']['bundle'])) {
      $bundle_key = $entity_info['entity keys']['bundle'];
      $form[$bundle_key] = array(
        '#type' => 'value',
        '#value' => isset($entity->{$bundle_key}) ? $entity->{$bundle_key} : NULL,
      );
    }

    // Add a form element for the entity label, if we have the necessary
    // information to make one.
    if (isset($entity_info['entity keys']['label'])) {
      $label_key = $entity_info['entity keys']['label'];
      $form[$label_key] = array(
        '#type' => 'textfield',
        '#title' => t('Label'),
        '#required' => TRUE,
        '#default_value' => isset($entity->{$label_key}) ? $entity->{$label_key} : '',
      );
    }

    if (!empty($entity_info['fieldable'])) {
      field_attach_form($this->entityType, $entity, $form, $form_state);
    }

    return $form;
  }

  /**
   * Form validation handler for this operation.
   *
   * Receives the same parameters as the form builder as a convenience.
   */
  function formValidate($form, &$form_state, $entity_type, $entity, $operation_path) {
    // Notify field widgets to validate their data.
    entity_form_field_validate($this->entityType, $form, $form_state);
  }

  /**
   * Form submit handler for this operation.
   *
   * Receives the same parameters as the form builder as a convenience.
   */
  function formSubmit($form, &$form_state, $entity_type, $entity, $operation_path) {
    $entity_info = entity_get_info($entity_type);

    // Add the bundle property to the entity if the entity type supports bundles
    // and the form provides a value for the bundle key. Especially new entities
    // need to have their bundle property pre-populated before we invoke
    // entity_form_submit_build_entity().
    // This duplicates what entity_ui_form_submit_build_entity() does, but all
    // that does is call the entity admin UI controller, and we don't want a
    // dependency on that for just one of its methods which is pretty basic at
    // that.
    // @see EntityDefaultUIController::entityFormSubmitBuildEntity()
    if (!empty($entity_info['entity keys']['bundle']) && isset($form_state['values'][$entity_info['entity keys']['bundle']])) {
      $bundle_key = $entity_info['entity keys']['bundle'];
      $form_state[$entity_type]->{$bundle_key} = $form_state['values'][$bundle_key];
    }

    entity_form_submit_build_entity($entity_type, $form_state[$entity_type], $form, $form_state);
    $entity = $form_state[$entity_type];

    // The save operation will fail if the entity has no properties to save to
    // its base table. This is caused by a bug in drupal_write_record().
    // @see http://drupal.org/node/970338.

    // Save the entity.
    $entity->save();

    $message = $this->getOperationString('submit message', $entity_type, $entity, $operation_path);
    if (!empty($message)) {
      drupal_set_message($message);
    }

    // Redirect to the entity.
    if (empty($form_state['redirect'])) {
      $form_state['redirect'] = $this->getFormSubmitRedirect($this->entityType, $entity);
    }
  }

}

/**
 * Op handler for adding an entity: generic form.
 *
 * This can be used as the 'add' handler for entities that don't require any
 * form elements other than those provided by FieldAPI. Using this means you
 * don't need to provide a form builder for the edit/add form for your entities:
 * it is contained within this handler.
 *
 * See EntityOperationsOperationEditGeneric for requirements and limitations of
 * this handler.
 */
class EntityOperationsOperationAddGeneric extends EntityOperationsOperationEditGeneric {

  public $access_verb = 'create';

  /**
   * Returns strings for the operations.
   */
  function operationStrings() {
    return array(
      'tab title' => 'Add',
      'label' => 'Add',
      'description' => 'Create a new entity, with a bundle selection page if required.',
      // Note page title is set by setTitle().
    ) + parent::operationStrings();
  }

  /**
   * Properties for the menu item specific to this handler.
   */
  function menu_item($operation_path, $operation_definition, $loader_position) {
    $entity_info = entity_get_info($this->entityType);

    $form_id = $this->getFormID($this->entityType, $operation_path);

    $item = array(
      // Special path which doesn't have an entity loader.
      'path' => $entity_info['operations ui']['path'] . '/add',
      // We don't want a tab.
      'type' => MENU_CALLBACK,
      // Although we have EntityOperationsOperationForm in our ancestry, we use
      // the page callback as our output is not always a form.
      'page callback' => 'entity_operations_operation_page_callback',
      // Change the callback arguments to pass NULL for the entity parameter.
      'page arguments' => array(
        $this->entityType,
        get_class($this),
        $operation_path,
        // The path component after 'add', which should be the bundle name.
        // This is passed into the form and access callbacks as the $entity.
        $loader_position + 1,
      ),
      'access arguments' => array(
        $this->entityType,
        get_class($this),
        $operation_path,
        $loader_position + 1,
      ),
    );

    // Add in what the parent class has.
    $item += parent::menu_item($operation_path, $operation_definition, $loader_position);
    return $item;
  }

  /**
   * Determine access to the operation for the user.
   *
   * We override this to save the entity access callback some legwork: when
   * there is no entity and we are showing the selection list, determine access
   * by checking the user has access to create at least one bundle.
   * This is the same behaviour as _node_add_access().
   */
  function userAccess($entity_type, $entity, $params = array(), $account = NULL) {
    $entity_info = entity_get_info($this->entityType);
    // entity_get_info() defaults this to at least a ''.
    $bundle_key = $entity_info['entity keys']['bundle'];
    $verb = $this->getAccessVerb();

    // Multiple cases, depending on what circumstances we're here under and
    // whether the entity has multiple bundles.
    // Case 1: we are on /add, and the entity type has only one bundle (note
    // that even if hook_entity_info() doesn't define any bundles,
    // entity_get_info() defines one with the same name as the entity).
    // We're about to show the add form.
    if (count($entity_info['bundles']) == 1) {
      $values = array();
      if (!empty($bundle_key)) {
        // Babysit stupid PHP 5.4's overreacting and bad handling of arrays.
        $keys = array_keys($entity_info['bundles']);
        $unique_bundle_name = array_pop($keys);
        $values[$bundle_key] = $unique_bundle_name;
      }

      $new_entity = entity_create($entity_type, $values);
      $access = entity_access($verb, $entity_type, $new_entity, $account);
      return $access;
    }

    // Case 2: we are on /add, and we don't know which bundle to use. We're
    // about to show a list of links. Access is granted if the user has access
    // to at least one of the links.
    if (empty($entity)) {
      foreach (array_keys($entity_info['bundles']) as $bundle_name) {
        // Create a minimal entity with just the bundle for entity_access():
        // this always needs an entity, even for 'create' op.
        $values = array(
          $bundle_key => $bundle_name,
        );
        $new_entity = entity_create($entity_type, $values);

        $access = entity_access($verb, $entity_type, $new_entity, $account);
        if ($access) {
          return TRUE;
        }
      }
      // No access: return FALSE.
      return FALSE;
    }

    // Case 3: we are on /add/BUNDLE. We are about to show the add form for the
    // requested bundle, which is in fact the $entity parameter.
    $bundle_name = $entity;
    $values = array(
      $bundle_key => $bundle_name,
    );
    $new_entity = entity_create($entity_type, $values);
    $access = entity_access($verb, $entity_type, $new_entity, $account);
    return $access;
  }

  /**
   * Build the operation render array.
   *
   * @param $entity_type
   *  The entity type.
   * @param $entity
   *  The entity. Pass NULL if there is no entity in context.
   * @param $params
   *  (optional) An array of additional parameters. It is up to subclasses that
   *  require this to document what they expect.
   *
   * @return
   *  Output suitable for a page callback.
   */
  function build($entity_type, $entity, $params = array()) {
    // There are several different cases for two different results.
    $entity_info = entity_get_info($this->entityType);

    // The $entity parameter is in fact the bundle name.
    $bundle_name = $entity;

    if (count($entity_info['bundles']) == 1) {
      // Case 1: we are on /add, and the entity type has only one bundle (note
      // that even if hook_entity_info() doesn't define any bundles,
      // entity_get_info() defines one with the same name as the entity).
      // Just show the add form.
      $output = 'form';

      $values = array();

      // Create a new entity, setting the bundle key, if we need it.
      // Must use empty(), as entity_get_info() defaults this to ''.
      if (!empty($entity_info['entity keys']['bundle'])) {
        // Set a variable to avoid a strict warning with reset().
        $bundle_names = array_keys($entity_info['bundles']);
        $bundle_name = reset($bundle_names);

        $bundle_key = $entity_info['entity keys']['bundle'];
        $values[$bundle_key] = $bundle_name;
      }

      $entity = entity_create($entity_type, $values);
    }
    elseif (empty($bundle_name)) {
      // Case 2: we are on /add, and we don't know which bundle to use. Show a
      // list of links.
      $output = 'links';
    }
    else {
      // Case 3: we are on /add/BUNDLE. Show the add form.
      $output = 'form';

      // Create an entity of the right bundle.
      $values = array();
      $bundle_key = $entity_info['entity keys']['bundle'];
      $values[$bundle_key] = $bundle_name;

      $entity = entity_create($entity_type, $values);
    }

    // Now return the output, depending on what's required.
    switch ($output) {
      case 'links':
        return $this->bundleSelectionList($entity_type);
      case 'form':
        // Return the form.
        // This will:
        // - look up the form ID with our hook_forms().
        // - use the base form builder entity_operations_operation_form().
        // - go back into this handler's form().
        // - call entityForm(), which calls the parent method
        //   EntityOperationsOperationEditGeneric::entityForm().
        $form_id = $this->getFormID($entity_type, $this->path);
        return drupal_get_form($form_id, $entity_type, get_class($this), $this->path, $entity);
    }
  }

  /**
   * Form builder for this operation.
   */
  function form($form, &$form_state, $entity_type, $entity, $operation_path) {
    // Kill the page title.
    unset($form_state['entity_operation_form_elements']['page title']);

    // Kill the cancel link.
    unset($form_state['entity_operation_form_elements']['cancel link']);

    return $this->entityForm($form, $form_state, $entity_type, $entity, $operation_path, 'add');
  }

  /**
   * Helper to set the page title.
   *
   * We need this because we don't have it in our parent classes, and it's
   * expected by entity_operations_operation_page_callback().
   *
   * @see build() for the parameters.
   */
  function setTitle($entity_type, $entity, $params = array()) {
    // The $entity parameter is in fact the bundle name.
    $bundle_name = $entity;

    $entity_info = entity_get_info($entity_type);

    if (count($entity_info['bundles']) == 1) {
      // Special case when the entity type has only one bundle. This is either:
      //  - an entity that always has only one bundle (eg 'user') and where the
      //    bundle label is irrelevant
      //  - an entity that could have more than one bundle, but just doesn't in
      //    the current configuration (eg 'node'), and the bundle label probably
      //    is relevant.
      // In this situation, the user is taken directly to the form, but we need
      // to know whether or not to show the bundle label in the title. The only
      // way to be sure is to compare the bundle label and the entity type
      // label.
      $bundle_info = reset($entity_info['bundles']);
      $bundle_label = $bundle_info['label'];
      if ($entity_info['label'] == $bundle_label) {
        $show_bundle_label = FALSE;
      }
      else {
        $show_bundle_label = TRUE;
      }
    }
    elseif (!empty($bundle_name)) {
      // The bundle name has been specified: we are on the form.
      $show_bundle_label = TRUE;
    }
    else {
      // No bundle name, and there is more than one bundle: we are on the list.
      $show_bundle_label = FALSE;
    }

    if ($show_bundle_label) {
      // Get the bundle label.
      if (empty($bundle_name)) {
        $bundle_info = reset($entity_info['bundles']);
        $bundle_label = $bundle_info['label'];
      }
      else {
        $bundle_label = $entity_info['bundles'][$bundle_name]['label'];
      }

      drupal_set_title(t('Add %bundle-type-lower %entity-type-lower', array(
        '%entity-type-lower' => drupal_strtolower($entity_info['label']),
        '%bundle-type-lower' => drupal_strtolower($bundle_label),
      )), PASS_THROUGH);
    }
    else {
      drupal_set_title(t('Add %entity-type-lower', array(
        '%entity-type-lower' => drupal_strtolower($entity_info['label']),
      )), PASS_THROUGH);
    }
  }

  /**
   * Helper for build() that returns a list of bundle creation links.
   */
  function bundleSelectionList($entity_type) {
    $entity_info = entity_get_info($this->entityType);

    // Get the base path. We will probably be on ENTITYBASE/add, though of
    // course the declaration for this operation could have set a different
    // operation path. We create links with the bundle name appended to that,
    // which will also be handled by this operation.
    $base_path = $entity_info['operations ui']['path'] . '/' . $this->path;

    $items = array();
    foreach ($entity_info['bundles'] as $bundle_name => $bundle_info) {
      // Check access for each item.
      // Create a minimal entity with just the bundle for entity_access():
      // this always needs an entity, even for 'create' op.
      $values = array(
        $entity_info['entity keys']['bundle'] => $bundle_name,
      );
      $new_entity = entity_create($entity_type, $values);

      $verb = $this->getAccessVerb();
      $access = entity_access($verb, $entity_type, $new_entity);
      if ($access) {
        $items[] = l(t('Add @bundle', array('@bundle' => $bundle_info['label'])), "$base_path/$bundle_name");
      }
    }

    $build['links'][$bundle_name] = array(
      '#theme' => 'item_list',
      '#items' => $items,
    );

    return $build;
  }

}

/**
 * Generic handler for operations that are exposed as actions to other systems.
 *
 * These are made available by default to:
 *  - Views Bulk Operations
 *  - Services, as targeted actions on an entity.
 *
 * Because these don't use the form in the same way (or at all), the execution
 * of the action is in execute() rather than formSubmit(). If this needs
 * parameters in addition to the entity, these can be processed from form values
 * by formSubmitGetParameters(). This can then be called by VBO's own submit
 * handler to process its form values, and also allows Services to call
 * execute() directly with incoming remote data.
 *
 * See EntityOperationsOperationForm for details of how the form builder is
 * effectively split across different methods.
 *
 * Generally, subclasses should override:
 *  - operationStrings() to provide texts specific to their operation,
 *  - operationAccess() to determine when the operation is available,
 *  - execute() to carry out the action.
 * If the action requires configuration, then also:
 *  - form() to provide the form elements to collect values,
 *  - formSubmitGetParameters() to process the form values into a context array.
 *
 * See EntityOperationsOperationPublish for an example implementation using
 * just the basic form, and EntityOperationsOperationSetOwner for an example
 * requiring additional parameters.
 */
abstract class EntityOperationsOperationAction extends EntityOperationsOperationForm {

  /**
   * Returns basic information about the operation.
   *
   * @return
   *  In addition to the parent method's return, this can also include:
   *  - 'vbo': An array of options for the VBO operation.
   *  - 'parameters': An array describing the keys that execute() expects in its
   *    $parameters argument. The keys should be the key names that execute()
   *    expects, and the value should describe the type and be one of 'int',
   *    'string', or 'array'.
   */
  function operationInfo() {
    return array(
      'uses form' => TRUE,
      'provides vbo' => TRUE,
      // Extra info for VBO.
      'vbo' => array(),
      'provides views field' => FALSE,
      'provides services resource' => TRUE,
      'parameters' => array(),
    );
  }

  /**
   * Form builder for this operation.
   *
   * This should return the main part of the form, but without any of:
   *  - a confirmation question (if one is required)
   *  - a submit button
   *  - a cancel link
   * These are added as appropriate by getForm().
   */
  function form($form, &$form_state, $entity_type, $entity, $operation_path) {
    // Return at least an empty array.
    $form = array();
    return $form;
  }

  /**
   * Form submit handler for this operation.
   *
   * Subclasses should not override this, but formSubmitGetParameters() instead.
   */
  function formSubmit($form, &$form_state, $entity_type, $entity, $operation_path) {
    // Set the operation parameters on the handler so that subclasses that *do*
    // override this can get to it.
    $this->parameters = $this->formSubmitGetParameters($form, $form_state, $entity_type, $entity, $operation_path);

    // Operations may throw an exception: catch it and display the error.
    try {
      $this->execute($entity_type, $entity, $this->parameters);
    }
    catch (Exception $e) {
      drupal_set_message($e->getMessage(), 'error');

      // You probably don't want to redirect anywhere if the operation has gone
      // wrong.
      $form_state['redirect'] = '';

      return;
    }

    $message = $this->getOperationString('submit message', $entity_type, $entity, $operation_path, $this->parameters);
    if (!empty($message)) {
      drupal_set_message($message);
    }

    if (empty($form_state['redirect'])) {
      $form_state['redirect'] = $this->getFormSubmitRedirect($this->entityType, $entity);
    }
  }

  /**
   * Build a parameters array from the form values.
   *
   * @return
   *  An array of additional parameters for the operation's execute() method.
   */
  function formSubmitGetParameters($form, &$form_state, $entity_type, $entity, $operation_path) {
    $parameters = array(
      // This is just an example.
      //  'param' => TRUE,
    );

    return $parameters;
  }

  /**
   * Execute the action.
   *
   * This receives an extra parameter consisting of extra data. When coming from
   * the tab UI, this is built up by formSubmitGetParameters(); other ways in
   * such as VBO or Services must construct this themselves.
   *
   * @return
   *  No return is required, but when called by Services, the return value will
   *  be returned to the resource caller.
   */
  function execute($entity_type, $entity, $parameters = array()) {
  }

}

/**
 * Op handler for deleting an entity.
 */
class EntityOperationsOperationDelete extends EntityOperationsOperationAction {

  public $access_verb = 'delete';

  /**
   * Returns basic information about the operation.
   */
  function operationInfo() {
    return array(
      'label' => 'Delete',
      'description' => 'Produces a confirmation form to delete the entity.',
      // Services module already provides CRUD operations for entities, so we
      // don't expose this to it.
      'provides services resource' => FALSE,
    ) + parent::operationInfo();
  }

  /**
   * Returns strings for the operations.
   */
  function operationStrings() {
    return array(
      'tab title' => 'Delete',
      'page title' => 'Delete %label?',
      'button label' => t('Delete @entity-type'),
      'confirm question' => t('Are you sure you want to delete the @entity-type %label?'),
      'submit message' => t('The @entity-type %label has been deleted.'),
      // These intentionally have no replacements for the placeholders; these
      // are replaced in getOperationString().
    );
  }

  /**
   * Set the path to redirect the user to after form submission.
   */
  function getFormSubmitRedirect($entity_type, $entity) {
    // Overwrite the redirect, as there is no entity to return to.
    // Just send to the front page; there is nowhere else sensible to go to.
    return '<front>';
  }

  /**
   * The delete action.
   */
  function execute($entity_type, $entity, $parameters = array()) {
    list($id, ) = entity_extract_ids($entity_type, $entity);

    // Workaround for a bug in Entity API: entity_delete() causees a classed
    // entity's delete() method to be bypassed.
    // TODO: remove when https://www.drupal.org/node/1993818 is committed and
    // released.
    if (method_exists($entity, 'delete')) {
      $entity->delete();
    }
    else {
      entity_delete($entity_type, $id);
    }
  }

}

/**
 * Op handler for deleting an entity, with a confirmation button.
 *
 * This is intended for use within an operations fieldset, where there may
 * be several different submit buttons.
 */
class EntityOperationsOperationDeleteConfirm extends EntityOperationsOperationDelete {

  public $access_verb = 'delete';

  /**
   * Returns basic information about the operation.
   */
  function operationInfo() {
    return array(
      'label' => 'Delete',
      'description' => 'Produces a confirmation form to delete the entity.',
    ) + parent::operationInfo();
  }

  /**
   * Form builder for this operation.
   *
   * This should return the main part of the form, but without any of:
   *  - a confirmation question (if one is required)
   *  - a submit button
   *  - a cancel link
   * These are added as appropriate by getForm().
   */
  function form($form, &$form_state, $entity_type, $entity, $operation_path) {
    $substitutions = $this->getOperationStringSubstitutions($entity_type, $entity, $operation_path);
    $form['confirm'] = array(
      '#type' => 'checkbox',
      '#title' => t('Confirm deletion of this @entity-type.', $substitutions),
    );

    return $form;
  }

  /**
   * Form validation handler for this operation.
   *
   * Receives the same parameters as the form builder as a convenience.
   */
  function formValidate($form, &$form_state, $entity_type, $entity, $operation_path) {
    if (empty($form_state['values']['confirm'])) {
      form_set_error('confirm', t("You must confirm that you want to delete this."));
    }
  }

}

/**
 * Op handler for publishing an entity.
 *
 * Requires the entity to have a 'status' property.
 */
class EntityOperationsOperationPublish extends EntityOperationsOperationAction {

  // TODO: 'publish' usually requires stronger access than 'edit'.
  public $access_verb = 'edit';
  // Title: "Edit %entity".

  /**
   * Returns basic information about the operation.
   */
  function operationInfo() {
    return array(
      'label' => 'Publish',
      'description' => 'Produces a confirmation form to publish the entity.',
    ) + parent::operationInfo();
  }

  /**
   * Returns strings for the operations.
   *
   * @return
   *  An array containing the following keys:
   *  - 'form': An array of strings for the operation form, containing:
   *    - 'button label'
   *    - 'confirm question'
   *    - 'submit message'
   */
  function operationStrings() {
    return array(
      'tab title' => 'Publish',
      'page title' => 'Publish %label',
      'button label' => t('Publish'),
      'confirm question' => t('Are you sure you want to publish this %entity-type?'),
      'submit message' => t('%entity-type %label has been published.'),
    );
  }

  /**
   * Access callback: deny access if entity is unpublished.
   */
  function operationAccess($entity_type, $entity, $params = array()) {
    // Deny access if the entity is already published.
    $published = $entity->status;
    // We check the node is not unpublished rather than whether it is published,
    // so modules that extend the status property can also use this (assuming
    // that they don't want entities with a status higher than 1 to revert to
    // merely being published).
    if ($published != NODE_NOT_PUBLISHED) {
      return FALSE;
    }
    // We only deny access; entity_access() will take over.
  }

  /**
   * The publish action.
   */
  function execute($entity_type, $entity, $parameters = array()) {
    $entity->status = NODE_PUBLISHED;
    entity_save($entity_type, $entity);
  }

}

/**
 * Op handler for publishing an entity.
 *
 * Requires the entity to have a 'status' property.
 */
class EntityOperationsOperationUnPublish extends EntityOperationsOperationPublish {

  /**
   * Returns basic information about the operation.
   */
  function operationInfo() {
    return array(
      'label' => 'Unpublish',
      'description' => 'Produces a confirmation form to unpublish the entity.',
    ) + parent::operationInfo();
  }

  /**
   * Returns strings for the operations.
   *
   * @return
   *  An array containing the following keys:
   *  - 'form': An array of strings for the operation form, containing:
   *    - 'button label'
   *    - 'confirm question'
   *    - 'submit message'
   */
  function operationStrings() {
    return array(
      'tab title' => 'Unpublish',
      'page title' => 'Unpublish %label',
      'button label' => t('Unpublish'),
      'confirm question' => t('Are you sure you want to unpublish this %entity-type?'),
      'submit message' => t('%entity-type %label has been unpublished.'),
    );
  }

  /**
   * Access callback: deny access if entity is unpublished.
   */
  function operationAccess($entity_type, $entity, $params = array()) {
    // Deny access if the entity is already published.
    $published = $entity->status;
    // Use the constant, which allows module that extend the status property to
    // also use this, provided 'published' is 1.
    if ($published == NODE_NOT_PUBLISHED) {
      return FALSE;
    }
    // We only deny access; entity_access() will take over.
  }

  /**
   * The publish action.
   *
   * TODO: figure out a way of exposing this to:
   *  - hook_action_info
   *  - UI actions in admin list
   */
  function execute($entity_type, $entity, $parameters = array()) {
    $entity->status = NODE_NOT_PUBLISHED;
    entity_save($entity_type, $entity);
  }

}

/**
 * Op handler for setting the owner/author of an entity.
 *
 * Requires the entity to have a 'uid' property.
 *
 * This is mostly provided as an example of a more complex action, and it should
 * be noted that this will currently fail the access check!
 */
class EntityOperationsOperationSetOwner extends EntityOperationsOperationAction {

  /**
   * Returns basic information about the operation.
   */
  function operationInfo() {
    return array(
      'label' => 'Set owner',
      'description' => 'Produces a form to set the user that owns the entity.',
      'vbo' => array(
        'configurable' => TRUE,
      ),
      'parameters' => array(
        'uid' => 'int',
      ),
    ) + parent::operationInfo();
  }

  /**
   * Returns strings for the operations.
   */
  function operationStrings() {
    return array(
      'tab title' => 'Set author',
      'page title' => 'Set author of %label',
      'button label' => t('Set author'),
      'confirm question' => t('Are you sure you want to set the author of this %entity-type?'),
      'submit message' => t('Owner of %entity-type %label has been set to !account-name.'),
    );
  }

  /**
   * Return an array of string substitutions for t().
   *
   * We add the themed username for the submit message.
   */
  function getOperationStringSubstitutions($entity_type, $entity, $operation_path, $parameters = array()) {
    $substitutions = parent::getOperationStringSubstitutions($entity_type, $entity, $operation_path, $parameters);

    if (isset($parameters['uid'])) {
      $account = user_load($parameters['uid']);
      $substitutions['!account-name'] = theme('username', array('account' => $account));
    }

    return $substitutions;
  }

  /**
   * Return the form body, that is, the form without the submit button.
   */
  function form($form, &$form_state, $entity_type, $entity, $operation_path) {
    // TODO: default value.

    $form['owner'] = array(
      '#type' => 'textfield',
      '#title' => t('Owner'),
      '#autocomplete_path' => 'user/autocomplete',
      '#size' => '24',
      '#required' => TRUE,
      '#maxlength' => '60',
      '#description' => t('Enter the username to assign ownership of this entity to.'),
    );

    return $form;
  }

  /**
   * Build a parameters array from the form values.
   */
  function formSubmitGetParameters($form, &$form_state, $entity_type, $entity, $operation_path) {
    // Get the user out of the form value.
    $uid = db_query('SELECT uid from {users} WHERE name = :name', array(':name' => $form_state['values']['owner']))->fetchField();

    $parameters = array(
      'uid' => $uid,
    );

    return $parameters;
  }

  /**
   * Execute the operation.
   */
  function execute($entity_type, $entity, $parameters = array()) {
    $entity->uid = $parameters['uid'];
    $entity->save();
  }

}

/**
 * Op handler for Organic Groups group overview.
 */
class EntityOperationsOperationOGGroup extends EntityOperationsOperationPage {

  /**
   * Returns basic information about the operation.
   */
  function operationInfo() {
    return array(
      'label' => 'OG Group',
      'description' => 'Provides admin tools for working with the entity as an Organic Group.',
    ) + parent::operationInfo();
  }

  /**
   * Returns the menu item for this operation.
   */
  function menu_item($operation_path, $operation_definition, $loader_position) {
    $item = array(
      'title' => 'Group',
      // These won't work: these expect IDs not loaded entities. Hence we have
      // to define userAccess() and build() as wrappers.
      //'page callback' => 'og_ui_group_admin_overview',
      //'page arguments' => array($this->entityType, $loader_position),
      //'access callback' => 'og_ui_get_group_admin',
      //'access arguments' => array($this->entityType, $loader_position),
      'file path' => drupal_get_path('module', 'og_ui'),
      'file' => 'og_ui.admin.inc',
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );

    // Add in what the parent class has.
    $item += parent::menu_item($operation_path, $operation_definition, $loader_position);
    return $item;
  }

  /**
   * Determine access to the operation for the user.
   */
  function userAccess($entity_type, $entity, $params = array(), $account = NULL) {
    // This is just a wrapper.
    $access = og_ui_user_access_group('manage members', $entity_type, $entity->identifier());
    return $access;
  }

  /**
   * Page callback.
   *
   * We need this as a wrapper around og_ui_group_admin_overview() because
   * its parameters aren't compatible with our menu loaders.
   */
  function build($entity_type, $entity, $params = array()) {
    // This is just a wrapper.
    return og_ui_group_admin_overview($entity_type, $entity->identifier());
  }

}
