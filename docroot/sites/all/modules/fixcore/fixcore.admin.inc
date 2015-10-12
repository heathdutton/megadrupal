<?php

/**
 * @file
 * This module collects various fixes for Drupal core issues.
 *
 * It would be nice to get them committed to core at some point,
 * but in the meantime this can ease the pain.
 */

/**
 * Settings form at admin/settings/fixcore.
 */
function fixcore_settings_form($form, &$form_state) {
  global $user;

  $tr = 't';
  $form['intro'][] = array(
    '#type'          => 'item',
    '#markup'        => t('This module collects various fixes to core issues that have not been implemented yet. By default, all of these fixes are disabled, and you can enable them one by one.'),
  );
  $form['intro'][] = array(
    '#type'          => 'item',
    '#markup'        => t("When you update Drupal, you should check whether the enabled fixes still apply. At some point, some of the issues may get fixed in core, and the corresponding fix in this module may then start to cause problems, if it's still enabled."),
  );

  $form['admin'][] = array(
    '#type'          => 'item',
    '#markup'        => '<h3>' . t('Administrator issues') . '</h3>',
  );
  $perms = user_permission();
  $perm = $perms['administer permissions']['title'];
  $form['admin']['fixcore_protect_admin_role'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Protect the !Administrator_role setting (!Issue_link)', array('!Administrator_role' => '<a href="' . url('admin/config/people/accounts/settings', array('fragment' => 'edit-admin-role')) . '">' . $tr('Administrator role') . '</a>', '!Issue_link' => '<a href="' . url('http://drupal.org/node/1356964', array('absolute' => TRUE, 'external' => TRUE)) . '">' . t('Issue link') . '</a>')),
    '#default_value' => variable_get('fixcore_protect_admin_role', FALSE),
    '#description'   => t("Hide the control unless the user has the %Administer_permissions permission.", array('%Administer_permissions' => $perm)),
    '#disabled'      => !user_access('administer permissions'),
  );

  $form['admin']['fixcore_avoid_msoffice_404'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Avoid MSOffice Server Extensions %page_not_found errors (!Read_more)', array('%page_not_found' => $tr('page not found'), '!Read_more' => '<a href="' . url('http://www.msoffice-cltreq-asp.info/', array('absolute' => TRUE, 'external' => TRUE)) . '">' . $tr('Read more') . '</a>')),
    '#default_value' => variable_get('fixcore_avoid_msoffice_404', FALSE),
    '#description'   => t("If you're tired of seeing %page_not_found errors for %file1, %file2, %file3, and %file4, then turn this on.", array('%page_not_found' => $tr('page not found'), '%file1' => 'MSOffice/cltreq.asp', '%file2' => '_vti_bin/owssvr.dll', '%file3' => '_vti_bin/shtml.exe/_vti_rpc', '%file4' => '_vti_inf.html')),
  );

  $form['end_user'][] = array(
    '#type'          => 'item',
    '#markup'        => '<h3>' . t('End user issues') . '</h3>',
  );
  $form['end_user']['fixcore_password'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Newbie users (not) changing their password', array()),
    '#description'   => t('This is a severe usability and maintenance issue: Experience shows that newbie users who log in via a log-in link often forget to click the !Save button after changing their password. This causes endless frustration to your users and to you, because they always fail to log in. (!Issue_link)', array('!Save' => '[' . $tr('Save') . ']', '!Issue_link' => '<a href="' . url('http://drupal.org/node/601146', array('absolute' => TRUE, 'external' => TRUE)) . '">' . t('Issue link') . '</a>')),
  );
  $variables = array(
    '!useredit_link' => '<a href="' . url("user/$user->uid/edit") . "\">user/$user->uid/edit</a>",
    '!userview_link' => '<a href="' . url("user/$user->uid") . "\">user/$user->uid</a>",
  );
  $form['end_user']['fixcore_password']['fixcore_password_reminder'] = array(
    '#type' => 'radios',
    '#title' => t('Reminder'),
    '#default_value' => variable_get('fixcore_password_reminder', 0),
    '#options' => array(t('None'), t('Static'), t('Dynamic (uses JavaScript)')),
    '#description' => t('Add a reminder to the description below the password change widget on the !useredit_link page.', $variables) . '<br />' .
                      t("If your site is non-English, you should check whether the hint appears properly translated. If it doesn't, then please contribute the correct translation back to the !fixcore project.", array('!fixcore' => '<a href="' . url('http://localize.drupal.org/translate/projects/fixcore', array('absolute' => TRUE, 'external' => TRUE)) . '">fixcore</a>')),
  );
  $form['end_user']['fixcore_password']['fixcore_log_password_change'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Write a !watchdog entry when the user password is updated', array('!watchdog' => $tr('watchdog'))),
    '#default_value' => variable_get('fixcore_log_password_change', FALSE),
    '#description'   => t('Knowing whether they did change their password or not will help you to get them over that critical hurdle.'),
  );
  $user_permission = user_permission();
  $variables += array(
    '%administer_users' => $user_permission['administer users']['title'],
  );
  $form['end_user']['fixcore_redirect_user_edit'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Redirect the !useredit_link page to !userview_link on @Save (!Issue_link)', $variables + array('@Save' => '[' . $tr('Save') . ']', '!Issue_link' => '<a href="'. url('http://drupal.org/node/1524438', array('absolute' => TRUE, 'external' => TRUE)) .'">'. t('Issue link') .'</a>')),
    '#default_value' => variable_get('fixcore_redirect_user_edit', FALSE),
    '#description'   => t('This is a usability issue (related to the one above): Users can easily get confused when they submit a password change and the form comes back at them, seemingly begging to enter and confirm the new password again.<br />This fix redirects the @User|@Edit page to @User|@View on submit, to reinforce the confirmation, unless the user has the %administer_users permission.', $variables + array('@User' => $tr('User'), '@Edit' => $tr('Edit'), '@View' => $tr('View'), '%administer_users' => $tr('administer users'))),
  );
  if (module_exists('contact')) {
    $form['end_user']['fixcore_contact_select'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Allow preselecting the contact category'),
      '#default_value' => variable_get('fixcore_contact_select', FALSE),
      '#description'   => t('This is a usability issue: The <a href="@link">site-wide contact form</a> does not allow preselecting a category, and thus you cannot create category-specific links. Turn this on and add the index of the desired category to the path, such as !path.', array('@link' => url('contact'), '!path' => '<a href="' . url('contact/1') . '">contact/1</a>')),
    );
  }

  $form['display'][] = array(
    '#type'          => 'item',
    '#markup'        => '<h3>' . t('Display issues') . '</h3>',
    '#description'   => t('No I-prefer-it-this-way tweaks, only fixes for obvious bugs that interfere with intended use!'),
  );
  $form['display']['fixcore_empty_summary_token'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Empty summary token (!Issue_link)', array('!Issue_link' => '<a href="'. url('http://drupal.org/node/1300920', array('absolute' => TRUE, 'external' => TRUE)) .'">'. t('Issue link') .'</a>')),
    '#default_value' => variable_get('fixcore_empty_summary_token', FALSE),
    '#description'   => t("Core's %Teaser display typically uses the %Summary_or_trimmed format for displaying short versions of node bodies. However, the %node_summary token delivers only the summary and is empty if the node's summary field is not set, which is rarely useful. This fix implements the <a href='@link'>patch</a> to return the trimmed body if the summary is empty, using the %Trim_length of the node type's %Teaser display.",
                          array(
                            '%Teaser' => t('Teaser'),
                            '%Summary_or_trimmed' => t('Summary or trimmed'),
                            '%node_summary' => '[node:summary]',
                            '%Trim_length' => t('Trim length'),
                            '@link' => url('http://drupal.org/node/1300920#comment-6582536', array('absolute' => TRUE, 'external' => TRUE)),
                          )),
  );
  $form['display']['fixcore_node_unpublished'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Make the background of unpublished nodes a shade of red (like D6, but a little darker)'),
    '#default_value' => variable_get('fixcore_node_unpublished', FALSE),
    '#description'   => t("Users with the %administer_nodes permission have access to unpublished nodes. Drupal 6 core's node.css used to render them in a shade of red, but in Drupal 7 they are not differentiated from published nodes. If you prefer the Drupal 6 way, then turn this on.", array('%administer_nodes' => 'administer nodes')),
  );

  $form['development'][] = array(
    '#type'          => 'item',
    '#markup'        => '<h3>' . t('Development issues') . '</h3>',
  );
  $form['development']['fixcore_debug_access_denied'] = array(
    '#type'          => 'checkbox',
    '#title'         => t("Provide a debug_backtrace() when user 1 gets an 'access denied' error (!Issue_link)", array('!Issue_link' => '<a href="' . url('http://drupal.org/node/610568', array('absolute' => TRUE, 'external' => TRUE)) . '">' . t('Issue link') . '</a>')),
    '#default_value' => variable_get('fixcore_debug_access_denied', FALSE),
    '#description'   => t('This should never happen, but when it does, you need to find out where it occurs. Install the !Devel module to see the result.', array('!Devel' => '<a href="' . url('http://drupal.org/project/devel', array('absolute' => TRUE, 'external' => TRUE)) . '">Devel</a>')),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = '_fixcore_settings_form_submit';
  return $form;
}

function _fixcore_settings_form_submit($form, &$form_state) {
  drupal_flush_all_caches();
}
