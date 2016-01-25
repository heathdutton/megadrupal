(function($) {
  Drupal.behaviors.impression = {
    attach: function (context, settings) {
      var text = "";
      var possible = "abcdefghijklmnopqrstuvwxyz0123456789";
      for( var i=0; i < 38; i++ ){
        text += possible.charAt(Math.floor(Math.random() * possible.length));
      }
      $('form:not(.impression-class-processed)', context).each(function () {
        $(this).addClass('impression-class-processed');
        $(this).append($('<input>').attr({
          type: 'hidden',
          name: 'drupal_impression_id',
          value: text
        }));
      });
      $('body').bind('touchstart', function() {
        if (!$(this).hasClass("impression-class-processed")) {
          $.get('/impression/touch/' + text);
          $(this).addClass('impression-class-processed');
        }
      });
      $('body').bind('mousemove', function() {
        if (!$(this).hasClass("impression-class-processed")) {
          $.get('/impression/mousemove/' + text);
          $(this).addClass('impression-class-processed');
        }
      });
      $('body').bind('keyup', function() {
        if (!$(this).hasClass("impression-class-processed")) {
          $.get('/impression/keyup/' + text);
          $(this).addClass('impression-class-processed');
        }
      });
    }
  }
})(jQuery);
