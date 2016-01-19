<?php

/**
 * @file
 * Avatar editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring instead to use
 * views. That offers more flexibility to change a UI that will, more often than not,
 * be end-user facing.
 */

/**
 * UI controller.
 */
class AvatarUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {
    
    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'Avatars',
      'description' => 'Add edit and update avatars.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
    );
    
    // Change the overview menu type for the list of avatars.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    
    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => 'Add a avatar',
      'description' => 'Add a new avatar',
      'page callback'  => 'avatar_add_page',
      'access callback'  => 'avatar_access',
      'access arguments' => array('edit'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'avatar.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

    );
    
    // Add menu items to add each different type of entity.
    foreach (avatar_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'avatar_form_wrapper',
        'page arguments' => array(avatar_create(array('type' => $type->type))),
        'access callback' => 'avatar_access',
        'access arguments' => array('create', avatar_create(array('type' => $type->type))),
        'file' => 'avatar.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Menu item for viewing avatars
    $items['avatar/' . $wildcard . '/view'] = array(
    //$items[$this->path . '/avatar/' . $wildcard . '/edit'] = array(
      'title' => 'View',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      //'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );
    $items['avatar/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'avatar_page_title',
      'title arguments' => array(1),
      'page callback' => 'avatar_page_view',
      'page arguments' => array(1),
      'access callback' => 'avatar_access',
      'access arguments' => array('view', 1),
    );
    $items['avatar/' . $wildcard . '/popup'] = array(
      'page callback' => 'avatar_page_view',
      'page arguments' => array(1, 'popup'),
			'type' => MENU_CALLBACK,
      'access callback' => 'avatar_access',
      'access arguments' => array('view', 1),
    );
    // Loading and editing avatar entities
    $items['avatar/' . $wildcard . '/edit'] = array(
    //$items[$this->path . '/avatar/' . $wildcard] = array(
      'title' => 'Edit',
      'page callback' => 'avatar_form_wrapper',
      'page arguments' => array(1), //array($id_count + 1),
      'access callback' => 'avatar_access',
      'access arguments' => array('edit', 1), //array('edit', $id_count + 1),
      'weight' => 0,
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'avatar.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    
    $items['avatar/' . $wildcard . '/delete'] = array(
    //$items[$this->path . '/avatar/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'avatar_delete_form_wrapper',
      'page arguments' => array(1), //array($id_count + 1),
      'access callback' => 'avatar_access',
      'access arguments' => array('edit', 1), //array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'avatar.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    
    if (module_exists('devel')) {
      $items['avatar/' . $wildcard . '/devel'] = array(
        'title' => 'Devel',
        'page callback' => 'devel_load_object',
        'page arguments' => array('avatar', 1),
        'access arguments' => array('access devel information'),
        'type' => MENU_LOCAL_TASK,
        'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
        'file' => 'devel.pages.inc',
        'file path' => drupal_get_path('module', 'devel'),
        'weight' => 100,
      );
    }
    return $items;
  }
  
  
  /**
   * Create the markup for the add Avatar Entities page within the class
   * so it can easily be extended/overriden.
   */
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }
        
    return theme('avatar_add_list', array('content' => $content));
  }
  
}


/**
 * Form callback wrapper: create or edit a avatar.
 *
 * @param $avatar
 *   The avatar object being edited by this form.
 *
 * @see mmoguild_edit_form()
 */
function avatar_form_wrapper($avatar) {
  // Add the breadcrumb for the form's location.
  avatar_set_breadcrumb();
  return drupal_get_form('avatar_edit_form', $avatar);
}


/**
 * Form callback wrapper: delete a avatar.
 *
 * @param $avatar
 *   The avatar object being edited by this form.
 *
 * @see mmoguild_edit_form()
 */
function avatar_delete_form_wrapper($avatar) {
  // Add the breadcrumb for the form's location.
  avatar_set_breadcrumb();
  return drupal_get_form('avatar_delete_form', $avatar);
}


/**
 * Form callback: create or edit a avatar.
 *
 * @param $form
 * @param $form_state
 */
