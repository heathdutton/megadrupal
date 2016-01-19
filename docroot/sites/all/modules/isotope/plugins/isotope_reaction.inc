<?php
/**
 * @file
 * Contains the isotope_reaction subclass.
 */

/**
 * Expose isotope configurations as context reactions.
 */
class isotope_reaction extends context_reaction {
  /**
   * Editor form.
   */
  public function editor_form($context) {
    $form = $this->options_form($context);
    return $form;
  }

  /**
   * Submit handler for editor form.
   */
  public function editor_form_submit($context, $values) {
    return $values;
  }

  /**
   * Configuration form for the Isotope context reaction.
   */
  public function options_form($context) {
    $values = $this->fetch_from_context($context);
    $form = array(
      '#tree' => TRUE,
      '#title' => t('Isotope'),
      'pre' => array(
        '#type' => 'item',
        '#title' => t('Isotope Configuration'),
        '#description' => t('For detailed examples and documentation, please see !link.', array('!link' => l(t('The Isotope Manual'), 'http://isotope.metafizzy.co/docs/layout-modes.html'))
        ),
      ),
      'container_selector' => array(
        '#title' => t('Container Selector'),
        '#description' => t('CSS selector for the Isotope container. Example: .view-content.'),
        '#type' => 'textfield',
        '#default_value' => isset($values['container_selector']) ? $values['container_selector'] : '',
      ),
      'item_selector' => array(
        '#title' => t('Item Selector'),
        '#description' => t('CSS selector for the Isotope Items. Example: .views-row.'),
        '#type' => 'textfield',
        '#default_value' => isset($values['item_selector']) ? $values['item_selector'] : '',
      ),
      'layout_mode' => array(
        '#title' => t('Layout Mode'),
        '#description' => t('An !isotope_layout to use for this Isotope instance.',
          array('!isotope_layout' => l(t('Isotope Layout Mode'), 'http://isotope.metafizzy.co/docs/layout-modes.html'))
        ),
        '#type' => 'select',
        '#options' => array(
          'masonry' => 'masonry',
          'fitRows' => 'fitRows',
          'cellsByRow' => 'cellsByRow',
          'straightDown' => 'straightDown',
          'masonryHorizontal' => 'masonryHorizontal',
          'fitColumns' => 'fitColumns',
          'cellsByColumn' => 'cellsByColumn',
          'straightAcross' => 'straightAcross',
        ),
        '#default_value' => isset($values['layout_mode']) ? $values['layout_mode'] : 'masonry',
      ),
      'column_width' => array(
        '#title' => t('Column Width'),
        '#description' => t('The numeric value for the Column Width setting for the selected layout mode. Leave blank to exclude.'),
        '#type' => 'textfield',
        '#default_value' => isset($values['column_width']) ? $values['column_width'] : '',
      ),
      'row_height' => array(
        '#title' => t('Row Height'),
        '#description' => t('The numeric value for the Row Height setting for the selected layout mode. Leave blank to exlude.'),
        '#type' => 'textfield',
        '#default_value' => isset($values['row_height']) ? $values['row_height'] : '',
      ),
      'resizable' => array(
        '#title' => t('Resizable'),
        '#description' => t('Trigger layout logic when browser window is resized.'),
        '#type' => 'checkbox',
        '#default_value' => isset($values['resizable']) ? $values['resizable'] : TRUE,
      ),
      'resize_container' => array(
        '#title' => t('Resize Container'),
        '#description' => t('Resize the specified Isotope container after repositioning items.'),
        '#type' => 'checkbox',
        '#default_value' => isset($values['resize_container']) ? $values['resize_container'] : TRUE,
      ),
      'animation_engine' => array(
        '#title' => t('Animation Engine'),
        '#description' => t('An !isotope_animationengine configuration for this Isotope instance.',
          array(
            '!isotope_animationengine' => l(t('Isotope Animation Engine'), 'http://isotope.metafizzy.co/docs/options.html',
                array('fragment' => 'animationengine')),
          )
        ),
        '#type' => 'select',
        '#options' => array(
          'best-available' => 'Best Available',
          'css' => 'CSS',
          'jquery' => 'jQuery',
        ),
        '#default_value' => isset($values['animation_engine']) ? $values['animation_engine'] : 'best-available',
      ),
      'apply_css' => array(
        '#type' => 'checkbox',
        '#title' => t('Automatically apply height and width styles to Isotope items based on the configured Column Width and Row Height.'),
        '#description' => t('Apply style settings via jQuery to Isotope items based on the configuration specified above. Style settings applied are limited to those utilized by the specified Isotope Layout Mode. This option is not recommended for advanced implementations. See the !layout_documentation for clarification.', array('!layout_documentation' => l(t('Isotope Layout Modes Documentation'), 'http://isotope.metafizzy.co/docs/layout-modes.html'))),
        '#default_value' => isset($values['apply_css']) ? $values['apply_css'] : TRUE,
      ),
    );
    return $form;
  }

