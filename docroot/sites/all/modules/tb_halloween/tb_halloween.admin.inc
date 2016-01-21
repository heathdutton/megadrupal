<?php 

function tb_halloween_admin_settings($form, &$form_state) {
  $items = tb_halloween_get_items();
  $form = array();
  $form['#tree'] = TRUE;
  $thumbs = tb_halloween_get_thumbs();
  $animation_type = tb_halloween_animation_types_options();
  $closable = tb_halloween_closable_options();
  $flying_speed_options = tb_halloween_get_flying_speed_options();
  $swing_speed_options = tb_halloween_get_swing_speed_options();
  foreach($items as $item) {
    //$form[$item->id]['item'] = $item;
    $arr = get_object_vars($item);
    foreach ($arr as $key => $value) {
      $form[$item->id][$key] = array('#markup' => check_plain($value));
    }
    
    $form[$item->id]['pumpkin_image'] = array('#markup' => '<img src="' . tb_halloween_get_thumb($arr['pumpkin_image'], $arr['size']) . '"/>');
    $form[$item->id]['animation_type'] = array(
      '#markup' => $animation_type[$arr['animation_type']] . ($arr['animation_type'] != 'customize_region' ? "" : ('<br/>{<br/> top: ' . $arr['limit_top'] . ', <br/>right: ' . $arr['limit_right'] . ',<br/>bottom: ' . $arr['limit_bottom'] . ',<br/>left: ' . $arr['limit_left'] . '<br/>}'))
    );
    $form[$item->id]['flying_speed'] = array(
      '#markup' => $flying_speed_options[$arr['flying_speed']],
    );
    $form[$item->id]['swing_speed'] = array(
      '#markup' => $swing_speed_options[$arr['swing_speed']],
    );
    $form[$item->id]['closable'] = array(
      '#markup' => $closable[$arr['closable']],
    );
    $form[$item->id]['edit'] = array('#type' => 'link', '#title' => t('edit'), '#href' => "admin/structure/tb_halloween/$item->id/edit");
    $form[$item->id]['delete'] = array('#type' => 'link', '#title' => t('delete'), '#href' => "admin/structure/tb_halloween/$item->id/delete");
  }
  return $form;
}

function theme_tb_halloween_admin_settings($variables) {
  $form = $variables['form'];
  $rows = array();
  foreach (element_children($form) as $key) {
    if (isset($form[$key]['id'])) {
      $item = &$form[$key];
      $row = array();
      $row[] = drupal_render($item['pumpkin_image']);
      $row[] = drupal_render($item['number_pumpkins']);
      $row[] = drupal_render($item['hover_message']);
      $row[] = drupal_render($item['animation_type']);
      $row[] = drupal_render($item['flying_speed']);
      $row[] = drupal_render($item['swing_speed']);
      $row[] = drupal_render($item['delay_time']);
      $row[] = drupal_render($item['closable']);
      $row[] = drupal_render($item['extend_class']);
      $row[] = drupal_render($item['edit']);
      $row[] = drupal_render($item['delete']);
      $rows[] = array('data' => $row, 'class' => array('draggable'));
    }
  }
  $header = array(
    t('Image'),
    t('Number'),
    t('Message'),
    t('Animation Type'),
    t('Animation Speed'),
    t('Wing Speed'),
    t('Pause Time'),
    t('closable'),
    t('CSS Class'),
  );
  $header[] = array('data' => t('Actions'), 'colspan' => '2');
  return theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No item available. <a href="@link">Add item</a>.', array('@link' => url('admin/structure/tb_halloween/add'))), 'attributes' => array('id' => 'imenu')));
}

function tb_halloween_default_pumpkin() {
  $item = new stdClass();
  $item->pumpkin_image = 'pumpkin';
  $item->size = 'default';
  $item->number_pumpkins = 1;
  $item->hover_message = t("Wish you a scary Halloween!");
  $item->animation_type = 'random';
  $item->preset = 'custom_preset';
  $item->custom_preset = '300, 300, 100, 100, 1000, 100, 1000, 500, 100, 500';
  $item->animation_area = '';
  $item->flying_speed = 'default';
  $item->swing_speed = 'default';
  $item->delay_time = 200;
  $item->delaystart_time = 1000;
  $item->start_frame = 0;
  $item->closable = 'no';
  $item->extend_class = '';
  return $item;
}

