<?php
/**
 * @file
 * CiviCRM hook implmentations for civicrm_private_report module.
 *
 */


/**
 * Implements hook_civicrm_buildForm().
 */
function civicrm_private_report_civicrm_buildForm($formName, $form) {
  if (is_subclass_of($form, 'CRM_Report_Form')) {
    // Only do this for existing reports, not when creating a new report.
    if ($instance_id = $form->_defaultValues['id']) {
      // If this is a private report, ensure user has 'administer Reports' OR:
      // 1) the user is the private report owner, and
      // 2) the user has permission to access private reports.
      $is_private = FALSE;
      $user_is_owner = FALSE;
      $rows = _civicrm_private_report_get_rows(array('id', 'uid'), array('instance_id' => $instance_id));
      if (!empty($rows)) {
        $private_record = $rows[0];
        $is_private = (bool) $private_record['id'];
        global $user;
        $user_is_owner = (bool) ($private_record['uid'] == $user->uid);
        if ($is_private) {
          if (!user_access('administer Reports')) {
            if (!$user_is_owner || !user_access('administer own CiviCRM reports')) {
              drupal_access_denied();
              exit;
            }
          }
        }
      }

      // Only users with "administer reports" OR both "access Report Criteria" and "administer own CiviCRM
      // reports" should be getting the "Private Report" features.
      if (
        (user_access('access Report Criteria') && user_access('administer own CiviCRM reports')) ||
        (user_access('administer Reports'))
      ) {

        // Prime array of Drupal JavaScript settings.
        $settings = array(
          'civicrm_private_report' => array(
            'has_access' => TRUE,
          ),
        );

        // Add a new "save as" text field.
        $form->addElement(
            'text', 'save_as_name', t('Save a copy as')
        );
        // Add a "save copy" button
        $button_params = array(
          'class' => 'form-submit default',
        );
        $form->addElement('submit', 'do_save_as', t('Save copy'), $button_params);

        // If user has 'administer Reports', add a 'promote to site-wide report'
        // button.
        if ($is_private && user_access('administer Reports')) {
          $button_params = array(
            'class' => 'form-submit',
            'onClick' => "return confirm('" . t('Are you sure - change this user-private report into a site-wide report?') . "');",
          );
          $form->addElement('submit', 'do_promote', t('Promote to site-wide report'), $button_params);
        }

        // Unfortunately, CiviCRM will save this field's value into the report parameters,
        // thus it's automatically populated with the last entered value. Clear it.
        $defaults = array(
          'save_as_name' => '',
        );
        $form->setDefaults($defaults);

        // Tell report form if it should display the "update report" button (normally
        // CiviCRM is smarty enough to do this, but we're enabling this button for
        // users to whom CiviCRM won't normally give that access).
        if (
          $form->getVar('_instanceButtonName') == $form->controller->getButtonName() . '_save' ||
          $form->getVar('_chartButtonName')    == $form->controller->getButtonName()
        ) {
          $form->assign( 'updateReportButton', TRUE );
        }

        // Show/hide some form elements depending on user permissions:
        if ($is_private && !user_access('administer Reports')) {
          $settings['civicrm_private_report']['is_private'] = TRUE;
          $form->assign('instanceForm', TRUE);
          $form->removeElement('email_subject');
          $form->removeElement('email_to');
          $form->removeElement('email_cc');
          $form->removeElement('parent_id');
          $form->removeElement('permission');
          $form->removeElement('addToDashboard');
        }

        // Add custom form elements to CiviCRM's beginHookFormElements area.
        $tpl = CRM_Core_Smarty::singleton();
        $bhfe = $tpl->get_template_vars('beginHookFormElements');
        if (!$bhfe) {
          $bhfe = array();
        }
        $bhfe[] = 'save_as_name';
        $bhfe[] = 'do_save_as';
        $bhfe[] = 'do_promote';
        $form->assign('beginHookFormElements', $bhfe);

        // We use CiviCRM's beginHookFormElements to format the fields properly,
        // and Smarty to create the "Private Copy" accordion HTML. We'll then
        // use JavaScript to add that accordion into the form, and to move those
        // form fields into it.
        // Tell template if this is a user-private report and whether the user
        // is the owner.
        $tpl->assign('is_private', $is_private);
        $tpl->assign('user_is_owner', $user_is_owner);
        // Tell template the owner's name if it's not the user.
        if (isset($private_record) && is_array($private_record) && $private_record['uid']) {
          $owner_user = user_load($private_record['uid']);
          $tpl->assign('username', $owner_user->name);
        }
        // Parse the template, and store its output for passing to JavaScript.
        $template = dirname(__FILE__) . '/templates/privateCopy.tpl';
        $tpl_output = $tpl->fetch($template);
        $settings['civicrm_private_report']['html'] = $tpl_output;

        drupal_add_js($settings, array('type' => 'setting', 'scope' => JS_DEFAULT));
        drupal_add_js(drupal_get_path('module', 'civicrm_private_report') . "/resource/js/CRM_Report_Form.js");
      }
    }
  }
}

