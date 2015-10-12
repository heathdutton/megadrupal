<?php

/**
 * @file
 * Widgets for facets rendered as UI sliders with Min/Max respect.
 */

/**
 * Widget that renders facets as a UI slider.
 */
class SearchApiSwatchesWidgetSwatches extends FacetapiWidgetLinks {

  /**
   * Recursive function that converts the render array into an array that can be
   * passed to theme_item_list().
   *
   * @param array $build
   *   The facet's render array.
   *
   * @return array
   *   The "items" parameter for theme_item_list().
   */
  function buildListItems($build) {
    $settings = $this->settings->settings;

    // Initializes links attributes, adds rel="nofollow" if configured.
    $attributes = ($settings['nofollow']) ? array('rel' => 'nofollow') : array();
    $attributes += array('class' => $this->getItemClasses());

    // Builds rows.
    $items = array();
    foreach ($build as $value => $item) {
      $row = array('class' => array());
      $path_active = '';
      $path = '';
      // Load our color swatches.
      if (!empty($item['#indexed_value']) && $entity = entity_load($this->facet['map options']['field']['entity_type'], array($item['#indexed_value']))) {
        // only process swatches for entities that have an image
        $entity = reset($entity);
        if (isset($entity->{$settings['swatch_field']}) && isset($entity->{$settings['swatch_field']}['und'][0]['uri'])) {
          $style = ($item['#active'] == 0) ? $settings['swatch_imagestyle'] : $settings['swatch_imagestyle_select'];
          $path = file_create_url(image_style_url($style, $entity->{$settings['swatch_field']}['und'][0]['uri']));
          $path_active = file_create_url(image_style_url($settings['swatch_imagestyle_select'], $entity->{$settings['swatch_field']}['und'][0]['uri']));
          $swatch = theme('image', array(
            'path' => ($item['#active'] == 0) ? $path : $path_active,
            'alt' => $item['#markup'],
            'title' => $item['#markup'] . " (" . $item['#count'] . ")",
            'attributes' => array(
              'class' => array('facetapi-search-api-swatches-image'),
            ),
          ));
          if (($settings['swatch_showtext'] > 0) || ($path_active = '')) {
            $swatch .= '<span class="facetapi-search-api-swatches-text">' . $item['#markup'] . '</span>';
          }
        }
        elseif ($settings['swatch_shownoimage'] > 0) {
          $swatch = '<span class="facetapi-search-api-swatches-text">' . $item['#markup'] . '</span>';
        }
        // Initializes variables passed to theme hook.
        if (($settings['swatch_shownoimage'])  || (($settings['swatch_shownoimage'] == 0) && ($entity->{$settings['swatch_field']}['und'][0]['uri']))) {
          $attributes['onmouseover'] = "this.children[0].src='" . $path_active . "'";
          $attributes['onmouseout'] = "this.children[0].src='" . $path ."'";
          $variables = array(
            'text' => $swatch,
            'path' => $item['#path'],
            'facet' => $this->settings->facet,
            'options' => array(
              'attributes' => $attributes,
              'html' => TRUE,
              'query' => $item['#query'],
            ),
          );

          if ($settings['swatch_showtext'] > 0 ) {
            $variables['count'] = $item['#count'];
          }
          // Adds the facetapi-zero-results class to items that have no results.
          if (!$item['#count']) {
            $variables['options']['attributes']['class'][] = 'facetapi-zero-results';
          }

          // If the item has no children, it is a leaf.
          if (empty($item['#item_children'])) {
            $row['class'][] = 'leaf';
          }
          else {
            // If the item is active or the "show_expanded" setting is selected,
            // show this item as expanded so we see its children.
            if ($item['#active'] || !empty($settings['show_expanded'])) {
              $row['class'][] = 'expanded';
              $row['children'] = $this->buildListItems($item['#item_children']);
            }
            else {
              $row['class'][] = 'collapsed';
            }
          }

          // Gets theme hook, adds last minute classes.
          $class = ($item['#active']) ? 'facetapi-active' : 'facetapi-inactive';
          $variables['options']['attributes']['class'][] = $class;

          // Themes the link, adds row to items.
          $row['data'] = theme($item['#theme'], $variables);
          $items[] = $row;
        } // has image & show without images
      }
    }

    return $items;
  }

