(function ($) {
  // Define multiselect list behavior.
  Drupal.behaviors.commerceBpostMultiselectList = {
    attach: function (context, settings) {
      var left = $('.form-item-shipping-services-countries-list-available select');
      var right = $('.form-item-shipping-services-countries-list-enabled select');

      // Remove enabled countries from left list.
      var left_options = $('option', left).filter(function (index) {
        return (typeof settings.commercebpost_country_list == 'object') && !($(this).val() in settings.commercebpost_country_list);
      }).attr('selected', '');
      left.empty().append(left_options);

      // Remove disabled countries from right list.
      var right_options = $('option', right).filter(function (index) {
        return (typeof settings.commercebpost_country_list == 'object') && $(this).val() in settings.commercebpost_country_list;
      }).attr('selected', '');
      right.empty().append(right_options);

      // Select all the options in the right select list when submitting form.
      $('#edit-save:not(.multiselect-processed), #edit-shipping-services-refresh:not(.multiselect-processed)', context).addClass('multiselect-processed').bindFirst('mousedown', function () {
        $('option', right).attr('selected', 'selected');
      });

      // Move options from left to right.
      $('#edit-shipping-services-add').click(function (e) {
        e.preventDefault();
        var selection = $('option:selected', left);
        right.append(selection);
        right.sort_select_box();
      });

      // Move options from right to left.
      $('#edit-shipping-services-remove').click(function (e) {
        e.preventDefault();
        var selection = $('option:selected', right);
        left.append(selection);
        left.sort_select_box();
      });
    }
  };

  // Helper function to sort options from select list.
  $.fn.sort_select_box = function () {
    var my_options = $("#" + this.attr('id') + ' option');
    my_options.sort(function (a, b) {
      if (a.text > b.text) return 1;
      else if (a.text < b.text) return -1;
      else return 0
    })
    this.empty().append(my_options);
  }

  // Helper function to attach an event handler on top of the handler stack.
  $.fn.bindFirst = function (name, fn) {
    // bind as you normally would
    this.bind(name, fn);

    this.each(function () {
      var handlers = jQuery(this).data("events")[name.split('.')[0]];
      // take out the handler we just inserted from the end
      var handler = handlers.pop();
      // move it at the beginning
      handlers.splice(0, 0, handler);
    });
  };
})(jQuery);