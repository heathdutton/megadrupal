<?php

/**
 * @file
 * Admin settings for bibly module.
 */

/**
 * Form callback; bibly admin form.
 */
function bibly_admin_form($form) {

  $form['bibly_location'] = array(
    '#type' => 'select',
    '#title' => t('Insert bibly'),
    '#options' => array(
      'header' => t('Head'),
      'footer' => t('Footer'),
    ),
    '#default_value' => variable_get('bibly_location', 'footer'),
    '#description' => t('Whether the scripts are inserted in the head or footer of the page.'),
  );

  $form['bibly_linkVersion'] = array(
    '#type' => 'select',
    '#title' => t('Linked version'),
    '#options' => array(
      '' => t('None'),
      'ESV' => t('ESV - English Standard Version'),
      'HCSB' => t('HCSB - Holman Christian Standard Bible'),
      'KJV' => t('KJV - King James Version'),
      'NASB' => t('NASB - New American Standard Version'),
      'NCV' => t('NCV - New Century Version'),
      'NJKV' => t('NKJV - New King James Version'),
      'NIV' => t('NIV - New International Version'),
      'NET' => t('NET - New English Translation'),
      'NLT' => t('NLT - New Living Translation'),
      'MSG' => t('MSG - The Message'),
    ),
    '#default_value' => variable_get('bibly_linkVersion', ''),
    '#description' => t('The version selected when a person clicks a link. Leave blank to let the user select a version.'),
  );
  
  $form['bibly_enablePopups'] = array(
    '#type' => 'radios',
    '#title' => t('Enable popups'),
    '#options' => array(
      t('No'),
      t('YES'),
    ),
    '#default_value' => variable_get('bibly_enablePopups', 1), 
    '#description' => t('Determines whether or not the Biblical text is shown in a hover box when a person mouses over a Bible link.'),
  );

  $form['bibly_popupVersion'] = array(
    '#type' => 'select',
    '#title' => t('Popup version'),
    '#options' => array(
      'ESV' => t('ESV - English Standard Version'),
      'KJV' => t('KJV - King James Version'),
      'NET' => t('NET - New English Translation'),
    ),
    '#description' => t('The version to show in the popups.'),
    '#default_value' => variable_get('bibly_popupVersion', 'NET'),
  );

  $form['bibly_classname'] = array(
    '#type' => 'textfield',
    '#title' => t('Class name'),
    '#description' => t('A class name for bible links.'),
    '#default_value' => variable_get('bibly_classname', ''),
  );

  $form['bibly_startNodeId'] = array(
    '#type' => 'textfield',
    '#title' => t('Start node ID'),
    '#description' => t('The DOM ID of an element you want to limit bib.ly to checking. Leave blank to check the entire page (the &lt;body&gt; tag).'),
    '#default_value' => variable_get('bibly_startNodeId', ''),
  );

  return system_settings_form($form);
}