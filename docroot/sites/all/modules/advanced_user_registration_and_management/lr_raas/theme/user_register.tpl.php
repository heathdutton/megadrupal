<?php if (!$admin_access):
  if (user_is_logged_in()):
    drupal_goto('user');
  endif;
  $raas_api_key = trim(variable_get('lr_social_login_apikey'));
  if (!empty($raas_api_key)):
    print theme('lr_message');
    ?>
    <script>
      jQuery(document).ready(function () {
       initializeRegisterRaasForm();
        initializeSocialRegisterRaasForm();
          var isClear = 1;
          var formIntval;
        setTimeout(show_birthdate_date_block, 1000);
          formIntval = setInterval(function(){ jQuery('#lr-loading').hide();
             if (isClear > 0) {
                 clearInterval(formIntval);
             }
         }, 1000);
      });
    </script>
    <p><?php print $intro_text; ?></p>
    <label><?php print $sociallogin_widget_title; ?></label>
    <?php
    drupal_add_js(array('lrsociallogin' => $my_settings), 'setting');
    print theme('raas_social_widget_container');
    ?>
    <div class="raas-lr-form my-form-wrapper">
      <div id="registeration-container"></div>
      <?php
      print theme('lr_loading');
      print theme('lr_raas_popup');
      ?>
    </div>
  <?php
  endif;
endif;
