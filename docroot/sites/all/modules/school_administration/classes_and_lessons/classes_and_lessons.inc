<?php

/**
 * @file
 * Form generation, submission and validation functions for the clases_and_lessons module.
 */

// ======================================
// Form Generation Functions:
// ======================================

function classes_and_lessons_student_class_add_form($empty, $form_state, $school_no) {
  $form['class_name'] = array(
    '#type' => 'select',
    '#title' => t('Select class for !name', array('!name' => school_administration_formatted_full_name($school_no))),
    '#weight' => 1,
    '#required' => TRUE,
    '#description' => t('Select a class to add the student.'),
    '#options' => drupal_map_assoc(classes_and_lessons_listof_all_classes()),
  );
  $form['school_no'] = array(
    '#type' => 'value',
    '#value' => $school_no,
  );
    $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#weight' => 10,
  );
  return $form;
}

function classes_and_lessons_class_edit_form($form_id, $form_state, $nid = NULL) {
  $rids = variable_get('school_administration_teacher_roles', array());
  if (count($rids) < 1) {
    drupal_set_message(t('You should set teacher roles from !here', array('!here' => l('settings page', 'admin/config/school_administration'))), 'warning');
    drupal_goto('classes_and_lessons');
  }
  $teachers = school_administration_list_of_all_teachers();
  if (count($teachers) < 1) {
    drupal_set_message(t('You should have at least one teacher'), 'warning');
    drupal_goto('classes_and_lessons');
  }
  $class = classes_and_lessons_class_load($nid);
  $teacher_options[0] = '--';
  foreach ($teachers as $teacher) {
    $teacher_options[$teacher] = school_administration_formatted_full_name($teacher);
  }
  asort($teacher_options);
  $form['hr_teacher'] = array(
    '#type' => 'select',
    '#title' => t('Homeroom Teacher'),
    '#weight' => 2,
    '#default_value' => isset($class->hr_teacher1) ? $class->hr_teacher1 : 0,
    '#options' => $teacher_options,
  );
  $form['hr_teacher2'] = array(
    '#type' => 'select',
    '#title' => t('2nd Homeroom Teacher'),
    '#weight' => 3,
    '#default_value' => isset($class->hr_teacher2) ? $class->hr_teacher2 : 0,
    '#options' => $teacher_options,
  );
  $form['supervisor1'] = array(
    '#type' => 'select',
    '#title' => t('Supervisor'),
    '#weight' => 4,
    '#default_value' => isset($class->supervisor1) ? $class->supervisor1 : 0,
    '#options' => $teacher_options,
  );
  $form['supervisor2'] = array(
    '#type' => 'select',
    '#type' => 'select',
    '#weight' => 5,
    '#title' => t('Supervisor 2'),
    '#default_value' => isset($class->supervisor2) ? $class->supervisor2 : 0,
    '#options' => $teacher_options,
  );
  $form['supervisor3'] = array(
    '#type' => 'select',
    '#title' => t('Supervisor 3'),
    '#weight' => 6,
    '#default_value' => isset($class->supervisor3) ? $class->supervisor3 : 0,
    '#options' => $teacher_options,
  );
  $form['for_gender'] = array(
    '#type' => 'select',
    '#title' => t('For Gender'),
    '#weight' => 7,
    '#default_value' => isset($class->for_gender) ? $class->for_gender : 0,
    '#options' => array(
      t('Mixed'),
      t('Male'),
      t('Female'),
    ),
  );
  $form['class_capacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Capacity'),
    '#weight' => 8,
    '#size' => 20,
    '#default_value' => isset($class->capacity) ? $class->capacity : 0,
  );
  $form['class_notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Notes'),
    '#weight' => 9,
    '#resizable' => FALSE,
    '#cols' => 40,
    '#default_value' => isset($class->notes) ? $class->notes : '',
  );
  return $form;
}

