<?php
/**
 * @file
 * Views plugins used by Shared Content.
 */

/**
 * Shared Content assigned filter extension class.
 */
class SharedContentIndexAssigned extends SearchApiViewsHandlerFilterBoolean {

  // Do not handle operator.
  var $no_operator = TRUE;

  /**
   * Override query().
   */
  function query() {
    if ($this->value) {
      $no_assignments = TRUE;
      $index = sharedcontent_ui_selected_sharedcontent_index();
      $values = db_select('sharedcontent_assignment', 'a')
        ->fields('a', array('source'))
        ->condition('target', $index ? $index->uuid : FALSE)
        ->condition('status', 0)
        ->execute()->fetchCol();
      if (!empty($values)) {
        $filter = $this->query->createFilter('OR');
        // $filter will be NULL if there were errors in the query.
        if ($filter) {
          foreach ($values as $value) {
            $filter->condition($this->real_field, $value);
          }
          $this->query->filter($filter, $this->options['group']);
          $no_assignments = FALSE;
        }
      }
      if ($no_assignments) {
        $this->query->condition($this->real_field, FALSE, $this->operator, $this->options['group']);
      }
    }
  }
}

/**
 * Shared Content assigned filter extension class.
 */
class SharedContentIndexSelf extends SearchApiViewsHandlerFilterBoolean {

  // Do not handle operator.
  var $no_operator = TRUE;

  /**
   * Override query().
   */
  function query() {
    $index = sharedcontent_ui_selected_sharedcontent_index();
    if ($index) {
      $operator = $this->value ? '=' : '<>';
      $this->query->condition($this->real_field, $index->uuid, $operator, $this->options['group']);
    }
    elseif ($this->value) {
      $this->query->condition($this->real_field, NULL, '=', $this->options['group']);
    }
  }
}

/**
 * Default argument plugin to extract current selected node and to use it's tags
 * for fulltext search.
 */
class SharedContentIndexTags extends views_plugin_argument_default {

  /**
   * @todo doc
   */
  function get_argument() {
    $keywords = '';
    $index = sharedcontent_ui_selected_sharedcontent_index();
    if ($index) {
      $keywords .= $index->title;
      $keywords .= ' ' . $index->keywords;
      $keywords .= ' ' . $index->tags;
    }
    return $keywords;
  }

}

/**
 * Current selected index record.
 *
 * Default argument plugin to extract the id of the current selected
 * index record.
 */
class SharedContentIndexSelectedRecord extends views_plugin_argument_default {

  function get_argument() {
    $id = sharedcontent_ui_selected_sharedcontent_index_id();
    return $id ? $id : NULL;
  }
}
