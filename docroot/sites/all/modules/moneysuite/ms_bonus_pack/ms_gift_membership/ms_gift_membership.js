(function ($) {
  $(document).ready(function() {
    if ($('#edit-send-mail').attr('checked'))
      {
        $('#edit-receiver-address-wrapper').show();
        $('#edit-receiver-email-wrapper').hide();
      }
      else
      {
        $('#edit-receiver-address-wrapper').hide();
        $('#edit-receiver-email-wrapper').show();
      }
    $('#edit-send-mail-wrapper').click(function(){
      if ($('#edit-send-mail').attr('checked'))
      {
        $('#edit-send-email').attr('checked', '');
        $('#edit-receiver-address-wrapper').show();
        $('#edit-receiver-email-wrapper').hide();
      }
      else
      {
        $('#edit-send-email').attr('checked', 'checked');
        $('#edit-receiver-address-wrapper').hide();
        $('#edit-receiver-email-wrapper').show();
      }
    });

    if ($('#edit-send-email').attr('checked'))
      {
        $('#edit-receiver-address-wrapper').hide();
        $('#edit-receiver-email-wrapper').show();
      }
      else
      {
        $('#edit-receiver-address-wrapper').show();
        $('#edit-receiver-email-wrapper').hide();
      }
    $('#edit-send-email-wrapper').click(function(){
      if ($('#edit-send-email').attr('checked'))
      {
        $('#edit-send-mail').attr('checked', '');
        $('#edit-receiver-address-wrapper').hide();
        $('#edit-receiver-email-wrapper').show();
      }
      else
      {
        $('#edit-send-mail').attr('checked', 'checked');
        $('#edit-receiver-address-wrapper').show();
        $('#edit-receiver-email-wrapper').hide();
      }
    });

    $('.gift_row_3 .ms_gift_membership_status').click(dormant_clicked);
    $('.gift_row_4 .ms_gift_membership_status').click(printed_clicked);
  });

  function dormant_clicked() {
    var loader = $('#ms_gift_membership_ajax_loader div').clone();
    loader.appendTo($(this));
    var row_obj = $(this).parents('tr');
    var gift_card_id = row_obj.attr('rel');
    $.get('change/' + gift_card_id + '/4', function(data) {
      if (data == 'Success') {
        row_obj.removeClass('gift_row_3').addClass('gift_row_4');
        row_obj.children('.ms_gift_membership_status').html('Mailed');
        row_obj.children('.ms_gift_membership_status').unbind('click');
        row_obj.children('.ms_gift_membership_status').bind('click', printed_clicked);
      }
      else {
        alert(data);
        loader.remove();
      }
    });
  }

  function printed_clicked() {
    var loader = $('#ms_gift_membership_ajax_loader div').clone();
    loader.appendTo($(this));
    var row_obj = $(this).parents('tr');
    var gift_card_id = row_obj.attr('rel');
    $.get('change/' + gift_card_id + '/3', function(data) {
      if (data == 'Success') {
        row_obj.removeClass('gift_row_4').addClass('gift_row_3');
        row_obj.children('.ms_gift_membership_status').html('Pending');
        row_obj.children('.ms_gift_membership_status').unbind('click');
        row_obj.children('.ms_gift_membership_status').bind('click', dormant_clicked);
      }
      else {
        alert(data);
        loader.remove();
      }
    });
  }
})(jQuery);
