(function ($) {
  Drupal.behaviors.asf = {
    load: function (context, settings) {
      Drupal.behaviors.asf.change($('.iteration_toggler'),$('.iteration_toggler').val());
      $('.iteration_toggler').change(function() {
        Drupal.behaviors.asf.change($(this),$(this).val());
      });

      var items = $('.timepicker');
      if (items.length > 0 && !!items.datetimepicker) {
	      items.datetimepicker({
	        dateFormat: 'yy-mm-dd' , /* ISO DATE FORMAT */
	        timeFormat: 'hh:mm',
	        separator: ' ',
	        defaultDate: $(this).val()
	      });
      }
      $('.interation_end').change(function(){
        var value = $(this).val();
        var max = $(this).parents('.iteration_element').nextAll('.iteration_max');
        if (value == 0) {
          max.addClass('hide');
        }
        else if (value == 1 || value == 2) {
          max.removeClass('hide');
        }
      });
    },
    change: function(item, item_class) {
        item.parent('.form-item').siblings('.form-item').hide();
        item.parent('.form-item').siblings('.form-item').find('input.' + item_class + ', select.' + item_class).parents('.form-item').show();
    }
  };
  $(function () {
    Drupal.behaviors.asf.load();
  });
}(jQuery));
