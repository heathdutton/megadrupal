(function($){
  Drupal.behaviors.nestedCheckbox = {
    attach: function(context, settings) {
      $('ul.nested-checkboxes-top', context).tristate();
    }
  }
})(jQuery);
