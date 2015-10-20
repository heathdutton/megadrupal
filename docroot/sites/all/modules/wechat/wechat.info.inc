<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class WechatRequestMessageMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info['wechat_request_message']['properties'];

    $properties['to_user_name'] = array(
      'type' => 'text',
      'label' => t('To user name'),
      'description' => t('To user name of wechat request message.'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat request message',
      'schema field' => 'to_user_name',
      'required' => TRUE,
    );

    $properties['from_user_name'] = array(
      'type' => 'text',
      'label' => t('From user name'),
      'description' => t('From user name of wechat request message.'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat request message',
      'schema field' => 'from_user_name',
      'required' => TRUE,
    );

    $properties['msg_type'] = array(
      'label' => t('Message type'),
      'description' => t('Message type of wechat request message.'),
      'type' => 'token',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat request message',
      'options list' => 'wechat_request_message_type_options_list',
      'required' => TRUE,
      'schema field' => 'msg_type',
    );

    $properties['create_time'] = array(
      'type' => 'date',
      'label' => t('Create time'),
      'description' => t('Create time of wechat request message.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat request message',
      'schema field' => 'create_time',
      'required' => TRUE,
    );

    $properties['msg_id'] = array(
      'label' => t('Message ID'),
      'description' => t('Unique ID of wechat request message.'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat request message',
      'type' => 'text',
      'schema field' => 'msg_id',
    );

    return $info;
  }
}

class WechatResponseMessageMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info['wechat_response_message']['properties'];

    $properties['to_user_name'] = array(
      'type' => 'text',
      'label' => t('To user name'),
      'description' => t('To user name of wechat response message.'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat response message',
      'schema field' => 'to_user_name',
      'required' => TRUE,
    );

    $properties['from_user_name'] = array(
      'type' => 'text',
      'label' => t('From user name'),
      'description' => t('From user name of wechat response message.'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat response message',
      'schema field' => 'from_user_name',
      'required' => TRUE,
    );

    $properties['msg_type'] = array(
      'label' => t('Message type'),
      'description' => t('Message type of wechat response message.'),
      'type' => 'token',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat response message',
      'options list' => 'wechat_request_message_type_options_list',
      'required' => TRUE,
      'schema field' => 'msg_type',
    );

    $properties['create_time'] = array(
      'type' => 'date',
      'label' => t('Create time'),
      'description' => t('Create time of wechat response message.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat response message',
      'schema field' => 'create_time',
      'required' => TRUE,
    );

    $properties['func_flag'] = array(
      'type' => 'integer',
      'label' => t('Function flag'),
      'description' => t('Function flag of wechat response message.'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat response message',
      'schema field' => 'func_flag',
      'required' => TRUE,
    );

    $properties['rm_id'] = array(
      'type' => 'integer',
      'label' => t('wechat request message ID'),
      'description' => t('The ID of wechat request message.'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat response message',
      'schema field' => 'rm_id',
      'required' => FALSE,
    );
    return $info;
  }
}

class WechatUserMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info['wechat_user']['properties'];

    $properties['uid'] = array(
      'type' => 'integer',
      'label' => t('Drupal user ID'),
      'description' => t('The user ID of drupal user.'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat user',
      'schema field' => 'uid',
      'required' => TRUE,
    );

    $properties['openid'] = array(
      'type' => 'text',
      'label' => t('Open id'),
      'description' => t('Openid of wechat user.'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat user',
      'schema field' => 'openid',
      'required' => TRUE,
    );

    $properties['subscribe'] = array(
      'label' => t('subscribe'),
      'description' => t('subscribe status of wechat user.'),
      'type' => 'integer',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat user',
      'schema field' => 'subscribe',
    );

    $properties['nickname'] = array(
      'type' => 'text',
      'label' => t('nickname'),
      'description' => t('nickname of wechat user.'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat user',
      'schema field' => 'nickname',
    );

    $properties['sex'] = array(
      'label' => t('sex'),
      'description' => t('sex of wechat user.'),
      'type' => 'token',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat user',
      'options list' => 'wechat_wechatuser_sex_options_list',
      'schema field' => 'sex',
    );

    $properties['province'] = array(
      'type' => 'text',
      'label' => t('province'),
      'description' => t('province of wechat user.'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat user',
      'schema field' => 'province',
    );

    $properties['city'] = array(
      'type' => 'text',
      'label' => t('city'),
      'description' => t('city of wechat user.'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat user',
      'schema field' => 'city',
    );

    $properties['country'] = array(
      'type' => 'text',
      'label' => t('country'),
      'description' => t('country of wechat user.'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat user',
      'schema field' => 'country',
    );

    $properties['headimgurl'] = array(
      'type' => 'text',
      'label' => t('headimgurl'),
      'description' => t('headimgurl of wechat user.'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat user',
      'schema field' => 'headimgurl',
    );

    $properties['subscribe_time'] = array(
      'type' => 'date',
      'label' => t('Subscribe time'),
      'description' => t('Subscribe time of wechat user.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat user',
      'schema field' => 'subscribe_time',
      'required' => TRUE,
    );

    return $info;
  }
}
