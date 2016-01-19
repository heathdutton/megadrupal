<?php
/**
 * @file
 * Contains the table style plugin.
 */

/**
 * Style plugin to render each item as a row in a table.
 *
 * @ingroup views_style_plugins
 */
class views_mediatable_plugin_style_table extends views_plugin_style_table {

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {

    parent::options_form($form, $form_state);
    $form['#theme'] = 'views_mediatable_style_plugin_table';

    if (isset($form['error_markup'])) {
      return;
    }

    $form['description_markup']['#markup'] = '<div class="description form-item">' . t('test') . '</div>';

    $columns = $this->sanitize_columns($this->options['columns']);
    $field_names = $this->display->handler->get_field_labels();

    foreach ($columns as $field => $column) {
      $safe = str_replace(array('][', '_', ' '), '-', $field);
      // The $id of the column for dependency checking.
      $id = 'edit-style-options-columns-' . $safe;

      if (!empty($this->options['info'][$field]['responsive'])) {
        $media_option_default = $this->options['info'][$field]['responsive'];
      }

      else {
        $media_option_default = 'presist';
      }

      $form['info'][$field]['responsive'] = array(
        '#type' => 'select',
        '#options' => array(
          'persist' => 'Always Shown',
          'essential' => 'Shown with Option to Hide',
          'optional' => 'Hide on Smaller Resolutions with Option to Show',
        ),
        '#default_value' => $media_option_default,
        '#dependency' => array($id => array($field)),
      );
    }
  }

}
