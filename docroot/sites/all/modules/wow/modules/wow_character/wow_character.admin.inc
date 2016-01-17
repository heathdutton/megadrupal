<?php

/**
 * @file
 * Admin page callback file for the character module.
 */

/**
 * Menu callback; Character listing.
 */
function wow_character_admin() {

  $build['character_filter_form'] = drupal_get_form('wow_character_filter_form');
  $build['character_admin_account'] = drupal_get_form('wow_character_admin_account');

  return $build;
}

/**
 * Form builder; Return form for character administration filters.
 *
 * @ingroup forms
 * @see character_filter_form_submit()
 */
function wow_character_filter_form() {
  $session = isset($_SESSION['wow_character_overview_filter']) ? $_SESSION['wow_character_overview_filter'] : array();
  $filters = wow_character_filters();

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only characters where'),
    '#theme' => 'exposed_filters__characters',
  );
  foreach ($session as $filter) {
    list($type, $value) = $filter;
    $value = $filters[$type]['options'][$value];
    $t_args = array('%property' => $filters[$type]['title'], '%value' => $value);
    if ($i++) {
      $form['filters']['current'][] = array('#markup' => t('and where %property is %value', $t_args));
    }
    else {
      $form['filters']['current'][] = array('#markup' => t('%property is %value', $t_args));
    }
  }

  $form['filters']['status'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('clearfix')),
    '#prefix' => ($i ? '<div class="additional-filters">' . t('and where') . '</div>' : ''),
  );
  $form['filters']['status']['filters'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('filters')),
  );
  foreach ($filters as $key => $filter) {
    $form['filters']['status']['filters'][$key] = array(
      '#type' => 'select',
      '#options' => $filter['options'],
      '#title' => check_plain($filter['title']),
      '#default_value' => '[any]',
    );
  }

  $form['filters']['status']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['status']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => (count($session) ? t('Refine') : t('Filter')),
  );
  if (count($session)) {
    $form['filters']['status']['actions']['undo'] = array(
      '#type' => 'submit',
      '#value' => t('Undo'),
    );
    $form['filters']['status']['actions']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
    );
  }

  drupal_add_library('system', 'drupal.form');

  return $form;
}

/**
 * Form submission handler for wow_character_filter_form().
 */
function wow_character_filter_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];
  $filters = wow_character_filters();
  switch ($op) {
    case t('Filter'):
    case t('Refine'):
      // Apply every filter that has a choice selected other than 'any'.
      foreach ($filters as $filter => $options) {
        if (isset($form_state['values'][$filter]) && $form_state['values'][$filter] != '[any]') {
          $options = $filters[$filter]['options'];
          // Only accept valid selections offered on the dropdown, block bad input.
          if (isset($options[$form_state['values'][$filter]])) {
            $_SESSION['wow_character_overview_filter'][] = array($filter, $form_state['values'][$filter]);
          }
        }
      }
      break;

    case t('Undo'):
      array_pop($_SESSION['wow_character_overview_filter']);
      break;

    case t('Reset'):
      $_SESSION['wow_character_overview_filter'] = array();
      break;

    case t('Update'):
      return;
  }

  $form_state['redirect'] = 'admin/wow/characters';
  return;
}

/**
 * Form builder; Character administration page.
 *
 * @ingroup forms
 * @see character_admin_account_validate()
 * @see character_admin_account_submit()
 */
