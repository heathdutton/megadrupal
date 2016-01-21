<?php

/**
 * Course object for claiming course credit.
 */
class CourseObjectCredit extends CourseObject {

  function optionsDefinition() {
    $defaults = parent::optionsDefinition();
    $defaults['required'] = 0;
    return $defaults;
  }

  function take() {
    if (!$awarded = course_credit_get_credit_awarded($this->getCourseNid(), $this->getCourse()->getUser()->uid)) {
      // No credit awarded yet, let's try to award credit.

      if (course_credit_must_claim($this->getCourse()->getNode(), $this->getCourse()->getUser())) {
        // User must claim variable credit, so show them the claiming form.
        return drupal_get_form('course_credit_app_form', $this->getCourse()->getNode());
      }

      // There wasn't any variable credit to be awarded so let's try to claim
      // nonvariable credit.
      course_credit_award_credit($this->getCourse()->getNode(), $this->getCourse()->getUser());
    }

    // Get credit again to see if anything changed.
    $awarded = course_credit_get_credit_awarded($this->getCourseNid(), $this->getCourse()->getUser()->uid);
    if ($awarded) {
      return theme('course_credit_awarded', array('course_credit_awarded' => $awarded));
    }
    else {
      return t('Sorry, there is no credit to be awarded.');
    }
  }

  function save() {
    $this->setInstanceId($this->getCourseNid());
    return parent::save();
  }

  static function getMaxOccurences() {
    return 1;
  }

  function getCloneAbility() {
    return TRUE;
  }

}
