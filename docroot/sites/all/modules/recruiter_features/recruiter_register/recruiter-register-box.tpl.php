<?php

/**
 * @file
 * Default theme implementation for the recruiter register box.
 *
 * @ingroup themeable
 */

$t_options = array(
  'context' => 'recruiter_register',
);
?>
<div class="register-box">
  <h2><?php print t('No account yet?', array(), $t_options); ?></h2>
  <div class="register-text"><?php print t('Here you can register as applicant or recruiter.'); ?></div>
  <div class="register-box-links">
    <div><?php print l(t('Register as applicant', array(), $t_options), 'user/register'); ?></div>
    <div><?php print l(t('Register as recruiter', array(), $t_options), 'user/register/recruiter'); ?></div>
  </div>
</div>
