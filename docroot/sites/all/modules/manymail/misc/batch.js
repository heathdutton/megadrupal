(function ($) {

/**
 * Deletes the top scrolling behavior for errors if present
 * @see scrolltop.js
 */
delete Drupal.behaviors.ManyMailScrollTop;

/**
 * Attaches the modal dialog behavior.
 */
Drupal.behaviors.ManyMailDialog = {
  attach: function (context, settings) {
    $('#manymail-overlay').once('dialog', function () {
      $(this).dialog({
        dialogClass: 'manymail-overlay',
        closeOnEscape: false,
        draggable: false,
        resizable: false,
        modal: true,
        title: settings.batch.sendMessage,
        width: 500,
        height: 110
      });
    });
  }
};

/**
 * Attaches the batch behavior to progress bars.
 *
 * Modified to not use context and to use custom interval
 */
Drupal.behaviors.ManyMailBatch = {
  attach: function (context, settings) {
    $('#progress').once('batch', function () {
      var holder = $(this);

      // Success: redirect to the summary.
      var updateCallback = function (progress, status, pb) {
        if (progress == 100) {
          pb.stopMonitoring();
          window.location = settings.batch.uri + '&op=finished';
        }
      };

      var errorCallback = function (pb) {
        holder.prepend($('<p class="error"></p>').html(settings.batch.errorMessage));
        $('#wait').hide();
      };

      var progress = new Drupal.progressBar('updateprogress', updateCallback, 'POST', errorCallback);
      progress.setProgress(-1, settings.batch.initMessage);
      holder.append(progress.element);
      progress.startMonitoring(settings.batch.uri + '&op=do', settings.batch.throttlePause);
    });
  }
};

})(jQuery);
