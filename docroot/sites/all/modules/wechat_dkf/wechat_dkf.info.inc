<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class WechatKfMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info['wechat_kf']['properties'];

    $properties['kf_account'] = array(
      'type' => 'text',
      'label' => t('kf_account'),
      'description' => t('kf account of wechat kf.'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat kf',
      'schema field' => 'kf_account',
      'required' => TRUE,
    );

    $properties['nickname'] = array(
      'type' => 'text',
      'label' => t('nickname'),
      'description' => t('nickname of wechat kf.'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat kf',
      'schema field' => 'nickname',
      'required' => TRUE,
    );
	
    $properties['password'] = array(
      'type' => 'text',
      'label' => t('password'),
      'description' => t('password of wechat kf.'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat kf',
      'schema field' => 'password',
      'required' => TRUE,
    );
	
    $properties['kf_id'] = array(
      'type' => 'text',
      'label' => t('kf_id'),
      'description' => t('kf_id of wechat kf.'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat kf',
      'schema field' => 'kf_id',
      'required' => TRUE,
    );
    $properties['id'] = array(
      'label' => t('Wechat Kf ID'),
      'description' => t('Unique ID of wechat kf.'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer wechat kf',
      'type' => 'integer',
      'schema field' => 'id',
    );

    return $info;
  }
}

