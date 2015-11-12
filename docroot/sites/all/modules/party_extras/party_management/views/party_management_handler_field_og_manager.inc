<?php

/**
 * @file
 * Definition of clinks_party_handler_field_og_manager.
 * Based on core Views' views_handler_field_custom.inc.
 */

/**
 * A handler to provide a field for OG manager checkbox.
 */
class party_management_handler_field_og_manager extends views_handler_field {
  function query() {
    // do nothing -- to override the parent query.
  }

  function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
  }

  function render($values) {
    return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
  }

  function form_element_name() {
    // Make sure this value is unique for all the view fields
    return $this->options['id'];
  }

  function form_element_row_id($row_id) {
    // You could use values from $this->view->result[$row_id]
    // to provide complex row ids.
    return $row_id;
  }

  function views_form(&$form, &$form_state) {
    // The view is empty, abort.
    if (empty($this->view->result)) {
      return;
    }
    // This is not a group entity, abort.
    if (og_get_group('party', $this->view->args[0]) === FALSE) {
      return;
    }

    // Set form action to request_uri() explicitly. Otherwise it will be
    // something like 'admin/party/235/235' due to party dashboard's original
    // path.
    $form['#action'] = request_uri();

    $field_name = $this->form_element_name();
    $form[$field_name] = array(
      '#tree' => TRUE,
    );

    // At this point, the query has already been run, so we can access the results
    foreach ($this->view->result as $row_id => $row) {
      // Get assigned User from Party.
      ctools_include('context');
      $relationship = ctools_get_relationship('user_from_party');
      $party = party_load($row->pid);
      $context = ctools_context_create('entity:party', $party);
      $user_from_party = ctools_context_get_context_from_relationship($relationship, $context);

      // Store user object in the current row of the View so we can use reuse
      // it later in the submit handler.
      $this->view->result[$row_id]->party_user = $user_from_party;
      $form_element_row_id = $this->form_element_row_id($row_id);

      // Check if this Individual party has an associated Drupal user.
      if (!empty($user_from_party)) {
        $form[$field_name][$form_element_row_id] = array(
          '#type' => 'checkbox',
          '#title' => t('Editor'),
          '#default_value' => $this->is_editor($user_from_party->data),
          '#attributes' => array('class' => array('party-management-og-manager')),
        );
      }
      else {
        $form[$field_name][$form_element_row_id] = array(
          '#markup' => t('N/A'),
          '#attributes' => array('class' => array('party-management-og-manager')),
          '#prefix' => '<span title="' . t('No Drupal user is assigned to this individual.') .  '" style="cursor:help;">',
          '#suffix' => '</span>',
        );
      }
    }
  }

  // Submit handler of views_form().
  function views_form_submit($form, &$form_state) {
    $field_name = $this->form_element_name();
    // Get Group id for the Party we're on.
    $pid = $this->view->args[0];
    $group = og_get_group('party', $pid);

    foreach ($form_state['values'][$field_name] as $row_id => $value) {
      // Only take action if the checkbox has changed.
      if ($form[$field_name][$row_id]['#default_value'] == $value) {
        continue;
      }

      $account = $this->view->result[$row_id]->party_user->data;
      $party_label = $this->view->result[$row_id]->party_label;

      // Add a User to the OG group.
      if ($value === 1) {
        $values = array('entity' => $account);
        og_group($group->gid, $values);
        // Make this user a manager.
        og_role_grant($group->gid, $account->uid, PARTY_MANAGEMENT_OG_ADMIN_RID);
        drupal_set_message(t('The update has been performed on %party.', array('%party' => $party_label)));
      }
      // Remove a User from the OG group.
      elseif ($value === 0) {
        og_ungroup($group->gid, 'user', $account);
        drupal_set_message(t('The update has been performed on %party.', array('%party' => $party_label)));
      }
    }
  }

  // Check if a given user is member of the group and has the group admin role
  // as well.
  private function is_editor($account) {
    $pid = $this->view->args[0];
    $group = og_get_group('party', $pid);
    $is_member = og_is_member($group->gid, 'user', $account);
    $user_group_roles = og_get_user_roles($group->gid, $account->uid, FALSE);
    $is_admin = isset($user_group_roles[PARTY_MANAGEMENT_OG_ADMIN_RID]);

    if ($is_member && $is_admin) {
      return TRUE;
    }
    return FALSE;
  }
}

