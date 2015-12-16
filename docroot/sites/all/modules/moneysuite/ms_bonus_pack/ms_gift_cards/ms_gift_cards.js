(function ($) {
$(document).ready(function() {
  $('.gift_row_dormant .ms_gift_cards_status').click(dormant_clicked);
  $('.gift_row_printed .ms_gift_cards_status').click(printed_clicked);
});

function dormant_clicked() {
  var loader = $('#ms_gift_cards_ajax_loader div').clone();
  loader.appendTo($(this));
  var row_obj = $(this).parents('tr');
  var gift_card_id = row_obj.attr('rel');
  $.get('change/' + gift_card_id + '/printed', function(data) {
    if (data == 'Success') {
      row_obj.removeClass('gift_row_dormant').addClass('gift_row_printed');
      row_obj.children('.ms_gift_cards_status').html('Printed');
      row_obj.children('.ms_gift_cards_status').unbind('click');
      row_obj.children('.ms_gift_cards_status').bind('click', printed_clicked);
    }
    else {
      alert(data);
      loader.remove();
    }
  });
}

function printed_clicked() {
  var loader = $('#ms_gift_cards_ajax_loader div').clone();
  loader.appendTo($(this));
  var row_obj = $(this).parents('tr');
  var gift_card_id = row_obj.attr('rel');
  $.get('change/' + gift_card_id + '/dormant', function(data) {
    if (data == 'Success') {
      row_obj.removeClass('gift_row_printed').addClass('gift_row_dormant');
      row_obj.children('.ms_gift_cards_status').html('Dormant');
      row_obj.children('.ms_gift_cards_status').unbind('click');
      row_obj.children('.ms_gift_cards_status').bind('click', dormant_clicked);
    }
    else {
      alert(data);
      loader.remove();
    }
  });
}
})(jQuery);
