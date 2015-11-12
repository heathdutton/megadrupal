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
class WoWToonFeedItemUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {

    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'Activity Feed Items',
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
      'title' => 'Add a feed item',
      'description' => 'Add a new feed item',
      'page callback'  => 'wowtoon_feed_item_add_page',
      'access callback'  => 'wowtoon_feed_item_access',
      'access arguments' => array('edit'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'wowtoon_feed_item.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

    );

    // Add menu items to add each different type of entity.
    foreach (wowtoon_feed_item_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'wowtoon_feed_item_form_wrapper',
        'page arguments' => array(wowtoon_feed_item_create(array('type' => $type->type))),
        'access callback' => 'wowtoon_feed_item_access',
        'access arguments' => array('create', wowtoon_feed_item_create(array('type' => $type->type))),
        'file' => 'wowtoon_feed_item.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Menu item for viewing avatars
    $items['activity_feed/' . $wildcard . '/view'] = array(
    //$items[$this->path . '/mmoavatar/' . $wildcard . '/edit'] = array(
      'title' => 'View',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      //'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );
    $items['activity_feed/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'wowtoon_feed_item_page_title',
      'title arguments' => array(1),
      'page callback' => 'wowtoon_feed_item_page_view',
      'page arguments' => array(1),
      'access callback' => 'wowtoon_feed_item_access',
      'access arguments' => array('view', 1),
    );

    // Loading and editing avatar entities
    $items['activity_feed/' . $wildcard . '/edit'] = array(
    //$items[$this->path . '/mmoavatar/' . $wildcard] = array(
      'title' => 'Edit',
      'page callback' => 'wowtoon_feed_item_form_wrapper',
      'page arguments' => array(1), //array($id_count + 1),
      'access callback' => 'wowtoon_feed_item_access',
      'access arguments' => array('edit', 1), //array('edit', $id_count + 1),
      'weight' => 0,
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'wowtoon_feed_item.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );

    $items['activity_feed/' . $wildcard . '/delete'] = array(
    //$items[$this->path . '/mmoavatar/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'wowtoon_feed_item_delete_form_wrapper',
      'page arguments' => array(1), //array($id_count + 1),
      'access callback' => 'wowtoon_feed_item_access',
      'access arguments' => array('edit', 1), //array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'wowtoon_feed_item.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );

    if (module_exists('devel')) {
      $items['activity_feed/' . $wildcard . '/devel'] = array(
        'title' => 'Devel',
        'page callback' => 'devel_load_object',
        'page arguments' => array('wowtoon_feed_item', 1),
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
 * @see alter_ego_edit_form()
 */
function wowtoon_feed_item_form_wrapper($avatar) {
  // Add the breadcrumb for the form's location.
  wowtoon_feed_item_set_breadcrumb();
  return drupal_get_form('wowtoon_feed_item_edit_form', $avatar);
}


/**
 * Form callback wrapper: delete a avatar.
 *
 * @param $avatar
 *   The avatar object being edited by this form.
 *
 * @see alter_ego_edit_form()
 */
function wowtoon_feed_item_delete_form_wrapper($avatar) {
  // Add the breadcrumb for the form's location.
  //alter_ego_set_breadcrumb();
  return drupal_get_form('wowtoon_feed_item_delete_form', $avatar);
}


/**
 * Form callback: create or edit a avatar.
 *
 * @param $avatar
 *   The avatar object to edit or for a create form an empty avatar object
 *     with only a avatar type defined.
 */
function wowtoon_feed_item_edit_form($form, &$form_state, $avatar) {
  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Activity Feed Item Name'),
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
    '#value' => t('Save wowtoon_feed_item'),
    '#submit' => $submit + array('wowtoon_feed_item_edit_form_submit'),
  );

  if (!empty($avatar->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete wowtoon_feed_item'),
      '#suffix' => l(t('Cancel'), 'activity_feed/' . $avatar->aid),
      '#submit' => $submit + array('wowtoon_feed_item_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'wowtoon_feed_item_edit_form_validate';

  return $form;
}


/**
 * Form API validate callback for the avatar form
 */
function wowtoon_feed_item_edit_form_validate(&$form, &$form_state) {
  $avatar = $form_state['wowtoon_feed_item'];

  // Notify field widgets to validate their data.
  field_attach_form_validate('wowtoon_feed_item', $avatar, $form, $form_state);
}


/**
 * Form API submit callback for the avatar form.
 *
 * @todo remove hard-coded link
 */
function wowtoon_feed_item_edit_form_submit(&$form, &$form_state) {
  global $user;
  // Save the avatar and go back to the list of avatars
  $avatar = entity_ui_controller('wowtoon_feed_item')->entityFormSubmitBuildEntity($form, $form_state);

  // Add in created and changed times.
  if ($avatar->is_new = isset($avatar->is_new) ? $avatar->is_new : 0){
    $avatar->created = time();
  }

  if (empty($avatar->uid)) {
    $avatar->uid = $user->uid;
  }

  $avatar->changed = time();

  $avatar->save();
  $form_state['redirect'] = 'activity_feed/' . $avatar->aid;
}

/**
 * Form API submit callback for the delete button.
 *
 * @todo Remove hard-coded path
 */
function wowtoon_feed_item_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'activity_feed/' . $form_state['wowtoon_feed_item']->achid . '/delete';
}


/**
 * Form callback: confirmation form for deleting a avatar.
 *
 * @param $avatar
 *   The avatar to delete
 *
 * @see confirm_form()
 */
function wowtoon_feed_item_delete_form($form, &$form_state, $avatar) {
  $form_state['wowtoon_feed_item'] = $avatar;

  $form['#submit'][] = 'wowtoon_feed_item_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete wowtoon_feed_item %name?', array('%name' => $avatar->name)),
    wowtoon_feed_item_uri($avatar),
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for alter_ego_delete_form
 */
function wowtoon_feed_item_delete_form_submit($form, &$form_state) {
  $avatar = $form_state['wowtoon_feed_item'];

  wowtoon_feed_item_delete($avatar);

  drupal_set_message(t('The wowtoon_feed_item %name has been deleted.', array('%name' => $avatar->name)));
  watchdog('wowtoon_feed_item', 'Deleted wowtoon_feed_item %name.', array('%name' => $avatar->name));

  $form_state['redirect'] = 'admin/content/wowtoon_feed_items';
}



/**
 * Page to add Avatar Entities.
 *
 * @todo Pass this through a proper theme function
 */
function wowtoon_feed_item_add_page() {
  $controller = entity_ui_controller('wowtoon_feed_item');
  return $controller->addPage();
}


/**
 * Displays the list of available avatar types for avatar creation.
 *
 * @ingroup themeable
 */
function theme_wowtoon_feed_item_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="wowtoon-feed-item-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer avatar types')) {
      $output = '<p>' . t('Avatar Entities cannot be added because you have not created any avatar types yet. Go to the <a href="@create-avatar-type">avatar type creation page</a> to add a new avatar type.', array('@create-avatar-type' => url('admin/structure/alter_ego_types/add'))) . '</p>';
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
function wowtoon_feed_item_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Content'), 'admin/content'),
    l(t('Avatars'), 'admin/content/avatars'),
  );

  drupal_set_breadcrumb($breadcrumb);
}



