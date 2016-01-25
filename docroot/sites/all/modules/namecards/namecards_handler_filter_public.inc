<?php
/**
 * @file
 * Class definition for a custom views handler
 *
 * Filters the view results based on whether a given contact is
 * public or privately viewable and whether or not a namecard
 * administrator is allowed to see private contacts.
 * @author bgunn
 *
 */
class namecards_handler_filter_public extends views_handler_filter {
  function admin_summary() { }

  function operator_form(&$form, &$form_state) { }

  /**
   * Override function query() of parent class.
   * @see views_handler_filter::query()
   */
  function query() {
    global $user;
    $uid = $user->uid;

    $private_table_alias = $this->query->ensure_table('field_data_namecards_namecard_public');
    $node_table_alias = $this->query->ensure_table('node');
    $conditional_statement = "$private_table_alias.namecards_namecard_public_value = 1 OR ($private_table_alias.namecards_namecard_public_value = 0 AND $node_table_alias.uid = :uid)";
    $private_contacts_visible = variable_get('namecards_privacy_settings_options', 0);
    if (user_access('administer namecards', $user) != TRUE || (user_access('administer namecards', $user) == TRUE  && variable_get('namecards_privacy_settings_options', 0) == 0)) {
      $group = $this->query->set_where_group();
      $this->query->add_where_expression($group, $conditional_statement, array(':uid' => $uid));
    }
  }
}