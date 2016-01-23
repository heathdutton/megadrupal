<?php
/**
 * @file
 * This is a class to house our custom plugin details.
 */

class PasswordRecoveryPlugin {

  public static $defaultTextFieldLabel = 'Username or E-mail Address';
  public static $defaultSubmitButtonLabel = 'Submit';
  public static $ctoolsCategory = 'Widgets';
  public static $ctoolsContentTypeName = 'password_recovery_widget_content_type';
  public static $description = 'Embed the password recovery form.';
  public static $icon = 'icon_password_recovery.png';
  public static $adminTitle = 'Password Recovery';

  /**
   * Generates the Ctools info array for the register plugin process.
   *
   * @return array
   *   The Ctools plugin definition.
   */
  public static function generateCtoolsPluginInfo() {
    return array(
      'title' => self::$adminTitle,
      'description' => self::$description,
      // 'single' => TRUE means has no subtypes.
      'single' => TRUE,
      // Constructor.
      'content_types' => array(self::$ctoolsContentTypeName),
      // Name of a function which will render the block.
      'render callback' => 'password_recovery_widget_content_type_render',
      // The default context.
      'defaults' => array(),
      // This explicitly declares the config form.
      // Without this line, the func would be
      // ctools_plugin_example_password_recovery_content_type_edit_form.
      'edit form' => 'password_recovery_widget_content_type_edit_form',
      // Icon goes in the directory with the content type.
      'icon' => self::$icon,
      'category' => self::$ctoolsCategory,
            // This example does not provide 'admin info',
            // which would populate the
            // panels builder page preview.
    );
  }

  /**
   * Creates a stdClass that Ctools uses for HTML via the "content" property.
   *
   * @param String $subtype
   *   Ctools subtype.
   * @param array $conf
   *   Ctools conf.
   * @param array $args
   *   Ctools args.
   * @param String $context
   *   Ctools context.
   * 
   * @return \stdClass 
   *   Returns a stdClass that should at least have a "content" property.
   */
  public static function renderBlock($subtype, $conf, $args, $context) {
    $block = new stdClass();

    if (!empty($conf)) {
      module_load_include('inc', 'user', 'user.pages');
      $textfield_label = (empty($conf['password_recovery_widget_field_label']) ? self::$defaultTextFieldLabel : $conf['password_recovery_widget_field_label']);
      $submit_label = (empty($conf['password_recovery_widget_submit_label']) ? self::$defaultSubmitButtonLabel : $conf['password_recovery_widget_submit_label']);

      $form = drupal_get_form('user_pass');
      $form['name']['#title'] = t(check_plain($textfield_label));
      $form['actions']['submit']['#value'] = t(check_plain($submit_label));
      $block->content = drupal_render($form);
    }
    return $block;
  }

  /**
   * Called by Ctools to generate a form used for configuring this plugin.
   *
   * @param array $form
   *   Drupal form array.
   * @param array $form_state
   *   Drupal form state array.
   *
   * @return array
   *   A Form array.
   */
  public static function editForm($form, &$form_state) {
    $conf = $form_state['conf'];
    $textfield_label = (empty($conf['password_recovery_widget_field_label']) ? self::$defaultTextFieldLabel : $conf['password_recovery_widget_field_label']);
    $submit_label = (empty($conf['password_recovery_widget_submit_label']) ? self::$defaultSubmitButtonLabel : $conf['password_recovery_widget_submit_label']);

    $form['password_recovery_widget_field_label'] = array(
      '#type' => 'textfield',
      '#title' => t('Textfield Label'),
      '#size' => 50,
      '#required' => TRUE,
      '#description' => t('The password recovery textfield label.'),
      '#default_value' => !empty($textfield_label) ? $textfield_label : '',
      '#prefix' => '<div class="clear-block no-float">',
      '#suffix' => '</div>',
    );
    $form['password_recovery_widget_submit_label'] = array(
      '#type' => 'textfield',
      '#title' => t('Button Label'),
      '#size' => 50,
      '#required' => TRUE,
      '#description' => t('The submit button label value.'),
      '#default_value' => !empty($submit_label) ? $submit_label : '',
      '#prefix' => '<div class="clear-block no-float">',
      '#suffix' => '</div>',
    );
    return $form;
  }

  /**
   * Handles the edit form submissoin process.
   *
   * @param array $form
   *   Drupal form array.
   * @param array $form_state
   *   Drupal form state array.
   */
  public static function editFormSubmit($form, &$form_state) {

    foreach (element_children($form) as $key) {
      if (!empty($form_state['values'][$key])) {
        $form_state['conf'][$key] = $form_state['values'][$key];
      }
    }
  }
}
