(function($) {
  $(document).ready(function() {
    // Un-hide the "add parameter" button and hijack the button's click event.
    $('#edit-conditions-plugins-query-param-values-add-another').removeClass('element-hidden').click(function(e) {
      e.preventDefault();
      var lastRowCopy = $('#context-query-param-elements-table table tbody tr').last().clone();
      var attrReplace = function(attr, value) {
        var dividers = (attr == 'name' ? ['[', ']'] : ['-', '-']);
        var pattern = new RegExp('.*?\\' + dividers[0] + '(\\d+)\\' + dividers[1], ["i"]);
        var match = pattern.exec(value);
        if (match != null) {
          var incremented = Number(match[1]) + 1;
          return value.replace(dividers[0] + match[1] + dividers[1], dividers[0] + incremented + dividers[1]);
        }
        else {
          return value;
        }
      };

      // Flip from odd to even and vice versa.
      if (lastRowCopy.hasClass('odd')) {
        lastRowCopy.removeClass('odd').addClass('even');
      }
      else {
        lastRowCopy.removeClass('even').addClass('odd');
      }

      // Set inputs to defaults.
      lastRowCopy.find('input[type="text"]').val('');
      lastRowCopy.find('input[type="checkbox"]').val(1);

      // For each child item, update the delta in the input name + ID + div class.
      lastRowCopy.find('input[name^="conditions[plugins][query_param][values][parameters]"], select[name^="conditions[plugins][query_param][values][parameters]"]')
        .attr('id', function(i, n) {return attrReplace('id', $(this).attr('id'));})
        .attr('name', function(i, n) {return attrReplace('name', $(this).attr('name'));});

      lastRowCopy.find('div.form-item')
        .attr('class', function(i, n) {return attrReplace('class', $(this).attr('class'));});

      $('#context-query-param-elements-table table tbody').append(lastRowCopy);
    });
  });
})(jQuery);
