
(function ($) {

  Drupal.behaviors.eightball = {
    attach: function(context, settings) {
      $("#eightball-div").click(function() {
        $.get(settings.basePath + 'eightball', function(data){
          $("#eightball-div").fadeOut('slow', function(){
            $("#eightball-div", context)
              .removeClass('eightball-ball')
              .addClass('eightball-answer')
              .html('<br>' + data)
              .fadeIn('slow', function(){
                $("#eightball-div").delay(3000).fadeOut('slow', function(){
                  $("#eightball-div")
                    .removeClass('eightball-answer')
                    .addClass('eightball-ball')
                    .html('<span onclick=eightball()>&nbsp;</span>')
                    .fadeIn('slow');
                });
              });
          });          
        });
      });
    }
  }

})(jQuery);
