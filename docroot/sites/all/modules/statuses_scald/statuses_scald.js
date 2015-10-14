(function($) {

Drupal.behaviors.statuses_scald = {
  'attach': function(context, settings) {
    $('.statuses-text-main').once('statuses_scald', function() {
      $(this).bind('drop', function(e) {
        var dt = e.originalEvent.dataTransfer;
        var sas = dt.getData("Text");
        var atom = Drupal.dnd.sas2array(sas);
        if (atom) {
          e.preventDefault();
          var destination = $(this).parents('form').find('.form-item-atom-sid input');
          var context = destination.data('scald-context');
          // Save the SAS into text field so that it can be processed server
          // side.
          destination.val(sas);
          // Update the preview asynchronously.
          Drupal.dnd.fetchAtom(context, atom.sid, function() {
            destination.parents('form').find('.atom-preview').html(Drupal.dnd.Atoms[atom_id].contexts[context]);
          });
        }
      });
      $(this).parents('form').find('.form-item-atom-sid').hide();
    });
  }
};

})(jQuery);
