<?php

/**
 * Returns settings for features pipeexplosion
 */
function features_pipexplosion_settings() {
  $form = array();

  $form['node'] = array(
    '#title' => t('Content Types'),
    '#type' => 'fieldset',
  );

  $form['node']['features_pipexplosion_node_user_permission'] = array(
    '#type' => 'checkbox',
    '#title' => t('User Permissions'),
    '#description' => t("Adds in the default content type permissions to export (create [type] content, edit own [type] content, etc.)."),
    '#default_value' => variable_get('features_pipexplosion_node_user_permission', 1),
  );

  if (module_exists('strongarm')) {
    $form['node']['features_pipexplosion_node_og_permission'] = array(
      '#type' => 'checkbox',
      '#title' => t('Variables'),
      '#description' => t("Adds in the default og permissions to export."),
      '#default_value' => variable_get('features_pipexplosion_node_og_permission', 1),
    );
  }


  if (module_exists('strongarm')) {
    $form['node']['features_pipexplosion_node_variable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Variables'),
      '#description' => t("This setting tries to detect variables that haven't been added yet via looking for variables that end in the content type's machine name."),
      '#default_value' => variable_get('features_pipexplosion_node_variable', 1),
    );
  }

  if (module_exists('comment')) {
    $form['node']['features_pipexplosion_node_comment_field'] = array(
      '#type' => 'checkbox',
      '#title' => t('Comment fields'),
      '#description' => t("Adds in fields attached to comments for the content types."),
      '#default_value' => variable_get('features_pipexplosion_node_comment_field', 1),
    );
  }

  if (module_exists('views')) {
    $form['node']['features_pipexplosion_node_views_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Views'),
      '#description' => t("Adds in views that reference the content types."),
      '#default_value' => variable_get('features_pipexplosion_node_views_view', 0),
    );
  }

  if (module_exists('context')) {
    $form['node']['features_pipexplosion_node_context'] = array(
      '#type' => 'checkbox',
      '#title' => t('Context'),
      '#description' => t("Adds in contextes that reference these content types."),
      '#default_value' => variable_get('features_pipexplosion_node_context', 1),
    );
  }

  $form['user_role'] = array(
    '#title' => t('User roles'),
    '#type' => 'fieldset',
  );

  $form['user_role']['features_pipexplosion_user_role_user_permission'] = array(
    '#type' => 'checkbox',
    '#title' => t('User Permissions'),
    '#description' => t("Adds in all permissions currently set for a role."),
    '#default_value' => variable_get('features_pipexplosion_user_role_user_permission', 1),
  );

  return system_settings_form($form);
}
