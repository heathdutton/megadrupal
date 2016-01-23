(function ($) {

  Drupal.behaviors.progress_bar = {
    attach: function (context, settings) {
      if (document === context) {
        var top = $('#toolbar').length > 0 ?  $('#toolbar').height() : 0;
        var backgroundColor = '#09afa0';
        var radius = '25px';
        $.ObibaProgressBarController().update({
          barCssOverride: {background: backgroundColor, top: top},
          spinnerCssOverride: {
            'top': top + 18,
            iconCssOverride:{
              'border-top-color': backgroundColor,
              'border-left-color': backgroundColor,
              'width': radius,
              'height': radius
            }
          }
        });
      }
    }
  }

})(jQuery);
