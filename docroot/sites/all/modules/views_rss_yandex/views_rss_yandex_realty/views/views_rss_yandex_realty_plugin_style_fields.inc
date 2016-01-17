<?php

/**
 * @file
 * Extends the views_rss_plugin_style_fields class to provide RSS for Yandex.Realty.
 */

class views_rss_yandex_realty_plugin_style_fields extends views_rss_plugin_style_fields {
  function option_definition() {
    $options = parent::option_definition();
    // Unset not needed options.
    foreach (array('namespaces', 'channel', 'item', 'feed_settings') as $key) {
      unset($options[$key]);
    }

    // Offer element defaults.
    $offer_elements = views_rss_get('yandex_realty_offer_elements');
    if (count($offer_elements)) {
      foreach ($offer_elements as $module => $module_offer_elements) {
        foreach (array_keys($module_offer_elements) as $element) {
          list($namespace, $element_name) = views_rss_extract_element_names($element, 'yandex_realty_offer');
          $options['offer'][$namespace][$module][$element_name] = array('default' => NULL);
        }
      }
    }

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    // Unset not needed form elements.
    foreach (array('namespaces', 'channel', 'item', 'feed_settings') as $key) {
      unset($form[$key]);
    }

    $handlers = $this->display->handler->get_handlers('field');
    // Field chooser.
    $field_names = array('' => '--');
    foreach ($handlers as $field => $handler) {
      if ($label = $handler->label()) {
        $field_names[$field] = $label;
      }
      else {
        $field_names[$field] = $handler->ui_name();
      }
    }

    $element_groups = views_rss_get('element_groups');
    // Offer elements settings.
    $offer_elements = views_rss_get('yandex_realty_offer_elements');
    if (count($offer_elements)) {
      foreach ($offer_elements as $module => $module_offer_elements) {
        foreach ($module_offer_elements as $element => $definition) {
          if (!isset($definition['configurable']) || $definition['configurable']) {
            list($namespace, $element_name) = views_rss_extract_element_names($element, 'yandex_realty_offer');
            // Add fieldset for namespace if not yet added.
            if (!isset($form['offer'][$namespace])) {
              $form['offer'][$namespace] = array(
                '#type' => 'fieldset',
                '#title' => t('Yandex.Realty offer elements : @namespace', array('@namespace' => $namespace)),
                '#description' => t('Select fields containing relevant values for &lt;offer&gt; elements in "@namespace" namespace. See <a href="@guide_url">Views RSS documentation</a> for more information.', array(
                  '@namespace' => $namespace,
                  '@guide_url' => url('http://drupal.org/node/1344136'),
                )),
                '#collapsible' => TRUE,
                '#collapsed' => FALSE,
              );
            }
            // Prepare form element.
            $default_value = NULL;
            if (!empty($this->options['offer'][$namespace][$module][$element_name])) {
              $default_value = $this->options['offer'][$namespace][$module][$element_name];
            }
            $form_item = array(
              '#type' => 'select',
              '#title' => filter_xss(isset($definition['title']) ? $definition['title'] : $element_name),
              '#description' => filter_xss(isset($definition['description']) ? $definition['description'] : NULL),
              '#options' => $field_names,
              '#default_value' => $default_value,
            );
            // Allow to overwrite default form element.
            if (!empty($definition['settings form'])) {
              $form_item = array_merge($form_item, $definition['settings form']);
              // Make sure that #options is an associative array.
              if (!empty($definition['settings form']['#options'])) {
                $form_item['#options'] = views_rss_map_assoc($definition['settings form']['#options']);
              }
            }
            // Allow to add into possible options.
            if (!empty($definition['settings form options']) && is_array($definition['settings form options'])) {
              $form_item['#options'] = array_merge($definition['settings form options'], $field_names);
            }
            // Add help link if provided.
            if (isset($definition['help']) && $definition['help']) {
              $form_item['#description'] .= ' ' . l('[?]', $definition['help'], array('attributes' => array('title' => t('Need more information?'))));
            }
            // Check if element should be displayed in a subgroup.
            if (isset($definition['group']) && $definition['group']) {
              // Add a subgroup to the form if it not yet added.
              if (!isset($form['offer'][$namespace][$module][$definition['group']])) {
                // Does module provide the group definition?
                $group_title = !empty($element_groups[$module][$definition['group']]['title']) ? $element_groups[$module][$definition['group']]['title'] : $definition['group'];
                $group_description = !empty($element_groups[$module][$definition['group']]['description']) ? $element_groups[$module][$definition['group']]['description'] : NULL;
                $form['offer'][$namespace][$module][$definition['group']] = array(
                  '#type' => 'fieldset',
                  '#title' => filter_xss($group_title),
                  '#description' => filter_xss($group_description),
                  '#collapsible' => TRUE,
                  '#collapsed' => TRUE,
                  // Do not include group into form element parents.
                  '#parents' => array('style_options', 'offer', $namespace, $module),
                );
              }
              $form['offer'][$namespace][$module][$definition['group']][$element_name] = $form_item;
            }
            // Display element normally (not within a subgroup).
            else {
              $form['offer'][$namespace][$module][$element_name] = $form_item;
            }
          }
        }
      }
    }
  }

  /**
   * Map views row result to an RSS item.
   */
  function map_rows($rows) {
    // Fields must be pre-rendered starting from version 2.3 of Views module.
    $rendered = $raw = array();
    $keys = array_keys($this->view->field);
    foreach ($rows as $count => $row) {
      $this->view->row_index = $count;
      foreach ($keys as $id) {
        $rendered[$count][$id] = $this->view->field[$id]->theme($row);
        // Also let's keep raw value for further processing.
        $field_name = 'field_' . $id;
        if (!empty($row->$field_name)) {
          $raw[$count][$id] = $row->$field_name;
        }
      }
    }

    // Rewrite view rows to XML item rows.
    $items = $raw_items = array();
    $offer_elements = views_rss_get('yandex_realty_offer_elements');
    foreach ($rendered as $id => $row) {
      $item = $raw_item = array();
      foreach ($offer_elements as $module => $module_item_elements) {
        foreach (array_keys($module_item_elements) as $element) {
          list($namespace, $element_name) = views_rss_extract_element_names($element, 'yandex_realty_offer');
          // Assign values for all elements, not only those defined in view settings.
          // If element value is not defined in view settings, let's just assign NULL.
          // It will not be passed to final theme function anyway during processing
          // taking place in template_preprocess_views_view_views_rss_yandex_realty().
          if (
            isset($this->options['offer'][$namespace][$module][$element_name])
            && isset($row[$this->options['offer'][$namespace][$module][$element_name]])
          ) {
            $item[$module][$element] = $row[$this->options['offer'][$namespace][$module][$element_name]];
          }
          elseif (isset($this->options['offer'][$namespace][$module][$element_name])
            && substr($this->options['offer'][$namespace][$module][$element_name], 0, 24) == 'views_rss_yandex_realty_') {
            $item[$module][$element] = $this->options['offer'][$namespace][$module][$element_name];
          }
          else {
            $item[$module][$element] = NULL;
          }
          // Keep raw values too.
          if (
            !empty($this->options['offer'][$namespace][$module][$element_name])
            && !empty($raw[$id][$this->options['offer'][$namespace][$module][$element_name]])
          ) {
            $raw_item[$module][$element] = $raw[$id][$this->options['offer'][$namespace][$module][$element_name]];
          }

        }
      }
      $items[$id] = $item;
      $raw_items[$id] = $raw_item;
    }

    $this->view->views_rss['raw_items'] = $raw_items;
    return $items;
  }
}
