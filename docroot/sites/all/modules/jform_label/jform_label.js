(function($){
  $.fn.extend({
    //pass the options variable to the function
    formLabel: function(options) {
      //Set the default values, use comma to separate the settings, example:
      var defaults = {
        jformLabelFocusedColor : Drupal.settings.jformLabelFocusedColor,
        jformLabelFocusedBgColor : Drupal.settings.jformLabelFocusedBgColor,
        jformLabelFadeOpacity : Drupal.settings.jformLabelFadeOpacity,
        jformLabelFadeDuration: Drupal.settings.jformLabelFadeDuration
      }

      var options =  $.extend(defaults, options);

      return this.each(function() {
        var o = options;
        var obj = $(this);

        // Get all LI in the UL.
        var items = $("label", obj);

        // Each of the.
        items.each(function(){
          objInput = $("#"+$("#"+$(this).attr("for")).attr('id'));
          //not handling should be attached
          if( objInput.length == 0) return;
          if(($(objInput).attr("type") == "text") || ($(objInput).attr("type") == "password") || ($(objInput).is('textarea'))){

            var text = $(this).text();
            var passwordField = false;
            $(objInput).val(text);
            if (objInput.attr("type") == "password") {
              $(objInput).get(0).type = 'text';
              passwordField = true;
            }
            objInput.focus(function () {
              if($(this).val() == text) {
                $(this).css({
                  'color':o.jformLabelFocusedColor,
                  'background-color':o.jformLabelFocusedBgColor
                });

                $(this).stop().animate({
                  opacity: o.jformLabelFadeOpacity
                }, o.jformLabelFadeDuration);
              }
              else{
                if (passwordField==true) {
                  $(objInput).get(0).type = 'password';
                }
              }
            });
            objInput.blur(function () {
              $(this).css({
                'color':'',
                'background-color':'',
                'opacity':1.0
              });
              if($(this).val() == "") {
                $(this).val(text);
              }
            });
            objInput.keydown(function(e){
              if((e.keyCode == 16) || (e.keyCode == 9)) {
                return;
              }

              if (passwordField==true) {
                $(objInput).get(0).type = 'password';
              }
              if($(this).val() == text) {
                $(this).val("");

                $(this).css({
                  'color':'',
                  'background-color':'',
                  'opacity':1.0
                });
              }
            });
            $(this).css("display","none");
          };
        });
      });
    }
  });
})(jQuery);
