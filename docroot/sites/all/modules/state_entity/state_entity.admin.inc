<?php

/**
 * @file
 * state_entity editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring instead to use
 * views. That offers more flexibility to change a UI that will, more often than not,
 * be end-user facing.
 */

/**
 * UI controller.
 */
class state_entityUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {
    
    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'State Entity',
      'description' => 'Add edit and update State Entity.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
    );
    
    // Change the overview menu type for the list of State Entity.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    
    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => 'Add a State Entity',
      'description' => 'Add a new State Entity',
      'page callback'  => 'state_entity_add_page',
      'access callback'  => 'state_entity_access',
      'access arguments' => array('edit'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'state_entity.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

    );
    
    // Add menu items to add each different type of entity.
    foreach (state_entity_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'state_entity_form_wrapper',
        'page arguments' => array(state_entity_create(array('type' => $type->type))),
        'access callback' => 'state_entity_access',
        'access arguments' => array('edit', 'edit ' . $type->type),
        'file' => 'state_entity.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing state_entity entities
    $items[$this->path . '/state_entity/' . $wildcard] = array(
      'page callback' => 'state_entity_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'state_entity_access',
      'access arguments' => array('edit', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'state_entity.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    $items[$this->path . '/state_entity/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );
    
    $items[$this->path . '/state_entity/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'state_entity_delete_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'state_entity_access',
      'access arguments' => array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'state_entity.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    
    // Menu item for viewing State Entity
    $items['state_entity/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'state_entity_page_title',
      'title arguments' => array(1),
      'page callback' => 'state_entity_page_view',
      'page arguments' => array(1),
      'access callback' => 'state_entity_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    return $items;
  }
  
  
  /**
   * Create the markup for the add state_entity Entities page within the class
   * so it can easily be extended/overriden.
   */ 
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }    
        
    return theme('state_entity_add_list', array('content' => $content));
  }
  
}


/**
 * Form callback wrapper: create or edit a state_entity.
 *
 * @param $state_entity
 *   The state_entity object being edited by this form.
 *
 * @see state_entity_edit_form()
 */
function state_entity_form_wrapper($state_entity) {
  // Add the breadcrumb for the form's location.
  state_entity_set_breadcrumb();
  return drupal_get_form('state_entity_edit_form', $state_entity);
}


/**
 * Form callback wrapper: delete a state_entity.
 *
 * @param $state_entity
 *   The state_entity object being edited by this form.
 *
 * @see state_entity_edit_form()
 */
function state_entity_delete_form_wrapper($state_entity) {
  // Add the breadcrumb for the form's location.
  //state_entity_set_breadcrumb();
  return drupal_get_form('state_entity_delete_form', $state_entity);
}


/**
 * Form callback: create or edit a state_entity.
 *
 * @param $state_entity
 *   The state_entity object to edit or for a create form an empty state_entity object
 *     with only a State Entity type defined.
 */
function state_entity_edit_form($form, &$form_state, $state_entity) {

  // Add the field related form elements.
  $form_state['state_entity'] = $state_entity;
  field_attach_form('state_entity', $state_entity, $form, $form_state);

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
    '#value' => t('Save state entity'),
    '#submit' => $submit + array('state_entity_edit_form_submit'),
  );
  
  if (!empty($state_entity->state_entity_id)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete state entity'),
      '#suffix' => l(t('Cancel'), 'admin/content/state_entity'),
      '#submit' => $submit + array('state_entity_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'state_entity_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the state_entity form
 */
function state_entity_edit_form_validate(&$form, &$form_state) {
  $state_entity = $form_state['state_entity'];
  
  // Notify field widgets to validate their data.
  field_attach_form_validate('state_entity', $state_entity, $form, $form_state);
}


/**
 * Form API submit callback for the state_entity form.
 * 
 * @todo remove hard-coded link
 */
function state_entity_edit_form_submit(&$form, &$form_state) {
  
  $state_entity = entity_ui_controller('state_entity')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the state_entity and go back to the list of State Entity
  
  // Add in created and changed times.
  if ($state_entity->is_new = isset($state_entity->is_new) ? $state_entity->is_new : 0){
    $state_entity->created = time();
  }

  $state_entity->changed = time();
  
  $state_entity->save();
  $form_state['redirect'] = 'admin/content/state_entity';
}

/**
 * Form API submit callback for the delete button.
 * 
 * @todo Remove hard-coded path
 */
function state_entity_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/content/state_entity/state_entity/' . $form_state['state_entity']->state_entity_id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a state_entity.
 *
 * @param $state_entity
 *   The state_entity to delete
 *
 * @see confirm_form()
 */
function state_entity_delete_form($form, &$form_state, $state_entity) {
  $form_state['state_entity'] = $state_entity;

  $form['#submit'][] = 'state_entity_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete state_entity?'),
    'admin/content/state_entity/state_entity',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );
  
  return $form;
}

/**
 * Submit callback for state_entity_delete_form
 */
function state_entity_delete_form_submit($form, &$form_state) {
  $state_entity = $form_state['state_entity'];

  state_entity_delete($state_entity);

  drupal_set_message(t('The state_entity has been deleted.'));
  watchdog('state_entity', 'Deleted State Entity.');

  $form_state['redirect'] = 'admin/content/state_entity';
}



/**
 * Page to add state_entity Entities.
 *
 * @todo Pass this through a proper theme function
 */
function state_entity_add_page() {
  $controller = entity_ui_controller('state_entity');
  return $controller->addPage();
}


/**
 * Displays the list of available State Entity types for state_entity creation.
 *
 * @ingroup themeable
 */
function theme_state_entity_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="state_entity-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer state entity types')) {
      $output = '<p>' . t('State Entity Entities cannot be added because you have not created any State Entity types yet. Go to the <a href="@create-state_entity-type">state_entity type creation page</a> to add a new State Entity type.', array('@create-state_entity-type' => url('admin/structure/state_entity_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No State Entity types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}





/**
 * Sets the breadcrumb for administrative state_entity pages.
 */
function state_entity_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Content'), 'admin/content'),
    l(t('State Entity'), 'admin/content/state_entity'),
  );

  drupal_set_breadcrumb($breadcrumb);
}



