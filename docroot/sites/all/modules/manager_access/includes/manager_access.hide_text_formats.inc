<?php
/**
 * @file
 *
 * Created by IntelliJ IDEA.
 * User: svip
 * Date: 12.09.13
 * Time: 12:01
 * To change this template use File | Settings | File Templates.
 */

/**
 * Implements hook_field_widget_form_alter().
 */
function manager_access_field_widget_form_alter(&$element, &$form_state, $context) {
    if (!user_access("manager access show hidden text formats") && !empty($context['instance']['widget']['type']) && in_array($context['instance']['widget']['type'], manager_access_formatter_supported_widgets())) {
        if (!empty($context['instance']['widget']['settings']['use_default_formatter'])
            && $form_state['build_info']['form_id'] != 'field_ui_field_edit_form'
        ) {
            $element['#after_build'][] = 'manager_access_pre_render_hide_formats';
        }
    }
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function manager_access_form_field_ui_field_edit_form_alter(&$form, &$form_state) {
    $instance = $form['#instance'];
    if (in_array($instance['widget']['type'], manager_access_formatter_supported_widgets())) {
        $form['instance']['widget']['settings'] += array('use_default_formatter' => array(
            '#type' => 'checkbox',
            '#title' => t('Use default text format for widget, hide other settings'),
            '#default_value' => $instance['widget']['settings']['use_default_formatter'],
            '#weight' => 17,
        ));
    }
}

/**
 * Return array of supported widgets for hiding settings
 * @return array
 */
function manager_access_formatter_supported_widgets() {
    return array('text_textarea');
}

/**
 * Implements hook_field_widget_info_alter().
 */
function manager_access_field_widget_info_alter(&$info) {
    foreach (manager_access_formatter_supported_widgets() as $widget) {
        $info[$widget]['settings']['use_default_formatter'] = 0;
    }
}

/**
 * Hide Text formats options
 */
function manager_access_pre_render_hide_formats($element) {
    if (!empty($element['format'])) {
        drupal_add_css(drupal_get_path("module", "manager_access") . "/css/manager_access.css");
        $element['format']['format']['#value'] = $element['format']['format']['#default_value'];
        $element['format']['#attributes']['class'][] = "manager-access-hide";
    }
    return $element;
}