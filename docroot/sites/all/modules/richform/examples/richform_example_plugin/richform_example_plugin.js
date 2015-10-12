(function ($) {
  RichForm.validate.example_plugin = function (id, data) {
    $.each(data, function (index, pluginData){
      switch(pluginData.example_plugin) {
        case 'palindrome':
          str = $('#' + id).val();
          revStr = str.split('').reverse().join('');
          if (str != revStr) {
            RichForm.error(id, pluginData['error_message']);
          }
          break;
      }
    });
  }
})(jQuery);
