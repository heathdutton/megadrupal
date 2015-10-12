(function ($) {

Drupal.behaviors.Thingiview = {
  attach: function (context, settings) {
    thingiurlbase = settings.v3dm_thingiviewjs.path;
    $('.thingiview').each(function() {
      var id = $(this).attr('id');
      var file = settings.v3dm_thingiviewjs.files[id];
      var thingiview = new Thingiview(id);
      thingiview.setObjectColor(settings.v3dm_thingiviewjs.objectColor);
      thingiview.setObjectMaterial(settings.v3dm_thingiviewjs.objectMaterial);
      thingiview.setBackgroundColor(settings.v3dm_thingiviewjs.backgroundColor);
      thingiview.setRotation(settings.v3dm_thingiviewjs.rotation);

      thingiview.initScene();
      //thingiview.setShowPlane(settings.v3dm_thingiviewjs.showPlane);
      thingiview.setCameraView(settings.v3dm_thingiviewjs.cameraView);
      thingiview.setCameraZoom(settings.v3dm_thingiviewjs.cameraZoom);

      thingiview.loadSTL(file);
    });
  }
};

})(jQuery);