function wow_character_admin_account() {
  $guild_rank_module = module_exists('wow_guild_rank');

  $header = array(
    'name' => array('data' => t('Name'), 'field' => 'c.name', 'class' => array('sort-link')),
    'status' => array('data' => t('Status'), 'field' => 'c.status'),
    'username' => array('data' => t('Username'), 'field' => 'c.uid'),
    'class' => array('data' => t('Class'), 'field' => 'c.class'),
    'race' => array('data' => t('Race'), 'field' => 'c.race'),
    'level' => array('data' => t('Level'), 'field' => 'c.level'),
  );

  if ($guild_rank_module) {
    $header['rank'] = array('data' => t('Rank'), 'field' => 'c.rank');
  }

  $query = db_select('wow_characters', 'c');
  wow_character_build_filter_query($query);

  $count_query = clone $query;
  $count_query->addExpression('COUNT(c.cid)');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('c')
    ->limit(50)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $query->execute();

  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array('class' => array('container-inline')),
  );
  $options = array();
  foreach (module_invoke_all('wow_character_operations') as $operation => $array) {
    $options[$operation] = $array['label'];
  }
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => 'unblock',
  );
  $options = array();
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  $status = array(t('blocked'), t('active'));
  if ($guild_rank_module) {
    $ranks = wow_guild_ranks();
  }
  foreach ($result as $character) {
    $options[$character->cid] = array(
      'name' => array('data' => theme('wow_character_name', array('character' => $character)), 'class' => 'name'),
      'status' => $status[$character->status],
      'username' => theme('username', array('account' => user_load($character->uid))),
      'class' => theme('wow_character_class', array('character' => $character)),
      'race' => theme('wow_character_race', array('character' => $character)),
      'level' => $character->level,
    );
    if ($guild_rank_module && $character->gid == wow_guild_default('gid') && isset($ranks[$character->rank])) {
      $options[$character->cid]['rank'] = $ranks[$character->rank]->name;
    }
    else {
      $options[$character->cid]['rank'] = '';
    }
  }

  $form['characters'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No character available.'),
    '#attributes' => array('class' => array('table')),
  );
  $form['pager'] = array('#markup' => theme('pager'));

  return $form;
}

/**
 * Form submission handler for wow_character_admin_account().
 */
function wow_character_admin_account_submit($form, &$form_state) {
  $operations = module_invoke_all('wow_character_operations', $form, $form_state);
  $operation = $operations[$form_state['values']['operation']];
  // Filter out unchecked characters.
  $characters = array_filter($form_state['values']['characters']);
  if ($function = $operation['callback']) {
    // Add in callback arguments if present.
    if (isset($operation['callback arguments'])) {
      $args = array_merge(array($characters), $operation['callback arguments']);
    }
    else {
      $args = array($characters);
    }
    call_user_func_array($function, $args);

    drupal_set_message(t('The update has been performed.'));
  }
}

/**
 * Form validation handler for wow_character_admin_account().
 */
function wow_character_admin_account_validate($form, &$form_state) {
  $form_state['values']['characters'] = array_filter($form_state['values']['characters']);
  if (count($form_state['values']['characters']) == 0) {
    form_set_error('', t('No characters selected.'));
  }
}

/**
 * Form builder; Configure character settings for this site.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function wow_character_admin_settings() {

  // Roles and main settings.
  $form['roles_main'] = array(
    '#type' => 'fieldset',
    '#title' => t('Registration of main character and roles'),
  );
  $form['roles_main']['wow_character_set_main'] = array(
    '#type' => 'radios',
    '#title' => t('Who can set main character?'),
    '#default_value' => variable_get('wow_character_set_main', 1),
    '#options' => array(
      0 => t('Administrators only'),
      1 => t('Users'),
    ),
  );

  if (module_exists('wow_guild_rank')) {
    $guild_name = wow_guild_default('name');

    if (!empty($guild_name)) {
      $options = array('attributes' => array('title' => t("Site's default guild")));
      $link = l($guild_name, 'admin/config/wow/guild/default', $options);

      $form['roles_main']['wow_character_use_guild_ranks'] = array(
        '#type' => 'checkbox',
        '#title' => t('Use guild ranks to define main character'),
        '#default_value' => variable_get('wow_character_use_guild_ranks', 1),
        '#description' => t('When checked, bypass the settings above for !guild_link members. The status of main will be granted for the character having the highest rank in the guild.', array('!guild_link' => $link)),
      );
    }
  }

  // Character entity settings.
  module_load_include('inc', 'wow', 'wow.admin');
  $form['entity'] = wow_entity_settings('wow_character');

  return system_settings_form($form);
}

/**
 * List character administration filters that can be applied.
 */
