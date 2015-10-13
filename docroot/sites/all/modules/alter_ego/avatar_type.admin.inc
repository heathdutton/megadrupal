<?php

/**
 * @file
 * Model type editing UI.
 */

/**
 * UI controller.
 */
class AvatarTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage model entity types, including adding and removing fields and the display of fields.';
    return $items;
  }
}

/**
 * Generates the model type editing form.
 */
function avatar_type_form($form, &$form_state, $model_type, $op = 'edit') {

  if ($op == 'clone') {
    $model_type->label .= ' (cloned)';
    $model_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $model_type->label,
    '#description' => t('The human-readable name of this model type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($model_type->type) ? $model_type->type : '',
    '#maxlength' => 32,
//    '#disabled' => $model_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'avatar_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this model type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['data']['#tree'] = TRUE;
  $form['data']['use_single_avatar'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only allow user to have one of these avatars?'),
    '#default_value' => (is_array($model_type->data) && array_key_exists('use_single_avatar', $model_type->data))?$model_type->data['use_single_avatar']:0,
    '#description' => t('If this is checked then the user can add one avatar to their account, otherwise add avatar links dissapear.')
  );
  $form['data']['use_popup'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use popup when users hover over a link to this avatar?'),
    '#default_value' => (is_array($model_type->data) && array_key_exists('use_popup', $model_type->data))?$model_type->data['use_popup']:1,
  );
  $form['data']['image_field'] = array('#markup' => t("Avatars can automatically replace the user's picture.  To do so add an image field to this avatar and set that field in this form."));
  // Only show for saved types.
  if (!empty($model_type->type)) {
    // See if there is an image field attached to this type.
    $attached_fields = field_info_instances('avatar', $model_type->type);
    if (!empty($attached_fields)) {
      $image_fields = array('' => '-- none --');
      foreach ($attached_fields as $field_name => $field) {
        if ($field['widget']['type'] == 'image_image') {
          $image_fields[$field_name] = $field_name;
        }
      }
      
      if (count($image_fields) > 1) {
        $form['data']['image_field'] = array(
          '#type' => 'select',
          '#options' => $image_fields,
          '#title' => t('Use the following field as the users picture.'),
          '#default_value' => !empty($model_type->data['image_field'])?$model_type->data['image_field']:'',
        );
      }
    }
  }
  //$form['data']['sample_data'] = array(
  //  '#type' => 'checkbox',
  //  '#title' => t('An interesting model switch'),
  //  '#default_value' => !empty($model_type->data['sample_data']),
  //);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Avatar type'),
    '#weight' => 40,
  );

  //Locking not supported yet
  /*if (!$model_type->isLocked() && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete model type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('model_type_form_submit_delete')
    );
  }*/
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function avatar_type_form_submit(&$form, &$form_state) {
  $avatar_type = entity_ui_form_submit_build_entity($form, $form_state);
  $avatar_type->save();
  $form_state['redirect'] = 'admin/structure/avatar_types';
}

/**
 * Form API submit callback for the delete button.
 */
function avatar_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/avatar_types/manage/' . $form_state['avatar_type']->type . '/delete';
}
