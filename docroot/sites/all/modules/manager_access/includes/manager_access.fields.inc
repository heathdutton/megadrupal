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
 * manager access administration page
 */
function manager_access_fields_settings_form($form, &$form_state) {
    $manager_access_settings = variable_get('manager_access_settings', array());

    $user_roles = manager_access_get_roles();
    $node_types = node_type_get_types();
    $fields = manager_access_get_fields();
    manager_access_add_other_modules($fields);
    drupal_alter("manager_access_add_fields", $fields);

    $form = array();
    $form['#attached']['css'][] = array(
        'data' => drupal_get_path('module', 'manager_access') . '/css/manager_access.css',
        'type' => 'file'
    );
    $form['#attached']['js'][] = array(
        'data' => drupal_get_path('module', 'manager_access') . '/js/manager_access.js',
        'type' => 'file'
    );


    foreach ($user_roles as $role_id => $role) {
        foreach ($node_types as $node_type) {
            $type = $node_type->type;
            if (isset($fields[$type])) {
                if ($role_id != 0) {
                    $form[MANAGER_ACCESS_KEY][$role_id][$type] = array(
                        '#type' => 'fieldset',
                        '#attributes' => array(
                            'class' => array($role, 'fieldset' . $type)
                        ),
                        '#title' => check_plain($node_type->name),
                        '#collapsible' => FALSE,
                    );
                    foreach ($fields[$type] as $fields_key => $field) {
                        $fkey = MANAGER_ACCESS_KEY . "_" . $role_id . "_" . $type . "_" . $fields_key;
                        $form[MANAGER_ACCESS_KEY][$role_id][$type][$fkey] = array(
                            '#type' => 'checkbox',
                            '#attributes' => array('class' => array('users')),
                            '#title' => check_plain($field),
                            '#default_value' => (isset($manager_access_settings[$fkey])) ? $manager_access_settings[$fkey] : 0,
                        );
                    }

                    $fkey = MANAGER_ACCESS_KEY . "_" . $role_id . "_" . $type . "_custom_fields";
                    $form[MANAGER_ACCESS_KEY][$role_id][$type][$fkey] = array(
                        '#type' => 'textarea',
                        '#title' => t("Custom fields"),
                        '#attributes' => array('class' => array('manager-access-custom-fields')),
                        '#default_value' => (isset($manager_access_settings[$fkey])) ? $manager_access_settings[$fkey] : "",
                    );
                }
            }
        }
    }

    $form['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
    $form['#submit'][] = 'manager_access_fields_settings_submit';
    return $form;
}

/**
 * Submit admin form.
 */
function manager_access_fields_settings_submit($form, &$form_state) {
    $values = $form_state['values'];
    foreach ($values as $key => $value) {
        if (strpos($key, MANAGER_ACCESS_KEY) !== 0) {
            unset($values[$key]);
        }
    }
    variable_set('manager_access_settings', $values);
    drupal_set_message(t('Changes have been saved.'));
}

/**
 * Implementation of theme_form
 *
 * @param array $variables
 *   theme vars
 *
 * @return string
 *  form
 */
function theme_manager_access_fields_settings_form($variables) {
    $form = $variables['form'];
    $user_roles = manager_access_get_roles();
    $node_types = node_type_get_types();
    $header = $user_roles;

    foreach ($node_types as $node_type_key => $node_type) {
        $row = array();
        foreach ($user_roles as $role_id => $role) {
            $row[] = drupal_render($form[MANAGER_ACCESS_KEY][$role_id][$node_type_key]);
        }
        $rows[] = $row;
    }

    $output = theme('table', array('header' => $header, 'rows' => $rows));
    $output .= drupal_render_children($form);
    return $output;
}


/**
 * Get entity fields
 * @return array
 */
function manager_access_get_fields() {
    $query = db_select('field_config_instance', 't');
    $query->fields('t', array('field_name', 'bundle'));
    $query->where('entity_type = :type', array(':type' => 'node'));
    $query->orderBy('bundle');
    $result = $query->execute();
    $fields = $result->fetchAll();
    $fields_tmp = array();
    // Restructure fields array.
    foreach ($fields as $field) {
        $fields_tmp[$field->bundle][$field->field_name] = $field->field_name;
    }

    $node_base_fields = array(
        'author' => 'Authoring information',
        'revision_information' => 'Revision information',
        'options' => 'Publishing options'
    );
    // added base fields to each
    foreach ($fields_tmp as $key => $value) {
        $fields_tmp[$key] += $node_base_fields;
    }
    return $fields_tmp;
}


/**
 * Get user roles
 * @return array
 */
function manager_access_get_roles($add_all = TRUE, $add_anonymous = TRUE) {
    $user_roles = user_roles();
    if (!$add_anonymous && isset($user_roles[1])) {
        unset($user_roles[1]);
    }
    if ($add_all) {
        $user_roles[0] = 'all';
    }
    ksort($user_roles);
    return $user_roles;
}


/**
 * Add fields provided other modules.
 * @param $fields
 */
function manager_access_add_other_modules(&$fields) {
    $module_info = system_get_info('module');
    $modules = array();
    foreach (module_implements('form_node_form_alter') as $module) {
        if (module_hook($module, 'form_node_form_alter')) {
            switch ($module) {
                case 'pathauto':
                    $module_form = array('path' => array(), 'node' => NULL);
                    break;
                case 'nodesinblock':
                    $module_form = array('nodesinblock' => array());
                    break;
                default:
                    $module_form = array();
                    break;
            }
            $form_state = array('method' => NULL, 'rebuild' => NULL, 'cache' => NULL);
            if ($module == 'pathauto') {
                $module_form = array('path' => NULL);
            }
            else {
                @call_user_func_array($module . '_' . 'form_node_form_alter', array(&$module_form, $form_state));
            }
            $MANAGER_ACCESS_MODULE_KEY = array_keys($module_form);
            $MANAGER_ACCESS_MODULE_KEY = (isset($MANAGER_ACCESS_MODULE_KEY[0])) ? $MANAGER_ACCESS_MODULE_KEY[0] : NULL;

            if (!empty($MANAGER_ACCESS_MODULE_KEY)) {
              $modules[$MANAGER_ACCESS_MODULE_KEY] = $module_info[$module]['name'];
            }
        }
    }
    asort($modules);

    drupal_alter("manager_access_add_fields_provided_modules", $modules);

    foreach ($fields as $key => $field) {
        $fields[$key] += $modules;
    }
}

/**
 * hook_manager_access_add_fields_provided_modules_alter().
 * @param $fields
 */
function manager_access_manager_access_add_fields_provided_modules_alter(&$fields) {

}


/**
 * hook_manager_access_add_fields_alter().
 * @param $fields
 */
function manager_access_manager_access_add_fields_alter(&$fields) {

}