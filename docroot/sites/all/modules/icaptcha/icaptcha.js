(function ($) {

Drupal.behaviors.iCaptcha = {
  attach: function(){

    $(document).ready(
      function(){

        $('.icaptcha').parents('form').submit(

          function(){
            if (!ICAPTCHA.verify()){
              $(this).find('.js-widget-icaptcha-wrapper').addClass('error');
              return false;
            }
          }

        );

      }
    );

  }
}

}) (jQuery);