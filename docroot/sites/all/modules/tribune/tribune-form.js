(function($) {
Drupal.behaviors.tribune_form = {
  attach: function() {
    if ($('[name=tribune_type]:checked').length == 0) {
      $('[name=tribune_type]:first').attr('checked', 'checked').change();
    }
  }
};

})(jQuery);
