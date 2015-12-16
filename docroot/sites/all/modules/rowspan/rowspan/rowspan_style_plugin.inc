<?php
/**
 * @file
 * Provide an rowspan style plugin for Views. This file is autoloaded by views.
 */

/**
 * Implements of views_plugin_style().
 */
class RowspanStylePlugin extends views_plugin_style_table {

  /**
   * BLABLA
   *
   * @param array $records
   *   it's the number of rows
   *
   * @param array $groupings
   *   for group
   *
   * @param null $group_rendered
   *   if this parameter isn't explicitely set modify the output to
   *   be fully backward compatible to code before Views 7.x-3.0-rc2
   *
   * @return array
   *   it contain sequence of rows, and metadata "rowspan"
   */
  function renderGrouping($records, $groupings = array(), $group_rendered = NULL) {
    // This is for backward compability, when $groupings was a string containing
    // the ID of a single field.
    if (is_string($groupings)) {
      $rendered = $group_rendered === NULL ? TRUE : $group_rendered;
      $groupings = array(array('field' => $groupings, 'rendered' => $rendered));
    }

    // Make sure fields are rendered.
    $this->render_fields($this->view->result);
    $sets = array();
    $meta_data = array();
    if ($groupings) {
      foreach ($records as $index => $row) {
        // Iterate through configured grouping fields to determine the
        // hierarchically positioned set where the current row belongs to.
        // While iterating, parent groups, that do not exist yet, are added.
        $set = & $sets;
        foreach ($groupings as $info) {
          $field = $info['field'];
          $rendered = isset($info['rendered']) ? $info['rendered'] : $group_rendered;
          $rendered_strip = isset($info['rendered_strip']) ? $info['rendered_strip'] : FALSE;
          $grouping = '';
          $group_content = '';
          // Group on the rendered version of the field, not the raw.  That way,
          // we can control any special formatting of the grouping field through
          // the admin or theme layer or anywhere else we'd like.
          if (isset($this->view->field[$field])) {
            $group_content = $this->get_field($index, $field);
            if ($this->view->field[$field]->options['label']) {
              $group_content = $this->view->field[$field]->options['label'] . ': ' . $group_content;
            }
            if ($rendered) {
              $grouping = $group_content;
              if ($rendered_strip) {
                $group_content = $grouping = strip_tags(htmlspecialchars_decode($group_content));
              }
            }
            else {
              $grouping = $this->get_field_value($index, $field);
              // Not all field handlers return a scalar value,
              // e.g. views_handler_field_field.
              if (!is_scalar($grouping)) {
                $grouping = md5(serialize($grouping));
              }
            }
          }

          // Create the group if it does not exist yet.
          if (empty($set[$grouping])) {
            $set[$grouping]['group'] = $group_content;
            $set[$grouping]['rows'] = array();
            $set[$grouping]['field'] = $info['field'];
            $set[$grouping]['rowspan'] = 0;
            $meta_data[$index][$info['field']]['rowspan'] = & $set[$grouping]['rowspan'];
          }
          else {
            $meta_data[$index][$info['field']]['skip'] = TRUE;
          }
          $set[$grouping]['rowspan']++;
          // Move the set reference into the row set of the group we
          // just determined.
          $set = & $set[$grouping]['rows'];
        }
        // Add the row to the hierarchically positioned row set we
        // just determined.
        $set[$index] = $row;
      }
    }
    // Create a single group with an empty grouping field.
    $sets = array(
      'group' => '',
      'group_meta' => $meta_data,
      'rows' => $records,
    );
    // If this parameter isn't explicitely set modify the output to be fully
    // backward compatible to code before Views 7.x-3.0-rc2.
    // @TODO Remove this as soon as possible e.g. October 2020
    if ($group_rendered === NULL) {
      $old_style_sets = array();
      foreach ($sets as $group) {
        $old_style_sets[$group['group']] = $group['rows'];
      }
      $sets = $old_style_sets;
    }

    return $sets;
  }

  /**
   * Render the grouping sets.
   *
   * Plugins may override this method if they wish some other way of handling
   * grouping.
   *
   * @param array $sets
   *   Array containing the grouping sets to render.
   *
   * @return string
   *   Rendered output of given grouping sets.
   */
  function renderGroupingSets($sets) {
    $output = '';

    if ($this->uses_row_plugin()) {
      foreach ($sets['rows'] as $index => $row) {
        $this->view->row_index = $index;
        $set['rows'][$index] = $this->row_plugin->render($row);
      }
      $output .= theme($this->theme_functions(),
        array(
          'view' => $this->view,
          'options' => $this->options,
          'rows' => $sets['rows'],
          'title' => $sets['group'],
          'grouping_meta' => $sets['group_meta'],
        )
      );
    }

    unset($this->view->row_index);
    return $output;
  }

  /**
   * Render the display in this style.
   */
  function render() {
    if ($this->uses_row_plugin() && empty($this->row_plugin)) {
      debug('views_plugin_style_default: Missing row plugin');
      return;
    }

    // Group the rows according to the grouping instructions, if specified.
    $sets = $this->renderGrouping(
      $this->view->result,
      $this->options['grouping'],
      TRUE
    );

    return $this->renderGroupingSets($sets);
  }
}
