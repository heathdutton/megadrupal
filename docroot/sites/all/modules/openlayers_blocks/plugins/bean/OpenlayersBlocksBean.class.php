<?php
/**
 * @file
 * Openlayers Blocks bean plugin.
 */

class OpenlayersBlocksBean extends BeanPlugin {
  /**
   * Declares default block settings.
   */
  public function values() {
    return array(
      'openlayers_map' => array(),
    );
  }

  /**
   * Builds extra settings for the block edit form.
   */
  public function form($bean, $form, &$form_state) {
    $form['openlayers_map'] = array(
      '#type' => 'select',
      '#title' => t('Map'),
      '#description' => t('This is the map that will be rendered in the block.'),
      '#options' => openlayers_map_options(),
      '#default_value' => $bean->openlayers_map
    );
    return $form;
  }

  /**
   * Displays the bean.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {
    return openlayers_render_map($bean->openlayers_map);

    // This will be when we'll be using renderable arrays in OpenLayers.
    /*
    $preset = openlayers_preset_load($bean->openlayers_map);
    $content['openlayers_map'] = openlayers_render_map($preset->data);
    return $content;
    */
  }
}
