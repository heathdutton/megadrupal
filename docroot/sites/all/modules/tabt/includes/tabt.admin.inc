<?php
/**
 * @file
 * Adminpages for the TabT-module.
 */

/**
 * Implements hook_form() for the settings-page.
 */
function tabt_admin_form($form, &$form_state) {
  $form['tabt_note'] = array(
    '#markup' => t('<strong>Note:</strong> You may have to clear all caches after changing these settings.'),
  );
  $form['tabt_api'] = array(
    '#type' => 'textfield',
    '#title' => t('TabT-API'),
    '#default_value' => variable_get('tabt_api', 'http://api.vttl.be/0.7/?wsdl'),
    '#size' => 20,
    '#description' => t('The URL to the TabT-API. Note that this needs to be at least version !version.', array('!version' => TABT_REQ_VERSION)),
    '#required' => TRUE,
  );
  $form['tabt_club_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Club ID'),
    '#default_value' => variable_get('tabt_club_id', 'Vl-B000'),
    '#size' => 10,
    '#maxlength' => 10,
    '#description' => t('The ID of your club as listed on !tabtsite', array('!tabtsite' => l('http://' . variable_get('tabt_site', 'competitie.vttl.be') . '/index.php?menu=1', 'http://' . variable_get('tabt_site', 'competitie.vttl.be') . '/index.php?menu=1'))),
    '#required' => TRUE,
  );

  $form['tabt_club_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Club name'),
    '#default_value' => variable_get('tabt_club_name'),
    '#size' => 10,
    '#description' => t('The name of your club. This will be used in pagetitles.'),
    '#required' => TRUE,
  );

  $form['tabt_site'] = array(
    '#type' => 'textfield',
    '#title' => t('Competition site'),
    '#default_value' => variable_get('tabt_site', 'competitie.vttl.be'),
    '#field_prefix' => 'http://',
    '#field_suffix' => '/',
    '#size' => 10,
    '#description' => t('The url of the competition site.'),
  );

  $form['tabt_teamspage'] = array(
    '#type' => 'textfield',
    '#title' => t('TabT Teamspage link'),
    '#default_value' => variable_get('tabt_teamspage', 'teams'),
    '#size' => 40,
    '#description' => t('The path to the teamslisting.'),
    '#element_validate' => array('_tabt_validate_path'),
    '#required' => TRUE,
  );

  $form['tabt_teamspage_title'] = array(
    '#type' => 'textfield',
    '#title' => t('TabT Teamspage title'),
    '#default_value' => variable_get('tabt_teamspage_title', 'Teams @club'),
    '#size' => 40,
    '#description' => t('The pagetitle of teams overview page. <em>@club</em> will be replace by the clubname.'),
    '#required' => TRUE,
  );

  $form['tabt_locale'] = array(
    '#type' => 'textfield',
    '#title' => t('Locale'),
    '#default_value' => variable_get('tabt_locale'),
    '#size' => 10,
    '#description' => t('Set the locale to use. See !site for a list of locales.', array('!site', l('Oracle.com', 'http://dsc.sun.com/dev/gadc/tools/lc/lc.jsp'))),
  );

  $form['tabt_caching'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cache TabT data'),
    '#default_value' => variable_get('tabt_caching', 0),
    '#size' => 10,
    '#description' => t('Use caching for data gathered from the TabT-API and site.'),
  );

  return system_settings_form($form);
}

/**
 * Implements hook_form() for the credentialsettings-page.
 */
function tabt_admin_credentials_form($form, &$form_state) {
  $form['tabt_note'] = array(
    '#markup' => t('<strong>Note:</strong> You may have to clear all caches after changing these settings.'),
  );
  $form['tabt_credentials_user'] = array(
    '#type' => 'textfield',
    '#title' => t('TabT username'),
    '#default_value' => variable_get('tabt_credentials_user'),
    '#size' => 10,
    '#description' => t('Your username at !site', array('!site' => l('http://' . variable_get('tabt_site', 'competitie.vttl.be'), 'http://' . variable_get('tabt_site', 'competitie.vttl.be')))),
    '#required' => TRUE,
  );

  $form['tabt_credentials_password'] = array(
    '#type' => 'password',
    '#title' => t('TabT password'),
    '#default_value' => variable_get('tabt_credentials_password'),
    '#size' => 10,
    '#description' => t('Your password at !site', array('!site' => l('http://' . variable_get('tabt_site', 'competitie.vttl.be'), 'http://' . variable_get('tabt_site', 'competitie.vttl.be')))),
    '#required' => TRUE,
    '#element_validate' => array('_tabt_validate_credentials'),
  );

  return system_settings_form($form);
}

