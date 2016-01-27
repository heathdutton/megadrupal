<?php

/**
 * Menu callback function generating view modes add/edit form
 * 
 * @param array $form
 * @param array $form_state
 * @return array 
 */
function media_fevm_admin_overview($form, &$form_state) {
    $view_modes = media_fevm_get_view_modes();
    
    $form['media_fevm_view_modes'] = array(
        '#theme' => 'media_fevm_admin_overview_form',
        '#tree' => TRUE,
        '#prefix' => '<div id="media-fevm-view-modes">',
        '#suffix' => '</div>',
    );
    
    if(isset($form_state['values']) && $form_state['values']['op'] == t('Add')) {
        $view_mode = $form_state['values']['media_fevm_view_modes']['new'];
        unset($form_state['values']['media_fevm_view_modes']['new']);
        $form_state['storage']['new'][$view_mode['machine_name']] = $view_mode;
        $view_modes += $form_state['storage']['new'];
    }
    
    foreach($view_modes as $name => $mode) {
        $form['media_fevm_view_modes'][$name] = array(
            'label' => array(
                '#type' => 'textfield',
                '#default_value' => $mode['label'],
            ),
            'machine_name' => array(
                '#type' => 'machine_name',
                '#default_value' => $name,
                '#machine_name' => array(
                    'source' => array('media_fevm_view_modes', $name, 'label'),
                    'exists' => 'media_fevm_get_view_modes'
                ),
            ),
            'custom settings' => array(
                '#type' => 'checkbox',
                '#default_value' => $mode['custom settings']
            ),
            array(),
        );
    }
    
    $form['media_fevm_view_modes']['new'] = array(
        'label' => array(
            '#type' => 'textfield',
        ),
        'machine_name' => array(
            '#type' => 'machine_name',
            '#machine_name' => array(
                'source' => array('media_fevm_view_modes', 'new', 'label'),
                'exists' => 'media_fevm_get_view_modes'
            ),
            '#required' => FALSE,
        ),
        'custom settings' => array(
            '#type' => 'checkbox',
        ),
        'add' => array(
           '#type' => 'button',
           '#value' => t('Add'),
           '#ajax' => array(
                'wrapper' => 'media-fevm-view-modes',
                'callback' => 'media_fevm_admin_overview_callback',
                'method' => 'replace',
            )
        )
    );
    
    $form['#submit'] = array('media_fevm_admin_overview_submit');
    
    return system_settings_form($form);
}

/**
 * Form media_fevm_admin_overview submit function
 * 
 * @param array $form
 * @param array $form_state 
 */
function media_fevm_admin_overview_submit(&$form, &$form_state) {
    unset($form_state['values']['media_fevm_view_modes']['new']);
}

/**
 * Ajax callback function for media_fevm_admin_overview form element
 * 
 * @param array $form
 * @param array $form_state
 * @return array 
 */
function media_fevm_admin_overview_callback($form, $form_state) {
    return $form['media_fevm_view_modes'];
}

/**
 * Implementation of theme_media_fevm_admin_overview_form
 * 
 * @param type $variables
 * @return type 
 */
function theme_media_fevm_admin_overview_form($variables) {
    $elements = $variables['elements'];
    
    $rows = array();
    foreach(element_children($elements) as $key) {
        $row = array();
        if($key == 'new') {
            $elements[$key]['label']['#value'] = '';
            $elements[$key]['machine_name']['#value'] = '';
            $elements[$key]['custom settings']['#value'] = '';
        }
        $row[] = drupal_render($elements[$key]['label']) . drupal_render($elements[$key]['machine_name']);
        $row[] = drupal_render($elements[$key]['custom settings']);
        $row[] = isset($elements[$key]['add']) ? drupal_render($elements[$key]['add']) : '';
        $rows[] = $row;
    }
    
    $table = array(
        '#theme' => 'table',
        '#header' => array('Label', 'Custom settings', ''),
        '#rows' => $rows,
    );
    
    return drupal_render($table);
}
