<?php
/**
 * @file
 */
/**
 * function for send all data to gateway.
 */
function page_send_response_data() {
  $api_key     = $_POST['api_key'];
  $api_token   = $_POST['api_token'];
  $data_name   = $_POST['api_key'];
  $order_id    = $_POST['vpc_OrderInfo'];
  $order       = commerce_order_load($order_id);
  $profile_id  = $order->commerce_customer_billing['und'][0]['profile_id'];
  $profile     = commerce_customer_profile_load($profile_id);
  $data_name   = $profile->commerce_customer_address['und'][0]['first_name'];
  $data_amount = $order->commerce_order_total['und'][0]['amount']/100;
  $data_email  = $order->mail;
  $data_phone  = $profile->field_phone_no['und'][0]['value'];
  $virtualpaymentclienturl = $_POST['virtualPaymentClientURL'];
  $vpcurl      = $virtualpaymentclienturl . "?api_key=$api_key&auth_token=$api_token&data_name=$data_name&data_phone=$data_phone&data_email=     $data_email&data_amount=$data_amount&data_readonly=data_amount&intent=buy";
  drupal_goto($vpcurl);
}
