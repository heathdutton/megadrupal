<?php
/**
 * @file
 * Select Content type for Weibo AutoPost and select image field for uploading.
 */

/**
 * Weibo AutoPost admin config.
 */
function weibo_autopost_admin_config() {
  $form['global'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['global']['weibo_autopost_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Default enable Weibo AutoPost in node form.'),
    '#default_value' => variable_get('weibo_autopost_enable', 1),
  );
  $form['global']['weibo_autopost_need_publish'] = array(
    '#type' => 'checkbox',
    '#title' => t('Need publish.'),
    '#default_value' => variable_get('weibo_autopost_need_publish', 1),
  );
  $form['global']['weibo_autopost_title_prefix'] = array(
    '#type' => 'textfield',
    '#maxlength' => '3',
    '#size' => '4',
    '#title' => t('Weibo Autopost title prefix.'),
    '#default_value' => variable_get('weibo_autopost_title_prefix', ''),
  );
  $form['global']['weibo_autopost_title_suffix'] = array(
    '#type' => 'textfield',
    '#size' => '4',
    '#maxlength' => '3',
    '#title' => t('Weibo Autopost title suffix.'),
    '#default_value' => variable_get('weibo_autopost_title_suffix', ''),
  );
  $form['config']['content_type'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select content type for Weibo Autopost'),
  );
  $form['config']['content_type']['description'] = array(
    '#value' => t('Weibo Autopost will send [title] + [body] truncated to 120 characters + [node page link]'),
  );
  $contentinfo = node_type_get_types();
  foreach ($contentinfo as $content_type) {
    $field_instances = field_info_instances('node', $content_type->type);
    $allow_field = array();
    foreach ($field_instances as $field) {
      if ($field['widget']['module'] == 'image' || $field['widget']['module'] == 'file') {
        $allow_field[$field['field_id']] = t($field['field_name']);
      }
    }
    $allow_field[0] = t('none');
    $form['config']['content_type'][$content_type->type] = array(
      '#type' => 'fieldset',
      '#title' => t($content_type->type),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['config']['content_type'][$content_type->type]['weibo_autopost_ct_' . $content_type->type] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable this content type for Weibo AutoPost'),
      '#default_value' => variable_get('weibo_autopost_ct_' . $content_type->type, 0),
    );
    $form['config']['content_type'][$content_type->type]['weibo_autopost_field_' . $content_type->type] = array(
      '#title' => t('Select a field for weibo image'),
      '#type' => 'radios',
      '#parents' => array('weibo_autopost_field_' . $content_type->type),
      '#options' => $allow_field,
      '#default_value' => variable_get('weibo_autopost_field_' . $content_type->type, 0),
    );
  }
  return system_settings_form($form);
}
