<?php
/**
 * @file
 * A base class for the "reference type" plugin.
 */

/**
 * Class reference_table_formatter_base_type
 */
abstract class reference_table_formatter_base_type {

  protected $field = NULL;
  protected $instance = NULL;
  protected $settings = NULL;

  /**
   * Standard plugin constructor.
   *
   * @param $field
   *   A field definition for this plugin.
   * @param $instance
   *   A field instance.
   * @param $settings
   *   A settings array from the field API.
   */
  public function __construct($field, $instance, $settings) {
    $this->field = $field;
    $this->instance = $instance;
    $this->settings = $settings;
  }

  /**
   * Get a list of bundles which can be referenced by this field.
   *
   * @return array
   *   A list of bundles.
   */
  public abstract function get_bundles();

  /**
   * The entity types that is referenced by the field.
   *
   * @return string
   */
  public abstract function entity_name();

  /**
   * Get a list of properties off the target entity which are renderable.
   *
   * Make plugin authors explicity define which properties can be rendered
   * so that sensitive information isn't leaked accidently or without a
   * trusted role. A good example is the password on a user reference.
   *
   * @return array
   *   An array of properties on the target entity which can be rendered.
   */
  public abstract function renderable_properties();


  /**
   * Load the target entity from the given item.
   *
   * @param $item
   *   Content items from the field.
   * @return object
   *   A loaded entity.
   */
  public abstract function load_entity_from_item($item);

  /**
   * Get the fields and properties which can be rendered.
   *
   * @return array
   */
  function get_renderable_fields() {
    $properties = $this->get_entity_properties();
    $fields = $this->get_bundle_fields();
    return array_merge($properties, $fields);
  }

  /**
   * Get the properties off an entity that are available for rendering.
   *
   * @return array
   *   An array of name label, key values.
   */
  function get_entity_properties() {
    $entity_property_info = entity_get_property_info($this->entity_name());
    $renderable_properties = $this->renderable_properties();
    $keyed_properties = array();
    foreach ($entity_property_info['properties'] as $name => $info) {
      if (in_array($name, $renderable_properties)) {
        $keyed_properties[$name] = $this->get_entity_property_label($info);
      }
    }
    return $keyed_properties;
  }

  /**
   * Get the label of a property.
   *
   * @param $info
   *   The info array from the entity API.
   *
   * @return string
   *   The label to use for the given property.
   */
  function get_entity_property_label($info) {
    return $info['label'];
  }

  /**
   * Load all of the fields that can be displayed for a field and instance.
   *
   * @return array
   *   An array of name label, key values.
   */
  function get_bundle_fields() {
    $bundles = $this->get_bundles($this->field, $this->instance);
    $entity = $this->entity_name();
    $keyed_fields = array();
    foreach ($bundles as $bundle) {
      $fields = field_info_instances($entity, $bundle);
      foreach ($fields as $field_name => $info) {
        $keyed_fields[$field_name] = $info['label'];
      }
    }
    return $keyed_fields;
  }

  /**
   * Get view modes from the referenced entity.
   *
   * @return array
   *   An array of possible view modes to use for field rendering.
   */
  public function get_view_modes() {
    $entity_info = entity_get_info($this->entity_name());
    $bundles = array('default' => 'Default');
    foreach ($entity_info['view modes'] as $name => $info) {
      if ($info['custom settings']) {
        $bundles[$name] = $info['label'];
      }
    }
    return $bundles;
  }

  /**
   * Get a summary of the configured fields.
   *
   * @return string
   *   A translated string summarising the plugin settings.
   */
  public function get_summary() {
    $summaries = array();
    if ($this->show_all_fields()) {
      $summaries[] = t('Showing all fields.');
    }
    else {
      $field_list = $this->get_configured_field_list();
      $summaries[] = t('Showing @field_list fields.', array('@field_list' => implode($field_list, ', ')));
    }
    $summaries[] = $this->show_header() ? t('Showing a header.') : t('Not showing a header.');
    $summaries[] = $this->hide_empty() ? t('Hiding empty columns.') : t('Showing empty columns.');
    $summaries[] = t('Using the "@view_mode" view mode.', array('@view_mode' => $this->settings['view_mode']));
    return implode(' ', $summaries);
  }

