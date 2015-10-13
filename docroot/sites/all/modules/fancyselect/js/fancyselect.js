(function ($) {
  Drupal.behaviors.fancyselect = {
    attach: function(context, settings) {
      $(window).load(function() {
        var $fancyselect_dom_selector = Drupal.settings.fancyselectSettings.domSelector;
        jQuery($fancyselect_dom_selector).each(function(){
          jQuery(this).fancySelect();
        });
      });
  }
  }
}(jQuery));
