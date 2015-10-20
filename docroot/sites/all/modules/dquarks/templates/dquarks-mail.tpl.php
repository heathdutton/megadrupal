<?php
/**
 * @file
 * Customize the e-mails sent by dquarks after successful submission.
 *
 * This file may be renamed "dquarks-mail-[nid].tpl.php" to target a
 * specific dquarks e-mail on your site. Or you can leave it
 * "dquarks-mail.tpl.php" to affect all dquarks e-mails on your site.
 *
 * Available variables:
 * - $node: The node object for this dquarks.
 * - $submission: The dquarks submission.
 * - $email: The entire e-mail configuration settings.
 * - $user: The current user submitting the form.
 * - $ip_address: The IP address of the user submitting the form.
 *
 * The $email['email'] variable can be used to send different e-mails to
 * different users when using the "default" e-mail template.
 */
?>


<?php print t(variable_get("dquarksz_submited_date", 'Submitted on @date'), array('@date' => format_date(time(), 'small'))) ?>

<?php if ($user->uid): ?>
  <?php print t(variable_get("dquarksz_submited_authentified_user", 'Submitted by user: @username [@ip_address]'), array('@username' => $user->name, '@ip_address' => $ip_address)) ?><br/>
<?php else: ?>
  <?php print t(variable_get("dquarksz_submited_anony_user", 'Submitted by anonymous user: [@ip_address]'), array('@ip_address' => $ip_address)) ?><br/>
<?php endif; ?>


<?php print variable_get("dquarksz_mail_header", 'Submitted values are') ?>:

<?php
// Print out all the dquarks fields. This is purposely a theme function call
// so that you may remove items from the submitted tree if you so choose.
print theme('dquarks_mail_fields', array(
          'cid' => 0,
          'value' => $form_values['submitted_tree'],
          'node' => $node, 'indent' => "\n",
        ));
if ($node->dquarks['show_score'] && $node->dquarks['mail_add_result']):
  ?>
  <?php
  print t(variable_get("dquarksz_score", 'Your score : !score points'), array("!score" => $score)) . "\n";
endif;
?>

<?php print variable_get("dquarksz_mail_result", t('The results of this submission may be viewed at:')) ?><br/>

<?php
print url('node/' . $node->nid . '/submission/' . $sid, array('absolute' => TRUE));  ?>
