<?php

/**
 * @file
 * Administration pages.
 */



/**
 * Callback for settings form.
 */
function formforall_admin_settings_form($form, &$form_state) {
  $form = array();

  // Register link for user who don't have an account yet.
  if (!variable_get('formforall_user_id', FALSE)) {
    $form['formforall_registration'] = array(
      '#type' => 'markup',
      '#markup' => t(
        "You don't have a FormForAll account ? !register_link",
        array(
          '!register_link' => l(t("Register now !"), FORMFORALL_URL),
        )
      ),
    );
  }

  // Profile page url.
  $profile_page = l(
    t("profile page"),
    FORMFORALL_URL . '/users/me',
    array(
      'fragment' => 'api',
      'external' => TRUE,
    )
  );

  // User ID field.
  $form['formforall_user_id'] = array(
    '#title' => t("User ID"),
    '#description' => t("User ID can be found on your !profile_page.", array('!profile_page' => $profile_page)),
    '#type' => 'textfield',
    '#size' => 36,
    '#required' => TRUE,
    '#default_value' => variable_get('formforall_user_id', FALSE),
    '#element_validate' => array('formforall_userid_element_validate'),
  );

  // API key field.
  $form['formforall_api_key'] = array(
    '#title' => t("API key"),
    '#description' => t("API key can be found on your !profile_page.", array('!profile_page' => $profile_page)),
    '#type' => 'textfield',
    '#size' => 100,
    '#required' => TRUE,
    '#default_value' => variable_get('formforall_api_key', FALSE),
    '#element_validate' => array('formforall_apikey_element_validate'),
  );

  return system_settings_form($form);
}


/**
 * Validate User ID.
 */
function formforall_userid_element_validate($element, &$form_state) {
  if (!formforall_is_valid_id($element['#value'])) {
    form_error(
      $element,
      t('%name must be a valid UUID.', array('%name' => $element['#title']))
    );
  }
}


/**
 * Validate API key.
 */
function formforall_apikey_element_validate($element, &$form_state) {
  if (!preg_match('/^[a-zA-Z0-9]{100,100}$/', $element['#value'])) {
    form_error(
      $element,
      t('%name must be a 100 alphanumeric characters long string.', array('%name' => $element['#title']))
    );
  }
}


/**
 * Admin page with a forms listing.
 */
function formforall_admin_forms_page() {
  // Get forms (force cache reset).
  $forms = formforall_get_forms(TRUE);

  // Header.
  $header = array(
    t("ID"),
    t("Title"),
    t("Allowed hosts"),
    t("Operations"),
  );

  $rows = array();
  if (!empty($forms)) {
    foreach ($forms as $form) {
      // Operation links: view, edit.
      $operations = array(
        '#theme' => 'links',
        '#links' => array(
          array(
            'title' => t("View"),
            'href' => FORMFORALL_URL . '/forms/' . $form['id'],
            'attributes' => array('target' => '_blank'),
          ),
          array(
            'title' => t("Edit"),
            'href' => FORMFORALL_URL . '/users/me/forms/' . $form['id'],
            'attributes' => array('target' => '_blank'),
          ),
        ),
        '#attributes' => array('class' => array('links', 'inline')),
      );

      // Build row.
      $rows[] = array(
        $form['id'],
        check_plain($form['title']),
        implode(', ', $form['allowedHost']),
        drupal_render($operations),
      );
    }
  }

  return array(
    'table' => array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t("No forms."),
    ),
    'link' => array(
      '#theme' => 'link',
      '#text' => t("Manage your forms on FormForAll."),
      '#path' => FORMFORALL_URL . '/users/me/forms',
      '#options' => array(
        'html' => FALSE,
        'attributes' => array(
          'target' => '_blank',
        ),
      ),
    ),
  );
}