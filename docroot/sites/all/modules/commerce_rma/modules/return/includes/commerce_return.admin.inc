<?php

/**
 * @file
 * Commerce return admin UI class.
 */

/**
 * UI controller.
 */
class CommerceReturnUIController extends EntityDefaultUIController {

  /**
   * Overridden EntityDefaultUIController::hook_menu().
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $this->id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%entity_object';

    unset($items[$this->path . '/add']);
    unset($items[$this->path . '/clone']);

    $items['admin/commerce/orders/%commerce_order/edit/return'] = array(
      'title' => 'Create a new return',
      'page callback' => 'commerce_return_new_order_wrapper',
      'page arguments' => array(3),
      'access callback' => 'commerce_return_access',
      'access arguments' => array('create'),
      'type' => MENU_LOCAL_ACTION,
      'file' => 'includes/commerce_return.admin.inc',
      'file path' => drupal_get_path('module', 'commerce_return'),
    );

    $items[$this->path . '/' . $wildcard] = array(
      'title callback' => 'entity_label',
      'title arguments' => array($this->entityType, $this->id_count),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('commerce_return_view_form', $this->id_count),
      'access callback' => 'commerce_return_access',
      'access arguments' => array('view', $this->id_count),
      'file' => 'includes/commerce_return.admin.inc',
      'file path' => drupal_get_path('module', 'commerce_return'),
    );

    $items[$this->path . '/' . $wildcard . '/view'] = array(
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -9,
    );

    $items[$this->path] = array(
      'title' => 'Returns',
      'description' => 'Manage returns in the store.',
      'page callback' => 'commerce_return_overview_form',
      'access callback' => 'commerce_return_access',
      'access arguments' => array('view'),
      'file' => 'includes/commerce_return.admin.inc',
      'file path' => drupal_get_path('module', 'commerce_return'),
    );

    $items[$this->path . '/settings'] = array(
      'title' => 'Settings',
      'description' => 'Global settings for Commerce Returns.',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('commerce_return_settings'),
      'access arguments' => array('administer commerce returns'),
      'type' => MENU_LOCAL_TASK,
      'file' => 'includes/commerce_return.admin.inc',
      'file path' => drupal_get_path('module', 'commerce_return'),
    );

    return $items;
  }
}

/**
 * Menu callback: Return the new return form based on commerce_order object that
 * function receives.
 *
 * @param stdClass $order
 *   A fully loaded Commerce Order object.
 *
 * @return array
 *   The new return form array.
 */
function commerce_return_new_order_wrapper(stdClass $order) {
  return entity_ui_get_form('commerce_return', commerce_return_new($order), 'add');
}

/**
 * Builds the entity view form.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 * @param CommerceReturn $return.
 *   A fully loaded CommerceReturn entity.
 *
 * @return array
 *   An array to be rendered.
 */
function commerce_return_view_form($form, &$form_state, CommerceReturn $return) {
  $form['commerce_return'] = entity_view('commerce_return', array($return));
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'commerce_return') . '/theme/commerce_return.theme.css'
  );

  return $form;
}

/**
 * Builds the entity overview form.
 */
function commerce_return_overview_form() {
  $view = views_get_view('commerce_return_overview');
  $view->override_url = $_GET['q'];
  $form['view'] = array('#markup' => $view->preview());

  return $form;
}

/**
 *
 */

/**
 * Form callback: create or edit a return.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 * @param CommerceReturn $return
 *   A fully loaded CommerceReturn object.
 * @param string $op
 *   The operation to proceed.
 *
 * @return array
 *   The updated form array.
 */
