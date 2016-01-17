(function($) {

Drupal.crush.commands.commands = function(parsed) {
  commands = [];
  for (command in Drupal.settings.crushCommands) {
    commands.push(command);
  }

  Drupal.crush.lineOut(Drupal.t('This is a list of all defined commands. Type "commands" to see this list.'));
  Drupal.crush.lineOut(Drupal.t('Type "help <command>" to find out more about <command>.'));
  Drupal.crush.lineOut(commands.sort().join(', '));
};

})(jQuery);
