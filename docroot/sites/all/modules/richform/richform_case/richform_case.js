(function ($) {
  RichForm.correction.case = function (id, data) {
    if (data == 'upper') {
      $('#'+ id).val($('#'+ id).val().toUpperCase());
    }
    if (data == 'lower') {
      $('#'+ id).val($('#'+ id).val().toLowerCase());
    }
  }
})(jQuery);
