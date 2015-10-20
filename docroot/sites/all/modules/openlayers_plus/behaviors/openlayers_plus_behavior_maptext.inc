<?php
/**
 * @file
 * Inserting all blocks in a region on the map, with toggle buttons for 
 * each block
 */

class openlayers_plus_behavior_maptext extends openlayers_behavior {
  /**
   * Override of options_init().
   */
  public function options_init() {
    $options = array();
    return $options;
  }

  /**
   * Override of options_form().
   */
  public function options_form($defaults = array()) {
    $regionlist = system_region_list(variable_get('theme_default'), REGIONS_ALL);
    $options = array();

    $options['regions']['draggable'] = array(
        '#type' => 'checkbox',
        '#title' => t("Drag Maptext"),
        '#description' => t('Enable draggable maptext control.'),
        '#default_value' => isset($defaults['regions']['draggable']) ? $defaults['regions']['draggable'] : FALSE,
    );
    foreach ($regionlist as $key => $val) {
      $options['regions'][$key]['name'] = array(
        '#type' => 'checkbox',
        '#title' => t($val),
        '#description' => t('Enable this region.'),
        '#default_value' => isset($defaults['regions'][$key]['name']) ? $defaults['regions'][$key]['name'] : FALSE,
      );
      $options['regions'][$key]['fieldset'] = array(
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#type' => 'fieldset',
        '#title' => t('Toggle blocks in region @val', array('@val' => $val)),
      );
      $options['regions'][$key]['fieldset']['toggle'] = array(
        '#type' => 'checkbox',
        '#title' => t('Toggle blocks in region @val', array('@val' => $val)),
        '#default_value' => isset($defaults['regions'][$key]['fieldset']['toggle']) ? $defaults['regions'][$key]['fieldset']['toggle'] : FALSE,
      );
      $options['regions'][$key]['fieldset']['title'] = array(
        '#type' => 'textfield',
        '#title' => t('Title'),
        '#description' => t('Title for the button group.'),
        '#default_value' => isset($defaults['regions'][$key]['fieldset']['title']) ? $defaults['regions'][$key]['fieldset']['title'] : FALSE,
      );

      $options['regions'][$key]['fieldset']['collapse'] = array(
        '#type' => 'checkbox',
        '#title' => t('Collapse feature for buttons'),
        '#default_value' => isset($defaults['regions'][$key]['fieldset']['collapse']) ? $defaults['regions'][$key]['fieldset']['collapse'] : FALSE,
      );
    }

    return $options;
  }

  /**
   * Render.
   */
  public function render(&$map) {
    $reaction = context_get_plugin('reaction', 'block');

    $regions = array();
    $toggle = array();
    $button_titles = array();
    $collapsible = array();

    $count = 0;

    foreach ($this->options['regions'] as $region => $active) {
      if ($active['name'] === 1) {

        if ($active['fieldset']['title'] != "") {
          $button_titles[$region] = $active['fieldset']['title'];
        }

        if ($active['fieldset']['collapse'] === 1) {
          $collapsible[$region] = 1;
        }

        $blocks = block_list($region);
        if (module_exists('context')) {
          $reaction = context_get_plugin('reaction', 'block');
          $contextblocks = $reaction->block_list($region);
          $blocks = array_merge($contextblocks, $blocks);
        }
        $regions[$region] = array();

        foreach ($blocks as $key => $block) {
          if ($active['fieldset']['toggle'] === 1) {
            $toggle[$count] = $key;
          }

          $markup = drupal_render($block->content);
          $regions[$region][$key] = Array(
              'tree' => TRUE,
              'collapsible' => TRUE,
              'collapsed' => FALSE,
              'type' => 'fieldset',
              'title' => $block->subject ,
              'markup' => $markup,
          );
          $count = $count + 1;
        }

      };
    }

    $options = Array (
        'regions' => $regions,
        'Toggle' => $toggle,
        'Btn_Titles' => $button_titles,
        'collapse' => $collapsible,
        'drag' => $this->options['regions']['draggable'],
    );

    drupal_add_js(array('openlayers_plus' => $options), 'setting');
    drupal_add_js('misc/jquery.cookie.js','file');
    drupal_add_css(drupal_get_path('module', 'openlayers_plus') . '/behaviors/openlayers_plus_behavior_maptext_toggle.css');
    drupal_add_js(drupal_get_path('module', 'openlayers_plus') . '/behaviors/openlayers_plus_behavior_maptext.js');

    return $options;
  }
}
