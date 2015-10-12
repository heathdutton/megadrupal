(function ($) {

  Drupal.behaviors.views_table_filter = {
    attach: function (context, settings) {
      if (settings['views_table_filter']) {
        for (var view in settings['views_table_filter']) {
          var form = $('input.views-table-filter[value="' + view + '"]').eq(0).closest('form');
          if (form.length > 0) {
            $('.table-header-filter', context).once().each(process_view);
          }
        }
      }

      function process_view() {
        var header_widget_wrapper = $(this);
        var classes = header_widget_wrapper.attr('class').split(' ');
        for (var i in classes) {
          if (/^table-header-filter-/.test(classes[i])) {
            var filter = classes[i].replace(/^table-header-filter-/, '');
            var name = settings['views_table_filter'][view][filter];
            if (!name) {
              continue;
            }
            var input = form.find('[name="' + name + '"],[name="' + name + '[]"],[name="' + name + '[value][date]"]');
            var widget = input.eq(0).closest('.views-widget');
            if (widget.length < 1) {
              continue;
            }
            var widget_wrapper = widget.closest('.views-exposed-widget');
            if (widget_wrapper.length < 1) {
              continue;
            }

            widget_wrapper.hide();
            var widget_clone = widget.clone(true);
            widget_clone.find('[id]').each(add_id_suffix);
            var input_clone = widget_clone.find('[name="' + name + '"],[name="' + name + '[]"],[name="' + name + '[value][date]"]');

            input_clone.change(function() {input.val(input_clone.val()); input.change();});
            input_clone.keyup(function() {setTimeout(function() {input.val(input_clone.val());}, 100);});

            header_widget_wrapper.append(widget_clone);
          }
        }
      }

      function add_id_suffix() {
        var element = $(this);
        element.attr('id', element.attr('id') + '-views-table-filter');
      }
    }
  };


})(jQuery);
