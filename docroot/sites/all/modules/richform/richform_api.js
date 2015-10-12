var RichForm = RichForm || { validate: {}, correction :{}, placeholder :{}};
RichForm.hasError = {};

(function ($) {
  Drupal.behaviors.RichForm = {
    attach: function (context) {
      if (Drupal.settings.RichForm.correction) {
        $.each(Drupal.settings.RichForm.correction, function (id, modules){
          $('#'+ id).change(function(){
            $.each(modules, function(module, data){
              RichForm.correction[module](id, data);
            });
          });
        });
      }
      if (Drupal.settings.RichForm.validate){
        var last_id = '';
        $.each(Drupal.settings.RichForm.validate, function(id, modules){
          last_id = id;
          RichForm.errorinit(id);
          $('#'+ id).change(function(){
            RichForm.errordeletall(id);
            $.each(modules, function(module, data){
              RichForm.validate[module](id, data);
            });
          }).addClass('RichForm-validate');
        });
        if (last_id != '') {
          $('#'+ last_id).parents('form:first').submit(function(){
            var hasError = false;
            $('.RichForm-validate').trigger('change');
            $.each(RichForm.hasError, function (id, error) {
              if (error) {
                hasError = true;
              }
            });
            return !hasError;
          });
        }
      }
    }
  };

  RichForm.error = function(id, message) {
    $('#'+ id).addClass('error');
    $(RichForm.placeholder[id] + ' ul').append('<li>'+ message +'</li>');
    $(RichForm.placeholder[id]).show();
    RichForm.hasError[id] = true;
  }

  RichForm.errorinit = function(id) {
    if (Drupal.settings.RichForm.placeholder && Drupal.settings.RichForm.placeholder[id]) {
      RichForm.placeholder[id] = '#'+ Drupal.settings.RichForm.placeholder[id];
      $(RichForm.placeholder[id]).append('<ul></ul>').addClass('messages error richform').hide();
    }
    else {
      if (document.getElementById(id + '-richformerror') === null) {
        $('#' + id).after('<div id="'+ id +'-richformerror" class="messages error richform" style="display:none;"><ul></ul></div>');
        RichForm.placeholder[id] = '#'+ id + '-richformerror';
      }
    }
  }

  RichForm.errordeletall = function(id) {
    $(RichForm.placeholder[id] +' li').remove();
    $('#'+ id).removeClass('error');
    $(RichForm.placeholder[id]).hide();
    RichForm.hasError[id] = false;
  }
})(jQuery);
