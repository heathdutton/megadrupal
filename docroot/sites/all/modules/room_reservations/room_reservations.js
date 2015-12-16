(function ($) {

Drupal.behaviors.room_reservations = {};

Drupal.behaviors.room_reservations.attach = function(context) {
  // set default Category tab if one is set in URL anchor  
  var anchor = window.location.hash;
  if (anchor) {
    $('.room-tabs a.active').removeClass('active');
    $('.room-tabs li a[href=' + anchor + ']').addClass('active');
    $('.panel').hide();
    $(anchor).show();
  }
  
  // show the selected category panel
  $('.room-tabs a').click(function() {
    $this = $(this);
    $('.panel').hide();
    $('.room-tabs a.active').removeClass('active');
    $this.addClass('active').blur();
    var panel = $this.attr('href');
    $(panel).fadeIn(250);
    return false;
  });

  // change calendar date displayed
  $('#edit-date-datepicker-popup-0').change(function() {
    var datebits = $(this).val().split('/');
    var formatarr = Drupal.settings.room_reservations.dateformat.split('/');
    var dateobj = new Object();
    $.each(formatarr, function(index, value) {
      dateobj[value] = datebits[index];
    });
    var val = dateobj.m + '/' + dateobj.d;
    var path = window.location.href;
    var loc = path.lastIndexOf('room_reservations');
    var end = loc + 17;
    var newpath = path.substring(0, end).concat('/');
    window.location.href = newpath.concat(val);
  });

  // show form fields for text message confirmation and reminder
  $('#edit-textmsg').each(function() {
    if ($(this).attr('checked')) {
      $('#txtmsg-fields').slideDown('fast');
    }
    else {
      $('#txtmsg-fields').slideUp('fast');
    }
  });
  $('#edit-textmsg').click(function() {
    if ($(this).attr('checked')) {
      $('#txtmsg-fields').slideDown('fast');
    }
    else {
      $('#txtmsg-fields').slideUp('fast');
    }
  });
};
}(jQuery));
