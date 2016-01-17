<?php
/**
 * @file
 * Context Bean plugin.
 */

class ContextBean extends BeanPlugin {
  /**
   * Declares default block settings.
   */
  public function values() {
    $values = array();
    $contexts = context_context_list();
    foreach ($contexts as $context) {
      $values[$context] = array(
        'beans' => '',
        'view_mode' => 'default',
        'css_class' => '',
      );
    }
    return array(
      'contexts' => $values,
      'view_mode' => 'default',
    );
  }
  /**
   * Builds extra settings for the block edit form.
   */
  public function form($bean, $form, &$form_state) {
    $form = array();
    $bean_view_modes = array();
    $entity_info = entity_get_info();
    foreach ($entity_info['bean']['view modes'] as $key => $value) {
      $bean_view_modes[$key] = $value['label'];
    }
    $all_beans = bean_get_all_beans();
    $bean_list = array();
    if (!empty($all_beans)) {
      foreach ($all_beans as $loaded_bean) {
        if ($loaded_bean->bundle() != 'context_bean') {
          $bean_list[$loaded_bean->internalIdentifier()] = $loaded_bean->label();
        }
      }
    }
    $form['contexts'] = array(
      '#type' => 'vertical_tabs',
      '#tree' => TRUE,
    );
    if (!empty($bean_list)) {
      $contexts = context_context_list();
      $i = 0;
      foreach ($contexts as $context) {
        $form['contexts'][$context] = array(
          '#type' => 'fieldset',
          '#tree' => TRUE,
          '#title' => t($context),
          '#group' => 'contexts',
          '#weight' => $i++,
        );
        $form['contexts'][$context]['beans'] = array(
          '#type' => 'select',
          '#title' => t('Beans'),
          '#options' => $bean_list,
          '#default_value' => $bean->contexts[$context]['beans'],
          '#required' => FALSE,
          '#multiple' => TRUE,
        );
        $form['contexts'][$context]['view_mode'] = array(
          '#type' => 'select',
          '#title' => t('View Mode'),
          '#options' => $bean_view_modes,
          '#default_value' => $bean->contexts[$context]['view_mode'],
          '#required' => FALSE,
          '#multiple' => FALSE,
        );
        $form['contexts'][$context]['css_class'] = array(
          '#type' => 'textfield',
          '#title' => t('CSS Class'),
          '#default_value' => $bean->contexts[$context]['css_class'],
          '#size' => 30,
          '#maxlength' => 64,
        );
      }
    }
    else {
      drupal_set_message(t("No beans detected. Make some beans to put inside your context bean."), 'warning');
    }
    return $form;
  }

  /**
   * Displays the context bean.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {
    $contexts = context_get();
    if (!empty($contexts)) {
      $active_contexts = array();
      foreach ($contexts['context'] as $context) {
        $active_contexts[$context->name] = $context->name;
      }
      // Set markup index as empty.
      $content['#markup'] = '';
      // Loop through contexts and extract beans for display.
      foreach ($bean->data['contexts'] as $key => $values) {
        if (in_array($key, $active_contexts)) {
          $display_beans = bean_load_multiple($bean->data['contexts'][$key]['beans']);
          $view_mode = $bean->data['contexts'][$key]['view_mode'];
          $css_class = $bean->data['contexts'][$key]['css_class'];
          //Render each child bean into the markup of context bean.
          foreach ($display_beans as $bean_child) {
            $content['#markup'] .= theme('context_bean_child',
              array(
                'entity' => $bean_child,
                'view_mode' => $view_mode,
                'css_class' => $css_class,
              )
            );
          }
        }
      }
    }
    return $content;
  }
}

