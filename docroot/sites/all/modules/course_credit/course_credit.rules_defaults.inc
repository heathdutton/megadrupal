<?php

/**
 * Implements hook_default_rules_configuration().
 */
function course_credit_default_rules_configuration() {
  $configs = array();
  $credit_types = course_credit_get_credit_types();
  foreach ($credit_types as $type => $credit_type) {
    $set = rules_and(array(
      'course_credit' => array('type' => 'course_credit', 'label' => t('Credit instance')),
      'user' => array('type' => 'user', 'label' => t('User')),
      'course_enrollment' => array('type' => 'course_enrollment', 'label' => t('Course enrollment')),
    ));
    $set->label = t('@type eligibility', array('@type' => $credit_type->title));
    $set->tags[] = 'course_credit';

    $configs["course_credit_eligibility_$type"] = $set;
  }

  return $configs;
}
