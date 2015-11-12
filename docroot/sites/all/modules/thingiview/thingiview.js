(function ($) {

  window.onload = function() {
    console.log(Drupal.settings.thingiview);
    thingiurlbase = Drupal.settings.thingiview.thingiurlbase;
    thingiview = new Thingiview("viewer");
    thingiview.setObjectColor(Drupal.settings.thingiview.ObjectColor);
    thingiview.initScene();
    thingiview.loadSTL(Drupal.settings.thingiview.file);
  }

})(jQuery);