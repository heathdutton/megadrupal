<?php
/**
 * @file
 * Template file for authrized users.
 */
?><?php
  global $user;
  $user_data = $user->data;
  $vkuid = $user_data['vk_openapi']['vkuid'];
?>  

<div id="vk_api_transport"></div>
<script type="text/javascript">
  window.vkAsyncInit = function() {
    VK.init({
      apiId: <?php print $variables['apiID']; ?>,
      nameTransportPath: "<?php print $variables['path']; ?>",
      status: true
    });
  }

  (function() {
    var el = document.createElement("script");
    el.type = "text/javascript";
    el.charset = "windows-1251";
    el.src = "http://vkontakte.ru/js/api/openapi.js";
    el.async = true;
    document.getElementById("vk_api_transport").appendChild(el);
  }());
</script>