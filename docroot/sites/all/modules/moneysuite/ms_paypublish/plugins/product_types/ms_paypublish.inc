<?php

$fields = field_info_fields();
$bundles = field_info_bundles('node');
$field_options = array();
foreach ($fields as $field) {
  if (isset($field['bundles']['node']) AND !empty($field['bundles']['node'])) {
    $supported_bundles = array();
    foreach ($field['bundles']['node'] as $bundle_name) {
      if (!empty($bundles[$bundle_name])) {
        $supported_bundles[$bundle_name] = $bundles[$bundle_name]['label'];
      }
    }
    // Let's include this field in the list.
    $field_options[$field['field_name']] = t("@field_name : Used in @bundles",
      array('@field_name' => $field['field_name'], '@bundles' => implode(', ', $supported_bundles)));
  }
}

/**
 * @TODO set URL for tutorials when they exist
 */
$plugin = array(
  'plan_name' => t('Pay to Publish Plan'),
  'plan_name_plural' => t('Pay to Publish Plans'),
  'purchase_name' => t('Paid Content'),
  'purchase_name_plural' => t('Paid Content'),
  'allow_admin_add_purchase' => TRUE,
  'custom_elements' => array(
    'fields_settings' => array(
      '#type' => 'fieldset',
      '#title' => t('Fields Settings'),
      '#description' => t("To see a video explaining how to use this feature, click <a href='@url'>here</a>.",
        array('@url' => url(''))),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    ),
  ),
  'plan_options' => array(
    'sticky' => array(
      'title' => t('Make sticky'),
      'description' => t('Make your content sticky so that it will stay at the top of lists.'),
      'optional' => TRUE,
    ),
    'promote' => array(
      'title' => t('Promote content'),
      'description' => t('Promote your content to the front page.'),
      'optional' => TRUE,
    ),
  ),
  'purchase_fields' => array(
    'nid' => array(
      'data_type' => 'node',
      'title' => t('Node'),
      'description' => t('The node associated with this record.'),
      'autocomplete_path' => 'ms_core/autocomplete/node',
      'autocomplete_transform' => 'ms_core_create_autocomplete_key',
      'autocomplete_extract' => 'ms_core_extract_nid_from_autocomplete_key',
      'field_type' => 'entityreference',
      'field_value_column' => 'target_id',
      'token_type' => 'node',
      'token_callback' => 'ms_paypublish_load_node_from_purchase',
    ),
  ),
  'purchase_tokens' => array(
    'publish_uri' => array(
      '#type' => 'value',
      '#data_type' => 'uri',
      '#title' => t('Publish URL'),
      '#description' => t('The url where the user can choose the publishing plan.'),
      '#default_value' => '',
      '#getter_callback' => 'ms_paypublish_publish_uri_getter_callback',
    ),
    'denial_notes' => array(
      '#type' => 'value',
      '#data_type' => 'text',
      '#title' => t('Denial notes'),
      '#description' => t('The notes entered by the admin when content in the approval queue is denied.'),
      '#default_value' => '',
    ),
  ),
  'purchase_statuses' => array(
    'approval_queue' => t('Approval queue'),
    'approval_denied' => t('Approval denied'),
  ),
  'plan_fields' => array(
    'show_add' => array(
      '#type' => 'checkbox',
      '#data_type' => 'boolean',
      '#array_parents' => array('product'),
      '#title' => t('Offer on Publish and Promote / Extend pages?'),
      '#description' => t("This will allow users to publish or Promote / Extend to existing content using this plan after the content has been created."),
      '#default_value' => 1,
    ),
    'allowed_fields' => array(
      '#type' => 'checkboxes',
      '#array_parents' => array('fields_settings'),
      '#title' => t('Allowed Fields'),
      '#data_type' => 'text',
      '#options' => $field_options,
      '#getter_callback' => 'ms_paypublish_allowed_fields_getter_callback',
      '#default_value' => array(),
      '#description' => t("Select which fields are allowed when using this plan. If no fields are checked, all fields will be allowed."),
    ),
  ),
  'emails' => array(
    'payment' => array(
      'subject' => t('Payment Received'),
      'body' => t('Your payment of [ms_core_payment:paymentAmount] has been received and [ms_products_plan:main-unit] [ms_products_plan:main-unit] have been added to your content: [ms_products_purchase:ms-paypublish-nid:title] ([ms_products_purchase:ms-paypublish-nid:url:absolute]).
It will expire on [ms_products_purchase:expiration:short].
To purchase additional time, please click here: [ms_products_purchase:ms-paypublish-publish-uri]'),
    ),
    'cancel' => array(
      'subject' => t('Recurring Payments Cancelled'),
      'body' => t("Your recurring payments for [ms_products_purchase:ms-paypublish-nid:title] ([ms_products_purchase:ms-paypublish-nid:url:absolute]) have been cancelled. Your content will expire [ms_products_purchase:expiration:short]"),
    ),
    'expiring' => array(
      'subject' => t('Content Expiring Soon!'),
      'body' => t('Your Content: [ms_products_purchase:ms-paypublish-nid:title] ([ms_products_purchase:ms-paypublish-nid:url:absolute]) is expiring soon.
To Promote / Extend, click here: [ms_products_purchase:ms-paypublish-publish-uri]'),
    ),
    'expire' => array(
      'subject' => t('Content Expired'),
      'body' => t('Your Content: [ms_products_purchase:ms-paypublish-nid:title] ([ms_products_purchase:ms-paypublish-nid:url:absolute]) is expired.
To republish, click here: [ms_products_purchase:ms-paypublish-publish-uri]'),
    ),
    'approval' => array(
      'extra' => TRUE,
      'subject' => t('Content Published'),
      'subject_title' => t('Publishing Approved Subject'),
      'body_title' => t('Publishing Approved Body'),
      'body_description' => t('This email is sent when the admin approves a piece of content in the approval queue.'),
      'body' => t('Your content has been approved for publishing: [ms_products_purchase:ms-paypublish-nid:title] ([ms_products_purchase:ms-paypublish-nid:url:absolute]).
Thank you for your patience.'),
    ),
    'denial' => array(
      'extra' => TRUE,
      'subject' => t('Publishing Denied'),
      'subject_title' => t('Publishing Denied Subject'),
      'body_title' => t('Publishing Denied Body'),
      'body_description' => t('This email is sent when the admin denies a piece of content in the approval queue.'),
      'body' => t('Your content has been denied for publishing: [ms_products_purchase:ms-paypublish-nid:title] ([ms_products_purchase:ms-paypublish-nid:url:absolute]).
Reason: [ms_products_purchase:ms-paypublish-denial-notes]'),
    ),
  ),
);