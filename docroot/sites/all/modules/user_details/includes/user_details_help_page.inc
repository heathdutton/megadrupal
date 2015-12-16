<?php
/**
 * @file
 * Help page
 */

function user_details_help_page() {
  $intro = t('The "User Details" module comes with two blocks, "User Detils: logged-in user" and "User Details: authored user".');
  $intro .= '<br/><br/>';

  $background = '<h3>' . t('Background') . '</h3>';
  $background .= t('The idea for "User Details" started as a request from an end-user, to have a centreal place for quick links to important locations on the website.') . '<br />';
  $background .= t('For Drupal 6 this was just some <a href="http://drupal.org/node/395578">custom code</a> you could add to a  custom block.') . '<br/>';
  $background .= t('For Drupal 7 the "User Details" module has expanded to its own module, which allows non-coders to alter and customize to thier liking.');
  $background .= '<br/><br/>';

  $overview = '<h3>' . t('Overview') . '</h3>';
  $overview .= t('The "User Details" module is a basic collection of user data (or variables) and commonly used links in a block. In no way is this a required part of any website, only for the ease of naviagtion.') . '<br/>';
  $overview .= t('Below is a list of all the stats and links that can be turned on as part of the "User Details" blocks.');
  $overview .= t('The links come with some default images, but you can change these by uploading an image to your web server and typing in the location for each image.');
  $overview .= '<br/><br/>';
  $overview_loggedin = '<div class="overview-loggedin" style="width: 48%; float: left;">';
  $overview_loggedin .= '<h3>' . t('User Details: loggedin user') . '</h3>';
  $overview_loggedin .= '<ul>';
  $overview_loggedin .= '<li>' . t('User avatar') . '</li>';
  $overview_loggedin .= '<li>' . t('Joined date') . '</li>';
  $overview_loggedin .= '<li>' . t('Post count') . '</li>';
  $overview_loggedin .= '<li>' . t('User points') . '</li>';
  $overview_loggedin .= '<li>' . t('Role') . '</li>';
  $overview_loggedin .= '<li>' . t('Unread private messeges') . '</li>';
  $overview_loggedin .= '<li>' . t('Homepage link') . '</li>';
  $overview_loggedin .= '<li>' . t('Profile link') . '</li>';
  $overview_loggedin .= '<li>' . t('Profile edit link') . '</li>';
  $overview_loggedin .= '<li>' . t('Private message link') . '</li>';
  $overview_loggedin .= '<li>' . t('Create link') . '</li>';
  $overview_loggedin .= '<li>' . t('Admin link') . '</li>';
  $overview_loggedin .= '<li>' . t('Panels link') . '</li>';
  $overview_loggedin .= '<li>' . t('Views link') . '</li>';
  $overview_loggedin .= '<li>' . t('Performance link') . '</li>';
  $overview_loggedin .= '<li>' . t('Logout link') . '</li>';
  $overview_loggedin .= '<li>' . t('Custom link one') . '</li>';
  $overview_loggedin .= '<li>' . t('Custom link two') . '</li>';
  $overview_loggedin .= '<li>' . t('Custom link three') . '</li>';
  $overview_loggedin .= '<li>' . t('Custom link four') . '</li>';
  $overview_loggedin .= '<li>' . t('Custom link five') . '</li>';
  $overview_loggedin .= '<li>' . t('Recent Content') . '</li>';
  $overview_loggedin .= '</ul>';
  $overview_loggedin .= '</div>';
  $overview_authored = '<div class="overview-authored" style="width: 48%; float: left;">';
  $overview_authored .= '<h3>' . t('User Details: authored user') . '</h3>';
  $overview_authored .= '<ul>';
  $overview_authored .= '<li>' . t('User avatar') . '</li>';
  $overview_authored .= '<li>' . t('Joined date') . '</li>';
  $overview_authored .= '<li>' . t('Post count') . '</li>';
  $overview_authored .= '<li>' . t('User points') . '</li>';
  $overview_authored .= '<li>' . t('Role') . '</li>';
  $overview_authored .= '<li>' . t('Profile link') . '</li>';
  $overview_authored .= '<li>' . t('Private message link') . '</li>';
  $overview_authored .= '<li>' . t('Recent Content') . '</li>';
  $overview_authored .= '</ul>';
  $overview_authored .= '</div>';
  $closing = '<div style="float: none; clear: both;">';
  $closing .= '<br/><br/>';
  $closing .= t('NOTE:') . '<br/>';
  $closing .= t('*All users will be able to see: stats, user links content list.') . '<br/>';
  $closing .= t('*All admin will be able to see: stats, user links, content list and admin links.') . '<br/>';
  $closing .= t('*The custom links have a dropdown menu to determine which type of user will be able to view them.');
  $closing .= '<br/><br/>';
  $closing .= t('This module is under active development. Any feature request, bug report or issue you might find, please make a new issue in the ' . '<a href="http://drupal.org/project/issues/user_details?categories=All">' . '"User Details" issue queue') . '</a>.';
  $closing .= '</div>';
  $hr = '<hr>';

  $form['intro']['markup'] = array(
    '#markup' => $intro,
  );
  $form['background']['markup'] = array(
    '#markup' => $background,
  );
  $form['overview']['markup'] = array(
    '#markup' => $overview,
  );
  $form['overview_loggedin']['markup'] = array(
    '#markup' => $overview_loggedin,
  );
  $form['overview_authored']['markup'] = array(
    '#markup' => $overview_authored,
  );
  $form['closing']['markup'] = array(
    '#markup' => $closing,
  );
  $form['hr']['markup'] = array(
    '#markup' => $hr,
  );
  return $form;
}