<?php

/**
 * @file
 * Hooks documentation.
 */

/**
 * Defines style settings array for ctools modals used for modal_entities.
 *
 * @return array
 *   Settings array for ctools modal.
 *
 * @see modal_entities_modal_entities_style_info()
 * @see ctools_ajax_sample_page()
 */
function hook_modal_entities_style_info() {
  $throbber = theme('image', array('path' => ctools_image_path('loading_animation.gif', 'modal_entities'), 'alt' => t('Loading...'), 'title' => t('Loading')));
  $styles = array(
    'modal-entities-small' => array(
      'modalSize' => array(
        'type' => 'fixed',
        'width' => 300,
        'height' => 300,
      ),
      'modalOptions' => array(
        'opacity' => 0,85,
        'background' => '#000',
      ),
      'animation' => 'fadeIn',
      'modalTheme' => 'ModalEntitiesPopup',
      'throbber' => $throbber,
      'closeText' => t('Close'),
    ),
    'modal-entities-large' => array(
      'modalSize' => array(
        'type' => 'scale',
        'width' => 0.8,
        'height' => 0.8,
      ),
      'modalOptions' => array(
        'opacity' => 0.85,
        'background' => '#000',
      ),
      'animation' => 'fadeIn',
      'modalTheme' => 'ModalEntitiesPopup',
      'throbber' => $throbber,
      'closeText' => t('Close'),
    ),
  );

  return $styles;
}