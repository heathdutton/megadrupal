<?php
/**
 * @file
 * Contains all CiviCRM hook implementations
 *
 */

/**
 * Implements hook_civicrm_pageRun().
 */
function civicrm_activity_ical_civicrm_pageRun($page) {
  $page_name = $page->getVar('_name');

  // Conditionally add a status message pointing to the activities iCal feed.
  // Only on the user dashboard, and only if the current user has permissions
  if ($page_name == 'CRM_Contact_Page_View_UserDashBoard' && user_access('access CiviCRM activity iCalendar feed')) {
    $tpl = CRM_Core_Smarty::singleton();
    // Only if this CiviCRM version is showing activities on the user dashboard (3.4.5 and earlier do not)
    if (isset($tpl->_tpl_vars['activity_rows']) || isset($tpl->_tpl_vars['activity_rowsEmpty'])) {
      // Only if the current user is the same as the current dashboard contact
      global $user;
      $current_contact_id = civicrm_activity_ical_get_id('contact_id', $user->uid);
      if ($current_contact_id == $page->_contactId) {
        $link = l(t('Feed details...'), 'civicrm_activity_ical/settings/user');
        CRM_Core_Session::setStatus(ts('Assigned activities are now accessible as an iCalendar feed.') . ' '. $link);
      }
    }
  }
}