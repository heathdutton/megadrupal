<?php

/**
 * @file
 * Change Password Template file.
 */
$raas_api_key = trim(variable_get('lr_social_login_apikey'));
if (!empty($raas_api_key)):
  ?>
  <script>
    jQuery(document).ready(function () {
      initializeChangePasswordRaasForms();
    });
  </script>
  <div class="messages" style="display:none">
    <h2 class="element-invisible">Error message</h2>
    <ul>
      <li id="messageinfo">

      </li>
      <div class="clear"></div>
    </ul>
  </div>
  <div class="my-form-wrapper">
    <div id="changepasswordbox"></div>
    <div id="setpasswordbox"></div>
  </div>
<?php
endif;
