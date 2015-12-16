(function ($) {
  Drupal.behaviors.olebs =  {
    attach: function(context, settings) {

        var data = $(context).data('openlayers');
        if (jQuery.isEmptyObject(data)) {
          return;
        }

      $(".form-item-baselayers select").change(function(e) {
        var map = $(this).closest('form').find("input[name='map']").val();
        if (data.map.map_name == map) {
          var layer = $(this).val();
          var layers = data.openlayers.layers.slice();
          for(var i=0, len=layers.length; i<len; i++) {
            if (layers[i].drupalID == layer) {
              data.openlayers.setBaseLayer(layers[i]);
            }
          }
        }
      });

      $(".form-item-baselayers input[type='radio']").change(function(e) {
        var map = $(this).closest('form').find("input[name='map']").val();
        if (data.map.map_name == map) {
          var layer = $(this).val();
          var layers = data.openlayers.layers.slice();
          for(var i=0, len=layers.length; i<len; i++) {
            if (layers[i].drupalID == layer) {
              data.openlayers.setBaseLayer(layers[i]);
            }
          }
        }
      });

      $(".form-item-overlays input[type='checkbox']").change(function(e) {

        var checked = this.checked;
        var map = $(this).closest('form').find("input[name='map']").val();

        if (data.map.map_name == map) {
          var layer = $(this).val();
          var layers = data.openlayers.layers.slice();
          for(var i=0, len=layers.length; i<len; i++) {
            if (layers[i].drupalID == layer) {
              layers[i].setVisibility(checked);
            }
          }
        }
      });

    }
  };
})(jQuery);