function classes_and_lessons_tcl_edit_form($empty, $form_state, $tcl_id = NULL) {
  $rids = variable_get('school_administration_teacher_roles', array(1));
  if (count($rids) == 1 && $rids[0] == 1) {
    drupal_set_message(t('You should set teacher roles from !here', array('!here' => l('settings page', 'admin/config/school_administration'))), 'warning');
    return;
  }
  
  $teachers = school_administration_list_of_all_teachers();
  if (count($teachers) < 1) {
    drupal_set_message(t('You should have at least one teacher'), 'warning');
    return;
  }
  
  $sql = 'SELECT nid, title FROM {node} n WHERE n.type = :type AND n.status = :status ORDER BY nid DESC';
  $num_lessons = count(db_query($sql, array(':type' => 'lesson', ':status' => 1))->fetchCol());
  if ($num_lessons < 1) {
    drupal_set_message(t('You should have at least one lesson. !link', array('!link' => l('Add Lesson', 'node/add/lesson'))), 'warning');
    return;
  }
  $lessons = db_query($sql, array(':type' => 'lesson', ':status' => 1));
  $lesson_options[0] = '--';
  foreach ($lessons as $lesson) {
    $lesson_options[$lesson->nid] = $lesson->title;
  }
  asort($lesson_options);
  
  $teacher_options[0] = '--';
  foreach ($teachers as $teacher) {
    $teacher_options[$teacher] = school_administration_formatted_full_name($teacher);
  }
  asort($teacher_options);
  
  if ($tcl_id != NULL) {
    $tcl = classes_and_lessons_tcl_by_id($tcl_id);
    $title = t('Edit Teacher-Class-Lesson');
  }
  $form['tcl'] = array(
    '#type' => 'fieldset',
    '#title' => isset($title)? $title : t('Add Teacher-Class-Lesson'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tcl']['class_name'] = array(
    '#type' => 'select',
    '#title' => t('Class'),
    '#weight' => 1,
    '#required' => TRUE,
    '#default_value' => isset($tcl->class) ? $tcl->class : '',
    '#options' => drupal_map_assoc(classes_and_lessons_listof_all_classes()),
  );
  $form['tcl']['lesson_hour'] = array(
    '#type' => 'textfield',
    '#title' => t('Lesson Hours'),
    '#weight' => 2,
    '#size' => 5,
    '#required' => TRUE,
    '#description' => t('Weekly hours of the lesson'),
    '#default_value' => isset($tcl->lesson_hour) ? $tcl->lesson_hour : '',
  );

  $string = str_replace("\r", '', variable_get('classes_and_lessons_lessons'));
  $options = explode("\n", $string);
  array_walk($options, 'trim');
  sort($options);
  $form['tcl']['lesson'] = array(
    '#type' => 'select',
    '#title' => t('Lesson'),
    '#weight' => 3,
    '#required' => TRUE,
    '#default_value' => isset($tcl->lesson) ? $tcl->lesson : '',
    '#options' => $lesson_options,
  );
  $form['tcl']['teacher'] = array(
    '#type' => 'select',
    '#title' => t('Teacher'),
    '#weight' => 4,
    '#required' => TRUE,
    '#default_value' => isset($tcl->teacher) ? $tcl->teacher : '',
    '#options' => $teacher_options,
  );
  if (isset($tcl->id)) {
    $form['tcl']['tclid'] = array(
      '#type' => 'value',
      '#value' => $tcl->id,
    );
    $form['tcl']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 10,
    );
  }
  $form['tcl']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#weight' => 10,
  );
  return $form;
}

function classes_and_lessons_student_class_delete_form($empty, $form_state, $class, $student) {
  $form['class'] = array(
    '#type' => 'value',
    '#value' => $class,
  );
  $form['student'] = array(
    '#type' => 'value',
    '#value' => $student,
  );
  $form['submit'] = array(
    '#prefix' => t("You are about to delete !student from !class ", 
      array(
        '!student' => school_administration_formatted_full_name($student), 
        '!class' => $class,
      )
    ),
    '#id' => 1,
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#weight' => 10,
  );
  return $form;
}

// ======================================
// Form Submit Functions:
// ======================================

function classes_and_lessons_student_class_delete_form_submit($form_id, &$form_state) {
  db_delete('classes_and_lessons_student_classes')
  ->condition('school_no', $form_state['values']['student'], '=')
  ->condition('class', $form_state['values']['class'], '=')
  ->condition('academic_year', SCHOOL_ADMINISTRATION_C_YEAR, '=')
  ->condition('term', SCHOOL_ADMINISTRATION_C_TERM, '=')
  ->execute();
  drupal_set_message(
    t('!student is removed from !class..', array(
      '!student' => school_administration_formatted_full_name($form_state['values']['student']),
      '!class' => $form_state['values']['class'],
    ))
  );
  $form_state['redirect'] = 'classes_and_lessons/student_classes/' . $form_state['values']['student'];
}

function classes_and_lessons_student_class_add_form_submit($form_id, &$form_state) {
  $fields = array('class' => $form_state['values']['class_name'],
    'academic_year' => SCHOOL_ADMINISTRATION_C_YEAR,
    'term'          => SCHOOL_ADMINISTRATION_C_TERM,
    'class'         => $form_state['values']['class_name'],
    'school_no'     => $form_state['values']['school_no'],
  );
  db_insert('classes_and_lessons_student_classes')
  ->fields($fields)
  ->execute();
  drupal_set_message(t('The student is added to the new class..'));
}

function classes_and_lessons_tcl_edit_form_submit($form_id, &$form_state) {
  $fields = array('class' => $form_state['values']['class_name'],
    'academic_year' => SCHOOL_ADMINISTRATION_C_YEAR,
    'term'          => SCHOOL_ADMINISTRATION_C_TERM,
    'lesson_hour' => $form_state['values']['lesson_hour'],
    'lesson'      => $form_state['values']['lesson'],
    'teacher'     => $form_state['values']['teacher'],
  );
  if (isset($form_state['values']['tclid'])) {
    if ($form_state['clicked_button']['#parents'][0] == 'delete') {
      db_delete('classes_and_lessons_tcl')
      ->condition('id', $form_state['values']['tclid'], '=')
      ->execute();
      drupal_set_message(
        t('Teacher-Class-Lesson match with id = !id is deleted..', 
          array('!id' => $form_state['values']['tclid'])
        )
      );
      $path_arr = explode('/', current_path());
      array_pop($path_arr);
      $form = drupal_retrieve_form('classes_and_lessons_tcl_edit_form', $form_state);
      drupal_redirect_form($form, implode('/', $path_arr));
    }
    else {
      db_update('classes_and_lessons_tcl')
      ->fields($fields)
      ->condition('id', $form_state['values']['tclid'], '=')
      ->execute();
      drupal_set_message(t('Teacher-Class-Lesson match is updated..'));
    }
  }
  else {
    db_insert('classes_and_lessons_tcl')
    ->fields($fields)
    ->execute();
    drupal_set_message(t('New Teacher-Class-Lesson matching is saved..'));
  }
}