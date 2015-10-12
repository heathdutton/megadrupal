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
 * Create settings form in drupal configuration setings section for module
 */
function manager_access_redirect_settings_form() {
    $user_roles = manager_access_get_roles(FALSE, FALSE);

    $form['manager_access_redirect'] = array(
        '#type' => 'fieldset',
        '#title' => t('Login redirect'),
        '#collapsible' => FALSE,
    );

    $form['manager_access_redirect']['manager_access_redirect_on'] = array(
        '#type' => 'checkbox',
        '#title' => t('Use redirect.'),
        '#default_value' => variable_get('manager_access_redirect_on', 0),
    );

    $form['manager_access_redirect']['manager_access_redirect_path'] = array(
        '#type' => 'textfield',
        '#title' => t('Redirect to page'),
        '#default_value' => variable_get('manager_access_redirect_path', '<front>'),
        '#size' => 60,
        '#desctioption' => t('Specify page by using its path. Example path is blog for the blog page. <front> is the front page. <current> is the current page. Precede with http:// for an external URL. Leave empty to redirect to a default page.'),
        '#states' => array(
            'visible' => array(
                ':input[name="manager_access_redirect_on"]' => array('checked' => TRUE),
            ),
        ),
    );

    $form['manager_access_redirect']['manager_access_redirect_roles'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Redirect users with roles'),
        '#default_value' => variable_get('manager_access_redirect_roles', array()),
        '#desctioption' => t('Redirect only the selected role(s). If you select no roles, all users will be redirected.'),
        '#options' => $user_roles,
        '#states' => array(
            'visible' => array(
                ':input[name="manager_access_redirect_on"]' => array('checked' => TRUE),
            ),
        ),
    );

    return system_settings_form($form, TRUE);
}


/**
 * Helper submit function.
 */
function manager_access_redirect_submit($form, &$form_state) {
    $redirect_on = variable_get('manager_access_redirect_on', 0);
    $redirect_path = variable_get('manager_access_redirect_path', '<front>');
    $redirect_roles = variable_get('manager_access_redirect_roles', array());
    if ($redirect_on && !empty($redirect_roles) && !empty($redirect_path) && drupal_valid_path($redirect_path)) {
        GLOBAL $user;
        foreach ($user->roles as $user_role_id => $user_role_name) {
            if (in_array($user_role_id, $redirect_roles)) {
                if (isset($_GET['destination']) && !empty($_GET['destination'])) {
                    $_GET['destination'] = $redirect_path;
                }
                else {
                    drupal_goto($redirect_path);
                }
            }
        }
    }
}