  /**
   * Check if the header should be shown on the front-end.
   *
   * @return bool
   *   If the header should be shown.
   */
  public function show_header() {
    return $this->settings['show_header'] == '1';
  }

  /**
   * Should empty columns be hidden.
   *
   * @return bool
   *   If the empty columns should be squashed.
   */
  public function hide_empty() {
    return $this->settings['hide_empty'] == '1';
  }

  /**
   * The list of fields which will be shown to the user during render.
   *
   * @return array
   *   An array of field names.
   */
  public function get_configured_field_list() {
    $configured_fields = array();
    if ($this->show_all_fields()) {
      $configured_fields = array_keys($this->get_renderable_fields());
    }
    else {
      foreach ($this->settings['fields'] as $field_name => $value) {
        if ($field_name === $value) {
          $configured_fields[] = $field_name;
        }
      }
    }
    return $configured_fields;
  }

  /**
   * Render a table of content from the plugin.
   *
   * @param $items
   *   The content items to render.
   *
   * @return array
   *   A table array.
   */
  public function render_table($items) {
    // The fields that are renderable on this specific bundle.
    $renderable_fields = $this->get_renderable_fields();
    // The fields the admin has decided to render.
    $fields_to_render = $this->get_configured_field_list();
    foreach ($fields_to_render as $i => $field_name) {
      if (!isset($renderable_fields[$field_name])) {
        unset($fields_to_render[$i]);
      }
    }
    $table = array(
      '#theme' => 'table',
      '#rows' => array(),
    );
    // Optionally display a table header.
    if ($this->settings['show_header']) {
      $table['#header'] = array();
      foreach ($fields_to_render as $field_name) {
        $label = $renderable_fields[$field_name];
        $table['#header'][] = $label;
      }
    }
    // Render each field as a cell in a row.
    foreach ($items as $item) {
      $row = &$table['#rows'][];
      foreach ($fields_to_render as $field_name) {
        $loaded_item = $this->load_entity_from_item($item);
        if ($this->is_field($field_name)) {
          if (field_get_items($this->entity_name(), $loaded_item, $field_name)) {
            $renderable_field = field_view_field($this->entity_name(), $loaded_item, $field_name, $this->settings['view_mode']);
            $renderable_field['#label_display'] = 'hidden';
          }
          else {
            $renderable_field = array();
          }
        }
        else {
          $renderable_field = array('#markup' => $loaded_item->{$field_name});
        }
        $row[] = render($renderable_field);
      }
    }
    if ($this->hide_empty()) {
      $this->hide_empty_columns($table);
    }
    return $table;
  }

  /**
   * Hide columns in a table which are empty.
   *
   * @param $table
   *   A renderable table array.
   */
  public function hide_empty_columns(&$table) {
    $rows = &$table['#rows'];
    $display_columns = array_fill(0, count($rows[0]), FALSE);
    foreach ($rows as $row) {
      foreach ($row as $column_index => $column) {
        if (!empty($column)) {
          $display_columns[$column_index] = TRUE;
        }
      }
    }
    foreach ($display_columns as $index => $display_column) {
      if ($display_column === FALSE) {
        unset($table['#header'][$index]);
        foreach ($rows as &$row) {
          unset($row[$index]);
        }
      }
    }
  }

  /**
   * Check if a key is a property or field.
   *
   * @param $field_name
   *   A machine name of a field.
   *
   * @return bool
   *   If the given key is a field.
   */
  public function is_field($field_name) {
    $field_list = $this->get_bundle_fields();
    return isset($field_list[$field_name]);
  }


  /**
   * Check if the user has left the field checkboxes blank, rendering them all.
   *
   * @return bool
   *   Check if we should show all of the fields.
   */
  public function show_all_fields() {
    return count($this->settings['fields']) == 0 || (count(array_unique($this->settings['fields'])) === 1 && reset($this->settings['fields']) == '0');
  }
}
