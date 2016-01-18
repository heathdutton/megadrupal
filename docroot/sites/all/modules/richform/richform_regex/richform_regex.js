(function ($) {
  RichForm.validate.regex = function (id, data) {
    $.each(data, function (index, regexdata){
      separator = regexdata.regex.substr(0,1);
      end = regexdata.regex.lastIndexOf(separator);
      rp = new RegExp(regexdata.regex.substr(1, end - 1), regexdata.regex.substr(end + 1));
      if (!$('#'+ id).val().match(rp)) {
        RichForm.error(id, regexdata['error_message']);
      }
    });
  }

  RichForm.correction.regex = function (id, data) {
    $.each(data, function (index, regexdata){
      separator = regexdata.from.substr(0,1);
      end = regexdata.from.lastIndexOf(separator);
      rp = new RegExp(regexdata.from.substr(1, end - 1), regexdata.from.substr(end + 1) + 'g');
      $('#'+ id).val($('#'+ id).val().replace(rp, regexdata.to));
    });
  }
})(jQuery);
