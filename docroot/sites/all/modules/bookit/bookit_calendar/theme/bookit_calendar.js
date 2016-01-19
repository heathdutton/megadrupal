(function($) {
  $(document).ready(function() {
    // TODO: Make it dynamic.
    $("tr.editable").selectable({
      filter: "td.day",
      selected: function(event, ui) {
        $('#dialog').dialog('open');
      }
    });

    $("tr.editable input").hover(function() {
      $(this).parents('td.day').addClass('hover');
    }, function() {
      $(this).parents('td.day').removeClass('hover');
    });

    $('#bookit-calendar-admin-form').append('<div id="dialog" title="Bulk update"><input id="update-text" type="text" size="15"/></div>');

    $('#dialog').dialog({
      autoOpen:false,
      buttons: {
        Update: function() {
          value = $('#update-text').val();
          $('tr.editable .ui-selected input:text').val(value).trigger('change');
          $('#update-text').val('').trigger('change');
          $(this).dialog("close");
        },
        Cancel: function() {
          $(this).dialog("close");
        }
      },
      close: function() {
        $('tr.editable .ui-selected').removeClass('ui-selected');
      }
    });
  });
})(jQuery);
