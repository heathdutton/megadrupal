(function($) {

Drupal.behaviors.nojs = {
  attach: function(context, settings) {
    $('body',context).removeClass('no-js').addClass('js');
  }
};

})(jQuery);
