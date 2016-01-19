<?php

/**
 * @file
 * Things editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring instead to use
 * views. That offers more flexibility to change a UI that will, more often than not,
 * be end-user facing.
 */

/**
 * UI controller.
 */
class ThingsUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {
    
    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'Things',
      'description' => 'Add edit and update things.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
    );
    
    // Change the overview menu type for the list of things.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    
    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => 'Add a things',
      'description' => 'Add a new things',
      'page callback'  => 'things_add_page',
      'access callback'  => 'things_access',
      'access arguments' => array('edit'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'things.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

    );
    
    // Add menu items to add each different type of entity.
    foreach (things_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'things_form_wrapper',
        'page arguments' => array(things_create(array('type' => $type->type))),
        'access callback' => 'things_access',
        'access arguments' => array('edit', 'edit ' . $type->type),
        'file' => 'things.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing things entities
    $items[$this->path . '/things/' . $wildcard] = array(
      'page callback' => 'things_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'things_access',
      'access arguments' => array('edit', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'things.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    $items[$this->path . '/things/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );
    
    $items[$this->path . '/things/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'things_delete_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'things_access',
      'access arguments' => array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'things.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    
    // Menu item for viewing things
    $items['things/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'things_page_title',
      'title arguments' => array(1),
      'page callback' => 'things_page_view',
      'page arguments' => array(1),
      'access callback' => 'things_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    return $items;
  }
  
  
  /**
   * Create the markup for the add Things Entities page within the class
   * so it can easily be extended/overriden.
   */ 
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }    
        
    return theme('things_add_list', array('content' => $content));
  }
  
}


/**
 * Form callback wrapper: create or edit a things.
 *
 * @param $things
 *   The things object being edited by this form.
 *
 * @see things_edit_form()
 */
function things_form_wrapper($things) {
  // Add the breadcrumb for the form's location.
  things_set_breadcrumb();
  return drupal_get_form('things_edit_form', $things);
}


/**
 * Form callback wrapper: delete a things.
 *
 * @param $things
 *   The things object being edited by this form.
 *
 * @see things_edit_form()
 */
function things_delete_form_wrapper($things) {
  // Add the breadcrumb for the form's location.
  //things_set_breadcrumb();
  return drupal_get_form('things_delete_form', $things);
}


/**
 * Form callback: create or edit a things.
 *
 * @param $things
 *   The things object to edit or for a create form an empty things object
 *     with only a things type defined.
 */
function things_edit_form($form, &$form_state, $things) {
  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Things Name'),
    '#default_value' => isset($things->name) ? $things->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );
  
  // Add the field related form elements.
  $form_state['things'] = $things;
  field_attach_form('things', $things, $form, $form_state);

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
    '#value' => t('Save things'),
    '#submit' => $submit + array('things_edit_form_submit'),
  );
  
  if (!empty($things->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete things'),
      '#suffix' => l(t('Cancel'), 'admin/content/things'),
      '#submit' => $submit + array('things_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'things_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the things form
 */
function things_edit_form_validate(&$form, &$form_state) {
  $things = $form_state['things'];
  
  // Notify field widgets to validate their data.
  field_attach_form_validate('things', $things, $form, $form_state);
}


/**
 * Form API submit callback for the things form.
 * 
 * @todo remove hard-coded link
 */
function things_edit_form_submit(&$form, &$form_state) {
  
  $things = entity_ui_controller('things')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the things and go back to the list of things
  
  // Add in created and changed times.
  if ($things->is_new = isset($things->is_new) ? $things->is_new : 0){
    $things->created = time();
  }

  $things->changed = time();
  
  $things->save();
  $form_state['redirect'] = 'admin/content/things';
}

/**
 * Form API submit callback for the delete button.
 * 
 * @todo Remove hard-coded path
 */
function things_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/content/things/things/' . $form_state['things']->things_id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a things.
 *
 * @param $things
 *   The things to delete
 *
 * @see confirm_form()
 */
function things_delete_form($form, &$form_state, $things) {
  $form_state['things'] = $things;

  $form['#submit'][] = 'things_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete things %name?', array('%name' => $things->name)),
    'admin/content/things/things',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );
  
  return $form;
}

/**
 * Submit callback for things_delete_form
 */
function things_delete_form_submit($form, &$form_state) {
  $things = $form_state['things'];

  things_delete($things);

  drupal_set_message(t('The things %name has been deleted.', array('%name' => $things->name)));
  watchdog('things', 'Deleted things %name.', array('%name' => $things->name));

  $form_state['redirect'] = 'admin/content/things';
}



/**
 * Page to add Things Entities.
 *
 * @todo Pass this through a proper theme function
 */
function things_add_page() {
  $controller = entity_ui_controller('things');
  return $controller->addPage();
}


/**
 * Displays the list of available things types for things creation.
 *
 * @ingroup themeable
 */
function theme_things_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="things-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer things types')) {
      $output = '<p>' . t('Things Entities cannot be added because you have not created any things types yet. Go to the <a href="@create-things-type">things type creation page</a> to add a new things type.', array('@create-things-type' => url('admin/structure/things_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No things types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}





/**
 * Sets the breadcrumb for administrative things pages.
 */
function things_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Content'), 'admin/content'),
    l(t('Things'), 'admin/content/things'),
  );

  drupal_set_breadcrumb($breadcrumb);
}



