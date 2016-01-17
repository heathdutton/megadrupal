<?php
/**
 * @file
 * Class definition
 *
 * Filters given view by search terms passed as arguments.
 *
 * @author bgunn
 *
 */
class namecards_handler_argument_browse_contacts_search_terms extends views_handler_argument_string {
  /**
   * Override function query() of parent class.
   * @see views_handler_argument_string::query()
   */
  function query($group_by = FALSE) {
    // Extract search terms.
    $search_terms = explode('+', $this->get_value());
    $conditional_statement = array();
    $placeholder = array();
    $full_content_table_alias = $this->query->ensure_table('field_data_namecards_namecard_full_content');
    $field = 'namecards_namecard_full_content_value';
    $num = count($search_terms);
    for ($i = 0; $i < $num; $i++) {
      // Add % wildcards to search terms.
      $placeholder[':search_term_' . $i] = '%' . $search_terms[$i] . '%';
      // Build the where clause.
      $conditional_statement[] = "UPPER($full_content_table_alias.$field) LIKE UPPER(:search_term_$i)";
    }
    // Merge conditional statement fragments into a single string.
    $conditional_statement = implode(' AND ', $conditional_statement);
    $group = $this->query->set_where_group();
    $this->query->add_where_expression($group, $conditional_statement, $placeholder);
  }
}