function commerce_return_form($form, &$form_state, CommerceReturn $return, $op = 'edit') {
  global $user;
  // Ensure the owner name is accessible if the uid is set.
  if (!empty($return->uid) && $owner = user_load($return->uid)) {
    $return->name = $owner->name;

    if (empty($return->mail)) {
      $return->mail = $owner->mail;
    }
  } else {
    $return->name = $user->name;
    $return->mail = $user->mail;
  }

  if (empty($return->created)) {
    $return->date = format_date(REQUEST_TIME, 'custom', 'Y-m-d H:i:s O');
  }

  // Add the field related form elements.
  $form_state['commerce_return'] = $return;
  field_attach_form('commerce_return', $return, $form, $form_state);

  // Hide the return total field from direct editing.
  $form['commerce_return_total']['#access'] = FALSE;

  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 99,
  );

  // Build an array of return status options grouped by return state.
  $options = array();

  foreach (commerce_return_state_get_title() as $name => $title) {
    foreach (commerce_return_statuses(array('state' => $name)) as $return_status) {
      $options[check_plain($title)][$return_status['name']] = check_plain($return_status['title']);
    }
  }

  // Add a section to update the status and leave a log message.
  $form['return_status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Return status'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'additional_settings',
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'commerce_order') . '/commerce_order.js',
        array(
          'type' => 'setting',
          'data' => array('status_titles' => commerce_return_status_get_title()),
        ),
      ),
    ),
    '#weight' => 20,
  );
  $form['return_status']['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#options' => $options,
    '#default_value' => $return->status,
  );
  $form['return_status']['status_original'] = array(
    '#type' => 'hidden',
    '#value' => $return->status,
    '#attributes' => array('id' => 'edit-status-original'),
  );
  $form['return_status']['log'] = array(
    '#type' => 'textarea',
    '#title' => t('Update log message'),
    '#description' => t('Provide an explanation of the changes you are making. This will provide a meaningful audit trail for updates to this return.'),
    '#rows' => 4,
  );

  // Add the user account and e-mail fields.
  $form['user'] = array(
    '#type' => 'fieldset',
    '#title' => t('User information'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#access' => user_access('administer commerce_return entities'),
    '#group' => 'additional_settings',
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'commerce_order') . '/commerce_order.js',
        array(
          'type' => 'setting',
          'data' => array('anonymous' => variable_get('anonymous', t('Anonymous'))),
        ),
      ),
    ),
    '#weight' => 30,
  );
  $form['user']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Owned by'),
    '#description' => t('Leave blank for %anonymous.', array('%anonymous' => variable_get('anonymous', t('Anonymous')))),
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => isset($return->name) ? $return->name : '',
    '#weight' => -1,
  );
  $form['user']['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Return contact e-mail'),
    '#description' => t('Defaults to the owner account e-mail address if left blank. Used for return communication.'),
    '#default_value' => !empty($return->mail) ? $return->mail : $return->mail,
  );

  // Add a log checkbox and timestamp field to a history tab.
  $form['return_history'] = array(
    '#type' => 'fieldset',
    '#title' => t('Return history', array(), array('context' => 'a drupal commerce return')),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'additional_settings',
    '#weight' => 40,
  );
  $form['return_history']['revision'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create new revision on update'),
    '#description' => t('If an update log message is entered, a revision will be created even if this is unchecked.'),
    '#default_value' => variable_get('commerce_return_auto_revision', TRUE),
    '#access' => user_access('administer commerce_return entities'),
  );
  $form['return_history']['date'] = array(
    '#type' => 'textfield',
    '#title' => t('Created on'),
    '#description' => t('Format: %time. The date format is YYYY-MM-DD and %timezone is the time zone offset from UTC. Leave blank to use the time of form submission.', array('%time' => !empty($return->date) ? date_format(date_create($return->date), 'Y-m-d H:i:s O') : format_date($return->created, 'custom', 'Y-m-d H:i:s O'), '%timezone' => !empty($return->date) ? date_format(date_create($return->date), 'O') : format_date($return->created, 'custom', 'O'))),
    '#maxlength' => 25,
    '#default_value' => !empty($return->created) ? format_date($return->created, 'custom', 'Y-m-d H:i:s O') : '',
  );
  $form['return_history']['created'] = array(
    '#type' => 'hidden',
    '#value' => !empty($return->created) ? format_date($return->created, 'short') : '',
    '#attributes' => array('id' => 'edit-created'),
  );
  $form['return_history']['changed'] = array(
    '#type' => 'hidden',
    '#value' => !empty($return->changed) ? format_date($return->changed, 'short') : '',
    '#attributes' => array('id' => 'edit-changed'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save return', array(), array('context' => 'a drupal commerce return')),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Form API validation callback for the type form.
 */
function commerce_return_form_validate($form, &$form_state) {
  field_attach_form_validate('commerce_return', $form_state['commerce_return'], $form, $form_state);
}

/**
 * Form API submit callback for the type form.
 */
function commerce_return_form_submit(&$form, &$form_state) {
  $commerce_return = entity_ui_form_submit_build_entity($form, $form_state);
  $commerce_return->save();
  $form_state['redirect'] = 'admin/commerce/store/returns';
}

/**
 * Form API submit callback for the delete button.
 */
function commerce_return_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/commerce/store/returns/manage/' . $form_state['commerce_return']->return_id . '/delete';
}

/**
 * Builds the discount settings form.
 */
function commerce_return_settings() {
  $form['return_settings'] = array(
    '#type' => 'vertical_tabs',
  );

  // Add the user account and e-mail fields.
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#access' => user_access('configure return settings'),
    '#group' => 'return_settings',
    '#weight' => 0,
  );
  $form['general']['return_is_activated'] = array(
    '#type' => 'checkbox',
    '#title' => t('Activate returns for customers'),
    '#description' => t('This options controls global customer access to return\'s creation workflow. It\'s only for emergency cases because a permission is already defined for that. (See "access return of goods" from permissions page)'),
    '#default_value' => variable_get('return_is_activated', TRUE),
  );

  return system_settings_form($form);
}
