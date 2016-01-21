(function($) {
 Drupal.behaviors.layouter_inside = {
  attach : function(context) {
    $('.layout-import').click(function() {import_to_layouter();});
    if (Drupal.settings.layouter_modal_style) { 
      $layouter_modal_div = $('#layouter-choose-layout-form').parent().parent();
      $layouter_modal_div.addClass('user-layouter-modal');
      $layouter_modal_div.find('.modal-header .close').empty();
    }
  }
}
})(jQuery);

