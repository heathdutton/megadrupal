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
      initializeForgotPasswordRaasForms();
    });
  </script>
  <div class="raas-lr-form my-form-wrapper">
    <div id="forgotpassword-container"></div>
  </div>
<?php
endif;