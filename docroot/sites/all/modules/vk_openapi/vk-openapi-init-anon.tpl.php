<?php
/**
 * @file
 * Template file for anonymous users.
 */
?>
<div id="vk_api_transport"></div>
<script type="text/javascript">
(function ($) {
  Drupal.behaviors.vkLogin = {
    attach: function (context, settings) {
      window.vkAsyncInit = function() {
        VK.init({
          apiId: <?php print $variables['apiID']; ?>,
          nameTransportPath: "<?php print $variables['path']; ?>",
          status: true
        });
        
        $('.vk_login').each(
          function(i) {
            elid = $(this).attr('id');
            VK.UI.button(elid);
            el = document.getElementById(elid);
          }
        );
        
        $('.vk_login tr td:nth-child(2) div div').html('Войти');
        $('.vk_login tr td:nth-child(4) div div').html('Контакте');
      };
    
      (function() {
         var el = document.createElement("script");
         el.type = "text/javascript";
         el.charset = "windows-1251";
         el.src = "http://vkontakte.ru/js/api/openapi.js";
         el.async = true;
         document.getElementById("vk_api_transport").appendChild(el);
      }());
    }
  };

})(jQuery);
</script>