function wow_character_filters() {
  // Regular filters.
  $filters = array();

  if (module_exists('wow_guild_rank')) {
    $ranks = array();
    foreach (wow_guild_ranks() as $key => $rank) {
      $ranks[$key] = $rank->name;
    }
    // The guild master rank is 0. Removes it to not conflict with anonymous rank.
    unset($ranks[0]);
    if (count($ranks)) {
      $filters['rank'] = array(
        'title' => t('rank'),
        'field' => 'c.rank',
        'options' => array(
          '[any]' => t('any'),
        ) + $ranks,
      );
    }
  }

  $filters['status'] = array(
    'title' => t('status'),
    'field' => 'c.status',
    'options' => array(
      '[any]' => t('any'),
      1 => t('active'),
      0 => t('blocked'),
    ),
  );

  $result = db_select('wow_characters', 'c')
    ->fields('c', array('region'))
    ->condition('uid', $GLOBALS['user']->uid)
    ->orderBy('isMain', 'DESC')
    ->execute()
    ->fetch();

  $races = wow_character_races($result ? $result->region : 'us');
  $filters['race'] = array(
    'title' => t('race'),
    'field' => 'c.race',
    'options' => array(
      '[any]' => t('any'),
    ) + array_map('_wow_character_name_callback', $races),
  );

  $classes = wow_character_classes($result ? $result->region : 'us');
  $filters['class'] = array(
    'title' => t('class'),
    'field' => 'c.class',
    'options' => array(
      '[any]' => t('any'),
    ) + array_map('_wow_character_name_callback', $classes),
  );

  return $filters;
}

function _wow_character_name_callback($item) {
  return $item->name;
}

/**
 * Extends a query object for character administration filters based on session.
 *
 * @param SelectQuery $query
 *   Query object that should be filtered.
 */
function wow_character_build_filter_query(SelectQuery $query) {
  $filters = wow_character_filters();
  // Extend Query with filter conditions.
  foreach (isset($_SESSION['wow_character_overview_filter']) ? $_SESSION['wow_character_overview_filter'] : array() as $filter) {
    list($key, $value) = $filter;
    $query->condition($filters[$key]['field'], $value);
  }
}

/**
 * Implements hook_wow_character_operations().
 */
function wow_character_wow_character_operations($form = array(), $form_state = array()) {
  $operations = array(
    'set main' => array(
      'label' => t('Set as main the selected characters'),
      'callback' => 'wow_character_operation_set_main',
    ),
    'unblock' => array(
      'label' => t('Unblock the selected characters'),
      'callback' => 'wow_character_operation_unblock',
    ),
    'block' => array(
      'label' => t('Block the selected characters'),
      'callback' => 'wow_character_operation_block',
    ),
  );

  return $operations;
}

/**
 * Callback function for admin mass set main characters.
 */
function wow_character_operation_set_main($characters) {

  foreach (wow_character_load_multiple($characters) as $character) {
    // Skip setting main character if they are already main.
    if ($character !== FALSE && $character->uid && wow_character_access('set main', $character)) {
      // For efficiency manually save the original character before applying
      // any changes.
      $character->original = clone $character;
      $character->isMain = 1;
      $character->save();
    }
  }
}

/**
 * Callback function for admin mass unblocking characters.
 */
function wow_character_operation_unblock($characters) {

  foreach (wow_character_load_multiple($characters) as $character) {
    // Skip unblocking character if they are already unblocked.
    if ($character !== FALSE && $character->uid && $character->status != 1) {
      // For efficiency manually save the original character before applying
      // any changes.
      $character->original = clone $character;
      $character->status = 1;
      $character->save();
    }
  }
}

/**
 * Callback function for admin mass blocking characters.
 */
function wow_character_operation_block($characters) {

  foreach (wow_character_load_multiple($characters) as $character) {
    // Skip unblocking character if they are already unblocked.
    if ($character !== FALSE && $character->status) {
      // For efficiency manually save the original character before applying
      // any changes.
      $character->original = clone $character;
      $character->status = 0;
      $character->save();
    }
  }
}
