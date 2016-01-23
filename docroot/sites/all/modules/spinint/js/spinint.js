/**
 * @file
 * JS bindings for spinint arrows.
 */

(function($){

  Drupal.behaviors.spinint = {
    attach: function (context, settings) {
      settings = Drupal.settings.spinint;
      if (settings.num_len) {
        num_len = settings.num_len;
      } else {
        num_len = null;
      }
      // Deal with form cache not displaying value properly on form validation
      // fail.
      $('.spinint').each(function(index){
        $input = $(this).parents('.spinint-wrapper').next('input');
        if (num_len != null) {
          $display_val = get_display_val($input.val(), num_len);
        } else {
          $display_val = $input.val();
        }
        $(this).text($display_val);
      });

      // Handle up-click.
      $('.spinint-up').unbind('click.spinint').bind('click.spinint', function() {
        $display = $(this).parents('.spinint-wrapper').find('.spinint');
        $input = $(this).parents('.spinint-wrapper').next('input');
        $val = $input.val() / 1.0 + 1;
        if (settings.max && $val > settings.max) {
          if (settings.min) {
            $val = settings.min;
          } else {
            $val = $val - 1;
          }
        }
        if (num_len != null) {
          $display_val = get_display_val($val, num_len);
        } else {
          $display_val = $val;
        }
        $display.text($display_val);
        $input.val($val).trigger('change');
        return false;
      });

      // Handle down-click.
      $('.spinint-down').unbind('click.spinint').bind('click.spinint', function(){
        $display = $(this).parents('.spinint-wrapper').find('.spinint');
        $input = $(this).parents('.spinint-wrapper').next('input');
        $val = $input.val() / 1.0 - 1;
        if (settings.min && $val < settings.min) {
          if (settings.max) {
            $val = settings.max;
          } else {
            $val = $val + 1;
          }
        }
        if (num_len != null) {
          $display_val = get_display_val($val, num_len);
        } else {
          $display_val = $val;
        }
        $display.text($display_val);
        $input.val($val).trigger('change');
        return false;
      });
    }
  };

})(jQuery);

function get_display_val(val, size) {
  var s = val+"";
  while (s.length < size) s = "0" + s;
  return s;
}