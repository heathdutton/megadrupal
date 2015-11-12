<?php
/**
 * @file
 * The commerce_fancy_image_attributes widget plugin class.
 *
 * The template for the plugin was taken from Commerce search API module
 */

/**
 * Widget that renders taxonomy terms as fancy attributes
 */
class CommerceSearchApiFancyImage extends FacetapiWidgetLinks {

  /**
   * Overrides FacetapiWidget:Links:settingsForm().
   */
  function settingsForm(&$form, &$form_state) {
    parent::settingsForm($form, $form_state);
    $form['widget']['widget_settings']['links'][$this->id]['display_title'] = array(
      '#type' => 'checkbox',
      '#title' => t('Append the title to the fancy image'),
      '#default_value' => !empty($this->settings->settings['display_title']) ? $this->settings->settings['display_title'] : FALSE,
      '#description' => t('If checked, the title of the option will be appended to the fancy image.'),
    );
    $form['widget']['widget_settings']['links'][$this->id]['display_count'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display the number of matching items'),
      '#default_value' => !empty($this->settings->settings['display_count']) ? $this->settings->settings['display_count'] : FALSE,
      '#description' => t('If checked, the number of matching items will be output with the color square.'),
    );
  }

  /**
   * Overrides FacetapiWidgetLinks::buildListItems().
   *
   * Add our css file and call our theme function to theme taxonomy terms as
   * fancy attributes.
   */
  function buildListItems($build) {
    $facet_info = $this->facet->getFacet();
    $color_hex_handler = variable_get('commerce_fancy_image_attributes_color_text', 'commerce_fancy_attributes');

    if ($facet_info['field type'] == 'taxonomy_term' && module_exists('commerce_fancy_image_attributes')) {
      $terms = taxonomy_term_load_multiple(array_keys($build));

      $first_term = reset($terms);
      $bundle = $first_term->vocabulary_machine_name;
      // Loop over the field instances to find an instance that use the
      // fancy (image) attributes formatter.
      $color_image_field_instance = NULL;
      $color_image_field_name = NULL;
      $found_color_image = FALSE;
      foreach (field_info_instances('taxonomy_term', $bundle) as $field_name => $instance) {
        if ($instance['display']['add_to_cart_form']['type'] == 'image') {
          $found_color_image = TRUE;
          $color_image_field_instance = $instance;
          $color_image_field_name = $field_name;
          break;
        }
      }

      // Loop over the field instances to find an instance that use the
      // fancy (text) attributes formatter.
      $color_hex_field_instance = NULL;
      $color_hex_field_name = NULL;

      if ($color_hex_handler == 'commerce_fancy_attributes') {
        $color_handler_type = 'commerce_fancy_attributes_color';
      }
      else {
        $color_handler_type = 'color_field_swatch';
      }

      foreach (field_info_instances('taxonomy_term', $bundle) as $field_name => $instance) {
        if ($instance['display']['add_to_cart_form']['type'] != $color_handler_type) {
          continue;
        }
        $found_color_hex = TRUE;
        $color_hex_field_instance = $instance;
        $color_hex_field_name = $field_name;
      }

      // Check if we should display the facet count.
      $display_count = !empty($this->settings->settings['display_count']);
      // Check if we should display the title
      $display_title = !empty($this->settings->settings['display_title']);
      // If we've found at least one field that uses the fancy attributes formatter.
      if (!empty($found_color_image)) {
        drupal_add_css(drupal_get_path('module', 'commerce_search_api') . '/commerce_search_api.css');
        foreach ($build as $value => &$item) {
          $display = field_get_display($color_image_field_instance, 'add_to_cart_form', $terms[$value]);

          $image = array();
          $variables = array();

          // Check for an image and make sure it is not a default image
          if (!empty($terms[$value]->{$color_image_field_name}[LANGUAGE_NONE][0]) &&
            empty($terms[$value]->{$color_image_field_name}[LANGUAGE_NONE][0]['is_default'])) {
            $image = $terms[$value]->{$color_image_field_name}[LANGUAGE_NONE][0];
            $name = $terms[$value]->name;
            $image['title'] = $name;
            $image['alt'] = $name;
            $variables = array(
              'item' => $image,
              'image_style' => $display['settings']['image_style'],
            );

            $item['#html'] = TRUE;
            $item['#markup'] = theme('image_formatter', $variables);
            $item['#count'] = NULL;

            // Append the title if option is enabled
                if($display_title){
                  $item['#markup'] .= ' ' . $name;
                }
          }
          else {
            // If an image is not found check for a hex color code
            if ($color_hex_handler == 'commerce_fancy_attributes') {
              if (!empty($terms[$value]->{$color_hex_field_name}[LANGUAGE_NONE][0])) {
                $item['#html'] = TRUE;
                $variables['hex'] = $terms[$value]->{$color_hex_field_name}[LANGUAGE_NONE][0]['value'];
                if ($display_count) {
                  $variables['title'] = $item['#count'];
                }
                $item['#markup'] = theme('commerce_search_api_fancy_attributes_color', $variables);
                $item['#count'] = NULL;
              }
              else {
                // If a hex code is not found try using the default image if any.
                $color_image_field_info = field_info_field($color_image_field_name);
                $default_image = file_load($color_image_field_info['settings']['default_image']);
                $image = (array) $default_image;
                $name = $terms[$value]->name;
                $image['title'] = $name;
                $image['alt'] = $name;
                $variables = array(
                  'item' => $image,
                  'image_style' => $display['settings']['image_style'],
                );

                $item['#html'] = TRUE;
                $item['#markup'] = theme('image_formatter', $variables);
                $item['#count'] = NULL;

                // Append the title if option is enabled
                if($display_title){
                  $item['#markup'] .= ' ' . $name;
                }
              }
            }
            elseif ($color_hex_handler == 'color_field') {
              if (!empty($terms[$value]->{$color_hex_field_name}[LANGUAGE_NONE][0])) {
                $display = field_get_display($color_hex_field_instance, 'add_to_cart_form', $terms[$value]);
                $width = $display['settings']['width'];
                $height = $display['settings']['height'];
                $rgb = $terms[$value]->{$color_hex_field_name}[LANGUAGE_NONE][0]['rgb'];
                $name = $terms[$value]->name;
                $item['#html'] = TRUE;
                $item['#markup'] = '<div title="' . $name . '" style="background: ' . $rgb . '; width: ' . $width . 'px; height: ' . $height . 'px;"></div>';
                $item['#count'] = NULL;
              }
              else {
                // If a hex code is not found try using the default image if any.
                $color_image_field_info = field_info_field($color_image_field_name);
                $default_image = file_load($color_image_field_info['settings']['default_image']);
                $image = (array) $default_image;
                $name = $terms[$value]->name;
                $image['title'] = $name;
                $image['alt'] = $name;
                $variables = array(
                  'item' => $image,
                  'image_style' => $display['settings']['image_style'],
                );

                $item['#html'] = TRUE;
                $item['#markup'] = theme('image_formatter', $variables);
                $item['#count'] = NULL;
                // Append the title if option is enabled
                if($display_title){
                  $item['#markup'] .= ' ' . $name;
                }
              }
            }
          }
        }
      }
    }
    $items = parent::buildListItems($build);
    return $items;
  }
}
