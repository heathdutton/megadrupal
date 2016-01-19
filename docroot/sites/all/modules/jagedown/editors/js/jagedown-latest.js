(function($) {
  
  Drupal.wysiwyg.editor.attach.jagedown = function(context, params, settings) {
    return $('#'+params.field).jagedown(settings);
  }
  
  Drupal.wysiwyg.editor.detach.jagedown = function(context, params, trigger) {
    if (trigger == 'serialize') {
      return;
    }
    if (typeof params != 'undefined') {
      $('#'+params.field).jagedown_remove();
    }
    else {
      $('#'+params.field).jagedown_remove();
    }
  }
  
  Drupal.wysiwyg.editor.instance.jagedown = {
    
    insert: function (content) {
    //console.log(content);
    },

    setContent: function (content) {
      $('#' + this.field).val(content);
    },

    getContent: function () {
      return $('#' + this.field).val();
    }
    
  }
  
})(jQuery);
