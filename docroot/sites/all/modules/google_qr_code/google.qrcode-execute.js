/**
*Code to load on page load
*/

(function ($) {
  Drupal.behaviors.jquery_qr_code = {
    attach: function(context, settings) {
      moduleBlock = $('#google-qr-code').html();
      if(moduleBlock.length > 0){
        if (Drupal.settings.googleQRcode.whenShow == "on_click"){
          // on click version
          $('.content #google-qr-code a').click(function(){
            $('.content #google-qr-code').html('');
            createQRcode();
            return false;
          });
        }else{
          // On page load version
          createQRcode();
        }
      }
    }
  }
})(jQuery);

/**
* Onclick function for displaying QR code from google
*/

function createQRcode(){
  googleQRarguments = "?chs=" + Drupal.settings.googleQRcode.width + "x" +
  Drupal.settings.googleQRcode.height + "&cht=qr&chl=" + document.URL +
  '&chld=H|0';
  output = '<div class="inner"><img src="https://chart.googleapis.com/chart'
 + googleQRarguments + '"></img></div>';
  jQuery('.content #google-qr-code').html(output);
}
