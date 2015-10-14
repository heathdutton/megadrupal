<?php

?>
<div id="user-login" class="node">
<div class="user-desc">
	<h1 class="title title-margin"><?php print t('Returning Members'); ?></h1>
	<p><?php print t('Welcome back, you can use the form below to login to the site'); ?></p>
  <p><?php print t('Not a member yet? register now using our ') . l('registration page', 'user/register'); ?>
</div>
<?php if (!empty($rendered)) print $rendered; ?>

</div>