/**
 * Implements hook_form() for the teamsetup-page.
 */
function tabt_admin_teams_form($form, &$form_state) {

  $teams_var = variable_get('tabt_teams', array());
  $seasons = tabt_get_all_teams_by_season();
  $seasons = array_reverse($seasons, TRUE);
  $form['seasons'] = array('#tree' => TRUE);

  foreach ($seasons as $season => $teams) {
    if ($teams->TeamCount > 0) {
      $season_name = tabt_get_seasonname($season);
      $form['seasons'][$season] = array(
        '#type' => 'fieldset',
        '#title' => check_plain($season_name),
        '#collapsible' => TRUE,
        '#collapsed' => !tabt_is_current_season($season),
        '#tree' => TRUE,
      );
      $form['seasons'][$season]['table'] = array(
        '#theme' => 'tabt_admin_teams_teamlist',
        '#tree' => TRUE,
      );

      $form['seasons'][$season]['table']['teams'] = array();
      foreach ($teams->TeamEntries as $team_entry) {
        $default = '';
        $default_title = '';
        $row = array();
        if (isset($teams_var[$team_entry->TeamId])) {
          if (isset($teams_var[$team_entry->TeamId]->Alias)) {
            $default = $teams_var[$team_entry->TeamId]->Alias;
          }
          if (isset($teams_var[$team_entry->TeamId]->Title)) {
            $default_title = $teams_var[$team_entry->TeamId]->Title;
          }
        }

        $row['teamid'] = array(
          '#markup' => $team_entry->TeamId,
        );
        $row['teamletter'] = array(
          '#markup' => $team_entry->Team,
        );
        $row['division'] = array(
          '#markup' => $team_entry->DivisionName,
        );
        $row['alias'] = array(
          '#type' => 'textfield',
          '#size' => '5',
          '#default_value' => $default,
        );
        $row['title'] = array(
          '#type' => 'textfield',
          '#size' => '5',
          '#default_value' => $default_title,
        );
        $form['seasons'][$season]['table']['teams'][$team_entry->TeamId] = $row;
      }
    }
  }
  // Submit button.
  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Submission function for tabt_admin_teams_form.
 */
function tabt_admin_teams_form_submit($form, &$form_state) {
  $team_var = array();
  $team_var_inversed = array();
  $team_titles = array();
  foreach ($form_state['values']['seasons'] as $season => $seasonform) {
    $teams = $seasonform['table']['teams'];
    foreach ($teams as $id => $team) {
      if ($team['alias'] != '' || $team['title'] != '') {
        $team_object = new stdClass();
        if ($team['alias'] != '') {
          if (isset($team_var_inversed[$season][$team['alias']])) {
            drupal_set_message(t('You cannot set duplicate aliases in the same season.', 'warning'));
            return;
          }
          else {
            $team_object->Alias = $team['alias'];
            $team_object->Season = $season;
            $team_var_inversed[$season][$team['alias']] = $id;
          }
        }
        if ($team['title'] != '') {
          $team_object->Title = $team['title'];
        }
        $team_var[$id] = $team_object;
      }
      variable_del('tabt_teams');
      if (count($team_var) > 0) {
        variable_set('tabt_teams', $team_var);
      }
    }
  }
  drupal_set_message(t('The teams were updated.'), 'status');
}

/**
 * Validation function to test the TabT credentials entered in.
 */
function _tabt_validate_credentials($element, &$form_state, $form) {
  if (empty($element['#value']) || empty($form['tabt_credentials_user']['#value'])) {
    form_error($element, t('You must enter valid credentials.'));
  }
  if (!_tabt_are_valid_credentials(array('Account' => $form['tabt_credentials_user']['#value'], 'Password' => $element['#value']))) {
    form_error($element, t('The credentials you entered are not valid.'));
  }
}