(function($) {

Drupal.crush.commands.ct = function(parsed) {
  if (parsed.args !== undefined && parsed.args.length !== 0) {
    var content_type = parsed.args.shift();
    if (Drupal.settings.crushContentTypes.indexOf(content_type) === -1) {
      Drupal.crush.lineOut(Drupal.t('No such content type.'));
      return;
    }
    destination = 'admin/structure/types/manage/' + content_type;
  }
  else {
    destination = 'admin/structure/types';
  }
  Drupal.crush.go(destination, parsed);
};

})(jQuery);
