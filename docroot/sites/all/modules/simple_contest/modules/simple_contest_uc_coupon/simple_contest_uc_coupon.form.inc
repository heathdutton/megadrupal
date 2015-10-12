<?php
// $Id: simple_contest_uc_coupon.form.inc,v 1.1.2.2 2010/07/09 04:36:13 vinoth.3v Exp $
/**
 * @file
 *
 * simple_contest_uc_couponn.form.inc 
 */


/**
 * Implementation of hook_form_alter().
 */
function _simple_contest_uc_coupon_simple_contest_content_type_settings_form_alter(&$form, $form_state, $form_id) {

    $type = $form['type']['#value'];
    $settings = simple_contest_get_settings($type);

    $form['simple_contest']['prize_coupons'] = array(
			'#type' => 'fieldset',
			'#title' => t('Prize Coupons'),
			'#collapsible' => TRUE,
			'#collapsed' => FALSE,
			'#description' => t('Ubercart coupons will be created for each winners based on the settings below. Enter values without symbols, for $15, enter "15" and choose Price as the discount type.'),
			'#weight' => 5,
    );

    $form['simple_contest']['prize_coupons']['uc_coupon_type'] = array(
			'#type' => 'select',
			'#title' => t('Coupon type'),
			'#default_value' => $settings['uc_coupon_type'],
			'#options' => array(
				'price' => 'Price',
				'percentage' => 'Percentage',				
    ),
    );

    $form['simple_contest']['prize_coupons']['uc_coupon_values'] = array(
			'#type' => 'fieldset',
			'#title' => t('Prize Coupons'),
			'#collapsible' => TRUE,
			'#collapsed' => FALSE,
			'#weight' => 5,
			'#tree' => TRUE,
    );

    for($count = 1; $count <= $settings['max_winners']; $count++){
      $form['simple_contest']['prize_coupons']['uc_coupon_values']['value_' . $count] = array(
				'#type' => 'textfield',
				'#title' => t('coupon value for winner ' . $count),
				'#size' => 30,
				'#default_value' => $settings['uc_coupon_values']['value_' . $count],
				'#required' => TRUE,
				'#weight' => $count,
      );
    }
    $options = array();
    for($i; $i<=31; $i++){
      $options[$i] = $i;
    }
    $form['simple_contest']['prize_coupons']['uc_coupon_expire_length'] = array(
			'#type' => 'textfield',
			'#title' => t('Expire after'),
			'#default_value' => $settings['uc_coupon_expire_length'],
			'#options' => $options,
    );
    $form['simple_contest']['prize_coupons']['uc_coupon_expire'] = array(
			'#type' => 'select',
			'#default_value' => $settings['uc_coupon_expire'],
			'#options' => array(
				'day' => 'Day(s)',
				'week' => 'Week(s)',
				'month' => 'Month(s)',
				'year' => 'Year(s)',
    ),
    );
    $form['#submit'][] = '_simple_contest_uc_coupon_content_type_settings_form_submit';
    $form['#validate'][] = '_simple_contest_uc_coupon_content_type_settings_form_validate';

}
