(function ($) {


  Drupal.behaviors.m2MachineName = {
    attach: function (context, settings) {
      $('.m2-machine-name', context).once('m2-machine-name', function () {
        var machine_name = $(this),
            machine_name_src_el = $('.m2-machine-name-src', machine_name),
            machine_name_dst_el = $('.m2-machine-name-dst', machine_name);
        var on_change = function(){
          var match_res = machine_name_src_el.attr('value').match(/[A-Za-z0-9_\x2D\x20]{1,1}/gi),
              new_value = match_res ? match_res.join('').toLowerCase().replace(/\x2D/g, '_').replace(/\x20/g, '_') : '';
          machine_name_dst_el.attr('value', new_value);
        }
        machine_name_src_el.keyup(on_change).change(on_change);
      });
    }
  };


  Drupal.behaviors.m2FormCollapsed = {
    attach: function (context, settings) {
      $('.form-collapsed', context).once('form-collapsed', function () {
        var form = $(this),
            collapsed_title = $('.collapsed-title', form),
            collapsed_data = $('.collapsed-data', form),
            is_have_error = $('input.error', form).length;
        collapsed_title.click(function(){
          collapsed_data.toggle(100, function(){
            collapsed_title.toggleClass('collapsed-title-collapsed', collapsed_data.is(':hidden'));
          });
        });
        if (is_have_error) collapsed_data.show();
        else collapsed_data.hide();
        collapsed_title.disableSelection();
        collapsed_title.addClass('collapsed-title-processed');
        collapsed_title.toggleClass('collapsed-title-collapsed', collapsed_data.is(':hidden'));
      });
    }
  };


})(jQuery);