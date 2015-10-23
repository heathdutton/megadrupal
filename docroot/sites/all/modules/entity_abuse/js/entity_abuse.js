(function($) {

  Drupal.ajax.prototype.commands.open_dialog = function(ajax, response, status) {

    var $dialog = $('#entity-abuse-dialog');

    if ($dialog.length == 0) {
      $('<div/>', { id: 'entity-abuse-dialog' }).appendTo('body');
    }

    $('#entity-abuse-dialog')
      .html(response.data)
      .dialog(response.settings);

    // Execute all behaviors for the modal dialog.
    Drupal.attachBehaviors($dialog, Drupal.settings);
  }

}(jQuery));