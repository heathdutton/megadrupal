(function ($) {
  Drupal.behaviors.scaldMap = {
    attach: function (context, settings) {
      $('body').once('scald-map', function() {
        if (typeof CKEDITOR !== 'undefined') {
          CKEDITOR.on('dialogDefinition', function (ev) {
            if (typeof Drupal.dndck4 !== 'undefined') {
              var dialogName = ev.data.name;
              var dialogDefinition = ev.data.definition;

              if (dialogName == 'atomProperties') {
                var infoTab = dialogDefinition.getContents('info');

                infoTab.add({
                  type: 'text',
                  label: 'Default Zoom Level',
                  id: 'zoomlevel',
                  setup: function (widget) {
                    if (Drupal.dnd.Atoms[widget.data.sid].meta.type === 'map') {
                      this.getElement().show();
                      var options = JSON.parse(widget.data.options);
                      this.setValue(options.zoomlevel);
                    }
                    else {
                      this.disable();
                      this.getElement().hide();
                    }
                  },
                  commit: function (widget) {
                    if (Drupal.dnd.Atoms[widget.data.sid].meta.type === 'map') {
                      // Copy the current options into a new object,
                      var options = JSON.parse(widget.data.options);
                      var value = this.getValue();
                      if (value != '') {
                        options.zoomlevel = value;
                      }
                      else {
                        delete options.zoomlevel;
                      }
                      widget.setData('options', JSON.stringify(options));
                    }
                  }
                });
              }
            }
          });
        }
      });
    }
  };
})(jQuery);