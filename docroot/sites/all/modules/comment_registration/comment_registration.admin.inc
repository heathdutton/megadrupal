<?php

/**
 * Implements hook_form_FORM_ID_alter().
 */
function comment_registration_form_node_type_form_alter(&$form, $form_state) {
  if (isset($form['type'])) {
    // Drupal core anonymous commenting dropdown
    $form['comment']['comment_anonymous']['#weight'] = -4;
    // Fieldset
    $form['comment_registration'] = array(
      '#type' => 'fieldset',
      '#title' => t('Comment Registration'),
      '#group' => 'additional_settings',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    // How to enable message
    $form['comment_registration']['message'] = array(
      '#type' => 'container',
      '#states' => array(
        'visible' => array(
          array(':input[name="comment_anonymous"]' => array('value' => '0')),
        ),
      ),
      'markup' => array(
        '#markup' =>
          'To configure Comment Registration select one of these options under <em>Anonymous commenting</em> in <em>Comment settings:</em><br />' .
          '<ul>' .
            '<li>Anonymous posters may leave their contact information</li>' .
            '<li>Anonymous posters must leave their contact information</li>' .
          '</ul>'
      ),
    );
    // Comment registration enabled?
    $form['comment_registration']['comment_registration_enabled'] = array(
      '#title' => 'Enable comment registration',
      '#description' => t('Allow users to register for accounts via the comment form.'),
      '#type' => 'checkbox',
      '#default_value' => variable_get('comment_registration_enabled_' . $form['#node_type']->type, 0),
      '#weight' => -10,
      '#access' => user_access('post comments', drupal_anonymous_user()),
      '#states' => array(
        'unchecked' => array(
          array(':input[name="comment_anonymous"]' => array('value' => '0')),
        ),
        'visible' => array(
          array(
            array(':input[name="comment_anonymous"]' => array('value' => '1')),
            'or',
            array(':input[name="comment_anonymous"]' => array('value' => '2')),
          ),
        ),
      ),
    );
    // Container
    $form['comment_registration']['container'] = array(
      '#type' => 'container',
      '#weight' => -3,
      '#states' => array(
        'visible' => array(
          array(':input[name="comment_registration_enabled"]' => array('checked' => TRUE)),
        ),
      ),
    );
    // Comment registration optional?
    $form['comment_registration']['container']['comment_registration_optional'] = array(
      '#title' => 'Comment registration is optional.',
      '#description' => t('Allow users to choose if they want to register or post anonymously. Note: Email Registration support is disabled with this option.'),
      '#type' => 'checkbox',
      '#default_value' => variable_get('comment_registration_optional_' . $form['#node_type']->type, 0),
      '#weight' => -2,
    );
    // Hide Comment body
    $form['comment_registration']['container']['comment_registration_hide_comment_body'] = array(
      '#title' => 'Hide Comment body field',
      '#description' => t('Hide core comment body. Deleting this field <a href="https://www.drupal.org/node/1038652">currently causes an error.</a>'),
      '#type' => 'checkbox',
      '#default_value' => variable_get('comment_registration_hide_comment_body_' . $form['#node_type']->type, 0),
      '#weight' => 3,
    );
    // Hide Homepage
    $form['comment_registration']['container']['comment_registration_hide_homepage'] = array(
      '#title' => 'Hide Homepage field',
      '#description' => t('Hide core comment Homepage field.'),
      '#type' => 'checkbox',
      '#default_value' => variable_get('comment_registration_hide_homepage_' . $form['#node_type']->type, 0),
      '#weight' => 4,
    );
    // Choose a role to assign during registration
    $options = array('_none' => "None");
    $roles = user_roles(TRUE);
    unset($roles[DRUPAL_AUTHENTICATED_RID]);
    $options = $options + $roles;

    $form['comment_registration']['container']['comment_registration_role'] = array(
      '#type' => 'radios',
      '#title' => t('Assign role during registration'),
      '#options' => $options,
      '#default_value' => variable_get('comment_registration_role_' . $form['#node_type']->type, '_none'),
      '#weight' => 5,
    );
  }
}