  /**
   * Display a message to the user.
   */
  public function execute() {
    foreach ($this->get_contexts() as $k => $v) {
      if (!empty($v->reactions[$this->plugin])) {

        // Build the settings array for the configured Isotope reaction.
        $reaction = $v->reactions[$this->plugin];

        if (isset($reaction['layout_mode']) && isset($reaction['container_selector'])) {
          // Top level Isotope settings.
          $settings = array(
            'isotope_config' => array(
              'animationEngine' => !empty($reaction['animation_engine']) ? $reaction['animation_engine'] : 'best-available',
              'layoutMode' => !empty($reaction['layout_mode']) ? $reaction['layout_mode'] : 'masonry',
              'itemSelector' => !empty($reaction['item_selector']) ? $reaction['item_selector'] : NULL,
              'resizable' => !empty($reaction['resizable']) ? $reaction['resizable'] : TRUE,
              'resizesContainer' => !empty($reaction['resizes_container']) ? $reaction['resizes_container'] : TRUE,
            ),
            'container_selector' => !empty($reaction['container_selector']) ? $reaction['container_selector'] : FALSE,
            'apply_css' => !empty($reaction['apply_css']) && $reaction['apply_css'] ? $this->get_css_settings($reaction) : FALSE,
          );

          // Layout specific Isotope settings.
          $settings['isotope_config'][$reaction['layout_mode']] = array(
            'rowHeight' => !empty($reaction['row_height']) ? $reaction['row_height'] : 240,
            'columnWidth' => !empty($reaction['column_width']) ? $reaction['column_width'] : NULL,
          );

          // Include the Javascript required to bind the behavior.
          $path = drupal_get_path('module', 'isotope');
          drupal_add_js($path . '/js/isotope_reaction.js', array('weight' => 1));

          // Include the Javascript to configure the Isotope Reaction.
          drupal_add_js(array('isotope_reaction' => array($k => array('configuration' => $settings))), 'setting');
        }
      }
    }
  }

  /**
   * Return CSS settings for isotope items based on configured layoutMode.
   *
   * @param (array) $reaction
   *   A reaction array from the execute function
   *
   * @return (array)
   *   A an array of jQuery css parameters.
   */
  protected function get_css_settings($reaction) {
    // The list of css settings and the deplay modes that utilize them.
    $options = array(
      'width' => array(
        'cellsByColumn',
        'cellsByRow',
        'masonry',
      ),
      'height' => array(
        'cellsByColumn',
        'cellsByRow',
        'masonryHorizontal',
      ),
    );

    // Set the css settings based on the configured options.
    foreach ($options as $setting => $modes) {
      if (in_array($reaction['layout_mode'], $modes)) {
        $css[$setting] = TRUE;
      }
    }
    return isset($css) ? $css : FALSE;
  }
}
