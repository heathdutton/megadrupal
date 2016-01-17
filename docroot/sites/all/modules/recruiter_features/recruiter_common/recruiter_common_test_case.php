<?php

/**
 * @file
 * Common test case for Recruiter tests.
 */

/**
 * This class serves as a helper for Recruiter test cases and contains useful
 * helper methods.
 *
 * WARNING: Currently the testing system does not correctly detect modules in
 * installation profile folders (e.g. profiles/recruiter/modules). Workaround:
 * create a link in sites/all/modules that points to
 * profiles/recruiter/modules
 *
 * Issue on drupal.org: http://drupal.org/node/1093420
 */
abstract class RecruiterWebTestCase extends DrupalWebTestCase {

  /**
   * Returns a new user with role 'adminstrator'.
   */
  public function getAdminUser() {
    $role = user_role_load_by_name('administrator');
    return $this->createUser($role);
  }

  /**
   * Returns a new user with role 'recruiter'.
   */
  public function getRecruiterUser() {
    $role = user_role_load_by_name('recruiter');
    // @todo role ids are completely broken, if modules are enable in the wrong
    // order.
    return $this->createUser($role);
  }

  /**
   * Returns a new user with role 'applicant'.
   *
   * Requires 'recruiter_resume' feature.
   */
  public function createApplicantUser() {
    $role = user_role_load_by_name('applicant');
    // @todo role ids are completely broken, if modules are enable in the wrong
    // order.
    return $this->createUser($role);
  }

  /**
   * Helper function that creates a user object with the given role.
   */
  protected function createUser($role) {
    $edit = array();
    $edit['name'] = $this->randomName();
    $edit['mail'] = $edit['name'] . '@example.com';
    // @todo role ids are completely broken, if modules are enable in the wrong
    // order.
    $edit['roles']  = array($role->rid => $role->name);
    $edit['pass']   = user_password();
    $edit['status'] = 1;

    $user = user_save(drupal_anonymous_user(), $edit);
    $user->pass_raw = $edit['pass'];
    return $user;
  }
}