function avatar_edit_form($form, &$form_state, $avatar) {
  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Avatar Name'),
    '#default_value' => isset($avatar->name) ? $avatar->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );
  
  /*
  $form['data']['#tree'] = TRUE;
  $form['data']['sample_data'] = array(
    '#type' => 'checkbox',
    '#title' => t('An interesting avatar switch'),
    '#default_value' => isset($avatar->data['sample_data']) ? $avatar->data['sample_data'] : 1,
  );
  */

  // Add the field related form elements.
  $form_state['avatar'] = $avatar;
  field_attach_form('avatar', $avatar, $form, $form_state);

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
    '#value' => t('Save avatar'),
    '#submit' => $submit + array('avatar_edit_form_submit'),
  );
  
  if (!empty($avatar->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete avatar'),
      '#suffix' => l(t('Cancel'), 'avatar/' . $avatar->aid),
      '#submit' => $submit + array('avatar_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'avatar_edit_form_validate';
  
  return $form;
}


/**
 * Form API validate callback for the avatar form
 */
function avatar_edit_form_validate(&$form, &$form_state) {
  $avatar = $form_state['avatar'];
  
  // Notify field widgets to validate their data.
  field_attach_form_validate('avatar', $avatar, $form, $form_state);
}


/**
 * Form API submit callback for the avatar form.
 *
 */
function avatar_edit_form_submit(&$form, &$form_state) {
  global $user;
  // Save the avatar and go back to the list of avatars
  $avatar = entity_ui_controller('avatar')->entityFormSubmitBuildEntity($form, $form_state);
  
  // Add in created and changed times.
  if ($avatar->is_new = isset($avatar->is_new) ? $avatar->is_new : 0) {
    $avatar->created = time();
  }
  
  if (empty($avatar->uid)) {
    $avatar->uid = $user->uid;
  }

  $avatar->changed = time();
  
  $avatar->save();
  $path = avatar_uri($avatar);
  $form_state['redirect'] = $path['path'];
}

/**
 * Form API submit callback for the delete button.
 *
 */
function avatar_form_submit_delete(&$form, &$form_state) {
  $path = avatar_uri($form_state['avatar']);
  $form_state['redirect'] = $path['path'] . '/delete';
}


/**
 * Form callback: confirmation form for deleting a avatar.
 *
 * @param $avatar
 *   The avatar to delete
 *
 * @see confirm_form()
 */
function avatar_delete_form($form, &$form_state, $avatar) {
  $form_state['avatar'] = $avatar;

  $form['#submit'][] = 'avatar_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete avatar %name?', array('%name' => $avatar->name)),
    avatar_uri($avatar),
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );
  
  return $form;
}

/**
 * Submit callback for mmoguild_delete_form
 */
function avatar_delete_form_submit($form, &$form_state) {
  $avatar = $form_state['avatar'];

  avatar_delete($avatar);

  drupal_set_message(t('The avatar %name has been deleted.', array('%name' => $avatar->name)));
  watchdog('avatar', 'Deleted avatar %name.', array('%name' => $avatar->name));

  $form_state['redirect'] = 'admin/content/avatars';
}



/**
 * Page to add Avatar Entities.
 *
 * @todo Pass this through a proper theme function
 */
function avatar_add_page() {
  $controller = entity_ui_controller('avatar');
  return $controller->addPage();
}


/**
 * Displays the list of available avatar types for avatar creation.
 *
 * @ingroup themeable
 */
function theme_avatar_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="avatar-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer avatar types')) {
      $output = '<p>' . t('Avatar Entities cannot be added because you have not created any avatar types yet. Go to the <a href="@create-avatar-type">avatar type creation page</a> to add a new avatar type.', array('@create-avatar-type' => url('admin/structure/mmoguild_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No avatar types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}


/**
 * Sets the breadcrumb for administrative avatar pages.
 */
function avatar_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Content'), 'admin/content'),
    l(t('Avatars'), 'admin/content/avatars'),
  );

  drupal_set_breadcrumb($breadcrumb);
}



