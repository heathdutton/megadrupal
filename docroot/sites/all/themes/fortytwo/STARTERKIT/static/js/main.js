(function ($, Drupal, window, document, undefined) {

  Drupal.behaviors.app = {
    attach: function (context, settings) {
      constants = Drupal.behaviors.fortytwoMain.constants;

      // Store responsive type
      var body = $('body');
      if (body.hasClass('layout-adaptive')) constants.LAYOUT = {
        fluid: false,
        adaptive: true
      };
      else if (body.hasClass('layout-fluid')) constants.LAYOUT = {
        fluid: true,
        adaptive: false
      };

      // Transit.js will fallback to frame based animation when transitions aren't supported
      // if( !$("html").hasClass( "csstransitions" )) $.fn.transition = $.fn.animate;

      /**
       * initiate prettify again for unprettified selects and fileinputs
       * f.e. after ajax requests containing those input types
       * (as this is attach function is fired as callback)
       */
      if (typeof Drupal.behaviors.prettify === 'function') {
        $('select:not(".no-prettify"), input[type="file"]:not(".no-prettify")', context).each( function() {
          new Drupal.behaviors.prettify({ el: this });
        });
      }
    }
  };

})(jQuery, Drupal, this, this.document);
