/**
 * Disclaimer module js file.
 */

(function ($) {
  /* Colorbox trigger does not work with behaviors (infinite loop), we use
   standard document ready. */
  if ($.cookie("disclaimerShow") == null) {
    $(document).ready(function(){
      // Prevent colorbox error.
      if (!$.isFunction($.colorbox)) {
        return;
      }
      var conf = Drupal.settings.disclaimer;
      // Launch colorbox.
      $.colorbox({
        inline:true,
        href:"#disclaimer",
        width:conf.width,
        height:conf.height,
        initialWidth:conf.initialwidth,
        initialHeight:conf.initialheight,
        // No esc or close on click.
        overlayClose:false,
        escKey:false,
        onLoad:function(){
          // Remove close button.
          $('#cboxClose').remove();
          // Bind click action.
          $('#disclaimer_enter').click(function () {
            var close = true;
            // Age form is set, check age.
            if (conf.ageform == 1) {
              close = false;
              var now = new Date();
              var date = now.getDate();
              var month = now.getMonth() + 1;
              var year = now.getFullYear();
              var optmonth = $("#edit-disclaimer-age-month option:selected").val();
              var optday = $("#edit-disclaimer-age-day option:selected").val();
              var optyear = $("#edit-disclaimer-age-year option:selected").val();
              var age = year - optyear;
              if (optmonth > month) {age--;} else {if(optmonth == month && optday >= date) {age--;}}
              // If current year, form not set.
              if (optyear == year) {
                alert(Drupal.t("Please fill in your birth date."));
              } else if (age < conf.limit) {
                alert(Drupal.t("Sorry, you are under age limit and are prohibited from entering this site!"));
                location.replace(conf.exiturl);
              } else {
                // Age limit ok.
                close = true;
              }
            }
            // Everything good, add cookie and close colorbox.
            if (close) {
              $.cookie(conf.cookie_name, '1', {
                path: conf.cookie_path,
                domain: conf.cookie_domain
              });
              $.colorbox.remove();
            }
          });
        }
      });
    });
  }

}(jQuery));
