<?php

/**
 * @file
 * Admin page callback file for the guild module.
 */

/**
 * Form builder; The guild information form.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function wow_guild_admin_default($form, &$form_state) {

  $realm = new stdClass();
  $realm->name = wow_guild_default('realm');
  $realm->region = wow_guild_default('region');

  $form['guild_information'] = array(
    '#type' => 'fieldset',
    '#title' => t('Guild details'),
  );
  $form['guild_information']['realm'] = array(
    '#type' => 'wow_realm',
    '#title' => t('Realm'),
  );
  $form['actions']['#type'] = 'actions';

  if (($guild = variable_get('wow_guild_default')) != NULL) {
    $form['guild_information']['realm']['#disabled'] = TRUE;
    $form['guild_information']['realm']['#default_value'] = wow_realm_format($realm);
    $form['guild_information']['name'] = array(
      '#type' => 'value',
      '#default_value' => $guild->name,
    );
    $form['guild_information']['guild'] = array(
      '#theme' => 'wow_admin_guild',
      '#guild' => wow_guild_load($guild->gid),
    );
    $form['guild_information']['site_default'] = array(
      '#type' => 'value',
      '#value' => $guild,
    );

    $form['guild_information']['#description'] = '<em>' . t('To delete guild data, you must disable and uninstall the WoW Guild module. This operation will erase all your data.') . '</em>';

    $form['actions']['refresh'] = array('#type' => 'submit', '#value' => t('Refresh'));

    $form['#submit'][] = 'wow_guild_admin_default_refresh';

    return $form;
  }
  else {
    $form['guild_information']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Guild name'),
      '#maxlength' => 24,
      '#size' => 30,
      '#default_value' => '',
      '#required' => TRUE,
    );

    $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

    return $form;
  }
}

/**
 * Validates the submitted guild-information form.
 */
function wow_guild_admin_default_validate($form, &$form_state) {
  $realm = $form_state['values']['realm'];
  $name = check_plain($form_state['values']['name']);

  if ($form_state['submitted'] && $name) {
    try {
      $form_state['temporary']['guild'] = wow_guild_fetch($realm->region, $realm->slug, $name, array('members', 'achievements'), array(), FALSE);
    }
    catch (WoWException $exception) {
      if ($exception->getCode() == 404) {
        $host = wow_api_info($realm->region, 'host');
        $url_guild = url("http://$host/wow/guild/$realm/$name/");
        $args = array('%name' => $name, '%realm' => $realm, '!url' => "<a href='$url_guild'>$url_guild</a>");
        form_set_error('name', t('The guild %name does not exist on %realm. You should first check that your guild exists at: !url.', $args));
      }
      else {
        watchdog_exception('wow_guild', $exception);
        form_set_error('guild_information', t('The website encountered an unexpected error. Please try again later.'));
      }
    }
  }
}

/**
 * Submits the validated guild-information form.
 */
function wow_guild_admin_default_submit($form, &$form_state) {
  $guild = $form_state['temporary']['guild'];

  $guild->gid = db_query('SELECT MAX(gid) FROM {wow_guild}')->fetchField() + 1;

  // Set as site default.
  $guild_default = new stdClass();
  $guild_default->region = $guild->region;
  $guild_default->gid = $guild->gid;
  $guild_default->name = $guild->name;
  $guild_default->realm = wow_realm_to_slug($guild->realm);

  variable_set('wow_guild_default', $guild_default);

  // Insert a new guild record.
  $guild->save();
}

/**
 * Refresh the guild-information form by fetching the guilde from service.
 */
function wow_guild_admin_default_refresh($form, &$form_state) {
  $guild = $form_state['temporary']['guild'];
  $guild->gid = wow_guild_default('gid');

  $guild->save();
  drupal_set_message(t('Guild data has been updated.'));
}

/**
 * Form builder; Configure guild settings for this site.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function wow_guild_admin_settings() {
  // Guild entity settings.
  module_load_include('inc', 'wow', 'wow.admin');
  $form['entity'] = wow_entity_settings('wow_guild');

  return system_settings_form($form);
}

/**
 * Returns the HTML for a guild information.
 *
 * @param array $variables
 *   An associative array containing:
 *   - guild: A render element representing a guild.
 *
 * @ingroup themeable
 */
function theme_wow_admin_guild($variables) {
  $guild = $variables['guild']['#guild'];

  $render = '<div class="name"><strong>' . t('Guild: @name', array('@name' => $guild->name)) . '</strong></div>';
  $render .= '<div class="side">' . t('Side: !side', array('!side' => wow_side($guild->side)->name)) . '</div>';
  $render .= '<div class="level">' . t('Level: @level', array('@level' => $guild->level)) . '</div>';
  $render .= '<div class="achievement">' . t('Achievement Points: @achievementPoints', array('@achievementPoints' => $guild->achievementPoints)) . '</div>';
  $render .= '<div class="last-modified"><em>' . t('Last modified: !lastModified', array('!lastModified' => format_date($guild->lastModified))) . '</em>';
  $render .= '<div class="last-fetched"><em>' . t('Last fetched: !lastFetched', array('!lastFetched' => format_date($guild->lastFetched))) . '</em>';

  return '<div id="wow-guild-' . $guild->gid . '" class="wow-guild">' . $render . '</div>';
}
