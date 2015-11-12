<?php

/**
* System Settings Form
*/
function wowguild_admin_form($form, &$form_state) {

  $aid = alter_ego_get_main_aid();
  if ($aid) {
    $toon = avatar_load($aid);
  }

  $form['guild'] = array(
      '#type' => 'fieldset',
      '#title' => 'Guild Settings',
      '#collapsible' => TRUE,
      '#weight' => -1,
  );


  $form['guild']['wowguild_guild_name'] = array('#type' => 'textfield',
    '#title' => t('Guild Name'),
    '#default_value' => variable_get('wowguild_guild_name', isset($toon->guild_name)?$toon->guild_name:''),
    '#required' => TRUE,
    '#description' => t('The exact name of the guild you are hosting.')
  );
  if ($form['guild']['wowguild_guild_name']['#default_value'] != variable_get('wowguild_guild_name', '')) {
    $form['guild']['warning'] = array(
        '#weight' => -1,
      	'#markup' => '<br /><strong>' . t('These values have been autoloaded!  They will not take effect until you hit save.') . '</strong>'
    );
  }

  $options = array('' => '') + wowguild_get_realms(TRUE);
  $form['guild']['field_wowtoon_realm_und'] = array('#type' => 'select',
    '#title' => t('Guild Realm'),
    '#default_value' => variable_get('wowguild_guild_realm', isset($toon->fullrealm)?$toon->fullrealm:''),
    '#required' => TRUE,
    '#options' => $options,
    '#description' => t('The realm that this guild is on.  <strong>Important!</strong> Realms are translated by language!  Make sure you have selected your locale below if your realm is not showing up properly!')
  );

  $form['guild']['wowguild_force_update_all'] = array(
      '#type' => 'checkbox',
      '#title' => t('Mark all toons for update'),
      '#description' => t('Checking this box will tell the cron job to update all toons on the next sequential runs.')
  );

  $form['default language'] = array(
        '#type' => 'fieldset',
        '#title' => 'Default Language Settings',
        '#collapsible' => TRUE,
        '#description' => t('<em>Note</em>: Changes will not display until characters are updated.'),
        '#weight' => 0
  );
  $form['default language']['wowguild_default_language'] = array(
        '#type' => 'radios',
        '#title' => t('wowguild selector language'),
        '#description' => t('French, German and Russian languages are only available on EU realms.'),
        '#options' => array(
          'en' => t('English'),
          'pl' => t('Portuguese'),
          'fr' => t('French'),
          'es' => t('Spanish'),
          'de' => t('German'),
          'ru' => t('Russian'),
        ),
        '#default_value' => variable_get('wowguild_default_language', 'en'),
  );

  $form['guild_ranks'] = array(
    '#type' => 'fieldset',
    '#title' => 'Guild Ranks',
    '#collapsible' => TRUE,
    '#weight' => 1,
  );

  $rank_names = variable_get('wowguild_guild_rank_names', array('Guild Master', 'Rank 1', 'Rank 2', 'Rank 3', 'Rank 4', 'Rank 5', 'Rank 6', 'Rank 7', 'Rank 8', 'Rank 9'));
  for ($i=0;$i<10;$i++) {
    $form['guild_ranks']['wowguild_guild_rank_' . $i] = array('#type' => 'textfield',
      '#title' => t('Guild Rank #' . $i . ' Name:'),
      '#default_value' => $rank_names[$i],
      '#required' => FALSE
    );
  }

  $form['api'] = array(
      '#type' => 'fieldset',
      '#title' => 'Battle.net API',
      '#collapsible' => TRUE,
      '#weight' => 2,
  );
  $form['api']['description'] = array('#markup' => t('Having an API key allows you to make more requests to the armory.  See !url for more information.', array('!url' => l('Commumity Platform API Forum', 'http://us.battle.net/wow/en/forum/2626217/'))));

  $form['api']['wowguild_armory_public_key'] = array('#type' => 'textfield',
      '#title' => t('Public Key'),
      '#default_value' => variable_get('wowguild_armory_public_key', ''),
      '#required' => FALSE
  );
  $form['api']['wowguild_armory_private_key'] = array('#type' => 'textfield',
      '#title' => t('Private Key'),
      '#default_value' => variable_get('wowguild_armory_private_key', ''),
      '#required' => FALSE
  );
  $form['api']['wowguild_log_armory_hits'] = array('#type' => 'checkbox',
      '#title' => t('Log Armory Hits'),
      '#default_value' => variable_get('wowguild_log_armory_hits', 1),
      '#description' => t('A database record will be recorded each time we hit the armory. See !report to see results.', array('!report' => l('report', 'admin/reports/wowguild/armory_hits'))),
      '#required' => FALSE
  );

  $form['cron'] = array(
      '#type' => 'fieldset',
      '#title' => 'Cron Updates',
      '#collapsible' => TRUE,
      '#description' => t('Manage what is updated during cron runs.'),
  		'#weight' => 3,
  );
  $form['cron']['wowguild_cronupdate'] = array('#type' => 'checkbox',
      '#title' => t('Update Characters during Cron Run?'),
      '#default_value' => variable_get('wowguild_cronupdate', 1),
      '#description' => t('Checking this box will automatically update the oldest characters in the database and refresh data from the armory every cron run.')
  );
  $form['cron']['wowguild_updatedelta'] = array('#type' => 'textfield',
      '#title' => t('Update Threshhold'),
      '#default_value' => variable_get('wowguild_updatedelta', 12),
      '#required' => TRUE,
      '#description' => t('Update characters that are more than XX hours old.')
  );
  $form['cron']['wowguild_updatecount'] = array('#type' => 'textfield',
      '#title' => t('Update Count'),
      '#default_value' => variable_get('wowguild_updatecount', 20),
      '#required' => TRUE,
      '#description' => t('How many characters should we update each run?  Be aware that if this number is set too high, Blizzard may temporally block your IP from the armory.')
  );

  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
      '#weight' => 100
  );

  $form['#theme'] = 'wowguild_admin_form';
  return $form;
}
/**
 * Implementation of form_validate()
 * Validate that the guild exists in the database or armory.
 *
 */
