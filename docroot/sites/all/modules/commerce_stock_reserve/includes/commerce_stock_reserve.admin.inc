<?php
/**
 * @file
 * Administration-related functions for the Commerce Stock Reserve module.
 */

/**
 * Form builder function for the module's configuration form.
 */
function commerce_stock_reserve_config_form($form, &$form_state) {
  $form['commerce_stock_reserve_expire_on_cron'] = array(
    '#type' => 'checkbox',
    '#title' => t('Release old reserved stock on cron'),
    '#description' => t('If enabled, reserved stock will be processed during cron runs. Old line items will be deleted after their stock is released. This guards against underselling, when users still have stock reserved in their shopping carts when sales close.'),
    '#default_value' => variable_get('commerce_stock_reserve_expire_on_cron', TRUE),
  );

  $form['cron'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cron settings'),
    '#description' => t('You can combine this approach with the !Commerce Cart Expiration module.', array(
      '!Commerce Cart Expiration' => l(
        t('Commerce Cart Expiration'),
        'https://drupal.org/project/commerce_cart_expiration'
      ),
    )),
    '#states' => array(
      'visible' => array(
        'input[name="commerce_stock_reserve_expire_on_cron"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['cron']['commerce_stock_reserve_expire_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Interval'),
    '#description' => t('The interval after which stock should be released. The default is 1800 seconds (half an hour).'),
    '#size' => 6,
    '#maxlength' => 8,
    '#field_suffix' => ' ' . t('seconds'),
    '#default_value' => variable_get('commerce_stock_reserve_expire_interval', 1800),
    '#element_validate' => array('element_validate_integer_positive'),
  );
  $form['cron']['commerce_stock_reserve_expire_date_ref'] = array(
    '#type' => 'select',
    '#title' => t('Reference date'),
    '#description' => t('The date and time that the interval relates to.'),
    '#options' => commerce_stock_reserve_expire_date_ref_options_list(),
    '#default_value' => variable_get('commerce_stock_reserve_expire_date_ref', 'order_changed'),
  );
  $form['cron']['commerce_stock_reserve_expire_restore'] = array(
    '#type' => 'checkbox',
    '#title' => t('Restore released stock to the product.'),
    '#description' => t("Whether to increment the product's stock quantity when the reserved stock is released."),
    '#default_value' => variable_get('commerce_stock_reserve_expire_restore', TRUE),
  );
  $form['cron']['commerce_stock_reserve_expire_only_cart'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only release stock from shopping carts'),
    '#description' => t("Only release stock on cron if the line item's order still has a 'cart' status."),
    '#default_value' => variable_get('commerce_stock_reserve_expire_only_cart', TRUE),
  );
  $form['cron']['commerce_stock_reserve_expire_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Limit'),
    '#description' => t('The maximum number of line items to process during a single cron run.'),
    '#size' => 3,
    '#maxlength' => 4,
    '#default_value' => variable_get('commerce_stock_reserve_expire_limit', 50),
    '#element_validate' => array('element_validate_integer_positive'),
  );
  return system_settings_form($form);
}