  /**
   * Adds the swatch field setting.
   */
  function settingsForm(&$form, &$form_state) {
    parent::settingsForm($form, $form_state);

    $form['widget']['widget_settings']['links'][$this->id]['swatch'] = array(
      '#type' => 'fieldset',
      '#title' => t('Swatch Settings'),
      '#description' => t('Swatch Settings.'),
      '#states' => array(
        'visible' => array(
          'select[name="widget"]' => array('value' => $this->id),
        ),
      ),
    );

    if ($form['#facetapi']['facet']['field api name']) {
      $field_info = field_info_field($form['#facetapi']['facet']['field api name']);
    }
    else {
      // Ok, exception time. The exception we came across is when field_api_name == false
      // there is some information in map_options['field']['key'] but we are going to do
      // string processing that is probably not safe.
      if ($key = $form['#facetapi']['facet']['map options']['field']['key']) {
        if ($keys = explode(":", $key)) {
          $count = count($keys);
          if ($count > 1) {
            // take the last but one, where the last item is the attribute of the field that we are looking for
            $fieldname = $keys[$count -2 ];
            $field_info = field_info_field($fieldname);
          }
        }
      }
    }
    
    if (isset($form['#facetapi']['facet']['map options']['field']['entity_type'])) {
      if ($form['#facetapi']['facet']['map options']['field']['entity_type'] == 'taxonomy') {
        if (isset($field_info['settings']['allowed_values'])) {
          $entity_fields = field_info_instances($form['#facetapi']['facet']['field type'], $field_info['settings']['allowed_values'][0]['vocabulary']);
        }
      }
      else {
        $entity_types = field_info_instances($form['#facetapi']['facet']['map options']['field']['entity_type']);
        $entity_fields = Array();
        foreach($entity_types as $entity_type) {
          $entity_fields = array_merge($entity_fields, $entity_type);
        }
      }
    }
    if (!isset($entity_fields)) {
      $form['widget']['widget_settings']['links'][$this->id]['swatch']['text'] = array(
        '#markup' => t('This facet has no images fields. You can not use swatches.'),
      );
      return;
    }
    $all_fields = field_info_fields();
    $supported_field_types = array('file', 'image');
    $valid_fields = array();

    // Get a list of all valid fields that we both support and are part of this entity
    foreach ($all_fields as $field) {
      if (array_key_exists($field['field_name'], $entity_fields)) {
        if (in_array($field['type'], array_values($supported_field_types))) {
          $valid_fields[$field['field_name']] = $entity_fields[$field['field_name']]['label'];
        }
      }
    }

    $valid_fields = array('' => '--') + $valid_fields;
    $form['widget']['widget_settings']['links'][$this->id]['swatch']['swatch_field'] = array(
      '#type' => 'select',
      '#title' => t('Swatch field'),
      '#default_value' => $this->settings->settings['swatch_field'],
      '#options' => $valid_fields,
      '#description' => t('Select the field you would like to use to pull the swatches.'),
      '#states' => array(
        'visible' => array(
          'select[name="widget"]' => array('value' => $this->id),
        ),
      ),
    );

    $options = Array();
    foreach(image_styles() as $key=>$value) {
      $options[$key] = $key;
    }
    $form['widget']['widget_settings']['links'][$this->id]['swatch']['swatch_imagestyle'] = array(
        '#type' => 'select',
        '#title' => t('Unselected Image Style'),
        '#default_value' => $this->settings->settings['swatch_imagestyle'],
        '#options' => $options,
        '#description' => t('Select the image style you would like to use when a facet is not selected.'),
        '#states' => array(
            'visible' => array(
                'select[name="widget"]' => array('value' => $this->id),
            ),
        ),
    );
    $form['widget']['widget_settings']['links'][$this->id]['swatch']['swatch_imagestyle_select'] = array(
        '#type' => 'select',
        '#title' => t('Selected Image Style'),
        '#default_value' => $this->settings->settings['swatch_imagestyle_select'],
        '#options' => $options,
        '#description' => t('Select the image style you would like to use when a facet is selected. This will also be used for hoovering.'),
        '#states' => array(
            'visible' => array(
                'select[name="widget"]' => array('value' => $this->id),
            ),
        ),
    );
    $form['widget']['widget_settings']['links'][$this->id]['swatch']['swatch_showtext'] = array(
        '#type' => 'checkbox',
        '#title' => t('Show Text'),
        '#default_value' => $this->settings->settings['swatch_showtext'],
        '#description' => t('Also show the facet text.'),
        '#states' => array(
            'visible' => array(
                'select[name="widget"]' => array('value' => $this->id),
            ),
        ),
    );

    $form['widget']['widget_settings']['links'][$this->id]['swatch']['swatch_shownoimage'] = array(
        '#type' => 'checkbox',
        '#title' => t('Show Swatches without image'),
        '#default_value' => $this->settings->settings['swatch_shownoimage'],
        '#description' => t('Show a Swatch that has no image (by rendering the text).'),
        '#states' => array(
            'visible' => array(
                'select[name="widget"]' => array('value' => $this->id),
            ),
        ),
    );

  }

  /**
   * Returns defaults for the settings this widget provides.
   */
  function getDefaultSettings() {
    $defaults = parent::getDefaultSettings();
    $defaults['swatch_field'] = '';
    $defaults['swatch_imagestyle'] = '';
    $defaults['swatch_showtext'] = 0;
    $defaults['swatch_shownoimage'] = 1;
    return $defaults;
  }

}
