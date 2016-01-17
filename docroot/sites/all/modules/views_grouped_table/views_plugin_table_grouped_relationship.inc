<?php
/**
 * @file
 * Contains the table grouped by relationship style plugin.
 */

/**
 * Style plugin to render each item as a row in a table.WRITEME
 *
 * @ingroup views_style_plugins
 */
class views_plugin_style_table_grouped_relationship extends views_plugin_style_table {

  /**
   * Contains the field aliases for the base ID fields of the relationships.
   * @var array
   */
  public $relationship_field_aliases;

  /**
   * Define default value for options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['grouped_relationships'] = array('default' => array());

    return $options;
  }

  /**
   * Options form.
   */
  function options_form(&$form, &$form_state) {
    $handlers = $this->display->handler->get_handlers('relationship');
    if (empty($handlers)) {
      $form['error_markup'] = array(
        '#markup' => '<div class="error messages">' . t('You need at least one relationship before you can configure your grouped table settings') . '</div>',
      );
      return;
    }

    // Get the main form from the parent plugin.
    parent::options_form($form, $form_state);

    $options = array();
    $options['base'] = t('Base table');
    foreach ($handlers as $id => $handler) {
      $options[$id] = $handler->options['label'];
    }

    $form['grouped_relationships'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#title' => t('Grouping relationships'),
      '#default_value' => $this->options['grouped_relationships'],
      '#description' => t('Select the relationships to use for grouping.'),
    );
  }

  /**
   * Add ourselves to the query.
   */
  function query() {
    parent::query();

    $handlers = $this->display->handler->get_handlers('relationship');

    // ARGH FormAPI checkbox values DIE DIE DIE.
    $grouped_relationships = array_filter($this->options['grouped_relationships']);
    //dsm($grouped_relationships, "options:grouped_relationships");

    // Ensure that each grouping relationship's base ID field is in the query,
    // and collect the field alias for this to use in rendering.
    $this->relationship_field_aliases = array();
    foreach ($grouped_relationships as $relationship_id) {
      // Handle the base table separately: it's already here, and the id is faked!
      if ($relationship_id == 'base') {
        $this->relationship_field_aliases['base'] = $this->view->base_field;
        continue;
      }

      $relationship = $handlers[$relationship_id];

      // Get the alias for the relationship's table.
      $table_alias = $relationship->alias;
      $field = $relationship->definition['base field'];

      // Store the array of field aliases, by relationship id.
      $this->relationship_field_aliases[$relationship_id] = $this->view->query->add_field($table_alias, $field);
    }
  }

}
