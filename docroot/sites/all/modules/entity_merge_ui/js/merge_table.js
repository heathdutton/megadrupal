(function ($) {
  Drupal.behaviors.entity_ui_merge_table = {
    attach: function(context) {
      $('#merge-entities-table tr').each(function() {
        var context = $(this);
        var all_radios = $('input[type=radio]', context);
        $('input[type=radio]:not(.processed)', context).change(function() {
          all_radios.addClass('processed');
          all_radios.not(this).attr('checked', null);
          all_radios.removeClass('processed');
        });
      });
    }
  }
})(jQuery);
