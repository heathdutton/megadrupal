<?php

/**
 * Filter by language.
 *
 * @ingroup views_filter_handlers
 */
class language_hierarchy_views_handler_filter_node_language extends language_hierarchy_views_handler_filter_language {

  function post_execute(&$values) {
    // Remove duplicated translations.
    foreach ($values as $key => $row) {
      $langcode = $row->{$this->table_alias . '_language'};

      // Language of node is the same as selected in filter.
      if (in_array($langcode, $this->value)) {
        continue;
      }

      $parents = array_keys(language_hierarchy_get_ancestors($langcode));

      foreach ($values as $parent_key => $parent_row) {
        $parent_langcode = $parent_row->{$this->table_alias . '_language'};
        if ($langcode != $parent_langcode && in_array($parent_langcode, $parents)) {
          // Remove duplicated paren translation.
          unset($values[$parent_key]);
        }
      }
    }
  }

  function op_simple() {
    parent::op_simple();

    // Add fields required to remove duplicated parent translations.
    $this->query->add_field($this->table_alias, 'tnid');
    $this->query->add_field($this->table_alias, 'language');
  }

}