function tb_halloween_add_form($form, &$form_state, $item = NULL) {
  $form = array();
  $current_item = $item;
  $default_item = tb_halloween_default_pumpkin();
  $item = $item ? $item : $default_item;
  $thumbs = tb_halloween_get_thumbs();
  $images_select = array();
  $images_select[] = '<div class="form-item form-type-select form-item-image">
  <label for="edit-image">';
  $images_select[] = t('Select Image');
  $images_select[] = '<span title="This field is required." class="form-required">*</span></label>
  <div class="form-select required" name="image" id="edit-image">';
  foreach($thumbs as $key => $detail) {
  	$thumb = $detail['thumb'];
  	$selected = ((!$current_item && $key == 'pumpkin') || $key == $item->pumpkin_image ? 'selected' : '');
    $images_select[] = '<a href="javascript:void(0)" class="' . $selected . '" onclick="Drupal.TBHalloween.selectImage(this);"><img src="' . tb_halloween_get_thumb($key, 'default') . '"/>
      <input type="hidden" name="select-image" value="' . $key . '"/>
    </a>';
  }
  $images_select[] = '</div>';
  $images_select[] = '<div class="description">';
  $images_select[] = t('Select the image for your animation.');
  $images_select[] = '</div>';
  $images_select[] = '</div>';
  
  $form["select-image"] = array(
    '#markup' => implode("", $images_select),
  );
  $form["pumpkin_image"] = array(
    '#type' => 'hidden',
    '#value' => isset($item->pumpkin_image) ? $item->pumpkin_image : $default_item->pumpkin_image,
  );

  $form["size"] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t("Image Size"),
    '#description' => t('Set the size of the image.'),
    '#options' => tb_halloween_sizes_options(),
    '#value' => isset($item->size) ? $item->size : $default_item->size,
    '#default_value' => isset($item->size) ? $item->size : $default_item->size,
  );

  $form["hover_message"] = array(
    '#type' => 'textarea',
    '#required' => TRUE,
    '#title' => t("Message"),
    '#description' => t('Message which will be displayed when you hover the image. You can use HTML, too.'),
    '#value' => isset($item->hover_message) ? $item->hover_message : $default_item->hover_message,
  );

  $form["animation_type"] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t("Animation Type"),
    '#description' => t("Select from 2 defined animation types.<br>(1) <strong>Random</strong>: The image will follow random positions.<br>(2) <strong>Path</strong>: The image follows a predefined animation path."),
    '#options' => tb_halloween_animation_types_options(),
    '#value' => isset($item->animation_type) ? $item->animation_type : $default_item->animation_type,
    '#default_value' => isset($item->animation_type) ? $item->animation_type : $default_item->animation_type,
  );
  
  $form["presets_wrapper"] = array(
    '#type' => 'container',
    '#required' => TRUE,
    '#states' => array(
      'visible' => array(
        'select[name="animation_type"]' => array(
          'value' => 'preset',
        ),
      ),
    ),
  );

  $form["presets_wrapper"]['preset'] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t("Path"),
    '#options' => tb_halloween_presets_options(),
    '#value' => isset($item->preset) ? $item->preset : $default_item->preset,
    '#default_value' => isset($item->preset) ? $item->preset : $default_item->preset,
    '#description' => t("Select from a set of pre-defined paths or create your custom animation path."),
  );
  
  $form["presets_wrapper"]["custom_preset_wrapper"] = array(
    '#type' => 'container',
    '#required' => TRUE,
    '#states' => array(
      'visible' => array(
        'select[name="preset"]' => array(
          'value' => 'custom_preset',
        ),
      ),
    ),
  );

  $form["presets_wrapper"]["custom_preset_wrapper"]['custom_preset'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t("Custom Path"),
    '#value' => isset($item->custom_preset) ? $item->custom_preset : $default_item->custom_preset,
    '#description' => t("Define your custom animation path. Add path coordinates as followed x1, y1, x2, y2, x3, y3, ..."),
  );

  $form["random_wrapper"] = array(
    '#type' => 'container',
    '#required' => TRUE,
    '#states' => array(
      'visible' => array(
        'select[name="animation_type"]' => array(
          'value' => 'random',
        ),
      ),
    ),
  );

  $form["random_wrapper"]["number_pumpkins"] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t("Number of Instances"),
    '#description' => t('Select whether you want to have more than 1 animation instance of the same image.'),
    '#options' => tb_halloween_number_pumpkins_options(),
    '#value' => isset($item->number_pumpkins) ? $item->number_pumpkins : $default_item->number_pumpkins,
    '#default_value' => isset($item->number_pumpkins) ? $item->number_pumpkins : $default_item->number_pumpkins,
  );

  $form["random_wrapper"]["animation_area"] = array(
    '#type' => 'textfield',
    '#title' => t("Animation Area"),
    '#description' => t('Defines a rectangular boundry area with 4 coordinates "Left, Top, Right, Bottom". The animation is limited to this area. If you leave it empty the full viewport will be used.'),
    '#options' => tb_halloween_number_pumpkins_options(),
    '#value' => isset($item->animation_area) ? $item->animation_area : $default_item->animation_area,
    '#default_value' => isset($item->animation_area) ? $item->animation_area : $default_item->animation_area,
  );
  
  $form["flying_speed"] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t("Animation Speed"),
    '#description' => t("Select the pre-defined animation speed. Default, Fast, Slow."),
    '#options' => tb_halloween_get_flying_speed_options(),
    '#value' => isset($item->flying_speed) ? $item->flying_speed : $default_item->flying_speed,
    '#default_value' => isset($item->flying_speed) ? $item->flying_speed : $default_item->flying_speed,
  );

  $form["swing_speed"] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t("Wing speed"),
    '#description' => t("Select from Default, Slow, Fast to set the wing speed."),
    '#options' => tb_halloween_get_swing_speed_options(),
    '#value' => isset($item->swing_speed) ? $item->swing_speed : $default_item->swing_speed,
    '#default_value' => isset($item->swing_speed) ? $item->swing_speed : $default_item->swing_speed,
  );

  $form["delay_time"] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t("Pause Time"),
    '#description' => t("On the animation path the image will pause. You can set the pausing time of the animation."),
    '#value' => isset($item->delay_time) ? $item->delay_time : $default_item->delay_time,
  );

  $form["delaystart_time"] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t("Delay Time"),
    '#description' => t("Animation will start after your defined delay time (ms)."),
    '#value' => isset($item->delaystart_time) ? $item->delaystart_time : $default_item->delaystart_time,
  );

  $form["start_frame"] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t("Start Frame"),
    '#description' => t("The animation is set in frames e.g. 45 frames/second. Set the frame number which you want the animation to start with. In case you have more than 1 animation this would be an option to display delayed frames for each flying image (to avoid synchronized animation). It will look more chaotic ;)"),
    '#value' => isset($item->start_frame) ? $item->start_frame : $default_item->start_frame,
  );

  $form["closable"] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t("Closable"),
    '#options' => tb_halloween_closable_options(),
    '#description' => t("Allow user to remove the animated images at the front-end."),
    '#value' => isset($item->closable) ? $item->closable : $default_item->closable,
    '#default_value' => isset($item->closable) ? $item->closable : $default_item->closable,
  );

  $form["extend_class"] = array(
    '#type' => 'textfield',
    '#title' => t("Custom CSS class"),
    '#description' => t("Add your custom CSS class to write custom styles."),
    '#value' => isset($item->extend_class) ? $item->extend_class : $default_item->extend_class,
  );

  $form['id'] = array(
    '#type' => 'hidden',
    '#required' => TRUE,
    '#value' => isset($item->id) ? $item->id : "",
    '#id' => array(
      'exists' => 'tb_halloween_id_load',
    ),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  return $form;
}

