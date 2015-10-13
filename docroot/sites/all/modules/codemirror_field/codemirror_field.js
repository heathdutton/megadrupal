(function($){
  Drupal.behaviors.codemirror_field = {
    attach: function(context, settings) {

      textareas = $('textarea.codemirror');

      $.each(textareas, function(i, textarea){

        if($(textarea).hasClass('codemirror-processed')) {
          return true;
        }

        instance = $.grep(this.className.split(" "), function(v, i){
          return v.indexOf('codemirror-instance') === 0;
        }).join();

        if(typeof settings.codemirror.settings[instance] != 'undefined') {

          if(typeof(settings.codemirror.settings[instance]['readOnly']) != 'undefined' && settings.codemirror.settings[instance]['readOnly'] != 'nocursor') {
            settings.codemirror.settings[instance]['readOnly'] = parseInt(settings.codemirror.settings[instance]['readOnly']);
          }

          CodeMirror.fromTextArea(textarea, settings.codemirror.settings[instance]);
        }

        $(textarea).addClass('codemirror-processed');

      });



    }
  }
}(jQuery));
