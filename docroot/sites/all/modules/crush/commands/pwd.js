(function($) {

Drupal.crush.commands.pwd = function(parsed) {
  current_path = $('iframe').attr('src').replace(Drupal.settings.basePath, '').replace('?q=', '');
  if (current_path === '') {
    current_path = '/';
  }
  Drupal.crush.lineOut(current_path);
};

})(jQuery);
