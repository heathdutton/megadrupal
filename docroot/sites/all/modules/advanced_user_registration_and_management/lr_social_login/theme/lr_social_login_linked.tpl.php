<?php
/**
 * @file
 *
 * Social Account Linking Interface theme.
 * */

print theme('lr_social_login_links');
if (isset($providers)) :
  foreach ($providers as $key => $provider) :
    if (!empty($provider)):
      ?>
      <div style="margin-bottom: 3px;">
        <?php
        $message = t('Connected with');
        if (isset($_SESSION['current_social_provider']) && (check_plain($key) == $_SESSION['current_social_provider'])) :
          $message = '<span style="color:green">' . t('Currently connected with') . '</span>';
        endif;
        ?>
        <div style="width:350px; float:left;">
          <?php print $message; ?> <?php print $provider; ?>
          <img style="margin-right: 5%;vertical-align: middle;"
               src="<?php print $GLOBALS['base_url'] ?>/<?php print drupal_get_path('module', 'lr_social_login') ?>/images/<?php print $provider ?>.png">
        </div>
        <div>
          <?php echo l(t('Delete'), 'user/' . $user->uid . '/lr_social_login/delete/' . $key . '/' . $provider, array('attributes' => array('style' => 'vertical-align: top;')));
          ?>
        </div>
      </div>
    <?php
    endif;
  endforeach;
endif;
