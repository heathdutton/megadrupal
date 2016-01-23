<?php

/**
 * @file
 * LeagueGame editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring instead to use
 * views. That offers more flexibility to change a UI that will, more often than not,
 * be end-user facing.
 */

/**
 * UI controller.
 */
class LeagueGameUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {
    
    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => t('League Games'),
      'description' => t('Access overview of all games.'),
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access league_game overview page', 'access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
    );
    
    // Change the overview menu type for the list of league_games.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    
    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => t('Add a league_game'),
      'description' => t('Add a new game'),
      'page callback'  => 'league_game_add_page',
      'access callback'  => 'league_game_access',
      'access arguments' => array('create', NULL),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'league_game.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

    );
    
    // Add menu items to add each different type of entity.
    foreach (league_game_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'league_game_form_wrapper',
        'page arguments' => array(league_game_create(array('type' => $type->type))),
        'access callback' => 'league_game_access',
        'access arguments' => array('create', $type->type),
        'file' => 'league_game.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing league_game entities

    // Menu item for viewing league_games
    $items['league_game/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'league_game_page_title',
      'title arguments' => array(1),
      'page callback' => 'league_game_page_view',
      'page arguments' => array(1),
      'access callback' => 'league_game_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    $items['league_game/' . $wildcard . '/view'] = array(
      'title' => 'View',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
    );
    $items['league_game/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'page callback' => 'league_game_form_wrapper',
      'page arguments' => array(1),
      'access callback' => 'league_game_access',
      'access arguments' => array('edit', 1),
      'type' => MENU_LOCAL_TASK,
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'league_game.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    $items['league_game/' . $wildcard . '/edit_result'] = array(
      'title' => t('Edit result'),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('league_game_form_result', 1),
      'access callback' => 'league_game_access',
      'access arguments' => array('edit result', 1),
      'type' => MENU_LOCAL_TASK,
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'league_game.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    
    $items['league_game/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'league_game_delete_form_wrapper',
      'page arguments' => array(1),
      'access callback' => 'league_game_access',
      'access arguments' => array('delete', 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'league_game.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );

    // Returns result per game
    $items['league_game/result'] = array(
      'page callback' => 'league_game_ajax_get_result',
      'type' => MENU_CALLBACK,
      'access arguments' => array('access content'),
      'page arguments' => array(2),
      'file' => 'league_game.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );

    /* Ajax returns. Needs work
    $items['league_game/status'] = array(
      'page callback' => 'league_game_update_status',
      'page arguments' => array(2, 3),
      'access callback' => 'league_game_access',
      'access arguments' => array('edit', 2),
      'type' => MENU_CALLBACK,
      'file' => 'league_game.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
*/
    // Admin settings.
    $items['admin/league/settings/league_game'] = array(
      'title' => t('Game'),
      'description' => t('Settings for the League Game'),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('league_game_admin'),
      'access arguments' => array('administer league_games'),
      'type' => MENU_LOCAL_TASK,
      'file' => 'league_game.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    
    return $items;
  }
  
  
  /**
   * Create the markup for the add LeagueGame Entities page within the class
   * so it can easily be extended/overriden.
   */ 
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }    
        
    return theme('league_game_add_list', array('content' => $content));
  }
  
}

/**
 * Implements hook_admin_paths().
 */
function league_game_admin_paths() {
  if (variable_get('node_admin_theme')) {
    $paths = array(
      'league_game/*/edit' => TRUE,
      'league_game/*/delete' => TRUE,
    );
    return $paths;
  }
}

/**
 * Form callback wrapper: create or edit a league_game.
 *
 * @param $league_game
 *   The league_game object being edited by this form.
 *
 * @see league_game_edit_form()
 */
function league_game_form_wrapper($league_game) {
  // Add the breadcrumb for the form's location.
  league_game_set_breadcrumb();
  return drupal_get_form('league_game_edit_form', $league_game);
}


/**
 * Form callback wrapper: delete a league_game.
 *
 * @param $league_game
 *   The league_game object being edited by this form.
 *
 * @see league_game_edit_form()
 */
function league_game_delete_form_wrapper($league_game) {
  // Add the breadcrumb for the form's location.
  //league_game_set_breadcrumb();
  return drupal_get_form('league_game_delete_form', $league_game);
}

/**
 * Administration form.
 *
 */
function league_game_admin($form, &$form_state) {
  $settings = variable_get('league_game_settings');

  $options['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('Game Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $options['general']['activate_path'] = array(
    '#type' => 'checkbox',
    '#title' => t('Activate path alias'),
    '#default_value' => $settings['general']['activate_path'],
    '#description' => t('Activate the generation of path aliases for games'),
  );
  $options['general']['path_alias'] = array(
    '#type' => 'textfield',
    '#title' => t('Path alias'),
    '#default_value' => $settings['general']['path_alias'],
    '#description' => t('The path structure for aliases, no trailling slashes'),
    '#states' => array(
      'visible' => array(
        ':input[name="league_game_settings[general][activate_path]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $options['general']['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('league_game_type'), // The token types that have specific context. Can be multiple token types like 'term' and/or 'user'
    '#global_types' => TRUE, // A boolean TRUE or FALSE whether to include 'global' context tokens like [current-user:*] or [site:*]. Defaults to TRUE.
    '#click_insert' => TRUE, // A boolean whether to include the 'Click this token to insert in into the the focused textfield' JavaScript functionality. Defaults to TRUE.
  );
  $options['ajax'] = array(
    '#type' => 'fieldset',
    '#title' => t('Ajax Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $options['ajax']['active'] = array(
    '#type' => 'checkbox',
    '#title' => t('Activate ajax refresh for game results'),
    '#default_value' => isset($settings['ajax']['active']) ? $settings['ajax']['active'] : 0,
    '#description' => t('Activate the ajax refresh over live games. It will only be activated if game is set as LIVE.'),
  );
  $options['ajax']['interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Ajax refresh interval'),
    '#default_value' => isset($settings['ajax']['interval']) ? $settings['ajax']['interval'] : 60000,
    '#description' => t('The time in ms for AJAX refresh'),
    '#states' => array(
      'visible' => array(
        ':input[name="league_game_settings[ajax][active]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $options['ajax']['cache'] = array(
    '#type' => 'textfield',
    '#title' => t('Ajax time for response in cache'),
    '#default_value' => isset($settings['ajax']['cache']) ? $settings['ajax']['cache'] : 20000,
    '#description' => t('The time in ms page response will be refresh'),
    '#states' => array(
      'visible' => array(
        ':input[name="league_game_settings[ajax][active]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $options['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $options['advanced']['game_bundle'] = array(
    '#type' => 'select',
    '#title' => t('Game bundle'),
    '#options' => league_game_get_league_game_types(),
    '#default_value' => $settings['advanced']['game_bundle'],
    '#description' => t('Bundle holding team information'),
  );
  $options['advanced']['stadium_bundle'] = array(
    '#type' => 'select',
    '#title' => t('Stadium bundle'),
    '#options' => league_get_nodes_types(),
    '#default_value' => $settings['advanced']['stadium_bundle'],
    '#description' => t('Bundle holding stadium information'),
  );
  $options['advanced']['matchday_bundle'] = array(
    '#type' => 'select',
    '#title' => t('Matchday bundle'),
    '#options' => league_get_nodes_types(),
    '#default_value' => $settings['advanced']['matchday_bundle'],
    '#description' => t('Bundle holding matchday information'),
  );

  $options['#tree'] = TRUE;
  $form['league_game_settings'] = $options;

  return system_settings_form($form);

}

/**
 * Form API validate callback for the league_game_admin form
 */
function league_game_admin_validate(&$form, &$form_state) {
  $values = $form_state['values']['league_game_settings'];
// Pending validation
//  if (!preg_match('#^(\w+/){1,2}\w+\.\w+$#',$values['general']['path_alias'])) {
//    form_set_error('league_game_settings][general][path_alias', t('Path must be a valid path'));
//  }
}

/**
 * Form callback: create or edit a league_game.
 *
 * @param $league_game
 *   The league_game object to edit or for a create form an empty league_game object
 *     with only a league_game type defined.
 */
function league_game_form_result($form, &$form_state, $league_game) {
  drupal_set_title(isset($league_game->name) ? $league_game->name : t('Edit result'));
  // Add the default field elements.
  $form['score_a'] = array(
    '#type' => 'textfield',
    '#title' => t('Goals %team', array('%team' => isset($league_game->team_a) ? league_get_node_title($league_game->team_a) : 'A')),
    '#default_value' => isset($league_game->score_a) ? $league_game->score_a : 0,
    '#maxlength' => 2,
    '#size' => 4,
    '#required' => TRUE,
    '#weight' => -25,
  );
  $form['score_b'] = array(
    '#type' => 'textfield',
    '#title' => t('Goals %team', array('%team' => isset($league_game->team_b) ? league_get_node_title($league_game->team_b) : 'B')),
    '#default_value' => isset($league_game->score_b) ? $league_game->score_b : 0,
    '#maxlength' => 2,
    '#size' => 4,
    '#required' => TRUE,
    '#weight' => -15,
  );
  $form['attendance'] = array(
    '#type' => 'textfield',
    '#title' => t('Attendance'),
    '#default_value' => isset($league_game->attendance) ? $league_game->attendance : 0,
    '#maxlength' => 6,
    '#size' => 8,
    '#required' => FALSE,
    '#weight' => -5,
  );
  $form['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#options' => league_game_get_status_type(),
    '#default_value' => isset($league_game->status) ? $league_game->status : 0,
    '#weight' => 10,
  );

  // Add the field related form elements.
  $form_state['league_game'] = $league_game;
//  field_attach_form('league_game', $league_game, $form, $form_state);

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
    '#value' => t('Save Game'),
    '#submit' => $submit + array('league_game_form_result_submit'),
  );

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'league_game_form_result_validate';
  return $form;
}

/**
 * Form API validate callback for the league_game result form
 */
function league_game_form_result_validate(&$form, &$form_state) {
  $values = $form_state['values'];
  if (!$values['status']) {
    if ($values['score_a'] + $values['score_b']) {
      form_set_error('status', t('You must set status to Live or Finished to save score. If Pending score mut be 0 - 0.'));
    }
  }
  if (!is_numeric($values['score_a'])) {
    form_set_error('score_a', t('Score must be an integer value'));
  } elseif (intval($values['score_a']) < 0) {
    form_set_error('score_a', t('Score must be positive value'));
  }
  if (!is_numeric($values['score_b'])) {
    form_set_error('score_b', t('Score must be an integer value'));
  } elseif (intval($values['score_b']) < 0) {
    form_set_error('score_b', t('Score must be positive value'));
  }
  if (!is_numeric($values['attendance'])) {
    form_set_error('attendance', t('Attendance must be an integer value'));
  } elseif (intval($values['attendance']) < 0) {
    form_set_error('attendance', t('Attendance must be positive value'));
  }
}

/**
 * Form API submit callback for the league_game result form.
 * 
 * @todo remove hard-coded link
 */
function league_game_form_result_submit(&$form, &$form_state) {
  global $user;  

  $league_game = entity_ui_controller('league_game')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the league_game and go back to the list of league_games
  
  $league_game->changed = time();
  drupal_alter('league_game_result_form_pre_save', $league_game);
  
  if ($league_game->save()) {
    league_game_save_next_game($league_game);
    drupal_alter('league_game_form_save', $league_game);
  }

  $form_state['redirect'] = 'admin/content/league_games';
}


/**
 * Form callback: create or edit a league_game.
 *
 * @param $league_game
 *   The league_game object to edit or for a create form an empty league_game object
 *     with only a league_game type defined.
 */
function league_game_edit_form($form, &$form_state, $league_game) {
  drupal_set_title(isset($league_game->name) ? $league_game->name : t('Add Game'));

  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Game Name'),
    '#default_value' => isset($league_game->name) ? $league_game->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -20,
  );
  $settings = variable_get('league_game_settings');
  $form['matchday'] = array(
    '#type' => 'select',
    '#title' => t('Matchday'),
    '#options' => league_get_nodes($settings['advanced']['matchday_bundle'], FALSE),
    '#default_value' => isset($league_game->matchday) ? $league_game->matchday : 0,
    '#weight' => -15,
  );
  $format = 'Y-m-d H:i';
  $form['date'] = array(
     '#type' => 'date_popup', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
     '#title' => t('Select a date'),
     '#default_value' => isset($league_game->date) ? date ($format, $league_game->date) : '2014-06-01 00:00:00',
     '#date_format' => $format,
     '#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
     '#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
     '#date_year_range' => '0:+1', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
     '#weight' => -10,
  );
  $form['stadium'] = array(
    '#type' => 'select',
    '#title' => t('Stadium'),
    '#options' => league_get_nodes($settings['advanced']['stadium_bundle']),
    '#default_value' => isset($league_game->stadium) ? $league_game->stadium : 0,
    '#weight' => -9,
  );
  $settings = variable_get('league_settings');
  $form['team_a'] = array(
    '#type' => 'select',
    '#title' => t('Team A'),
    '#options' => league_get_nodes($settings['advanced']['team_bundle']),
    '#default_value' => isset($league_game->team_a) ? $league_game->team_a : 0,
    '#weight' => -5,
  );
  $form['team_b'] = array(
    '#type' => 'select',
    '#title' => t('Team B'),
    '#options' => league_get_nodes($settings['advanced']['team_bundle']),
    '#default_value' => isset($league_game->team_b) ? $league_game->team_b : 0,
    '#weight' => -4,
  );
  $form['body'] = array(
    '#type' => 'text_format',
    '#title' => t('Description'),
    '#default_value' => isset($league_game->body) ? $league_game->body : '',
    '#format' => isset($league_game->format) ? $league_game->format : NULL,
  );
  $form['match_winner'] = array(
    '#type' => 'select',
    '#title' => t('Match winner'),
    '#empty_value' => 0,
    '#options' => league_game_get_all_titles(),
    '#default_value' => isset($league_game->match_winner) ? $league_game->match_winner : 0,
    '#weight' => 5,
  );
  $form['match_winner_pos'] = array(
    '#type' => 'select',
    '#title' => t('Match winner position'),
    '#options' => array(0 => t('Team A'), 1 => t('Team B')),
    '#default_value' => isset($league_game->match_winner_pos) ? $league_game->match_winner_pos : 0,
    '#weight' => 6,
  );
  $form['match_losser'] = array(
    '#type' => 'select',
    '#title' => t('Match looser'),
    '#empty_value' => 0,
    '#options' => league_game_get_all_titles(),
    '#default_value' => isset($league_game->match_losser) ? $league_game->match_losser : 0,
    '#weight' => 10,
  );
  $form['match_looser_pos'] = array(
    '#type' => 'select',
    '#title' => t('Match looser position'),
    '#options' => array(0 => t('Team A'), 1 => t('Team B')),
    '#default_value' => isset($league_game->match_looser_pos) ? $league_game->match_looser_pos : 0,
    '#weight' => 11,
  );
  // Add the field related form elements.
  $form_state['league_game'] = $league_game;
  field_attach_form('league_game', $league_game, $form, $form_state);

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
    '#value' => t('Save Game'),
    '#submit' => $submit + array('league_game_edit_form_submit'),
  );
  
  if (!empty($league_game->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete Game'),
      '#suffix' => l(t('Cancel'), 'admin/content/league_games'),
      '#submit' => $submit + array('league_game_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'league_game_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the league_game form
 */
function league_game_edit_form_validate(&$form, &$form_state) {
  $league_game = $form_state['league_game'];
  
  // Notify field widgets to validate their data.
  field_attach_form_validate('league_game', $league_game, $form, $form_state);
}


/**
 * Form API submit callback for the league_game form.
 * 
 * @todo remove hard-coded link
 */
function league_game_edit_form_submit(&$form, &$form_state) {
  global $user;  
  $league_game = entity_ui_controller('league_game')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the league_game and go back to the list of league_games
  
  // Add in created and changed times.
  if ($league_game->is_new = isset($league_game->is_new) ? $league_game->is_new : 0){
    $league_game->created = time();
    $league_game->uid = $user->uid;
    $league_game->language = LANGUAGE_NONE;
  }
  $league_game->date = strtotime($league_game->date);
  
  $league_game->changed = time();
  $league_game->format = $form_state['values']['body']['format'];
  $league_game->body = $form_state['values']['body']['value'];
  
  if ($league_game->save()) {
    $settings = variable_get('league_game_settings');
    if ($settings['general']['activate_path']) {
      league_game_save_path($league_game);
    }
    drupal_alter('league_game_form_save', $league_game);
  }

  $form_state['redirect'] = 'admin/content/league_games';
}

/**
 * Form API submit callback for the delete button.
 * 
 * @todo Remove hard-coded path
 */
function league_game_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'league_game/' . $form_state['league_game']->league_game_id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a league_game.
 *
 * @param $league_game
 *   The league_game to delete
 *
 * @see confirm_form()
 */
function league_game_delete_form($form, &$form_state, $league_game) {
  $form_state['league_game'] = $league_game;

  $form['#submit'][] = 'league_game_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete league_game %name?', array('%name' => $league_game->name)),
    'admin/content/league_games/league_game',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );
  
  return $form;
}

/**
 * Submit callback for league_game_delete_form
 */
function league_game_delete_form_submit($form, &$form_state) {
  $league_game = $form_state['league_game'];

  league_game_delete($league_game);

  drupal_set_message(t('The league_game %name has been deleted.', array('%name' => $league_game->name)));
  watchdog('league_game', 'Deleted league_game %name.', array('%name' => $league_game->name));

  $form_state['redirect'] = 'admin/content/league_games';
}



/**
 * Page to add LeagueGame Entities.
 *
 * @todo Pass this through a proper theme function
 */
function league_game_add_page() {
  $controller = entity_ui_controller('league_game');
  return $controller->addPage();
}


/**
 * Displays the list of available league_game types for league_game creation.
 *
 * @ingroup themeable
 */
function theme_league_game_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="league_game-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer league_game types')) {
      $output = '<p>' . t('LeagueGame Entities cannot be added because you have not created any league_game types yet. Go to the <a href="@create-league_game-type">league_game type creation page</a> to add a new league_game type.', array('@create-league_game-type' => url('admin/structure/league_game_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No league_game types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}

/**
 * Sets the breadcrumb for administrative league_game pages.
 */
function league_game_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Content'), 'admin/content'),
    l(t('League Games'), 'admin/content/league_games'),
  );

  drupal_set_breadcrumb($breadcrumb);
}

/**
 * Sets the path alias for league_game pages.
 */
function league_game_save_path($league_game) {
  if (!function_exists('ctools_cleanstring')) {
    module_load_include('inc', 'ctools', 'includes/cleanstring');
  }
  $clean_settings = array(
    'clean slash' => FALSE,
    'ignore words' => array(),
    'separator' => '_', 
    'replacements' => array(),
    'transliterate' => TRUE,
    'reduce ascii' => TRUE,
    'max length' => FALSE,
    'lower case' => TRUE,
  );
  $clean_title = ctools_cleanstring($league_game->name, $clean_settings);
  $league_game_type = league_game_type_load($league_game->type);
  $uri = league_game_uri($league_game);
  $settings = variable_get('league_game_settings');
  // Remove trailling slashes
  $alias = token_replace($settings['general']['path_alias'], array('league_game_type' => $league_game_type));
  $path = array('source' => $uri['path'], 'alias' => strtolower($alias) . '/' . $clean_title);
  path_save($path);
}

/**
 * Sets next game for looser and winner.
 */
function league_game_save_next_game($league_game) {
  if ( $league_game->status == 2 && $league_game->match_winner ) {
    if ( $game_winner = league_game_load($league_game->match_winner)) {
      if ( $league_game->match_winner_pos ) {
        $game_winner->team_b = $league_game->score_a > $league_game->score_b ? $league_game->team_a : $league_game->team_b;
      }
      else {
        $game_winner->team_a = $league_game->score_a > $league_game->score_b ? $league_game->team_a : $league_game->team_b;
      }
      $game_winner->changed = time();
      $game_winner->save();
    }
  }
  if ( $league_game->status == 2 && $league_game->match_losser ) {
    if ( $game_looser = league_game_load($league_game->match_losser)) {
      if ( $league_game->match_looser_pos ) {
        $game_looser->team_b = $league_game->score_a > $league_game->score_b ? $league_game->team_b : $league_game->team_a;
      }
      else {
        $game_looser->team_a = $league_game->score_a > $league_game->score_b ? $league_game->team_b : $league_game->team_a;
      }
      $game_looser->changed = time();
      $game_looser->save();
    }
  }
}

/**
 * Updates the status of a game.
 * 
 */
function league_game_update_status($gid, $status) {
  $league_game = league_game_load($gid);
  if ($league_game->status != $status  && array_key_exists($status, league_game_get_status_type())) {
    $league_game->changed = time();
    $league_game->status = $status;
    return $league_game->save();
  }
  else {
    return TRUE;
  }
}

/**
 * Callback function for AJAX request.
 *
 * Return goals per team game on json format
 * Uses menu_execute_active_handler to build page
 */

function league_game_ajax_get_result() {
  // Get the arguments from this fuction
  $args = func_get_args();
  $games = league_game_load_multiple($args);
  $result = array();
  foreach ($games as $game) {
    $result[$game->league_game_id] = array(
      'team_a' => $game->team_a,
      'team_b' => $game->team_b,
      'score_a' => $game->score_a,
      'score_b' => $game->score_b,
      'status' => $game->status,
    );
  }
  drupal_json_output($result);
}