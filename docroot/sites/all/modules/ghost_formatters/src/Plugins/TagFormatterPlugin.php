<?php

/**
 * @file
 * Contains a TagFormatterPlugin
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at previousnext dot com dot au
 */

namespace Drupal\ghost_formatters\Plugins;

use Drupal\ghost\Core\Formatters\Plugins\BaseFormatterPlugin;
use Drupal\ghost\Core\Formatters\Plugins\FormatterPluginInterface;

$plugin = array(
  'name' => 'ghost_tag',
  'class' => '\Drupal\ghost_formatters\Plugins\TagFormatterPlugin',
  'title' => t('Tag Formatter'),
);

/**
 * Class TagFormatterPlugin
 * @package Drupal\formatters\Plugins\Formatters
 */
class TagFormatterPlugin extends BaseFormatterPlugin implements FormatterPluginInterface {

  /**
   * Get the machine name for the formatter.
   *
   * @return string
   *   The machine name for the formatter.
   */
  public function getMachineName() {
    return 'ghost_tag';
  }

  /**
   * Get info for a plugin.
   *
   * @return array
   *   The plugin's info.
   */
  public function info() {

    return array(
      'label' => t('Tag'),
      'field types' => array('text'),
      'settings'  => array(
        'element' => 'div',
        'link' => '',
      ),
    );
  }

  /**
   * Get a settings form.
   *
   * @param mixed $field
   *   The field structure being configured.
   * @param array $instance
   *   The instance structure being configured.
   * @param string $view_mode
   *   The view mode being configured.
   * @param array $form
   *   The (entire) configuration form array, which will usually have no use
   *   here.
   * @param array $form_state
   *   The form state of the (entire) configuration form.
   * @param array $settings
   *   (Optional) An array of settings.
   *
   * @return array
   *   The form elements.
   */
  public function settingsForm($field, $instance, $view_mode, $form, &$form_state, array $settings = array()) {
    $element = array();

    if (isset($settings['element']) && array_key_exists($settings['element'], $this->getAllowedTags())) {
      $element_setting = $settings['element'];
    }
    else {
      $element_setting = reset($this->getAllowedTags());
    }

    $element['element'] = array(
      '#type'           => 'select',
      '#title'          => t('Element'),
      '#description'    => t('Select the HTML element to wrap the field in'),
      '#default_value'  => $element_setting,
      '#options'        => $this->getAllowedTags(),
    );

    $link_types = $this->getLinkTypes($instance);

    $element['link'] = array(
      '#title' => t('Link field to'),
      '#type' => 'select',
      '#default_value' => $settings['link'],
      '#empty_option' => t('Nothing'),
      '#options' => $link_types,
    );

    return $element;
  }

  /**
   * Return a short summary for the current formatter settings of an instance.
   *
   * If an empty result is returned, the formatter is assumed to have no
   * configurable settings, and no UI will be provided to display a
   * settings form.
   *
   * @param mixed $field
   *   The field structure.
   * @param array $instance
   *   The instance structure.
   * @param string $view_mode
   *   The view mode for which a settings summary is requested.
   * @param array $settings
   *   The forms settings.
   *
   * @return NULL|string
   *   The summary or NULL
   */
  public function settingsSummary($field, $instance, $view_mode, $settings) {

    $summary[] = t('Wrapped in a @element element', array(
      '@element' => filter_xss_admin($settings['element']),
    ));

    $link_types = $this->getLinkTypes($instance);
    if (isset($link_types[$settings['link']])) {
      $summary[] = filter_xss_admin($link_types[$settings['link']]);
    }

    return implode('<br />', $summary);
  }

  /**
   * Callback for hook_field_formatter_view().
   *
   * Builds a renderable array for a field value.
   *
   * @param string $entity_type
   *   The type of entity.
   * @param object $entity
   *   The entity being displayed.
   * @param array $field
   *   The field structure.
   * @param array $instance
   *   The field instance.
   * @param string $langcode
   *   The language associated with items.
   * @param array $items
   *   Array of values for this field.
   * @param array $display
   *   The display settings to use, as found in the 'display' entry of
   *   instance definitions. The array notably contains the following keys and
   *   values:
   *   - type: The name of the formatter to use
   *   - settings: The array of formatter settings.
   *
   * @return array
   *   A renderable array for the $items, as an array of child elements
   *   keyed by numeric indexes starting from 0.
   */
  public function view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
    $element = array();

    foreach ($items as $delta => $item) {
      $value = field_filter_xss($item['value']);
      $uri = NULL;

      // Check if the formatter involves a link.
      $link = $display['settings']['link'];
      if ($link == 'content') {
        $uri = entity_uri($entity_type, $entity);
      }
      elseif ($link) {
        if (isset($entity->$link)) {
          // Support for field translations.
          $language = field_language($entity_type, $entity, $field['field_name']);
          $link_field = $entity->$link;

          if (isset($link_field[$language])) {
            $link_values = $link_field[$language];
          }
        }
      }

      // Handle multiple link with values.
      if (isset($link_values)) {
        if (isset($link_values[$delta]['url'])) {
          $uri = array(
            'path' => $link_values[$delta]['url'],
            'options' => array('attributes' => $link_values[$delta]['attributes']),
          );
        }
        // If there are more values than link values unset the link.
        else {
          $uri = NULL;
        }
      }

      if (isset($uri['path']) && !empty($uri['path'])) {
        $value = l($value, $uri['path'], $uri['options']);
      }

      $element[$delta] = array(
        '#theme' => 'ghost_tag',
        '#tag' => $display['settings']['element'],
        '#value' => $value,
      );
    }

    return $element;
  }

  /**
   * Get allowed tags.
   *
   * @return array
   *   An array of allowed tags.
   */
  protected function getAllowedTags() {

    $defaults = array('div', 'span', 'h1', 'h2', 'h3', 'h4', 'h5', 'h6');

    $allowed = variable_get('ghost_formatters_allowed_tags', $defaults);

    $final = array();
    if (!empty($allowed)) {
      foreach ($allowed as $tag) {
        $final[$tag] = $tag;
      }
    }

    return $final;
  }

  /**
   * Get link types.
   *
   * @return array
   *   An array of link types.
   */
  protected function getLinkTypes($instance) {
    $link_types = array(
      'content' => t('Content'),
    );
    // If the link module is installed, also allow any link fields to be used.
    foreach (field_info_fields() as $field_key => $field_info) {
      if ($field_info['type'] == 'link_field') {
        $field_instance = field_info_instance($instance['entity_type'], $field_info['field_name'], $instance['bundle']);
        if ($field_instance) {
          $link_types[$field_key] = "$field_instance[label] ($field_info[field_name])";
        }
      }
    }
    return $link_types;
  }
}
