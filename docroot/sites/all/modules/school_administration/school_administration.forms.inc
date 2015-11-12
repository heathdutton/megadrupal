<?php

/**
 * @file
 * form build functions for the school_administration module.
 *
 * Copyright 2009 by Murat Tutumlu ("tutumlum", http://drupal.org/user/413570)
 */

function school_administration_student_withdraw_form() {
  $form['school_no'] = array(
    '#type' => 'textfield',
    '#weight' => 1,
    '#title' => t('School No'),
    '#required' => TRUE,
    '#description' => t('Multiple records are available by separating - (dash). e.g. 2122-2123'),
  );
  $form['withdraw'] = array(
    '#type' => 'fieldset',
    '#weight' => 2,
    '#title' => t('Withdrawal Information'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['withdraw']['date'] = array(
    '#type' => 'date',
    '#weight' => 2,
    '#required' => TRUE,
    '#default_value' => array('day' => date("j"), 'month' => date("n"), 'year' => date("Y")),
    '#title' => t('Withdrawal Date'),
  );
  //an empty filed is needed as default
  $options[''] = t('- Select -'); 
  $taxonomy = taxonomy_get_tree(variable_get('school_administration_withd'));
  foreach ($taxonomy as $option) {
    $options[$option->tid] = $option->name;
  }
  $form['withdraw']['reason'] = array(
    '#type' => 'select',
    '#title' => t('Withdraw Reason'),
    '#weight' => 3,
    '#required' => TRUE,
    '#options' => $options,
  );
  $form['withdraw']['doc'] = array(
    '#type' => 'textfield',
    '#title' => t('Given Documents In Withdrawal'),
    '#weight' => 4,
  );
  $form['withdraw']['dept_left'] = array(
    '#type' => 'textfield',
    '#title' => t('Debt Of Student Left'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['withdraw']['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('No documentation will be given in future'),
    '#weight' => 6,
  );
  $form['withdraw']['notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Notes'),
    '#weight' => 7,
    '#resizable' => FALSE,
    '#cols' => 58,
  );
  $form['submit'] = array(
    '#weight' => 8,
    '#type' => 'submit',
    '#value' => t('Insert Record'),
  );
  return $form;
}

function school_administration_staff_withdraw_form() {
  $form['staff_no'] = array(
    '#type' => 'textfield',
    '#weight' => 1,
    '#title' => t('Staff No'),
    '#required' => TRUE,
    '#size' => 22,
  );
  $form['date'] = array(
    '#type' => 'date',
    '#weight' => 2,
    '#required' => TRUE,
    '#default_value' => array('day' => date("j"), 'month' => date("n"), 'year' => date("Y")),
    '#title' => t('Withdrawal Date'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#weight' => 8,
    '#value' => t('Insert Record'),
  );
  return $form;
}

function school_administration_insert_portrait_form() {
  $form['pdir'] = array(
    '#type' => 'textfield',
    '#weight' => 0,
    '#title' => t('Portrait Photos Directory'),
    '#default_value' => 'portraits',
    '#description' => t('folder name in Drupal files directory'),
  );
  $form['options'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Image options'),
    '#default_value' => array(),
    '#options' => array(
      'keep_temp' => t('Keep results to view as file'),
      'keep_original' => t('Keep original files'),
      'size' => t('Resize files to 300x400 (PHP GD Library is needed)'),
      'face' => t('Find face on the image and crop as portrait (OpenCV is needed)'),
      'method2' => t('Use 2. method to crop portraits. (OpenCV is needed)'),
      'save' => t('Save photos to Database'),
    ),
  );  
  $form['submit'] = array(
    '#type' => 'submit',
    '#id' => 'add-portrait',
    '#value' => t('Insert Photos'),
    '#weight' => 1,
  );
  return $form;
}

function school_administration_admin() {
  $form['school_administration_s_year'] = array(
    '#type' => 'textfield',
    '#title' => t('Starting Year'),
    '#default_value' => variable_get('school_administration_s_year', 2000),
    '#size' => 4,
    '#maxlength' => 4,
    '#description' => t('Starting education year of school history in database'),
  );
  $form['school_administration_c_year'] = array(
    '#type' => 'textfield',
    '#title' => t('Current Academic Year of the system'),
    '#default_value' => SCHOOL_ADMINISTRATION_C_YEAR,
    '#size' => 4,
    '#maxlength' => 4,
    '#description' => t('If academic year is 2000-2001, type 2001<br />
      Default value is <b>!year0</b> before September 1, <b>!year1</b> after August 30.',
        array('!year0' => date("Y"), '!year1' => (date("Y") + 1))
      ),
  );
  $form['school_administration_c_term'] = array(
    '#type' => 'textfield',
    '#title' => t('Current term of the system'),
    '#default_value' => variable_get('school_administration_c_term', 1),
    '#description' => t('Current term of the system'),
  );
  $form['school_administration_new_user_options'] = array(
    '#type' => 'radios',
    '#title' => t("New user's password options"),
    '#key_type' => 'associative',
    '#default_value' => 0,
    '#options' => array(
      t("Print username and password on the screen"),
      t("Send one time login link to the user's e-mail to set password himself / herself"),
    ),
    '#description' => t("If an e-mail server is configured and e-mail address is provided always, it is better to use second option."),
  );
  $form['school_administration_full_name_format'] = array(
    '#type' => 'radios',
    '#title' => t('Full Name Format'),
    '#key_type' => 'associative',
    '#default_value' => 0,
    '#options' => array(
      t("Name1 Name2 Name3 Surname"),
      t("Title Name1 Name2 Name3 Surname"),
      t("SURNAME, Name1 Name2 Name3"),
    ),
    '#description' => t("Choose Name Format to use globally"),
  );
  $options = user_roles(TRUE);
  $form['school_administration_teacher_roles'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Teacher Roles'),
    '#default_value' => variable_get('school_administration_teacher_roles'),
    '#size' => min(12, count($options)),
    '#options' => $options,
    '#description' => t("Select role(s) which members will be visible as teacher in the system."),
  );
  $form['school_no'] = array(
    '#type' => 'fieldset',
    '#title' => t("Student & Staff ID Numbers"),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $num_range_description = t("Divide enrolling students to groups by setting 'g' at student ID format e.g.<br/>
    1 => Grade 7 Girls<br/>
    2 => Grade 7 Girls<br/>
    3 => Grade 7 Boys<br/>
    4 => Grade 7 Boys<br/>
    5 => Other Girls<br/>
    6 => Other Boys");
  $num_range_description_staff = t('Divide hired staff to groups by setting "g" at staff ID format e.g.<br/>
    7 => Administrative Staff<br/>
    8 => Academic Staff<br/>
    Be careful that <b>this should not intersect with "Department digits for students"</b>');

  if (module_exists('php')) {
    $num_range_description .= t('<br/><b>or</b> use PHP code which returns <b>print_r(array())</b> in format: number => Text. Enter PHP code between &lt;?php ?&gt;.');
  }

  $form['school_no']['school_administration_id_conf_use_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Drupal Default'),
    '#default_value' => variable_get('school_administration_id_conf_use_default', 1),
    '#tree' => FALSE,
    '#description' => t("Uncheck this if you want to use the customized Student and Staff ID numbers as Drupal user id instead of Drupal's default user id
      <br />Customization will be in format of yygxx
      <br />yy: last 2 digit of current academic year
      <br />d: group digits
      <br />x: registration order"
    ),
  );
  $form['school_no']['settings'] = array(
    '#type' => 'container',
    '#states' => array(
      // Hide the School No settings when using Drupal default.
      'invisible' => array(
        'input[name="school_administration_id_conf_use_default"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['school_no']['settings']['school_administration_std_id_len'] = array(
    '#type' => 'radios',
    '#title' => t('Number of available digits for registration order'),
    '#key_type' => 'associative',
    '#default_value' => variable_get('school_administration_std_id_len', 2),
    '#options' => array(
      2 => 2,
      3 => 3,
    ),
    '#description' => t("How many x's you want in configured <b>Student and Staff ID numbers</b>
      <br />Default value is 2 (it means you can register maximum 99 students to each group)"
    ),
  );
  $form['school_no']['settings']['school_administration_num_range'] = array(
    '#type' => 'textarea',
    '#title' => t('Department digits for students'),
    '#size' => 1,
    '#default_value' => variable_get('school_administration_num_range'),
    '#description' => $num_range_description,
  );
  $form['school_no']['settings']['school_administration_num_range_staff'] = array(
    '#type' => 'textarea',
    '#title' => t('Staff grouping digits'),
    '#size' => 1,
    '#default_value' => variable_get('school_administration_num_range_staff'),
    '#description' => $num_range_description_staff,
  );
  return system_settings_form($form);
}

function school_administration_staff_info($user_info = NULL) {
  $ranges = array_unique(school_administration_get_id_conf_range('staff'));
  $form['staff'] = array(
    '#type' => 'fieldset',
    '#weight' => -11,
    '#title' => t('Staff Information'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $date = isset($user_info->start_date) ? $user_info->start_date : date("Y-n-j");
  $d = explode('-', $date);
  $form['staff']['start_date'] = array(
    '#type' => 'date',
    '#weight' => 0,
    '#required' => TRUE,
    '#default_value' => array('year' => (int)$d[0], 'month' => (int)$d[1], 'day' => (int)$d[2]),
    '#title' => t('Start Date'),
  );
  if (variable_get('school_administration_id_conf_use_default', 1) != 1 && $user_info == NULL) {
    $form['staff']['num_range'] = array(
      '#type' => 'select',
      '#weight' => 1,
      '#title' => t('Staff ID Range'),
      '#required' => TRUE,
      '#options' => $ranges,
    );
  }
  $form['staff']['branch'] = array(
    '#type' => 'textfield',
    '#size' => 20,
    '#weight' => 3,
    '#title' => t('Branch'),
    '#default_value' => ($user_info == NULL) ? '' : $user_info->branch,
  );
  $form['staff']['contract'] = array(
    '#type' => 'textfield',
    '#size' => 20,
    '#weight' => 5,
    '#title' => t('Contract Type'),
    '#default_value' => ($user_info == NULL) ? '' : $user_info->contract_type,
  );
  $form['user_type'] = array(
    '#type' => 'value',
    '#value' => 'staff',
  );
  return $form;
}
  
function school_administration_parent_info($parenting_id = NULL, $parenting_data = array()){
  $flag = TRUE;
  $taxonomy = taxonomy_get_tree(variable_get('school_administration_relations'));
  foreach ($taxonomy as $option) {
    $options[$option->tid] = $option->name;
  }
  
  if ($parenting_id != NULL) {
    $parent_info = school_administration_parenting_info($parenting_id);
    $parenting_data[$parent_info->school_no] = $parent_info;
  }
  if ($parenting_id == NULL && empty($parenting_data)) {
    $parenting_data[] = array();
    $flag = FALSE;
  }
  foreach ($parenting_data as $school_no => $parent_info) {
    $form['parent_' . $school_no] = array(
      '#type' => 'fieldset',
      '#weight' => -20,
      '#title' => t('Parenting Information for !student (!school_no)', array('!student' => school_administration_formatted_full_name($school_no), '!school_no' => $school_no)),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    if (!$flag) {
      $form['parent_' . $school_no]['school_no_' . $school_no] = array(
        '#type' => 'textfield',
        '#title' => t('School No of Student'),
        '#weight' => 1,
        '#required' => TRUE,
        '#size' => 20,
      );
    }
    $form['parent_' . $school_no]['relation_' . $school_no] = array(
      '#type' => 'select',
      '#title' => t('Relationship'),
      '#weight' => 2,
      '#required' => TRUE,
      '#default_value' => !($flag) ? '' : $parent_info->relation,
      '#options' => $options,
    );
    $form['parent_' . $school_no]['guardian_' . $school_no] = array(
      '#type' => 'checkbox',
      '#title' => t('Guardian'),
      '#default_value' => !($flag) ? '' : $parent_info->guardian,
      '#weight' => 3,
    );
    $form['parent_' . $school_no]['student_resides_' . $school_no] = array(
      '#type' => 'checkbox',
      '#title' => t('Resides with student'),
      '#default_value' => !($flag) ? '' : $parent_info->resides_with_student,
      '#weight' => 3,
    );
  }
  $form['user_type'] = array(
    '#type' => 'value',
    '#value' => 'parent',
  );
  return $form;
}
  
function school_administration_student_info($user_info = NULL) {
  $ranges = array_unique(school_administration_get_id_conf_range('student'));
  $form['student'] = array(
    '#type' => 'fieldset',
    '#weight' => -11,
    '#title' => t('Study Information'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  if ($user_info == NULL) {
    if (variable_get('school_administration_id_conf_use_default', 1) != 1) {
      $form['student']['num_range'] = array(
        '#type' => 'select',
        '#weight' => 0,
        '#title' => t('School No Range'),
        '#required' => TRUE,
        '#options' => $ranges,
      );
    }
    $form['student']['entrance_exam_result'] = array(
      '#type' => 'textfield',
      '#weight' => 3,
      '#size' => 20,
      '#title' => t('Entrance Exam Result'),
    );
    $taxonomy = taxonomy_get_tree(variable_get('school_administration_relations'));
    foreach ($taxonomy as $option) {
      $options[$option->tid] = $option->name;
    }
    $form['student']['student_resides'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Student resides with'),
      '#weight' => 4,
      '#required' => TRUE,
      '#options' => $options,
    );
    $form['sibling'] = array(
      '#type' => 'textfield',
      '#weight' => 3,
      '#size' => 20,
      '#title' => t('Sibling School No'),
    );
  }
  else {
    $form['entrance_exam_result'] = array(
      '#type' => 'value',
      '#value' => $user_info->entrance_exam_result,
    );
  }
  $form['student']['level'] = array(
    '#type' => 'textfield',
    '#weight' => 1,
    '#title' => t('Level'),
    '#size' => 20,
    '#required' => TRUE,
    '#description' => t('Should be an integer'),
    '#default_value' => ($user_info == NULL) ? '' : $user_info->level,
  );
  $date = isset($user_info->enrollment_date) ? $user_info->enrollment_date : date("Y-n-j");
  $d = explode('-', $date);
  $form['student']['enrollment_date'] = array(
    '#type' => 'date',
    '#weight' => 2,
    '#title' => t('Enrollment Date'),
    '#required' => TRUE,
    '#default_value' => array('year' => (int)$d[0], 'month' => (int)$d[1], 'day' => (int)$d[2]),
  );
  $form['scholarship'] = array(
    '#type' => 'fieldset',
    '#weight' => 2,
    '#title' => t('Scholarship Information'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['scholarship']['scholarship'] = array(
    '#type' => 'textfield',
    '#title' => t('Scholarship'),
    '#weight' => 5,
    '#default_value' => ($user_info == NULL) ? '' : $user_info->scholarship,
  );
  $form['scholarship']['scholarship_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Scholarship Description'),
    '#weight' => 6,
    '#resizable' => FALSE,
    '#cols' => 88,
    '#rows' => 2,
    '#default_value' => ($user_info == NULL) ? '' : $user_info->scholarship_description,
  );
  $form['user_type'] = array(
    '#type' => 'value',
    '#value' => 'student',
  );
  return $form;
}

function school_administration_identity_info($user_info = NULL) {
  $form['identity'] = array(
    '#type' => 'fieldset',
    '#title' => t('Identity Information'),
    '#weight' => -12,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['identity']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#weight' => 0,
    '#size' => 20,
    '#default_value' => isset($user_info->title) ? $user_info->title : '',
  );
  $form['identity']['national_id_no'] = array(
    '#type' => 'textfield',
    '#title' => t('National ID No'),
    '#weight' => 0,
    '#size' => 20,
    '#default_value' => isset($user_info->national_id_no) ? $user_info->national_id_no : '',
  );
  $form['identity']['names'] = array(
    '#type' => 'textfield',
    '#title' => t('Name(s)'),
    '#weight' => 1,
    '#required' => TRUE,
    '#default_value' => isset($user_info->names) ? $user_info->names : '',
  );
  $form['identity']['surname'] = array(
    '#type' => 'textfield',
    '#title' => t('Surname'),
    '#weight' => 2,
    '#required' => TRUE,
    '#default_value' => isset($user_info->surname) ? $user_info->surname : '',
  );
  $dob = isset($user_info->dob) ? $user_info->dob : '1990-01-01';
  $d = explode('-', $dob);
  $form['identity']['dob'] = array(
    '#type' => 'date',
    '#title' => t('Date of birth'),
    '#weight' => 3,
    '#required' => TRUE,
    '#default_value' => array('year' => (int)$d[0], 'month' => (int)$d[1], 'day' => (int)$d[2]),
  );
  $form['identity']['pob'] = array(
    '#type' => 'textfield',
    '#title' => t('Place of Birth'),
    '#weight' => 4,
    '#required' => TRUE,
    '#size' => 20,
    '#default_value' => isset($user_info->pob) ? $user_info->pob : '',
  );
  $form['identity']['nationality'] = array(
    '#type' => 'textfield',
    '#title' => t('Nationality'),
    '#weight' => 5,
    '#required' => TRUE,
    '#size' => 20,
    '#default_value' => isset($user_info->nationality) ? $user_info->nationality : '',
  );
  $gender = array(
    t('Male'),
    t('Female'),
  );
  $form['identity']['gender'] = array(
    '#type' => 'radios',
    '#title' => t('Gender'),
    '#weight' => 6,
    '#required' => TRUE,
    '#key_type' => 'associative',
    '#options' => drupal_map_assoc($gender),
    '#default_value' => isset($user_info->gender) ? $user_info->gender : '',
  );
  $form['identity']['fathers_name'] = array(
    '#type' => 'textfield',
    '#title' => t("Father's Name"),
    '#weight' => 7,
    '#default_value' => isset($user_info->fathers_name) ? $user_info->fathers_name : '',
  );
  $form['identity']['mothers_name'] = array(
    '#type' => 'textfield',
    '#title' => t("Mother's Name"),
    '#weight' => 8,
    '#default_value' => isset($user_info->mothers_name) ? $user_info->mothers_name : '',
  );
  $form['identity']['home_address'] = array(
    '#type' => 'textarea',
    '#title' => t('Home Address'),
    '#resizable' => FALSE,
    '#weight' => 9,
    '#cols' => 58,
    '#rows' => 4,
    '#default_value' => isset($user_info->home_address) ? $user_info->home_address : '',
  );
  if (!isset($user_info->notes)) {
    $form['identity']['notes'] = array(
      '#type' => 'textarea',
      '#title' => t('Notes'),
      '#weight' => 10,
      '#resizable' => FALSE,
      '#cols' => 58,
      '#rows' => 2,
    );
  }
  return $form;
}
