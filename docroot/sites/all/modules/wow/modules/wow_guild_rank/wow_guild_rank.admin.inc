<?php

/**
 * @file
 * Admin page callback file for the guild_rank module.
 */

/**
 * Form builder; The guild ranks form.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function wow_guild_admin_ranks($form, &$form_state) {
  // Gather general information.
  $roles = array_map('check_plain', user_roles(TRUE));
  // Can't set authenticated role.
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  $ranks = wow_guild_ranks();

  $form['ranks'] = array(
    '#tree' => TRUE,
  );
  $order = 0;
  foreach ($ranks as $rid => $rank) {
    $form['ranks'][$rid]['#rank'] = (object) array(
      'rid' => $rid,
      'name' => $rank->name,
      'role' => $rank->role,
      'weight' => $order,
    );
    $form['ranks'][$rid]['#weight'] = $order;
    $form['ranks'][$rid]['weight'] = array(
      '#type' => 'textfield',
      '#title' => t('Weight for @title', array('@title' => $rank->name)),
      '#title_display' => 'invisible',
      '#size' => 4,
      '#default_value' => $order,
      '#attributes' => array('class' => array('rank-weight')),
    );
    $order++;
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('New rank name'),
    '#title_display' => 'invisible',
    '#size' => 12,
    '#maxlength' => 16,
    '#description' => t('Name'),
    '#prefix' => '<div class="name-input"><div class="add-new-placeholder">' . t('Add new rank') . '</div>',
    '#suffix' => '</div>',
  );
  $form['role'] = array(
    '#type' => 'select',
    '#title' => t('Role of new rank'),
    '#title_display' => 'invisible',
    '#options' => $roles,
    '#empty_option' => t('- Select a role -'),
    '#description' => t('Associated role for this rank.'),
  );
  $form['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add rank'),
    '#validate' => array('wow_guild_admin_rank_validate'),
    '#submit' => array('wow_guild_admin_rank_submit'),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save order'),
    '#submit' => array('wow_guild_admin_ranks_order_submit'),
  );

  return $form;
}

/**
 * Submit handler for the rank overview form. Update the rank weights.
 */
function wow_guild_admin_ranks_order_submit($form, &$form_state) {
  foreach ($form_state['values']['ranks'] as $rid => $rank_values) {
    $rank = $form['ranks'][$rid]['#rank'];
    $rank->weight = $rank_values['weight'];
    wow_guild_rank_save($rank);
  }
  drupal_set_message(t('The rank settings have been updated.'));
}

/**
 * Form to configure a single rank.
 *
 * @ingroup forms
 * @see guild_admin_rank_validate()
 * @see guild_admin_rank_submit()
 */
function wow_guild_admin_rank($form, $form_state, $rank) {
  $roles = array_map('check_plain', user_roles(TRUE));
  // Can't set authenticated role.
  unset($roles[DRUPAL_AUTHENTICATED_RID]);

  // Display the edit rank form.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Rank name'),
    '#default_value' => $rank->name,
    '#size' => 12,
    '#required' => TRUE,
    '#maxlength' => 16,
    '#description' => t('The name for this role. Example: "Officer", "Veteran", "Member".'),
  );
  $form['rid'] = array(
    '#type' => 'value',
    '#value' => $rank->rid,
  );
  $form['role'] = array(
    '#type' => 'select',
    '#title' => t('Role'),
    '#options' => $roles,
    '#empty_option' => t('- Select a role -'),
    '#default_value' => $rank->role,
    '#description' => t('Associated role for this rank.'),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save rank'),
  );
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete rank'),
    '#submit' => array('wow_guild_admin_rank_delete_submit'),
  );

  return $form;
}

/**
 * Form validation handler for the guild_admin_rank() form.
 */
