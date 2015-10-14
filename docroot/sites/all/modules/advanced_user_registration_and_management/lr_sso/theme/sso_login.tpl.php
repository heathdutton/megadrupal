<?php
/**
 * @file
 * sso login
 */
  print theme('lr_message');
  ?>
  <script>
    jQuery(document).ready(function () {
      initializeSocialRegisterRaasForm();
    });
  </script>

  <div class="raas-lr-form my-form-wrapper">
    <?php
    print theme('lr_loading');
    print theme('lr_raas_popup');
    ?>
  </div>
