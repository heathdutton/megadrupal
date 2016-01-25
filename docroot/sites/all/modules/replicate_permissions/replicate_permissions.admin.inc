<?php
/**
 * @file: Admin form
 */

function replicate_permissions_admin_form() {

  // Enable / disable per-content type permission
  $form['replicate_permissions_permissions']['replicate_permissions_per_content_type'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable per-content-type permissions'),
    '#description' => t('Per-content type permissions can be assigned in the <a href="@perms">permissions administration page</a>.', array('@perms' => url('admin/people/permissions'))),
    '#default_value' => variable_get('replicate_permissions_per_content_type', FALSE),
  );

  // Enable / disable per-node permission
  $form['replicate_permissions_permissions']['replicate_permissions_per_node'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable per-node permissions'),
    '#description' => t('For users to be able to replicate individual nodes,
    those nodes need to be marked as replicable in the edit form.
    The "Replicate replicable nodes" permission also needs to be granted for
    roles to be able to replicate nodes.'),
    '#default_value' => variable_get('replicate_permissions_per_node', FALSE),
  );

  return system_settings_form($form);
}