function tb_halloween_add_form_submit($form, &$form_state) {
  $input = $form_state['input'];
  $fields = array(
    'pumpkin_image' => $input['pumpkin_image'],
    'size' => $input['size'],
    'hover_message' => $input['hover_message'],
    'animation_type' => $input['animation_type'],
    'number_pumpkins' => $input['animation_type'] == 'random' ? intval($input['number_pumpkins']) : 1,
    'preset' => $input['preset'],
    'custom_preset' => $input['custom_preset'],
    'animation_area' => $input['animation_area'],
    'flying_speed' => intval($input['flying_speed']),
    'swing_speed' => intval($input['swing_speed']),
    'delay_time' => intval($input['delay_time']),
    'delaystart_time' => intval($input['delaystart_time']),
    'start_frame' => intval($input['start_frame']),
    'swing_speed' => intval($input['swing_speed']),
    'extend_class' => $input['extend_class'],
    'closable' => $input['closable'],
    'id' => $input['id'],
    'changed' => mktime(),
  );
  $item = tb_halloween_id_load($input['id']);
  if ($item) {
  	db_update('tb_halloween_items')
  	  ->fields($fields)
  	  ->condition('id', $item->id)
  	  ->execute();
  }
  else {
  	$fields['id'] = NULL;
  	$fields['created'] = mktime();
  	$query = db_insert('tb_halloween_items')
  	  ->fields($fields)
  	  ->execute();
  }
  $form_state['redirect'] = 'admin/structure/tb_halloween';  
}

function tb_halloween_confirm_delete($form, &$form_state, $item) {
  if($item) {
    // Always provide entity id in the same form key as in the entity edit form.
    $form['id'] = array('#type' => 'value', '#value' => $item->id);
    $form['#id'] = 'tb_halloween_confirm_delete';
    $form['#submit'] = array('tb_halloween_confirm_delete_submit');
    return confirm_form($form,
      t('Are you sure you want to delete the vocabulary %title?',
      array('%title' => $item->id)),
      'admin/structure/tb_halloween',
      t('Deleting an item. This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
    );
  }
}

function tb_halloween_confirm_delete_submit($form, &$form_state) {
  db_delete('tb_halloween_items')->condition('id', $form_state['values']['id'])->execute();
  $form_state['redirect'] = 'admin/structure/tb_halloween';
}