function wowguild_admin_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  // Replace realm with a valid realm if it matches.  (Forces match in case).
  $valid_realm = wowguild_is_valid_realm($values['field_wowtoon_realm_und']);
  $name = $values['wowguild_guild_name'];

  if ($valid_realm === FALSE) {
    form_set_error('field_wowtoon_realm_und', 'Invalid Realm');
  }
  else {

    $guild = wowguild_load_by_realm_name($valid_realm, $name);

    // If guild does not exist in database.
    if ($guild === FALSE) {

      // Load from cache or database.
      $guild = wowguild_create();
      $guild->name = $name;
      $guild->field_wowtoon_realm[LANGUAGE_NONE][0]['value'] = $valid_realm;
      $validate = $guild->validateGuild();
// kpr($guild);
// print($validate);
// die();

      switch ($validate) {
        case WOWTOON_UPDATE_INVALID_PAGE:
          form_set_error('name', 'Armory Returned Invalid Page.');
          break;
        case WOWTOON_UPDATE_ARMORY_DOWN:
          form_set_error('name', 'Armory Appears to be down.');
          break;
        case WOWTOON_UPDATE_TOO_MANY_REQUESTS:
          form_set_error('name', 'This webpage has made too many requests to the Armory.');
          break;
        case WOWTOON_UPDATE_CHARACTER_NOT_AVAILABLE:
          form_set_error('name', 'Guild not found on server.');
          break;
        case WOWTOON_UPDATE_CHARACTER_NOT_MODIFIED:
        case WOWTOON_UPDATE_CHARACTER_AVAILABLE:
          $form_state['values']['wowguild_guild_name'] = $guild->name;
          $form_state['values']['field_wowtoon_realm_und'] = $valid_realm;
          $form_state['wowguild'] = $guild;
          break;
      }
    }
    else {
      // Pass guild to submit.
      $form_state['wowguild'] = $guild;
    }
  }

}

/**
 * Implementation of form_submit()
 * Save passed varaibles, and update the menu item $menu['guild/$guild->gid'] to the main menu.
 */
function wowguild_admin_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $guild = $form_state['wowguild'];

  wowguild_save($guild);

  // See if Guild has Changed
  if ($values['wowguild_guild_name'] != variable_get('wowguild_guild_name', -999) &&
      $values['field_wowtoon_realm_und'] != variable_get('wowguild_guild_realm', -999)) {
    cache_clear_all();
  }
  $guild->reloadMembers();
  //$guild->reloadActivityFeed();

  $old_language = variable_get('wowguild_default_language', 'en');
  if (!empty($values['wowguild_force_update_all']) || $old_language != $values['wowguild_default_language']) {
    variable_set('wowguild_all_realms_last_update', 0);
    db_update('wowtoon_cached_data')
      ->fields(array('update_attempted' => 0))
      ->execute();
    db_update('wowguild_cached_data')
      ->fields(array('update_attempted' => 0))
      ->execute();
    // TODO: Remove Feed items?
  }


  // Save Ranks
  $rank_names = array();
  for ($i=0;$i<10;$i++) {
    $rank_names[$i] = $values['wowguild_guild_rank_' . $i];
  }
  variable_set('wowguild_guild_rank_names', $rank_names);

  variable_set('wowguild_guild_gid', $guild->gid);
  variable_set('wowguild_guild_name', $values['wowguild_guild_name']);
  variable_set('wowguild_guild_realm', $values['field_wowtoon_realm_und']);

  variable_set('wowguild_default_language', $values['wowguild_default_language']);

  variable_set('wowguild_armory_public_key', $values['wowguild_armory_public_key']);
  variable_set('wowguild_armory_private_key', $values['wowguild_armory_private_key']);
  variable_set('wowguild_log_armory_hits', $values['wowguild_log_armory_hits']);

  variable_set('wowguild_cronupdate', $values['wowguild_cronupdate']);
  variable_set('wowguild_updatedelta', $values['wowguild_updatedelta']);
  variable_set('wowguild_updatecount', $values['wowguild_updatecount']);

  // Saved menu link id.
  $mlid = variable_get('wowguild_guild_mlid', 0);
  if ($mlid) {
    $ml = menu_link_load($mlid);
    $ml['link_path'] = 'guild/' . $guild->gid;
  }
  else {
    $ml = array(
        'menu_name' => 'main-menu',
        'module' => 'wowguild',
        'link_path' => 'guild/' . $guild->gid,
        'router_path' => 'guild/%',
        'link_title' => t('Guild Roster'),
        'weight' => 5,
    );
  }
  menu_link_save($ml);
  variable_set('wowguild_guild_mlid', $ml['mlid']);

  drupal_set_message(t('Saved configuration for !guild.', array('!guild' => l($guild->name, 'guild/' . $guild->gid))));
}
