<?php

/**
 * @file
 * Invites editing UI.
 */

/**
 * UI controller.
 */
class InvitesUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {
    
    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'Invites',
      'description' => 'Add edit and update invites.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
    );
    
    // Change the overview menu type for the list of invites.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    
    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => 'Add an invites',
      'description' => 'Add a new invites',
      'page callback'  => 'invites_add_page',
      'access callback'  => 'invites_access',
      'access arguments' => array('create'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'invites.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

    );
    
    // Add menu items to add each different type of entity.
    foreach (invites_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'invites_form_wrapper',
        'page arguments' => array(invites_create(array('type' => $type->type))),
        'access callback' => 'invites_access',
        'access arguments' => array('create', NULL, NULL, $type->type),
        'file' => 'invites.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing invites entities
    $items[$this->path . '/invites/' . $wildcard] = array(
      'page callback' => 'invites_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'invites_access',
      'access arguments' => array('edit', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'invites.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    $items[$this->path . '/invites/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );
    
    $items[$this->path . '/invites/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'invites_delete_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'invites_access',
      'access arguments' => array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'invites.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    
    // Menu item for viewing invites
    $items['invites/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'invites_page_title',
      'title arguments' => array(1),
      'page callback' => 'invites_page_view',
      'page arguments' => array(1),
      'access callback' => 'invites_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    return $items;
  }
  
  
  /**
   * Create the markup for the add Invites Entities page within the class
   * so it can easily be extended/overriden.
   */ 
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }    
        
    return theme('invites_add_list', array('content' => $content));
  }
  
}


/**
 * Form callback wrapper: create or edit a invites.
 *
 * @param $invites
 *   The invites object being edited by this form.
 *
 * @see invites_edit_form()
 */
function invites_form_wrapper($invites) {
  // Add the breadcrumb for the form's location.
  invites_set_breadcrumb();
  return drupal_get_form('invites_edit_form', $invites);
}


/**
 * Form callback wrapper: delete a invites.
 *
 * @param $invites
 *   The invites object being edited by this form.
 *
 * @see invites_edit_form()
 */
function invites_delete_form_wrapper($invites) {
  // Add the breadcrumb for the form's location.
  //invites_set_breadcrumb();
  return drupal_get_form('invites_delete_form', $invites);
}


/**
 * Form callback: create or edit a invites.
 *
 * @param $invites
 *   The invites object to edit or for a create form an empty invites object
 *     with only an invites type defined.
 */
function invites_edit_form($form, &$form_state, $invites) {
  // Add the default field elements.
  $form['open_tag'] = array(
    '#markup' => '<div id="invites-form">',
    '#weight' => -1000,  
  );
  $form['close_tag'] = array(
    '#markup' => '</div>',
    '#weight' => 1000,  
  );
  $form['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Invitation Email'),
    '#default_value' => isset($invites->mail) ? $invites->mail : '',
    '#maxlength' => 255,
    '#required' => TRUE,  
    '#weight' => -1,
  );

  // Add the field related form elements.
  $form_state['invites'] = $invites;
  field_attach_form('invites', $invites, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,    
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send invitation'),
    '#submit' => $submit + array('invites_edit_form_submit'),
  );
  
  if (!empty($invites->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete invites'),
      '#suffix' => l(t('Cancel'), 'admin/content/invites'),
      '#submit' => $submit + array('invites_form_submit_delete'),
      '#weight' => 45,
    );
  }
  // Needed for loading an AJAX form in an 'inc' file.
  // form_load_include($form_state, 'inc', 'invites', 'invites.admin');
  
  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'invites_edit_form_validate';
  return $form;
}

/**
 * Form API validate callback for the invites form
 */
function invites_edit_form_validate(&$form, &$form_state) {
  if (!valid_email_address($form_state['values']['mail'])) {
    form_set_error('mail', t('The e-mail address you specified is not valid.'));
  }
  
  // Notify field widgets to validate their data.
  field_attach_form_validate('invites', $form_state['invites'], $form, $form_state);
}


/**
 * Form API submit callback for the invitrevert_alles form.
 * 
 * @todo remove hard-coded link
 */
function invites_edit_form_submit(&$form, &$form_state) {
  global $user;
  $invites = entity_ui_controller('invites')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the invites and go back to the list of invites
  
  // Add in created and changed times.
  if ($invites->is_new = isset($invites->is_new) ? $invites->is_new : 0){
    $invites->created = time();
  }
  
  if (empty($invites->invites_token)){
    $invites->invites_token = uniqid(rand(), true);
    $invites->inviter = $user->uid;
  }
  $invites->changed = time();
  // Check if the invitation mail have a user,
  // and insert it to the invitee if it does.
  $mail = $invites->mail;
  $uid = db_query("SELECT uid FROM {users} WHERE mail = :mail LIMIT 1", array(':mail' => $mail))->fetchField();
  if (!empty($uid)) {
    $invites->invitee = $uid;
  }
  $invites->save();
//  $form_state['redirect'] = 'admin/content/invites';
  drupal_set_message("An invitation sent to {$form_state['values']['mail']}");
}

/**
 * Form API submit callback for the delete button.
 * 
 * @todo Remove hard-coded path
 */
function invites_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/content/invites/invites/' . $form_state['invites']->invites_id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a invites.
 *
 * @param $invites
 *   The invites to delete
 *
 * @see confirm_form()
 */
function invites_delete_form($form, &$form_state, $invites) {
  $form_state['invites'] = $invites;

  $form['#submit'][] = 'invites_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete invites %mail?', array('%mail' => $invites->mail)),
    'admin/content/invites/invites',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );
  
  return $form;
}

/**
 * Submit callback for invites_delete_form
 */
function invites_delete_form_submit($form, &$form_state) {
  $invites = $form_state['invites'];

  invites_delete($invites);

  drupal_set_message(t('The invites %mail has been deleted.', array('%mail' => $invites->mail)));
  watchdog('invites', 'Deleted invites %mail.', array('%mail' => $invites->mail));

  $form_state['redirect'] = 'admin/content/invites';
}



/**
 * Page to add Invites Entities.
 *
 * @todo Pass this through a proper theme function
 */
function invites_add_page() {
  $controller = entity_ui_controller('invites');
  return $controller->addPage();
}


/**
 * Displays the list of available invites types for invites creation.
 *
 * @ingroup themeable
 */
function theme_invites_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="invites-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer invites types')) {
      $output = '<p>' . t('Invites Entities cannot be added because you have not created any invites types yet. Go to the <a href="@create-invites-type">invites type creation page</a> to add a new invites type.', array('@create-invites-type' => url('admin/structure/invites_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No invites types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}

/**
 * Sets the breadcrumb for administrative invites pages.
 */
function invites_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Content'), 'admin/content'),
    l(t('Invites'), 'admin/content/invites'),
  );

  drupal_set_breadcrumb($breadcrumb);
}



