(function($) {

Drupal.crush.commands.cd = function(parsed) {
  if (parsed.args !== undefined && parsed.args.length !== 0) {
    destination = parsed.args.shift();
    current_path = $('iframe').attr('src').replace(Drupal.settings.basePath, '').replace('?q=', '');
    cur_args = current_path.split('/');
    dest_args = destination.split('/');
    if (destination[0] == '/') {
      cur_args = [];
    }
    for (index in dest_args) {
      arg = dest_args[index];
      if (arg === '.' || arg === '') {
        // Do nothing.
      }
      else if (arg === '..') {
        // Go up a level, but not past the root of the Drupal installation.
        if (cur_args.length >= 1) {
          cur_args.pop();
        }
      }
      else {
        // Add a URL argument.
        cur_args.push(arg);
      }
    }
    Drupal.crush.go(cur_args.join('/'), parsed);
  }
};

})(jQuery);
