(function ($) {

Drupal.behaviors.JSC3D = {
  attach: function (context, settings) {
    $('.jsc3d').each(function() {
      var id = $(this).attr('id');
      var file = settings.v3dm_jsc3d.files[id];
      var viewer = new JSC3D.Viewer(this);
      viewer.setParameter('SceneUrl', file);

      var keys = ['InitRotationX', 'InitRotationY', 'InitRotationZ', 'ModelColor', 'BackgroundColor1', 'BackgroundColor2', 'BackgroundImageUrl', 'RenderMode', 'Definition', 'MipMapping', 'SphereMapUrl'];
      $.each(keys, function(index, value) {
        viewer.setParameter(value, settings.v3dm_jsc3d[value]);
      });

      viewer.init();
      viewer.update();
    });
  }
};

})(jQuery);
