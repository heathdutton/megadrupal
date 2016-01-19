<?php

$roles = (user_roles(TRUE));
unset($roles[DRUPAL_AUTHENTICATED_RID]);

$plugin = array(
  'plan_name' => t('Membership Plan'),
  'plan_name_plural' => t('Membership Plans'),
  'purchase_name' => t('Membership'),
  'purchase_name_plural' => t('Memberships'),
  'custom_elements' => array(
    'roles_settings' => array(
      '#type' => 'fieldset',
      '#title' => t('Roles Settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#group' => 'vertical_tabs',
    ),
    'advanced_roles' => array(
      '#type' => 'fieldset',
      '#title' => t('Advanced Roles Settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#group' => 'vertical_tabs',
      '#parents' => array('roles_settings'),
    ),
  ),
  'plan_fields' => array(
    'show_registration' => array(
      '#type' => 'checkbox',
      '#data_type' => 'boolean',
      '#title' => t('Show on New User Registration Screen?'),
      '#array_parents' => array('product'),
      '#default_value' => TRUE,
      '#description' => t("Adds this membership plan to the user registration screen, allowing new users to sign up for this membership plan at registration."),
    ),
    'show_account' => array(
      '#type' => 'checkbox',
      '#data_type' => 'boolean',
      '#title' => t('Show in the User Account?'),
      '#array_parents' => array('product'),
      '#default_value' => TRUE,
      '#description' => t("Adds this membership plan to the user account screen, allowing users to upgrade their membership."),
    ),
    'start_grant_roles' => array(
      '#type' => 'checkboxes',
      '#data_type' => 'text',
      '#title' => t('Role(s) to Grant at Start'),
      '#array_parents' => array('roles_settings'),
      '#description' => t("Role(s) to be assigned when the Membership starts."),
      '#options' => $roles,
      '#default_value' => array(),
      '#getter_callback' => 'ms_products_list_value_getter_callback',
    ),
    'expire_remove_roles' => array(
      '#type' => 'checkboxes',
      '#data_type' => 'text',
      '#title' => t('Role(s) to Remove at Expiration'),
      '#array_parents' => array('roles_settings'),
      '#description' => t("Role(s) to be removed when the Membership expires."),
      '#options' => $roles,
      '#default_value' => array(),
      '#getter_callback' => 'ms_products_list_value_getter_callback',
    ),
    'start_remove_roles' => array(
      '#type' => 'checkboxes',
      '#data_type' => 'text',
      '#title' => t('Role(s) to Remove at Start'),
      '#array_parents' => array('advanced_roles'),
      '#description' => t("Role(s) to be removed when the Membership starts."),
      '#options' => $roles,
      '#default_value' => array(),
      '#getter_callback' => 'ms_products_list_value_getter_callback',
    ),
    'expire_grant_roles' => array(
      '#type' => 'checkboxes',
      '#data_type' => 'text',
      '#title' => t('Role(s) to Grant at Expiration'),
      '#array_parents' => array('advanced_roles'),
      '#description' => t("Role(s) to be assigned when the Membership expires."),
      '#options' => $roles,
      '#default_value' => array(),
      '#getter_callback' => 'ms_products_list_value_getter_callback',
    ),
    'register_urls' => array(
      '#type' => 'textfield',
      '#data_type' => 'text',
      '#title' => t('Special Registration URLs'),
      '#array_parents' => array('advanced'),
      '#description' => t("To use, enter a comma separated list of URLs that the membership,
        plan should be shown on besides the regular one. For example, if I wanted
        this membership plan to show on the 'user/register/promotion' and the
        'user/register/package' pages, I would enter 'promotion,package'. Profile
        2 Registration Path is also supported, so if the path is 'premium/register',
        you can enter 'premium' here to show the plan on that page."),
      '#required' => FALSE,
      '#default_value' => '',
    ),
  ),
);
