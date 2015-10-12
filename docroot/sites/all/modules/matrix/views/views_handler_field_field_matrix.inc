<?php
/**
 * @file
 * Matrix field handler for views.
 */

/**
 * A field that displays fields.
 */
class matrix_handler_field_field extends views_handler_field_field {

  function row_list() {
    $field = $this->field_info;

    $output = array(t('None'));

    if ($field['type'] == 'matrix_custom') {
      $settings = unserialize($field['settings']['settings']);
      foreach ($settings['rows'] as $row_id => $row) {
        $output[$row_id] = t('@title (Row @i)', array('@title' => $row['title'], '@i' => $row_id));
      }
      // The settings don't always come out in row order so we sort preserving
      // keys.
      ksort($output);
    }
    else {
      $output = drupal_map_assoc(range(1, $field['settings']['rows_count']));
    }
    return $output;
  }

  function col_list() {
    $field = $this->field_info;

    $output = array(t('None'));

    if ($field['type'] == 'matrix_custom') {
      $settings = unserialize($field['settings']['settings']);
      foreach ($settings['cols'] as $col_id => $col) {
        $output[$col_id] = t('@title (Col @i)', array('@title' => $col['title'], '@i' => $col_id));
      }
      // The settings don't always come out in row order so we sort preserving
      // keys.
      ksort($output);
    }
    else {
      $output = drupal_map_assoc(range(1, $field['settings']['cols_count']));
    }
    return $output;
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['row'] = array(
      'default' => 0,
    );
    $options['col'] = array(
      'default' => 0,
    );

    $options['show_row_headers'] = array(
      'default' => TRUE,
      'bool' => TRUE,
    );

    $options['show_col_headers'] = array(
      'default' => TRUE,
      'bool' => TRUE,
    );

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    unset($form['delta']);

    $form['message'] = array(
      '#type' => 'markup',
      '#markup' => '<p>' . t('To only show one value (rather than the entire matrix) choose the cell below.') . '</p>',
    );
    $form['row'] = array(
      '#title' => t('Row'),
      '#type' => 'select',
      '#options' => $this->row_list(),
      '#default_value' => $this->options['row'],
      '#description' => t('The row to select the data from'),
    );

    $form['show_row_headers'] = array(
      '#type' => 'checkbox',
      '#title' => t("Output the table's header row labels?"),
      '#default_value' => $this->options['show_row_headers'],
    );

    $form['col'] = array(
      '#title' => t('Column'),
      '#type' => 'select',
      '#options' => $this->col_list(),
      '#default_value' => $this->options['col'],
      '#description' => t('The col to select the data from'),
    );

    $form['show_col_headers'] = array(
      '#type' => 'checkbox',
      '#title' => t("Output the table's left column labels?"),
      '#default_value' => $this->options['show_col_headers'],
    );
  }

  /**
   * Render all or part of the matrix according to the settings.
   *
   * Matrix bundles the entire table into the first item/delta slot so this
   * function only get's called once and is passed a render array.
   *
   * Assuming you use the default formatter of course! Alternative formatters
   * aren't going to work here. So beware!
   */
  function render_item($count, $item) {

    $element = $item['rendered'];

    if ($this->options['row']) {
      foreach ($element['#rows'] as $x => $row) {
        if ($x != $this->options['row']) {
          unset($element['#rows'][$x]);
        }
      }
    }

    if ($this->options['col']) {
      foreach ($element['#rows'] as $x => $row) {
        foreach ($row as $y => $col) {
          if ($y && $y != $this->options['col']) {
            unset($element['#rows'][$x][$y]);
          }
        }
      }

      foreach ($element['#header'] as $y => $header) {
        if ($y && $y != $this->options['col']) {
          unset($element['#header'][$y]);
        }
      }
    }

    if (!$this->options['show_col_headers']) {
      foreach ($element['#rows'] as $x => $row) {
        unset($element['#rows'][$x][0]);
      }
    }

    if (!$this->options['show_row_headers']) {
      $element['#header'] = array();
    }

    return $element;

  }
}
