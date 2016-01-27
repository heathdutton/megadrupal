<?php

/**
 * @file
 * Pollim editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring instead to use
 * views. That offers more flexibility to change a UI that will, more often than not,
 * be end-user facing.
 */

/**
 * UI controller.
 */
class PollimUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {
    
    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'Improved Polls',
      'description' => 'Add edit and update polls.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
    );
    
    // Change the overview menu type for the list of pollims.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    
    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => 'Add an Improved Poll',
      'description' => 'Add a new Poll',
      'page callback'  => 'pollim_add_page',
      'access callback'  => 'pollim_access',
      'access arguments' => array('edit'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'pollim.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

    );
    
    // Add menu items to add each different type of entity.
    foreach (pollim_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'pollim_form_wrapper',
        'page arguments' => array(pollim_create(array('type' => $type->type))),
        'access callback' => 'pollim_access',
        'access arguments' => array('edit', 'edit ' . $type->type),
        'file' => 'pollim.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing pollim entities
    $items[$this->path . '/pollim/' . $wildcard] = array(
      'page callback' => 'pollim_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'pollim_access',
      'access arguments' => array('edit', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'pollim.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    $items[$this->path . '/pollim/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );
    
    $items[$this->path . '/pollim/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'pollim_delete_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'pollim_access',
      'access arguments' => array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'pollim.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    
    // Menu item for viewing pollims
    $items['pollim/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'pollim_page_title',
      'title arguments' => array(1),
      'page callback' => 'pollim_page_view',
      'page arguments' => array(1),
      'access callback' => 'pollim_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    return $items;
  }
  
  
  /**
   * Create the markup for the add Pollim Entities page within the class
   * so it can easily be extended/overriden.
   */ 
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }    
        
    return theme('pollim_add_list', array('content' => $content));
  }
  
}


/**
 * Form callback wrapper: create or edit a pollim.
 *
 * @param $pollim
 *   The pollim object being edited by this form.
 *
 * @see pollim_edit_form()
 */
function pollim_form_wrapper($pollim) {
  // Add the breadcrumb for the form's location.
  pollim_set_breadcrumb();
  return drupal_get_form('pollim_edit_form', $pollim);
}


/**
 * Form callback wrapper: delete a pollim.
 *
 * @param $pollim
 *   The pollim object being edited by this form.
 *
 * @see pollim_edit_form()
 */
function pollim_delete_form_wrapper($pollim) {
  // Add the breadcrumb for the form's location.
  //pollim_set_breadcrumb();
  return drupal_get_form('pollim_delete_form', $pollim);
}


/**
 * Form callback: create or edit a pollim.
 *
 * @param $pollim
 *   The pollim object to edit or for a create form an empty pollim object
 *     with only a pollim type defined.
 */
function pollim_edit_form($form, &$form_state, $pollim) {
  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Poll name'),
    '#default_value' => isset($pollim->name) ? $pollim->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );

  // Add the field related form elements.
  $form_state['pollim'] = $pollim;
  field_attach_form('pollim', $pollim, $form, $form_state);

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
    '#value' => t('Save poll'),
    '#submit' => $submit + array('pollim_edit_form_submit'),
  );
  
  if (!empty($pollim->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete poll'),
      '#suffix' => l(t('Cancel'), 'admin/content/pollims'),
      '#submit' => $submit + array('pollim_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'pollim_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the pollim form
 */
function pollim_edit_form_validate(&$form, &$form_state) {
  $pollim = $form_state['pollim'];
  
  // Notify field widgets to validate their data.
  field_attach_form_validate('pollim', $pollim, $form, $form_state);
}


/**
 * Form API submit callback for the pollim form.
 * 
 * @todo remove hard-coded link
 */
function pollim_edit_form_submit(&$form, &$form_state) {
  
  $pollim = entity_ui_controller('pollim')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the pollim and go back to the list of pollims
  
  // Add in created and changed times.
  if ($pollim->is_new = isset($pollim->is_new) ? $pollim->is_new : 0){
    $pollim->created = time();
  }

  $pollim->changed = time();
  
  $pollim->save();
  $form_state['redirect'] = 'admin/content/pollims';
}

/**
 * Form API submit callback for the delete button.
 * 
 * @todo Remove hard-coded path
 */
function pollim_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/content/pollims/pollim/' . $form_state['pollim']->pollim_id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a pollim.
 *
 * @param $pollim
 *   The pollim to delete
 *
 * @see confirm_form()
 */
function pollim_delete_form($form, &$form_state, $pollim) {
  $form_state['pollim'] = $pollim;

  $form['#submit'][] = 'pollim_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete poll %name?', array('%name' => $pollim->name)),
    'admin/content/pollims/pollim',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );
  
  return $form;
}

/**
 * Submit callback for pollim_delete_form
 */
function pollim_delete_form_submit($form, &$form_state) {
  $pollim = $form_state['pollim'];

  pollim_delete($pollim);

  drupal_set_message(t('The poll %name has been deleted.', array('%name' => $pollim->name)));
  watchdog('pollim', 'Deleted pollim %name.', array('%name' => $pollim->name));

  $form_state['redirect'] = 'admin/content/pollims';
}



/**
 * Page to add Pollim Entities.
 *
 * @todo Pass this through a proper theme function
 */
function pollim_add_page() {
  $controller = entity_ui_controller('pollim');
  return $controller->addPage();
}


/**
 * Displays the list of available pollim types for pollim creation.
 *
 * @ingroup themeable
 */
function theme_pollim_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="pollim-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer pollim types')) {
      $output = '<p>' . t('Pollim Entities cannot be added because you have not created any pollim types yet. Go to the <a href="@create-pollim-type">pollim type creation page</a> to add a new pollim type.', array('@create-pollim-type' => url('admin/structure/pollim_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No pollim types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}





/**
 * Sets the breadcrumb for administrative pollim pages.
 */
function pollim_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Content'), 'admin/content'),
    l(t('Pollims'), 'admin/content/pollims'),
  );

  drupal_set_breadcrumb($breadcrumb);
}



