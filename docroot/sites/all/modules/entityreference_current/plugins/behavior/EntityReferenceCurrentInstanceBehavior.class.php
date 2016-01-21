<?php

class EntityReferenceCurrentInstanceBehavior extends EntityReference_BehaviorHandler_Abstract {

  /**
   * Generate a settings form for this handler.
   */
  public function settingsForm($field, $instance) {
    $field_name = $field['field_name'];

    $form['action'] = array(
      '#type' => 'select',
      '#title' => t('Action'),
      '#options' => array(
        'none' => t('Do nothing'),
        'hide' => t('Hide field'),
        'disable' => t('Disable field'),
      ),
      '#description' => t('Action to take when prepopulating field with values via Current Entity(Menu Object).'),
    );
    $form['action_on_edit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Apply action on edit'),
      '#description' => t('Apply action when editing an existing entity.'),
      '#states' => array(
        'invisible' => array(
          ':input[name="instance[settings][behaviors][current][action]"]' => array('value' => 'none'),
        ),
      ),
    );
    $form['fallback'] = array(
      '#type' => 'select',
      '#title' => t('Fallback behaviour'),
      '#description' => t('Determine what should happen if no values are provided via via Current Entity(Menu Object).'),
      '#options' => array(
        'none' => t('Do nothing'),
        'hide' => t('Hide field'),
        'form_error' => t('Set form error'),
        'redirect' => t('Redirect'),
      ),
    );

    // Get list of permissions.
    $perms = array();
    $perms[0] = t('- None -');
    foreach (module_list(FALSE, FALSE, TRUE) as $module) {
      // By keeping them keyed by module we can use optgroups with the
      // 'select' type.
      if ($permissions = module_invoke($module, 'permission')) {
        foreach ($permissions as $id => $permission) {
          $perms[$module][$id] = strip_tags($permission['title']);
        }
      }
    }

    $form['skip_perm'] = array(
      '#type' => 'select',
      '#title' => t('Skip access permission'),
      '#description' => t('Set a permission that will not be affected by the fallback behavior.'),
      '#options' => $perms,
    );
    $form['use_uid'] = array(
        '#type' => 'checkbox',
        '#title' => t('Use the Author for an entity if target type is user.'),
        '#description' => t('If the target type is User but the current page is a Node select the Author as the entity.'),
    );

    return $form;
  }
}
