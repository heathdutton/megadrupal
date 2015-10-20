(function($){
  Drupal.behaviors.equalHeight = {
    attach:function(context, settings) {
      var $el = $('.region-sidebar-second, .region-sidebar-first, #content');
      if ($el.length > 1) {
        $el.height('');
        $el.height($el.height());
      }
    }
  }
}(jQuery));
