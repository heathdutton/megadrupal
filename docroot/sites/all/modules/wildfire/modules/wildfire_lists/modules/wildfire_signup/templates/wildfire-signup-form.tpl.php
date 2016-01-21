<?php
/**
 * @file
 *    Template that contains the content of the Wildfire Signup form
 */
?>

<?php if (!empty($subscribed)): ?>
  <p class="signed-up">You have already signed up to receive the newsletter. If you like, you can <?php print l(t('change these settings'), 'user/' . $user->uid . '/edit'); ?>.</p>
<?php else: ?>
  <?php if (empty($signup_form)): ?>
    <p class="sign-up error">There was a problem displaying the signup form.</p>
  <?php else: ?>
    <p class="sign-up">Sign up for our newsletter to receive regular email updates.</p>
    <?php print drupal_render($signup_form); ?>
  <?php endif; ?>
<?php endif; ?>
