<?php

/** @file
 *
 * Hooks and other public code for the signup_share_track module.
 *
 */
function signup_share_track_rules_data_info() {
    return array(
        'user' => array(
            'label' => t('User'),
        ),
    );
}

/**
 * Implementation of hook_rules_event_info().
 */
function signup_share_track_rules_event_info() {
    return array(
        'signup_share_track_user_created_event' => array(
            'label' => t('After an User is Created'),
            'group' => t('signup_share_tracks'),
            'variables' => array(
                'user' => array('type' => 'user', 'label' => t('user')),
            ),
        ),
    );
}

/**
 * implementation of hook_rules_condition_info()
 */
function signup_share_track_rules_condition_info() {
    return array(
        'signup_share_track_total_users_1' => array(
            'label' => t('signup_share_track User == 1 Condition'),
            'group' => t('signup_share_tracks'),
            'arguments' => array(
                'user' => array('type' => 'user', 'label' => t('user')),
            ),
        ),
        'signup_share_track_total_users_3' => array(
            'label' => t('signup_share_track User >= 3 Condition'),
            'group' => t('signup_share_tracks'),
            'arguments' => array(
                'user' => array('type' => 'user', 'label' => t('user')),
            ),
        ),
    );
}

function signup_share_track_total_users_1($from_user) {
    $no_of_signedup_users = no_of_signedup_users($from_user->uid);
    return ($no_of_signedup_users == 1) ? TRUE : FALSE;
}

function signup_share_track_total_users_3($from_user) {
    $no_of_signedup_users = no_of_signedup_users($from_user->uid);
    return ($no_of_signedup_users >= 3) ? TRUE : FALSE;
}

function no_of_signedup_users($uid) {

    $query = db_query("select * from  signup_share_track WHERE uid = $uid");
    $result = $query->fetchAll();
    foreach ($result as $key => $value) {
        $count = $value->count;
    }
    return $count;
}

/**
 * Implementation of hook_rules_action_info().
 */
function signup_share_track_rules_action_info() {
    return array(
        'signup_share_track_action_for_sending_mail_1' => array(
            'label' => t('signup_share_track_action_for_sending_mail_1'),
            'group' => t('signup_share_tracks'),
            'parameter' => array(
                'user' => array(
                    'type' => 'user',
                    'label' => t('user object of the referring user'),
                ),
            ),
        ),
        'signup_share_track_action_for_sending_mail_3' => array(
            'label' => t('signup_share_track_action_for_sending_mail_3'),
            'group' => t('signup_share_tracks'),
            'parameter' => array(
                'user' => array(
                    'type' => 'user',
                    'label' => t('user object of the referring user'),
                ),
            ),
        ),
        'signup_share_track_action_for_changing_role' => array(
            'label' => t('signup_share_track_action_for_changing_role'),
            'group' => t('signup_share_tracks'),
            'parameter' => array(
                'user' => array(
                    'type' => 'user',
                    'label' => t('user object of the referring user'),
                ),
            ),
        ),
    );
}

function signup_share_track_action_for_changing_role($from_user) {

    //$uid = 123;// User ID of user that you want to add role to.
    $role_name = 'premium member'; // The name of the role to add.
    if ($role = user_role_load_by_name($role_name)) {
        user_multiple_role_edit(array($from_user->uid), 'add_role', $role->rid);
    }
    drupal_set_message("Premium Member Role added to $from_user->name");
}

/* * ********Mail************* */

function signup_share_track_action_for_sending_mail_1($from_user) {

    $user_data = $from_user->uid;
    $signup_share_track_name = $from_user->name;
    $signup_share_track_email = $from_user->mail;

    $rootpath = dirname(__FILE__);

    if (valid_email_address($signup_share_track_email)) {

        $email_content = '<p>Your Contact info is given below:</p><br>';
        $email_content .= '<p>User Name: ' . $signup_share_track_name . '</p><br>';

        //create a boundary string. It must be unique
        //so we use the MD5 algorithm to generate a random hash
        $random_hash = md5(date("jS M Y", time()));
        $boundary = "boundary-alt-" . $random_hash;
        $attboundary = "boundary-mixed-" . $random_hash;
        $mime_boundary = "==Multipart_Boundary_x{$random_hash}x";

        # Common Headers
        $headers = "From: www.comingsoon.com\nMIME-Version: 1.0\n" . "Content-Type: multipart/mixed;\n" . " boundary=\"{$mime_boundary}\"";

// multipart boundary
        $body = "Congratulations! You have created your first user!!\n\n" . "--{$mime_boundary}\n" . "Content-Type: text/html; charset=\"iso-8859-1\"\n" . "Content-Transfer-Encoding: 7bit\n\n" . $email_content . "\n\n";
        $body .= "--{$mime_boundary}\n";

        $subject = "Email From ComingSoon";

        $ok = mail($signup_share_track_email, $subject, $body, $headers);

//        if ($ok) {
//            drupal_set_message('Mail 1 has been sent.');
//        } else {
//            drupal_set_message('Mail 1 cannot be sent.');
//        }
//        return true;
    } else {
        drupal_set_message('Error in sending mail to $from_user->mail on creation of first user', 'error');
    }

    drupal_set_message("Mail sent to $from_user->mail on creation of first user");
}

function signup_share_track_action_for_sending_mail_3($from_user) {

    $user_data = $from_user->uid;
    $signup_share_track_name = $from_user->name;
    $signup_share_track_email = $from_user->mail;

    $rootpath = dirname(__FILE__);

    if (valid_email_address($signup_share_track_email)) {

        $email_content = '<p>Your Contact info is given below:</p><br>';
        $email_content .= '<p>User Name: ' . $signup_share_track_name . '</p><br>';

        //create a boundary string. It must be unique
        //so we use the MD5 algorithm to generate a random hash
        $random_hash = md5(date("jS M Y", time()));
        $boundary = "boundary-alt-" . $random_hash;
        $attboundary = "boundary-mixed-" . $random_hash;
        $mime_boundary = "==Multipart_Boundary_x{$random_hash}x";

        # Common Headers
        $headers = "From: www.comingsoon.com\nMIME-Version: 1.0\n" . "Content-Type: multipart/mixed;\n" . " boundary=\"{$mime_boundary}\"";

// multipart boundary
        $body = "Congratulations! You have created 3+ users!!\n\n" . "--{$mime_boundary}\n" . "Content-Type: text/html; charset=\"iso-8859-1\"\n" . "Content-Transfer-Encoding: 7bit\n\n" . $email_content . "\n\n";
        $body .= "--{$mime_boundary}\n";

        $subject = "Email From ComingSoon";

        $ok = mail($signup_share_track_email, $subject, $body, $headers);

//        if ($ok) {
//            drupal_set_message('Mail 3 has been sent.');
//        } else {
//            drupal_set_message('Mail 3 cannot be sent.');
//        }
//        return true;
    } else {
        drupal_set_message('Error in sending mail to $from_user->mail on creation of 3+ users', 'error');
    }

    drupal_set_message("Mail sent to $from_user->mail on creation of 3+ users");
}
