<?php

/**
 * @file
 * Course object classes for course relationships.
 */

/**
 * CourseObject that holds a Course.
 */
class CourseObjectCourse extends CourseObjectNode {

  /**
   * Valid node types are courses.
   */
  function getNodeTypes() {
    return course_get_types();
  }

  /**
   * Set up defaults.
   */
  function optionsDefinition() {
    $definition = parent::optionsDefinition();
    $definition['course_outline'] = 'course';
    $definition['node_type'] = 'course';
    $definition['course_goto'] = 'view';
    $definition['private'] = 0;
    return $definition;
  }

  function optionsForm(&$form, &$form_state) {
    parent::optionsForm($form, $form_state);
    $defaults = $defaults = $this->getOptions();

    // Course outline display handler.
    $outlines = array();
    $handlers = course_get_handlers('outline');
    foreach ($handlers as $outline_handlers) {
      if ($outline_handlers) {
        foreach ($outline_handlers as $key => $outline_handler) {
          $outlines[$key] = $outline_handler['name'];
        }
      }
    }

    if (empty($defaults['instance'])) {
      $form['node']['course_outline'] = array(
        '#title' => t('Course outline display'),
        '#type' => 'select',
        '#options' => $outlines,
      );
    }

    $form['node']['course_goto'] = array(
      '#type' => 'select',
      '#title' => t('Course entry point'),
      '#options' => array(
        'view' => t('View Course'),
        'take' => t('Take Course'),
      ),
      '#default_value' => $defaults['course_goto'],
      '#description' => t('Selecting "Take Course" will launch the user directly into taking the course, without viewing the course body.'),
    );
  }

  /**
   * Validate the referenced course to make sure that it does not contain this
   * course.
   */
  function optionsValidate(&$form, &$form_state) {
    preg_match('#\[nid: (\d+)\]#', $form['node']['instance']['#value'], $match);
    if (isset($match[1])) {
      $node = node_load($match[1]);
      $course = course_get_course($node);
      foreach ($course->getObjects() as $courseObject) {
        if ($courseObject->getInstanceId() == $this->getCourseNid()) {
          form_error($form['node']['instance'], t('Circular dependency detected. The specified course cannot contain the current course.'));
        }
      }

      if ($node->nid == $this->getCourseNid()) {
        form_error($form['node']['instance'], t('Circular dependency detected. The specified course cannot be the current course.'));
      }
    }
    parent::optionsValidate($form, $form_state);
  }

  /**
   * Create the new course with our node type and outline selection.
   */
  public function create($node = NULL) {
    $node = new stdClass;
    $node->type = $this->getOption('node_type');
    $node->title = $this->getTitle();
    $node->course['outline'] = $this->getOption('course_outline');
    parent::create($node);
  }

  /**
   * The take URL of the course is /takecourse.
   */
  function getTakeUrl() {
    if ($this->getOption('course_goto') == 'take') {
      return url("node/{$this->node->nid}/takecourse");
    }

    return parent::getTakeUrl();
  }

}
