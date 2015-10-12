(function ($) {

Drupal.behaviors.timelinr = {
  attach:  function(context) {
    if(Drupal.settings.timelinr) {
      jQuery.each(Drupal.settings.timelinr, function(id, data) {
        Drupal.timelinr.createWidget(id,data);
      });
    }
  }
};

Drupal.timelinr = {
  createWidget: function(id,args) {
    args = eval(args);
    jQuery().timelinr(args);
  }
};

})(jQuery);
