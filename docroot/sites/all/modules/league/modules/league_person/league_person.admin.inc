<?php

/**
 * @file
 * LeaguePerson editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring instead to use
 * views. That offers more flexibility to change a UI that will, more often than not,
 * be end-user facing.
 */

/**
 * UI controller.
 */
class LeaguePersonUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {
    
    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => t('League Persons'),
      'description' => t('Access overview of all persons.'),
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access league_person overview page', 'access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
    );
    
    // Change the overview menu type for the list of league_persons.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    
    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => t('Add a league person'),
      'description' => t('Add a new league_person'),
      'page callback'  => 'league_person_add_page',
      'access callback'  => 'league_person_access',
      'access arguments' => array('create'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'league_person.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

    );
    
    // Add menu items to add each different type of entity.
    foreach (league_person_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'league_person_form_wrapper',
        'page arguments' => array(league_person_create(array('type' => $type->type))),
        'access callback' => 'league_person_access',
        'access arguments' => array('create', $type->type),
        'file' => 'league_person.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing league_person entities

    // Menu item for viewing league_persons
    $items['league_person/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'league_person_page_title',
      'title arguments' => array(1),
      'page callback' => 'league_person_page_view',
      'page arguments' => array(1),
      'access callback' => 'league_person_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    $items['league_person/' . $wildcard . '/view'] = array(
      'title' => 'View',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
    );
    $items['league_person/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'page callback' => 'league_person_form_wrapper',
      'page arguments' => array(1),
      'access callback' => 'league_person_access',
      'access arguments' => array('edit', 1),
      'type' => MENU_LOCAL_TASK,
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'league_person.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    
    $items['league_person/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'league_person_delete_form_wrapper',
      'page arguments' => array(1),
      'access callback' => 'league_person_access',
      'access arguments' => array('delete', 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'league_person.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );

    // Admin settings.
    $items['admin/league/settings/league_person'] = array(
      'title' => t('Person'),
      'description' => t('Settings for the League Person'),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('league_person_admin_form'),
      'access arguments' => array('administer league_persons'),
      'type' => MENU_LOCAL_TASK,
      'file' => 'league_person.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );

    $items['league_person/autocomplete'] = array(
      'title' => 'Autocomplete persons',
      'page callback' => 'league_person_autocomplete',
      'access arguments' => array('view league_persons'),
      'type' => MENU_CALLBACK,
    );
    
    return $items;
  }
  
  
  /**
   * Create the markup for the add LeaguePerson Entities page within the class
   * so it can easily be extended/overriden.
   */ 
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }

    return theme('league_person_add_list', array('content' => $content));
  }
}

/**
 * Administration form.
 *
 */
function league_person_admin_form($form, &$form_state) {
  $settings = variable_get('league_person_settings');

  $options['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('Person Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $options['general']['activate_path'] = array(
    '#type' => 'checkbox',
    '#title' => t('Activate path alias'),
    '#default_value' => $settings['general']['activate_path'],
    '#description' => t('Activate the generation of path aliases for persons'),
  );
  // TODO set a path alias per bundle
  $options['general']['path_alias'] = array(
    '#type' => 'textfield',
    '#title' => t('Path alias'),
    '#default_value' => $settings['general']['path_alias'],
    '#description' => t('The path structure for aliases, no trailling slashes'),
    '#states' => array(
      'visible' => array(
        ':input[name="league_person_settings[general][activate_path]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $options['general']['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('league_person_type'), // The token types that have specific context. Can be multiple token types like 'term' and/or 'user'
    '#global_types' => TRUE, // A boolean TRUE or FALSE whether to include 'global' context tokens like [current-user:*] or [site:*]. Defaults to TRUE.
    '#click_insert' => TRUE, // A boolean whether to include the 'Click this token to insert in into the the focused textfield' JavaScript functionality. Defaults to TRUE.
  );
  $options['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $options['advanced']['player_bundle'] = array(
    '#type' => 'select',
    '#title' => t('Player bundle'),
    '#options' => league_person_get_league_person_types(),
    '#default_value' => $settings['advanced']['player_bundle'],
    '#description' => t('Bundle holding player information'),
  );
  $options['advanced']['coach_bundle'] = array(
    '#type' => 'select',
    '#title' => t('Coach bundle'),
    '#options' => league_person_get_league_person_types(),
    '#default_value' => $settings['advanced']['coach_bundle'],
    '#description' => t('Bundle holding coach information'),
  );
  $options['advanced']['referee_bundle'] = array(
    '#type' => 'select',
    '#title' => t('Referee bundle'),
    '#options' => league_person_get_league_person_types(),
    '#default_value' => $settings['advanced']['referee_bundle'],
    '#description' => t('Bundle holding referee information'),
  );

  $options['#tree'] = TRUE;
  $form['league_person_settings'] = $options;

  return system_settings_form($form);

}

/**
 * Form callback wrapper: create or edit a league_person.
 *
 * @param $league_person
 *   The league_person object being edited by this form.
 *
 * @see league_person_edit_form()
 */
function league_person_form_wrapper($league_person) {
  // Add the breadcrumb for the form's location.
  league_person_set_breadcrumb();
  return drupal_get_form('league_person_edit_form', $league_person);
}


/**
 * Form callback wrapper: delete a league_person.
 *
 * @param $league_person
 *   The league_person object being edited by this form.
 *
 * @see league_person_edit_form()
 */
function league_person_delete_form_wrapper($league_person) {
  // Add the breadcrumb for the form's location.
  //league_person_set_breadcrumb();
  return drupal_get_form('league_person_delete_form', $league_person);
}


/**
 * Form callback: create or edit a league_person.
 *
 * @param $league_person
 *   The league_person object to edit or for a create form an empty league_person object
 *     with only a league_person type defined.
 */
function league_person_edit_form($form, &$form_state, $league_person) {
  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Person Name'),
    '#default_value' => isset($league_person->name) ? $league_person->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -15,
  );
  $form['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name'),
    '#default_value' => isset($league_person->first_name) ? $league_person->first_name : '',
    '#maxlength' => 255,
    '#required' => FALSE,
    '#weight' => -10,
  );
  $form['given_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Given Name'),
    '#default_value' => isset($league_person->given_name) ? $league_person->given_name : '',
    '#maxlength' => 255,
    '#required' => FALSE,
    '#weight' => -5,
  );
  $settings = variable_get('league_settings');
  $form['team'] = array(
    '#type' => 'select',
    '#title' => t('Team'),
    '#options' => league_get_nodes($settings['advanced']['team_bundle']),
    '#default_value' => isset($league_person->team) ? $league_person->team : 0,
    '#weight' => -4,
  );
  $form['body'] = array(
    '#type' => 'text_format',
    '#title' => t('Description'),
    '#default_value' => isset($league_person->body) ? $league_person->body : '',
    '#format' => isset($league_person->format) ? $league_person->format : NULL,
  );

  // Add the field related form elements.
  $form_state['league_person'] = $league_person;
  field_attach_form('league_person', $league_person, $form, $form_state);

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
    '#value' => t('Save Person'),
    '#submit' => $submit + array('league_person_edit_form_submit'),
  );
  
  if (!empty($league_person->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete Person'),
      '#suffix' => l(t('Cancel'), 'admin/content/league_persons'),
      '#submit' => $submit + array('league_person_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'league_person_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the league_person form
 */
function league_person_edit_form_validate(&$form, &$form_state) {
  $league_person = $form_state['league_person'];
  
  // Notify field widgets to validate their data.
  field_attach_form_validate('league_person', $league_person, $form, $form_state);
}


/**
 * Form API submit callback for the league_person form.
 * 
 * @todo remove hard-coded link
 */
function league_person_edit_form_submit(&$form, &$form_state) {
  global $user;  

  $league_person = entity_ui_controller('league_person')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the league_person and go back to the list of league_persons
  
  // Add in created and changed times.
  if ($league_person->is_new = isset($league_person->is_new) ? $league_person->is_new : 0){
    $league_person->created = time();
    $league_person->uid = $user->uid;
    $league_person->language = LANGUAGE_NONE;
  }
  $league_person->changed = time();
  $league_person->format = $form_state['values']['body']['format'];
  $league_person->body = $form_state['values']['body']['value'];
  
  if ($league_person->save()) {
    $settings = variable_get('league_person_settings');
    if ($settings['general']['activate_path']) {
      league_person_save_path($league_person);
    }
  }

  $form_state['redirect'] = 'admin/content/league_persons';
}

/**
 * Form API submit callback for the delete button.
 * 
 * @todo Remove hard-coded path
 */
function league_person_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'league_person/' . $form_state['league_person']->league_person_id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a league_person.
 *
 * @param $league_person
 *   The league_person to delete
 *
 * @see confirm_form()
 */
function league_person_delete_form($form, &$form_state, $league_person) {
  $form_state['league_person'] = $league_person;

  $form['#submit'][] = 'league_person_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete league_person %name?', array('%name' => $league_person->name)),
    'admin/content/league_persons/league_person',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );
  
  return $form;
}

/**
 * Submit callback for league_person_delete_form
 */
function league_person_delete_form_submit($form, &$form_state) {
  $league_person = $form_state['league_person'];

  league_person_delete($league_person);

  drupal_set_message(t('The league_person %name has been deleted.', array('%name' => $league_person->name)));
  watchdog('league_person', 'Deleted league_person %name.', array('%name' => $league_person->name));

  $form_state['redirect'] = 'admin/content/league_persons';
}



/**
 * Page to add LeaguePerson Entities.
 *
 * @todo Pass this through a proper theme function
 */
function league_person_add_page() {
  $controller = entity_ui_controller('league_person');
  return $controller->addPage();
}


/**
 * Displays the list of available league_person types for league_person creation.
 *
 * @ingroup themeable
 */
function theme_league_person_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="league_person-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer league_person types')) {
      $output = '<p>' . t('League Person Entities cannot be added because you have not created any league_person types yet. Go to the <a href="@create-league_person-type">league_person type creation page</a> to add a new league_person type.', array('@create-league_person-type' => url('admin/structure/league_person_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No league_person types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}

/**
 * Sets the breadcrumb for administrative league_person pages.
 */
function league_person_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Content'), 'admin/content'),
    l(t('League Persons'), 'admin/content/league_persons'),
  );

  drupal_set_breadcrumb($breadcrumb);
}

function league_person_save_path($league_person) {
  if (!function_exists('ctools_cleanstring')) {
    module_load_include('inc', 'ctools', 'includes/cleanstring');
  }
  $clean_settings = array(
    'clean slash' => FALSE,
    'ignore words' => array(),
    'separator' => '_', 
    'replacements' => array(),
    'transliterate' => TRUE,
    'reduce ascii' => FALSE,
    'max length' => FALSE,
    'lower case' => TRUE,
  );
  $clean_title = ctools_cleanstring($league_person->name, $clean_settings);
  $league_person_type = league_person_type_load($league_person->type);
  $uri = league_person_uri($league_person);
  $settings = variable_get('league_person_settings');
  $alias = ctools_cleanstring(token_replace($settings['general']['path_alias'], array('league_person_type' => $league_person_type)), $clean_settings);
  $path = array('source' => $uri['path'], 'alias' => strtolower($alias) . '/' . $clean_title);
//  if($old_path = path_load($path->source)) {
  path_save($path);
}

/**
* This is just a copy of user_autocomplete().
*
* It works simply by searching usernames (and of course in Drupal usernames
* are unique, so can be used for identifying a record.)
*
* The returned $matches array has
* * key: string which will be displayed once the autocomplete is selected
* * value: the value which will is displayed in the autocomplete pulldown.
*
* In the simplest cases (see user_autocomplete()) these are the same, and
* nothing needs to be done. However, more more complicated autocompletes
* require more work. Here we demonstrate the difference by displaying the UID
* along with the username in the dropdown.
*
* In the end, though, we'll be doing something with the value that ends up in
* the textfield, so it needs to uniquely identify the record we want to access.
* This is demonstrated in ajax_example_unique_autocomplete().
*
* @param string $string
* The string that will be searched.
*/
function league_person_autocomplete($string = "") {
  $matches = array();
  if ($string) {
    $result = db_select('league_person')
      ->fields('league_person', array('name', 'league_person_id'))
      ->condition('name', db_like($string) . '%', 'LIKE')
      ->range(0, 10)
      ->execute();
    foreach ($result as $person) {
    // In the simplest case (see user_autocomplete), the key and the value are
    // the same. Here we'll display the uid along with the username in the
    // dropdown.
      $matches[$person->league_person_id] = check_plain($person->name) . " (id=$person->league_person_id)";
    }
  }
  drupal_json_output($matches);
} 
