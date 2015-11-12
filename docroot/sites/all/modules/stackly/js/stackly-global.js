// Add stacklyCfg to global scope
var stacklyCfg = {};
(function ($) {
  Drupal.behaviors.stacklyGlobal = {
    attach: function (context, settings) {
     //On click event for my stacks
     $('li.stackly-mystacks a', context).click(function(e){
        e.preventDefault();
      });
    }
  }
})(jQuery);
