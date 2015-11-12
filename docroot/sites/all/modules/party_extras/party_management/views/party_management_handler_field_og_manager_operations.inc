<?php

/**
 * @file
 * Definition of views_handler_field_og_manager_operations
 * Provides Edit and Manage links for editors of a Party.
 * NOTE: requires the Party id field to be added to the view.
 */

/**
 * A handler to provide Edit and Manage links.
 *
 * @ingroup views_field_handlers
 */
class party_management_handler_field_og_manager_operations extends views_handler_field {
  function query() {
    // do nothing -- to override the parent query.
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['edit_label'] = array('default' => '', 'translatable' => TRUE);
    $options['edit_link'] = array('default' => 'party/[pid]/edit', 'translatable' => FALSE);
    $options['manage_label'] = array('default' => '', 'translatable' => TRUE);
    $options['manage_link'] = array('default' => 'party/[pid]/group', 'translatable' => FALSE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['edit_label'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display for edit link'),
      '#default_value' => $this->options['edit_label'],
    );

    $form['edit_link'] = array(
      '#type' => 'textfield',
      '#title' => t('The URL of the edit page.'),
      '#description' => t('Use [pid] for the party id'),
      '#default_value' => $this->options['edit_link'],
    );
    
    $form['manage_label'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display for manage link'),
      '#default_value' => $this->options['manage_label'],
    );
    
    $form['manage_link'] = array(
      '#type' => 'textfield',
      '#title' => t('The URL of the manage editors page.'),
      '#description' => t('Use [pid] for the party id'),
      '#default_value' => $this->options['manage_link'],
    );
  }

  function render($values) {
    global $user;
    $account = clone $user;
    $party = party_load($values->pid);
    $links = array();

    // Edit link.
    if (party_access('edit', $party) && $this->is_editor($account, $values->pid)) {
      $edit_label = !empty($this->options['edit_label']) ? $this->options['edit_label'] : t('Edit');
      $edit_link = !empty($this->options['edit_link']) ? $this->options['edit_link'] : 'party/[pid]/edit';    
      $links[] = l($edit_label, preg_replace('/\[pid\]/', $values->pid, $this->options['edit_link']));
    }

    // Manage link.
    if ($this->is_editor($account, $values->pid)) {
      $manage_label = !empty($this->options['manage_label']) ? $this->options['manage_label'] : t('Manage');
      $manage_link = !empty($this->options['manage_link']) ? $this->options['manage_link'] : 'party/[pid]/group';
      $links[] = l($manage_label, preg_replace('/\[pid\]/', $values->pid, $this->options['manage_link']), array('query' => array('destination' => $_GET['q'])));
    }
    if (!empty($links)) {
      return implode(' | ', $links);
    }
    return FALSE;
  }

  // Check if a given user is member of the group and has the group admin role
  // as well.
  private function is_editor($account, $pid) {
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
