<?php

/**
 * @file
 * Mediabox crop forms.
 */

function mediabox_crop_form_styles($form, &$form_state, MediaboxEntity $mediabox) {
  
  $form = array();
  $styles = image_styles();

  $form_state['mediabox'] = $mediabox;
  
  $form['items'] = array(
    '#type' => 'vertical_tabs',
    '#tree' => TRUE
  );

  $form['styles'] = array(
    '#type' => 'vertical_tabs',
    '#tree' => TRUE,
  );
  
  $crops = mediabox_crop_db_get_by_entity_id($mediabox->id);
  foreach ($styles as $style) {
    
    $coords = mediabox_crop_get_default_coords_from_style($style);
    $name = $style['name'];
    
    $form['styles'][$name] = array(
      '#type' => 'fieldset',
      '#title' => $name,
      '#collapsible' => TRUE,
      '#group' => 'items',
      '#attributes' => array(
        'class' => array('crop-image-set'),
      ),
    );
    
    $file = file_load($mediabox->fid);
    $image_properties = array(
      'path' => $file->uri,
      'attributes' => array(
        'class' => array('jcrop-image'),
      ),
    );
    
        // ID
    $form['styles'][$name]['id'] = array(
      '#type' => 'hidden',
      '#value' => key_exists($name, $crops) ? $crops[$name]->id : '',
    );
    
        // Save
    $form['styles'][$name]['save'] = array(
      '#type' => 'checkbox',
      '#title' => t('Override crop settings?'),
      '#default_value' => key_exists($name, $crops) ? 1 : 0,
      '#attributes' => array(
        'class' => array('save'),
      ),
    );
    
        // X
    $form['styles'][$name]['x'] = array(
      '#type' => 'hidden',
      '#title' => 'X',
      '#size' => 15,
      '#maxlength' => 25,
      '#attributes' => array(
        'class' => array('jcrop-x'),
      ),
      '#default_value' => key_exists($name, $crops) ? $crops[$name]->x : $coords['x']
    );
    
        // Y
    $form['styles'][$name]['y'] = array(
      '#type' => 'hidden',
      '#title' => 'Y',
      '#size' => 15,
      '#maxlength' => 25,
      '#attributes' => array(
        'class' => array('jcrop-y'),
      ),
      '#default_value' => key_exists($name, $crops) ? $crops[$name]->y : $coords['y']
    );

    // X1
    $form['styles'][$name]['x1'] = array(
      '#type' => 'hidden',
      '#title' => 'X1',
      '#size' => 15,
      '#maxlength' => 25,
      '#attributes' => array(
        'class' => array('jcrop-x1'),
      ),
      '#default_value' => key_exists($name, $crops) ? $crops[$name]->x1 : $coords['x1']
    );

    // Y1
    $form['styles'][$name]['y1'] = array(
      '#type' => 'hidden',
      '#title' => 'Y1',
      '#size' => 15,
      '#maxlength' => 25,
      '#attributes' => array(
        'class' => array('jcrop-y1'),
      ),
      '#default_value' => key_exists($name, $crops) ? $crops[$name]->y1 : $coords['y1']
    );
    
        // Height
    $form['styles'][$name]['height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height'),
      '#size' => 15,
      '#maxlength' => 25,
      '#attributes' => array(
        'class' => array('jcrop-height'),
      ),
      '#default_value' => key_exists($name, $crops) ? $crops[$name]->height : $coords['h']
    );

    // Width
    $form['styles'][$name]['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#size' => 15,
      '#maxlength' => 25,
      '#attributes' => array(
        'class' => array('jcrop-width'),
      ),
      '#default_value' => key_exists($name, $crops) ? $crops[$name]->width : $coords['w']
    );
    
        // Min height
    $form['styles'][$name]['min-height'] = array(
      '#type' => 'hidden',
      '#value' => $coords['h'],
      '#attributes' => array(
        'class' => array('jcrop-min-height'),
      ),
    );

    // Min width
    $form['styles'][$name]['min-width'] = array(
      '#type' => 'hidden',
      '#value' => $coords['w'],
      '#attributes' => array(
        'class' => array('jcrop-min-width'),
      ),
    );
    
    $form['styles'][$name]['image'] = array(
      '#type' => 'item',
      '#markup' => theme('image', $image_properties),
    );
    
  }
  
    $form['#attached'] = array(
    'js' => array(drupal_get_path('module', 'mediabox_crop') . '/js/mediabox_crop_summary.js'),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  
  return $form;
}

function mediabox_crop_form_styles_submit($form, &$form_state) {
  $styles = $form_state['values']['styles'];
  $mediabox = $form_state['mediabox'];

  foreach ($styles as $style_name => $c) {
    $c['origin_image_style'] = $style_name;
    $c['entity_id'] = $mediabox->id;

    $mediabox_crop = new MediaboxCrop();
    $mediabox_crop->populateFromArray($c);

    // Save crop settings
    if (!empty($c['save']) && $c['save'] == 1) {
      // Check if user filled all mandatory fields
      if (isset($c['x']) && isset($c['y']) && !empty($c['x1']) && !empty($c['y1']) && !empty($c['height']) && !empty($c['width'])) {
        if (!mediabox_crop_db_save($mediabox_crop)) {
          drupal_set_message(t('Can not save media coordinates!'), 'error');
        }
      }
      // Remove crop settings
    }
    else {
      if (!mediabox_crop_db_delete($mediabox_crop)) {
        drupal_set_message(t('Can not delete media coordinates'), 'error');
      }
    }

  }
}

function mediabox_crop_form_style($mediabox, $image_style = NULL) {

  $file = file_load($mediabox->fid);
  $image_properties = array(
    'path' => $file->uri,
    'attributes' => array(
      'class' => array('jcrop-image'),
    ),
    'style_name' => $image_style,
    'width' => '',
    'height' => '',
  );

  $form['styles']['set']['image'] = array(
    '#type' => 'item',
    '#attributes' => array('id' => 'supertest'),
    '#markup' => empty($image_style) ? theme('image', $image_properties) : theme('image_style', $image_properties),
  );
  return $form;
}