/**
 * Implements hook_civicrm_validate().
 */
function civicrm_private_report_civicrm_validate($formName, &$fields, &$files, &$form) {
  $errors = array();
  if (is_subclass_of($form, 'CRM_Report_Form')) {
    if (isset($form->_submitValues['do_save_as']) && !isset($form->_submitValues['save_as_name'])) {
      $errors['save_as_name'] = t('Please provide a name for the new copy of this report.');
    }
  }
  return $errors;
}

/**
 * Implements hook_civicrm_pageRun().
 */
function civicrm_private_report_civicrm_pageRun(&$page) {

  if (get_class($page) == 'CRM_Report_Page_InstanceList') {
    // Remove from this list any user-private reports.
    $tpl = CRM_Report_Page_InstanceList::getTemplate();
    $list = $tpl->get_template_vars('list');
    if (is_array($list) && !empty($list)) {
      $private_instance_ids = array();
      $flag_user_has_private_reports = FALSE;
      global $user;

      // Get a list of all user-private reports.
      $rows = _civicrm_private_report_get_rows(array('instance_id', 'uid'));
      foreach ($rows as $row) {
        $private_instance_ids[] = $row['instance_id'];
        if ($row['uid'] == $user->uid) {
          // Mark if any private reports are found for the current user.
          $flag_user_has_private_reports = TRUE;
        }
      }
      if (!empty($private_instance_ids)) {
        $new_list = array();
        foreach ($list as $category => $instances) {
          $new_list[$category] = array();
          foreach ($instances as $instance_id => $instance) {
            if (!in_array($instance_id, $private_instance_ids)) {
              $new_list[$category][$instance_id] = $instance;
            }
          }
        }
        $tpl->assign('list', $new_list);

        // If any private reports are found for the current user, and he still
        // has the right permissions, tell user how to find them.
        if (user_access('administer own CiviCRM reports') && $flag_user_has_private_reports) {
          $args = array(
            '!link' => l(t('Manage user-private reports here.'), 'civicrm_private_report/list'),
          );
          CRM_Core_Session::setStatus(t('Some user-private reports were found for you, not listed below. !link', $args));
        }
      }
    }
  }
}

/**
 * Implements hook_civicrm_postProcess().
 */
function civicrm_private_report_civicrm_postProcess($formName, &$form) {
  if (is_subclass_of($form, 'CRM_Report_Form')) {
    $instance_id = $form->_defaultValues['id'];
    if (isset($form->_submitValues['do_save_as'])) {
      if (user_access('administer own CiviCRM reports')) {
        if ($instance_id) {
          require_once 'CRM/Report/BAO/Instance.php';
          CRM_Report_BAO_Instance::retrieve( array('id' => $instance_id), $instance_values );
          $instance_values['title'] = $form->_submitValues['save_as_name'];
          unset($instance_values['navigation_id']);
          unset($instance_values['id']);
          $bao = new CRM_Report_BAO_Instance();
          $bao->copyValues($instance_values);
          $bao->save();

          // Report success
          if ($bao->id) {
            CRM_Core_Session::setStatus(t('Your new report has been created.'));
          }

          // Mark the report as private to the current user.
          _civicrm_private_report_set_private_report($bao->id, TRUE);

          // Redirect to the new report
          drupal_goto('civicrm/report/instance/' . $bao->id, array('query' => array('reset' => '1')));
        }
      }
      else {
        drupal_access_denied();
        exit;
      }
    }
    elseif ($form->_submitValues['do_promote']) {
      if (user_access('administer Reports')) {
        // Remove user-private settings on this report instance.
        _civicrm_private_report_set_private_report($instance_id, FALSE);

        // Confirm to user that the report was promoted
        CRM_Core_Session::setStatus(t('This report has been promoted to site-wide availability.'));

        // Reload the report instance.
        drupal_goto('civicrm/report/instance/' . $instance_id, array('query' => array('reset' => '1')));
      }
      else {
        drupal_access_denied();
        exit;
      }
    }
  }
}
