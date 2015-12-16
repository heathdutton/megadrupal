Drupal.behaviors.InitializeMapplic = {
  attach: function (context, settings) {
    jQuery("#mapplic").mapplic(Drupal.settings.mapplic);
  }
};
