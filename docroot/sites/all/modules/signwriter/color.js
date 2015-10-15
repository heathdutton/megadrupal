/* $Id $ */
(function ($) {

  $(function() {
    farbify($('#edit-background'), '#background-farb');
    farbify($('#edit-hov-background'), '#hov_background-farb');
    farbify($('#edit-act-background'), '#act_background-farb');

    farbify($('#edit-foreground'), '#foreground-farb');
    farbify($('#edit-hov-foreground'), '#hov_foreground-farb');
    farbify($('#edit-act-foreground'), '#act_foreground-farb');

    farbify($('#edit-shadow-color'), '#shadow-color-farb');
    farbify($('#edit-hov-shadow-color'), '#hov_shadow-color-farb');
    farbify($('#edit-act-shadow-color'), '#act_shadow-color-farb');

    farbify($('#edit-border-color'), '#border-color-farb');
    farbify($('#edit-hov-border-color'), '#hov_border-color-farb');
    farbify($('#edit-act-border-color'), '#act_border-color-farb');
  });
  
  function updateColor(elt, farb) {
    var text = elt.val();
    if (text.length == 6)
      farb.setColor('#' + text);
  }
  
  function farbify(elt, wrapper) {
    var farb = $.farbtastic(wrapper);
    farb.linkTo(function(color) {
        elt
          .css('background-color', color)
          .css('color', this.hsl[2] > 0.5 ? '#000' : '#fff')
          .val(color.substring(1));
      });
    farb.setColor('#' + elt.val());
    elt.bind('keyup', function(){ updateColor(elt, farb); });
  }
})(jQuery);
