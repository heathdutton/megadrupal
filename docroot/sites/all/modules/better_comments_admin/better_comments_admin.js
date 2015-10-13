(function($) {
  Drupal.behaviors.betterCommentsAdmin = {
    attach: function(context) {
      var move = function() {
        var s = $(".pane-better-comments-admin-comment-preview");
        var st = $(window).scrollTop();
        var ot = s.parent().offset().top;

        s.css({
          position: st > ot ? 'fixed' : 'relative',
          top: st > ot ? '0px' : '',
          width: s.parent().width(),
        });
      };
      $(window).scroll(move);
      move();
    },
  }
})(jQuery);
