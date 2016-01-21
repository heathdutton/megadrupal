(function ($) {

Drupal.behaviors.addtocopy = {};
Drupal.behaviors.addtocopy.attach = function(context, settings) {
  var text = Drupal.settings.addtocopy.htmlcopytxt.split('[link]').join(window.location.href);
  $(Drupal.settings.addtocopy.selector, context).addtocopy({
    htmlcopytxt: text,
    minlen:Drupal.settings.addtocopy.minlen,
    addcopyfirst: parseInt(Drupal.settings.addtocopy.addcopyfirst)
  });
};

})(jQuery);
