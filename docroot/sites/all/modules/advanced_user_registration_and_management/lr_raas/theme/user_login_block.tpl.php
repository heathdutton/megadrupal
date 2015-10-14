<?php
if (user_is_logged_in()):
  drupal_goto('user');
endif;
$raas_api_key = trim(variable_get('lr_social_login_apikey'));
if (!empty($raas_api_key)):
  print theme('lr_message');
  ?>
  <script>
    jQuery(document).ready(function () {
      initializeLoginRaasForm();
      initializeSocialRegisterRaasForm();
    });
  </script>
  <p><?php print $intro_text; ?></p>
  <label><?php print $sociallogin_widget_title; ?></label>
  <div>
    <?php
    drupal_add_js(array('lrsociallogin' => $my_settings), 'setting');
    print theme('raas_social_widget_container');
    ?>
  </div>
  <div class="my-form-wrapper">
    <div id="login-container"></div>

    <?php
    print theme('lr_loading');
    print theme('lr_raas_popup');
    ?>
    <?php print $links; ?>
  </div>
<?php
endif;