function wow_guild_admin_rank_validate($form, $form_state) {
  if (!empty($form_state['values']['name'])) {
    if ($form_state['values']['op'] == t('Save rank')) {
      $rank = wow_guild_rank_load_by_name($form_state['values']['name']);
      if ($rank && $rank->rid != $form_state['values']['rid']) {
        form_set_error('name', t('The rank name %name already exists. Choose another rank name.', array('%name' => $form_state['values']['name'])));
      }
    }
    elseif ($form_state['values']['op'] == t('Add rank')) {
      if (wow_guild_rank_load_by_name($form_state['values']['name'])) {
        form_set_error('name', t('The rank name %name already exists. Choose another rank name.', array('%name' => $form_state['values']['name'])));
      }
    }
  }
  else {
    form_set_error('name', t('You must specify a valid rank name.'));
  }
}

/**
 * Form submit handler for the guild_admin_rank() form.
 */
function wow_guild_admin_rank_submit($form, $form_state) {
  $rank = (object) $form_state['values'];
  if ($form_state['values']['op'] == t('Save rank')) {
    wow_guild_rank_save($rank);
    drupal_set_message(t('The rank has been saved.'));
  }
  elseif ($form_state['values']['op'] == t('Add rank')) {
    wow_guild_rank_save($rank);
    drupal_set_message(t('The rank has been added.'));
  }
  $form_state['redirect'] = 'admin/config/wow/general/guild-ranks';
  return;
}

/**
 * Form submit handler for the guild_admin_rank() form.
 */
function wow_guild_admin_rank_delete_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/wow/general/guild-ranks/delete/' . $form_state['values']['rid'];
}

/**
 * Form to confirm rank delete operation.
 */
function wow_guild_admin_rank_delete_confirm($form, &$form_state, $rank) {
  $form['rid'] = array(
    '#type' => 'value',
    '#value' => $rank->rid,
  );
  return confirm_form($form, t('Are you sure you want to delete the rank %name ?', array('%name' => $rank->name)), 'admin/config/wow/general/guild-ranks', t('This action cannot be undone.'), t('Delete'));
}

/**
 * Form submission handler for guild_admin_rank_delete_confirm().
 */
function wow_guild_admin_rank_delete_confirm_submit($form, &$form_state) {
  wow_guild_rank_delete((int) $form_state['values']['rid']);
  drupal_set_message(t('The rank has been deleted.'));
  $form_state['redirect'] = 'admin/config/wow/general/guild-ranks';
}

/**
 * Returns HTML for the rank order and new rank form.
 *
 * @param array $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_wow_guild_admin_ranks($variables) {
  $form = $variables['form'];
  $roles = user_roles();

  $header = array(t('Name'), t('Weight'), t('Roles'), array('data' => t('Operations'), 'colspan' => 2));
  foreach (element_children($form['ranks']) as $weight) {
    $row = array();
    $row[] = check_plain($form['ranks'][$weight]['#rank']->name);
    $row[] = drupal_render($form['ranks'][$weight]['weight']);
    $row[] = !empty($form['ranks'][$weight]['#rank']->role) ? $roles[$form['ranks'][$weight]['#rank']->role] : '';
    $row[] = l(t('edit rank'), 'admin/config/wow/general/guild-ranks/edit/' . $weight);
    $row[] = l(t('delete rank'), 'admin/config/wow/general/guild-ranks/delete/' . $weight);

    $rows[] = array('data' => $row, 'class' => array('draggable'));
  }
  $rows[] = array('data' => array(drupal_render($form['name']), '', drupal_render($form['role']), drupal_render($form['add']), ''), 'colspan' => 6, 'class' => array('add-new'));

  drupal_add_css(drupal_get_path('module', 'wow_guild_rank') . '/wow_guild_rank.css');
  drupal_add_tabledrag('guild-ranks', 'order', 'sibling', 'rank-weight');

  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('rank-ui-overview'), 'id' => 'guild-ranks')));
  $output .= drupal_render_children($form);

  return $output;
}
